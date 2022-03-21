using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.IO;
using System.Net.Sockets;
using System.ServiceProcess;
using System.Threading;
using System.Xml.Serialization;
using System.Diagnostics;
using Microsoft.SqlServer.Management.Smo;
using Microsoft.SqlServer.Management.Common;
using System.Text;
using System.Data.Sql;
using System.Data;

namespace SinventCustomResolveService
{
    //https://msdn.microsoft.com/ru-ru/library/vstudio/zt39148a%28v=vs.110%29.aspx
    //installutil.exe SinventCustomResolveService.exe

    /** Сервис синхронизации.
     */
    public partial class MainService : ServiceBase, IBusinessLogic
    {
        private LogWriter m_logWriter = null;           ///< Журнал
        private ResolverServer m_resolverServer = null; ///< Сервер для получания соединений от resolver`а
        private ClientServer m_clientServer = null;     ///< Сервер для получания соединений и взаимодействия по сети с клиентом

        private string m_publisherDBInstance = null;    ///< Имя публикатора
        private string m_backupDirectory = null;        ///< Путь к каталогу backup и restore

        private string m_currentLanguage = "";              ///< Язык
        private string m_currentDBServerInstanceName = null;    ///< Имя текущего СУБД
        private string m_currentDBName = "";                    ///< Имя текущей базы данных
        private string m_mainDBServerInstanceName = null;       ///< Имя СУБД главного клиента
        private int m_lastMergeSessionId = 0;                   ///< Идентификатор последней сессии сихронизации
        private string m_lastMergeTimestamp = "0x00";           ///< Timestamp последней сессии сихронизации
        private string m_lastSnapshotTimestamp = "0x00";        ///< Timestamp последней сессии создания snapshot
        private int m_currentMergeAgentId = 0;                  ///< Идентификатор текущего агента синхронизации
        private int m_stoppedMergeAgentId = 0;                  ///< Идентификатор остановленного програмно агента синхронизации
        private int m_currentSnapshotAgentId = 0;               ///< Идентификатор текущего агента snapshot
        private string m_currentSnapshotAgentJobName = "";      ///< Имя job агента Snapshot
        private bool m_snapshotNeeded = false;                  ///< Snapshot необходим

        ///< Константы состояния job mssql
        private int StartedSessionState = 1;
        private int SucceededSessionState = 2;
        private int InProgressSessionState = 3;
        private int IdleSessionState = 4;
        private int RetryingSessionState = 5;
        private int FailedSessionState = 6;

        private int MaxJobRestartCount = 1; ///< Количество возможных перезапусков job

        private Timer m_timer;      ///< Таймер проверки состояния job

        public enum StartJobResultType
        {
            StartJobSuccessType = 0,    ///< Ok
            StartJobDBNotFoundType,     ///< База данных не найдена
            StartJobExceptionType,      ///< Exception при начале job
            StartJobRestartNeededType,  ///< Необходим перезапуск job
        }   ///< Результат начала job

        public MainService()
        {
            InitializeComponent();
        }

        /** Запустить сервис синхронизации.
         * @param args: аргументы запуска.
         */
        protected override void OnStart(string[] args)
        {
            int resolverPort = 55000;
            int clientPort = 55001;
            int logLevel = 0;
            string xmlFile = "sinventcustomresolveservice.xml";
            if (File.Exists(xmlFile))
            {
                using (Stream stream = new FileStream(xmlFile, FileMode.Open))
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.Settings));
                    SinventCustomResolveService.Settings xmlData = (SinventCustomResolveService.Settings)serializer.Deserialize(stream);
                    resolverPort = xmlData.resolverPort;
                    clientPort = xmlData.clientPort;
                    logLevel = xmlData.logLevel;
                    m_publisherDBInstance = xmlData.publisherDBInstance;
                    m_backupDirectory = xmlData.backupDirectory;
                }
            }
            else
            {
                SinventCustomResolveService.Settings xmlData = new SinventCustomResolveService.Settings();
                xmlData.resolverPort = resolverPort;
                xmlData.clientPort = clientPort;
                xmlData.logLevel = logLevel;
                m_publisherDBInstance = "UPE-SRV";
               // m_publisherDBInstance = Directory.GetCurrentDirectory();
                xmlData.publisherDBInstance = m_publisherDBInstance;
                using (Stream writer = new FileStream(xmlFile, FileMode.Create))
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.Settings));
                    serializer.Serialize(writer, xmlData);
                }
            }
            m_logWriter = new LogWriter(AppDomain.CurrentDomain.BaseDirectory + "SinventCustomResolveService.log", (LogWriter.LogLevel)logLevel);
            m_logWriter.logNormalMessage("start SinventCustomResolveService");

            m_resolverServer = new ResolverServer(this, resolverPort, m_logWriter);
            m_resolverServer.start();
            m_logWriter.logNormalMessage("start ResolverServer");

            m_clientServer = new ClientServer(this, clientPort, m_logWriter);
            m_clientServer.start();
            m_logWriter.logNormalMessage("start ClientServer");


            /*try
            {
                AnonymousMembershipProvider membershipProvider = new AnonymousMembershipProvider();
                DotNetFileSystemProvider fsProvider = new DotNetFileSystemProvider(Directory.GetCurrentDirectory(), false);

                m_ftpServer = new FtpServer(fsProvider, membershipProvider, "127.0.0.1");

                m_ftpServer.Start();
                m_logWriter.logNormalMessage("start FTP server");
            }
            catch (Exception ex)
            {

            }*/

            fillLastMergeSession();
            fillLastSnapshotTimestamp();

            m_timer = new Timer(this.onTimer);
            m_timer.Change(TimeSpan.FromSeconds(0), TimeSpan.FromSeconds(5));
        }

        /** Остановить сервис синхронизации.
         */
        protected override void OnStop()
        {
            m_logWriter.logNormalMessage("stop SinventCustomResolveService");

            m_clientServer.stop();
            m_logWriter.logNormalMessage("stop ClientServer");

            m_resolverServer.stop();
            m_logWriter.logNormalMessage("stop ResolverServer");

            //m_ftpServer.Stop();
            //m_logWriter.logNormalMessage("stop FTP server");
        }

        protected override void OnPause()
        {
            m_logWriter.logNormalMessage("pause SinventCustomResolveService");
        }

        protected override void OnContinue()
        {
            m_logWriter.logNormalMessage("continue SinventCustomResolveService");
        }

        /** Обработка полученного пакета.
         * @param client: сокет.
         * @param serverName: имя сервера (для того чтобы отличать ClientServer и ResolverServer).
         * @param size: размер пакета
         * @param type: тип пакета (enum PacketType).
         * @param packetData: пакет
         */
        byte[] IBusinessLogic.onPacket(Socket client, string serverName, long size, int type, byte[] packetData)
        {
            using (MemoryStream stream = new MemoryStream(packetData))
            {
                if (serverName == ClientServer.clientServerString)
                {
                    if (type == (int)BaseServer.PacketType.MANAGE_PACKET_TYPE)
                    {
                        XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.ManagePacket));
                        SinventCustomResolveService.ManagePacket managePacket = (SinventCustomResolveService.ManagePacket)serializer.Deserialize(stream);
                        if (managePacket.manageType == ManagePacket.ManageType.StartManageType)
                        {
                            if (String.IsNullOrEmpty(m_publisherDBInstance))
                            {
                                managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                                managePacket.errorCode = (int)ManagePacket.ErrorCode.NOT_SET_PUBLISHER_SERVER_NAME;
                                managePacket.errorMessage = "Not set SQL Server instance name for replication service";
                                using (MemoryStream streamOut = new MemoryStream())
                                {
                                    serializer.Serialize(streamOut, managePacket);
                                    return streamOut.ToArray();
                                }
                            }
                            if (String.IsNullOrEmpty(managePacket.dbServerInstanceName))
                            {
                                managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                                managePacket.errorCode = (int)ManagePacket.ErrorCode.NOT_SET_SERVER_NAME;
                                managePacket.errorMessage = "Not set SQL Server instance name";
                                using (MemoryStream streamOut = new MemoryStream())
                                {
                                    serializer.Serialize(streamOut, managePacket);
                                    return streamOut.ToArray();
                                }
                            }
                            else
                            {
                                if (managePacket.mainClient)
                                {
                                    m_clientServer.setMainClient(client);
                                    m_mainDBServerInstanceName = managePacket.dbServerInstanceName;
                                    managePacket.result = ManagePacket.ManageResult.AcceptManageResult;
                                }
                                else
                                {
                                    if (!managePacket.ownResolve && m_clientServer.getMainClient() == null)
                                    {
                                        managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                                        managePacket.errorCode = (int)ManagePacket.ErrorCode.NOT_START_MAIN_RESOLVE_APP;
                                        managePacket.errorMessage = "Main resolver client not started";
                                    }
                                    if (m_clientServer.setMobileClient(client))
                                    {
                                        managePacket.result = ManagePacket.ManageResult.AcceptManageResult;
                                        m_currentLanguage = managePacket.language;
                                        m_currentDBServerInstanceName = managePacket.dbServerInstanceName;
                                        managePacket.result = ManagePacket.ManageResult.AcceptManageResult;
                                        if (m_mainDBServerInstanceName != m_currentDBServerInstanceName)
                                        {
                                            using (MemoryStream streamOutToMainClient = new MemoryStream())
                                            {
                                                managePacket.manageType = SinventCustomResolveService.ManagePacket.ManageType.StartMainClientType;
                                                serializer.Serialize(streamOutToMainClient, managePacket);
                                                m_clientServer.sendToMainClient(type, streamOutToMainClient.ToArray());
                                                managePacket.manageType = ManagePacket.ManageType.StartManageType;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                                        managePacket.errorCode = (int)ManagePacket.ErrorCode.RESOLVE_PROCESS_EXIST;
                                        managePacket.errorMessage = "Resolve proccess exist: " + m_currentDBServerInstanceName;
                                    }
                                }
                                using (MemoryStream streamOut = new MemoryStream())
                                {
                                    serializer.Serialize(streamOut, managePacket);
                                    return streamOut.ToArray();
                                }
                            }
                        }
                        else if (managePacket.manageType == ManagePacket.ManageType.StopManageType)
                        {
                            string errorMessage = "";
                            m_snapshotNeeded = false;
                            stopJob(m_currentDBName, ref errorMessage);

                            managePacket.result = ManagePacket.ManageResult.AcceptManageResult;
                            using (MemoryStream streamOut = new MemoryStream())
                            {
                                serializer.Serialize(streamOut, managePacket);
                                if (managePacket.mainClient)
                                {
                                    m_clientServer.setMainClient(null);
                                    m_mainDBServerInstanceName = "";
                                }
                                else
                                {
                                    m_clientServer.setMobileClient(null);
                                    m_currentLanguage = "";
                                    if (m_mainDBServerInstanceName != m_currentDBServerInstanceName)
                                    {
                                        using (MemoryStream streamOutToMainClient = new MemoryStream())
                                        {
                                            managePacket.manageType = SinventCustomResolveService.ManagePacket.ManageType.StopMainClientType;
                                            serializer.Serialize(streamOutToMainClient, managePacket);
                                            m_clientServer.sendToMainClient(type, streamOutToMainClient.ToArray());
                                            managePacket.manageType = ManagePacket.ManageType.StopManageType;
                                        }
                                    }
                                    m_currentDBServerInstanceName = "";
                                }
                                return streamOut.ToArray();
                            }
                        }
                        else if (managePacket.manageType == ManagePacket.ManageType.NextStepManageType)
                        {
                            for (int i = 0; i < MaxJobRestartCount + 1; i++)
                            {
                                string errorMessage = "";
                                StartJobResultType startJobResult = startJob(managePacket.dbName, ref errorMessage);
                                if (startJobResult == StartJobResultType.StartJobSuccessType)
                                {
                                    m_currentDBName = managePacket.dbName;
                                    managePacket.result = ManagePacket.ManageResult.AcceptManageResult;
                                    break;
                                }
                                else if (startJobResult == StartJobResultType.StartJobDBNotFoundType)
                                {
                                    m_currentMergeAgentId = 0;
                                    m_currentDBName = "";
                                    m_currentSnapshotAgentId = 0;
                                    m_currentSnapshotAgentJobName = "";
                                    m_snapshotNeeded = false;
                                    managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                                    managePacket.errorCode = (int)ManagePacket.ErrorCode.DATABASE_NOT_FOUND;
                                    managePacket.errorMessage = "Database not found";
                                    break;
                                }
                                else if (startJobResult == StartJobResultType.StartJobExceptionType)
                                {
                                    m_currentMergeAgentId = 0;
                                    m_currentDBName = "";
                                    m_currentSnapshotAgentId = 0;
                                    m_currentSnapshotAgentJobName = "";
                                    m_snapshotNeeded = false;
                                    managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                                    managePacket.errorCode = (int)ManagePacket.ErrorCode.START_JOB_EXCEPTION;
                                    managePacket.errorMessage = errorMessage;
                                    break;
                                }
                                else if (startJobResult == StartJobResultType.StartJobRestartNeededType)
                                {
                                    if (i == MaxJobRestartCount)
                                    {
                                        m_currentMergeAgentId = 0;
                                        m_currentDBName = "";
                                        m_currentSnapshotAgentId = 0;
                                        m_currentSnapshotAgentJobName = "";
                                        m_snapshotNeeded = false;
                                        managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                                        managePacket.errorCode = (int)ManagePacket.ErrorCode.START_JOB_EXCEPTION;
                                        managePacket.errorMessage = errorMessage;
                                    }
                                }
                            }
                            using (MemoryStream streamOut = new MemoryStream())
                            {
                                serializer.Serialize(streamOut, managePacket);
                                return streamOut.ToArray();
                            }
                        }
                    }
                    else if (type == (int)BaseServer.PacketType.RESOLVE_PACKET_TYPE)
                    {
                        m_resolverServer.sendToAll(type, stream.ToArray());
                    }
                    else if (type == (int)BaseServer.PacketType.LIST_DATABASE_PACKET_TYPE)
                    {
                        XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.ListDatabasePacket));
                        SinventCustomResolveService.ListDatabasePacket listDatabasePacket =
                            (SinventCustomResolveService.ListDatabasePacket)serializer.Deserialize(stream);
                        string errorMessage = "";
                        listDatabasePacket.dbNames = getListDatabase(listDatabasePacket.username, listDatabasePacket.password, ref errorMessage);
                        if (String.IsNullOrEmpty(errorMessage))
                        {
                            listDatabasePacket.result = ListDatabasePacket.ListDatabaseResult.AcceptListDatabaseResult;
                        }
                        else
                        {
                            listDatabasePacket.result = ListDatabasePacket.ListDatabaseResult.RejectListDatabaseResult;
                            listDatabasePacket.errorMessage = errorMessage;
                        }
                        using (MemoryStream streamOut = new MemoryStream())
                        {
                            serializer.Serialize(streamOut, listDatabasePacket);
                            return streamOut.ToArray();
                        }
                    }
                    else if (type == (int)BaseServer.PacketType.BACKUP_DATABASE_PACKET_TYPE)
                    {
                        XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.BackupDatabasePacket));
                        SinventCustomResolveService.BackupDatabasePacket backupDatabasePacket =
                            (SinventCustomResolveService.BackupDatabasePacket)serializer.Deserialize(stream);

                        string dbName = backupDatabasePacket.dbName;
                        Process process = new Process();
                        process.StartInfo.FileName = "osql.exe";
                        string dirName = m_backupDirectory;
                        if (String.IsNullOrEmpty(dirName))
                        {
                            dirName = Directory.GetCurrentDirectory();
                        }
                        string bakFilename = dirName + "\\" + dbName + ".bak";
                        File.Delete(bakFilename);
                        string username = backupDatabasePacket.username;
                        string password = backupDatabasePacket.password;
                        process.StartInfo.Arguments = " -E -S " + m_publisherDBInstance + " -Q \"BACKUP DATABASE " + dbName + " TO DISK = \'" +
                            bakFilename + "\'\"";
                        if (!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
                        {
                            process.StartInfo.Arguments = " -U " + username + " -P " + password + " -S " + m_publisherDBInstance + " -Q \"BACKUP DATABASE " +
                                dbName + " TO DISK = \'" + bakFilename + "\'\"";
                        }

                        process.StartInfo.UseShellExecute = false;
                        process.StartInfo.RedirectStandardOutput = true;
                        process.StartInfo.RedirectStandardError = true;
                        m_logWriter.logNormalMessage("start osql");
                        process.Start();
                        string output = process.StandardOutput.ReadToEnd();

                        /*Encoding ibm866 = Encoding.GetEncoding("IBM 866);
                        Encoding unicode = Encoding.Unicode;
                        byte[] outputBytes = ibm866.GetBytes(output);
                        byte[] utf8Bytes = Encoding.Convert(ibm866, unicode, outputBytes);
                        char[] utf8Chars = new char[unicode.GetCharCount(utf8Bytes, 0, utf8Bytes.Length)];
                        unicode.GetChars(utf8Bytes, 0, utf8Bytes.Length, utf8Chars, 0);
                        string utf8BytesString = new string(utf8Chars);*/

                        m_logWriter.logNormalMessage("osql output:" + output);
                        process.WaitForExit();
                        int exitCode = process.ExitCode;
                        backupDatabasePacket.message = output;
                        bool bakFileExists = File.Exists(bakFilename);
                        if (exitCode == 0 && bakFileExists)
                        {
                            backupDatabasePacket.result = BackupDatabasePacket.BackupDatabaseResult.AcceptBackupDatabaseResult;
                        }
                        else
                        {
                            backupDatabasePacket.result = BackupDatabasePacket.BackupDatabaseResult.RejectBackupDatabaseResult;
                        }
                        using (MemoryStream streamOut = new MemoryStream())
                        {
                            serializer.Serialize(streamOut, backupDatabasePacket);
                            return streamOut.ToArray();
                        }
                    }
                    else if (type == (int)BaseServer.PacketType.RESTORE_DATABASE_PACKET_TYPE)
                    {
                        XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.RestoreDatabasePacket));
                        SinventCustomResolveService.RestoreDatabasePacket restoreDatabasePacket =
                            (SinventCustomResolveService.RestoreDatabasePacket)serializer.Deserialize(stream);

                        string dbName = restoreDatabasePacket.dbName;
                        Process process = new Process();
                        process.StartInfo.FileName = "osql.exe";
                        string dirName = m_backupDirectory;
                        if (String.IsNullOrEmpty(dirName))
                        {
                            dirName = Directory.GetCurrentDirectory();
                        }
                        string bakFilename = dirName + "\\" + dbName + ".bak";
                        string username = restoreDatabasePacket.username;
                        string password = restoreDatabasePacket.password;
                        process.StartInfo.Arguments = " -E -S " + m_publisherDBInstance + " -Q \"RESTORE DATABASE " + dbName + " FROM DISK = \'" +
                            bakFilename + "\'\"";
                        string password2 = " -E -S " + m_publisherDBInstance + " -Q \"RESTORE DATABASE " + dbName + " FROM DISK = \'" + bakFilename + "\'\"";
                        if (!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
                        {
                            process.StartInfo.Arguments = " -U " + username + " -P " + password + " -S " + m_publisherDBInstance + " -Q \"RESTORE DATABASE " +
                                dbName + " FROM DISK = \'" + bakFilename + "\'\"";
                        }

                        process.StartInfo.UseShellExecute = false;
                        process.StartInfo.RedirectStandardOutput = true;
                        process.StartInfo.RedirectStandardError = true;
                        m_logWriter.logNormalMessage("start osql");
                        process.Start();
                        string output = process.StandardOutput.ReadToEnd();

                        /*Encoding ibm866 = Encoding.GetEncoding("IBM 866");
                        Encoding unicode = Encoding.Unicode;
                        byte[] outputBytes = ibm866.GetBytes(output);
                        byte[] utf8Bytes = Encoding.Convert(ibm866, unicode, outputBytes);
                        char[] utf8Chars = new char[unicode.GetCharCount(utf8Bytes, 0, utf8Bytes.Length)];
                        unicode.GetChars(utf8Bytes, 0, utf8Bytes.Length, utf8Chars, 0);
                        string utf8BytesString = new string(utf8Chars);*/

                        m_logWriter.logNormalMessage("osql output:" + output);
                        process.WaitForExit();
                        int exitCode = process.ExitCode;
                        restoreDatabasePacket.message = output;
                        if (exitCode == 0)
                        {
                            restoreDatabasePacket.result = RestoreDatabasePacket.RestoreDatabaseResult.AcceptRestoreDatabaseResult;
                        }
                        else
                        {
                            restoreDatabasePacket.result = RestoreDatabasePacket.RestoreDatabaseResult.RejectRestoreDatabaseResult;
                        }
                        using (MemoryStream streamOut = new MemoryStream())
                        {
                            serializer.Serialize(streamOut, restoreDatabasePacket);
                            return streamOut.ToArray();
                        }
                    }
                    else if (type == (int)BaseServer.PacketType.REPLICATION_OBJECT_PACKET_TYPE)
                    {
                        XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.ReplicationObjectPacket));
                        SinventCustomResolveService.ReplicationObjectPacket replicationObjectPacket =
                            (SinventCustomResolveService.ReplicationObjectPacket)serializer.Deserialize(stream);
                        replicationObjectPacket.errorMessage = "";
                        for (int i = 0; i < replicationObjectPacket.dbNames.Count; i++)
                        {
                            string errorMessage = "";
                            replicationObjectPacket.dbNames[i].tables = getReplicationObjectTables(replicationObjectPacket.username,
                                replicationObjectPacket.password, replicationObjectPacket.dbNames[i].dbName, ref errorMessage);
                            if (!String.IsNullOrEmpty(errorMessage))
                            {
                                replicationObjectPacket.errorMessage += errorMessage;
                            }
                            replicationObjectPacket.dbNames[i].storedProcedures = getReplicationObjectStoredProcedures(replicationObjectPacket.username,
                                replicationObjectPacket.password, replicationObjectPacket.dbNames[i].dbName, ref errorMessage);
                            if (!String.IsNullOrEmpty(errorMessage))
                            {
                                replicationObjectPacket.errorMessage += errorMessage;
                            }
                            replicationObjectPacket.dbNames[i].userFunctions = getReplicationObjectUserFunctions(replicationObjectPacket.username,
                                replicationObjectPacket.password, replicationObjectPacket.dbNames[i].dbName, ref errorMessage);
                            if (!String.IsNullOrEmpty(errorMessage))
                            {
                                replicationObjectPacket.errorMessage += errorMessage;
                            }
                        }
                        if (String.IsNullOrEmpty(replicationObjectPacket.errorMessage))
                        {
                            replicationObjectPacket.result = ReplicationObjectPacket.ReplicationObjectResult.AcceptReplicationObjectResult;
                        }
                        else
                        {
                            replicationObjectPacket.result = ReplicationObjectPacket.ReplicationObjectResult.RejectReplicationObjectResult;
                        }
                        using (MemoryStream streamOut = new MemoryStream())
                        {
                            serializer.Serialize(streamOut, replicationObjectPacket);
                            return streamOut.ToArray();
                        }
                    }
                    else if(type == (int)BaseServer.PacketType.LIST_SERVER_PACKET_TYPE)
                    {
                        XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.ListServerPacket));
                        SinventCustomResolveService.ListServerPacket listServerPacket =
                            (SinventCustomResolveService.ListServerPacket)serializer.Deserialize(stream);

                        SqlDataSourceEnumerator instance = SqlDataSourceEnumerator.Instance;
                        DataTable table = instance.GetDataSources();
                        foreach (DataRow row in table.Rows)
                        {
                            string dbServerInstanceName = row["ServerName"].ToString() + "\\" + row["InstanceName"].ToString();
                            if (dbServerInstanceName != m_publisherDBInstance)
                            {
                                listServerPacket.serverNames.Add(dbServerInstanceName);
                            }
                        }
                        listServerPacket.result = ListServerPacket.ListServerResult.AcceptListServerResult;
                        using (MemoryStream streamOut = new MemoryStream())
                        {
                            serializer.Serialize(streamOut, listServerPacket);
                            return streamOut.ToArray();
                        }
                    }
                }
                else if (serverName == ResolverServer.resolverServerString)
                {
                    m_resolverServer.setClient(client);
                    m_clientServer.sendToAll(type, stream.ToArray());
                }
            }
            return null;
        }

        /** Клиент отсоединился.
         * @param client: сокет.
         * @param serverName: имя сервера (для того чтобы отличать ClientServer и ResolverServer).
         */
        void IBusinessLogic.clientDisconnect(Socket client, string serverName)
        {
            if (serverName == ClientServer.clientServerString)
            {
                m_clientServer.clientDisconnect(client);
                if (client != m_clientServer.getMainClient())
                {
                    if (m_mainDBServerInstanceName != m_currentDBServerInstanceName)
                    {
                        XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.ManagePacket));
                        using (MemoryStream streamOutToMainClient = new MemoryStream())
                        {
                            SinventCustomResolveService.ManagePacket managePacket = new SinventCustomResolveService.ManagePacket();
                            managePacket.manageType = SinventCustomResolveService.ManagePacket.ManageType.StopMainClientType;
                            managePacket.mainClient = false;
                            managePacket.ownResolve = false;
                            managePacket.dbServerInstanceName = m_currentDBServerInstanceName;
                            managePacket.dbName = "";
                            managePacket.language = m_currentLanguage;
                            managePacket.errorCode = 0;
                            managePacket.errorMessage = "";
                            managePacket.result = ManagePacket.ManageResult.AcceptManageResult;

                            serializer.Serialize(streamOutToMainClient, managePacket);
                            m_clientServer.sendToMainClient((int)BaseServer.PacketType.MANAGE_PACKET_TYPE, streamOutToMainClient.ToArray());
                            managePacket.manageType = ManagePacket.ManageType.StopManageType;
                        }
                    }
                    m_currentLanguage = "";
                    m_currentDBServerInstanceName = "";
                }
                else
                {
                    m_mainDBServerInstanceName = "";
                }
            }
        }

        private List<string> getListDatabase(string username, string password, ref string errorMessage)
        {
            List<string> result = new List<string>();

            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            m_logWriter.logNormalMessage("start get databse list");
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Integrated Security=SSPI";
                if(!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
                {
                    sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";User Id=" + username + ";Password=" +
                        password;
                }
                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT name FROM SYS.DATABASES WHERE name " +
                    "NOT IN('master', 'model', 'msdb', 'tempdb', 'Resource', 'distribution' , 'reportserver', 'reportservertempdb')";

                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        string dbName = reader.GetString(0);
                        if (!dbName.ToLower().StartsWith("reportserver$"))
                        {
                            result.Add(dbName);
                        }
                    }
                }
                sqlConnection.Close();
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
                m_logWriter.logNormalMessage(exception.InnerException.Message);
                errorMessage += exception.Message + "\r\n" + exception.InnerException.Message;
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
            return result;
        }

        private List<string> getReplicationObjectTables(string username, string password, string dbName, ref string errorMessage)
        {
            List<string> result = new List<string>();

            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            m_logWriter.logNormalMessage("start get replication object tables");
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Integrated Security=SSPI;Database=" + dbName;
                if(!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
                {
                    sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";User Id=" + username + ";Password=" + password + ";Database=" + dbName;
                }
                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT name FROM sys.sysobjects WHERE category != 2 AND xType = 'U' ORDER BY name";

                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        result.Add(reader.GetString(0));
                    }
                }
                sqlConnection.Close();
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
                m_logWriter.logNormalMessage(exception.InnerException.Message);
                errorMessage += exception.Message + "\r\n" + exception.InnerException.Message;
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
            return result;
        }

        private List<string> getReplicationObjectStoredProcedures(string username, string password, string dbName, ref string errorMessage)
        {
            List<string> result = new List<string>();

            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            m_logWriter.logNormalMessage("start get replication object stored procedures");
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Integrated Security=SSPI;Database=" + dbName;
                if (!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
                {
                    sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";User Id=" + username + ";Password=" + password + ";Database=" + dbName;
                }
                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT * FROM sys.sysobjects WHERE category != 2 AND xType = 'P' ORDER BY name";

                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        result.Add(reader.GetString(0));
                    }
                }
                sqlConnection.Close();
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
                m_logWriter.logNormalMessage(exception.InnerException.Message);
                errorMessage += exception.Message + "\r\n" + exception.InnerException.Message;
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
            return result;
        }

        private List<string> getReplicationObjectUserFunctions(string username, string password, string dbName, ref string errorMessage)
        {
            List<string> result = new List<string>();

            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            m_logWriter.logNormalMessage("start get replication object user functions");
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Integrated Security=SSPI;Database=" + dbName;
                if (!string.IsNullOrEmpty(username) && !string.IsNullOrEmpty(password))
                {
                    sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";User Id=" + username + ";Password=" + password + ";Database=" + dbName;
                }
                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT * FROM sys.sysobjects WHERE category != 2 AND xType = 'FN' ORDER BY name";

                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        result.Add(reader.GetString(0));
                    }
                }
                sqlConnection.Close();
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
                m_logWriter.logNormalMessage(exception.InnerException.Message);
                errorMessage += exception.Message + "\r\n" + exception.InnerException.Message;
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
            return result;
        }


        /** Начать работу синхронизации.
         * @param dbName: имя базы данных.
         * @param errorMessage: сообщение с ошибкой.
         */
        private StartJobResultType startJob(string dbName, ref string errorMessage)
        {
            SqlConnection sqlConnection = null;
            SqlCommand sqlCommandMerge = null;
            SqlCommand sqlCommandSnapShot = null;
            string jobName = null;
            StartJobResultType result = StartJobResultType.StartJobDBNotFoundType;
            if (String.IsNullOrEmpty(dbName))
            {
                errorMessage = "dbName is null or empty";
                return StartJobResultType.StartJobExceptionType;
            }
            m_logWriter.logNormalMessage("start merge job for dbName = " + dbName);
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";
                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScriptMerge = "SELECT id, name FROM distribution.dbo.MSmerge_agents WHERE publisher_db=\'" + dbName + "\' AND subscriber_name=\'" +
                    m_currentDBServerInstanceName + "\'";
                string sqlScriptSnapShot = "SELECT id, name FROM distribution.dbo.MSsnapshot_agents WHERE publisher_db=\'" + dbName + "\'";

                sqlCommandMerge = new SqlCommand(sqlScriptMerge, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommandMerge.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        m_currentMergeAgentId = reader.GetInt32(0);
                        jobName = reader.GetString(1);
                        break;
                    }
                }

                sqlCommandSnapShot = new SqlCommand(sqlScriptSnapShot, sqlConnection);
                using (SqlDataReader reader = sqlCommandSnapShot.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        m_currentSnapshotAgentId = reader.GetInt32(0);
                        m_currentSnapshotAgentJobName = reader.GetString(1);
                        break;
                    }
                }
                sqlConnection.Close();
                if (!String.IsNullOrEmpty(jobName))
                {
                    using (SqlConnection execConnection = new SqlConnection(sqlConnectionString))
                    {
                        string sqlScript2 = "EXEC msdb.dbo.sp_start_job @job_name=\'" + jobName + "\'";
                        Server server = new Server(new ServerConnection(execConnection));
                        server.ConnectionContext.ExecuteNonQuery(sqlScript2);
                        result = StartJobResultType.StartJobSuccessType;
                    }
                }
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
                m_logWriter.logNormalMessage(exception.InnerException.Message);
                errorMessage += exception.Message + "\r\n" + exception.InnerException.Message;
                if (stopJob(dbName, ref errorMessage))
                {
                    result = StartJobResultType.StartJobRestartNeededType;
                }
                else
                {
                    result = StartJobResultType.StartJobExceptionType;
                }
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
            return result;
        }

        /** Начать работу snapshot.
         * @param dbName: имя базы данных.
         * @param jobName: имя работы синхронизации.
         * @param errorMessage: сообщение с ошибкой.
         */
        private StartJobResultType startSnapshotJob(string dbName, string jobName, ref string errorMessage)
        {
            m_snapshotNeeded = false;
            SqlConnection sqlConnection = null;
            StartJobResultType result = StartJobResultType.StartJobDBNotFoundType;
            if (String.IsNullOrEmpty(dbName))
            {
                errorMessage = "dbName is null or empty";
                return StartJobResultType.StartJobExceptionType;
            }
            m_logWriter.logNormalMessage("start snapshot job for dbName = " + dbName);
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";
                if (!String.IsNullOrEmpty(jobName))
                {
                    using (SqlConnection execConnection = new SqlConnection(sqlConnectionString))
                    {
                        string sqlScript = "EXEC msdb.dbo.sp_start_job @job_name=\'" + jobName + "\'";
                        Server server = new Server(new ServerConnection(execConnection));
                        server.ConnectionContext.ExecuteNonQuery(sqlScript);
                        result = StartJobResultType.StartJobSuccessType;
                    }
                }
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
                m_logWriter.logNormalMessage(exception.InnerException.Message);
                errorMessage += exception.Message + "\r\n" + exception.InnerException.Message;
                if (stopJob(dbName, ref errorMessage))
                {
                    result = StartJobResultType.StartJobRestartNeededType;
                }
                else
                {
                    result = StartJobResultType.StartJobExceptionType;
                }
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
            return result;
        }

        /** Окончить работу синхронизации.
         * @param dbName: имя базы данных.
         * @param errorMessage: сообщение с ошибкой.
         */
        private bool stopJob(string dbName, ref string errorMessage)
        {
            m_stoppedMergeAgentId = 0;
            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            string jobName = null;
            bool result = false;
            if(String.IsNullOrEmpty(dbName))
            {
                errorMessage = "dbName is null or empty";
                return true;
            }
            m_logWriter.logNormalMessage("stop job for dbName = " + dbName);
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";
                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT id, name FROM distribution.dbo.MSmerge_agents WHERE publisher_db=\'" + dbName + "\' AND subscriber_name=\'" +
                    m_currentDBServerInstanceName + "\'";

                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        m_stoppedMergeAgentId = reader.GetInt32(0);
                        jobName = reader.GetString(1);
                        break;
                    }
                }
                sqlConnection.Close();
                if (!String.IsNullOrEmpty(jobName))
                {
                    using (SqlConnection execConnection = new SqlConnection(sqlConnectionString))
                    {
                        string sqlScript2 = "EXEC msdb.dbo.sp_stop_job @job_name=\'" + jobName + "\'";
                        Server server = new Server(new ServerConnection(execConnection));
                        server.ConnectionContext.ExecuteNonQuery(sqlScript2);
                        result = true;
                    }
                }
            }
            catch (Microsoft.SqlServer.Management.Common.ExecutionFailureException /*exception*/)
            {
                //job finished
                result = true;
                System.Threading.Thread.Sleep(5000);
                
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
                m_logWriter.logNormalMessage(exception.InnerException.Message);
                if (!String.IsNullOrEmpty(errorMessage))
                {
                    errorMessage += "\r\n";
                }
                errorMessage += exception.Message + "\r\n" + exception.InnerException.Message;
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
            if (!result)
            {
                m_stoppedMergeAgentId = 0;
            }
            return result;
        }

        /** Заполнить идентификатор последней сессии сихронизации.
         */
        private void fillLastMergeSession()
        {
            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";

                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT MAX(session_id) FROM distribution.dbo.MSmerge_sessions";

                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        m_lastMergeSessionId = reader.GetInt32(0);
                        break;
                    }
                }
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
        }

        /** Заполнить timestamp последней сессии создания snapshot.
         */
        private void fillLastSnapshotTimestamp()
        {
            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";

                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT MAX(timestamp) FROM distribution.dbo.MSsnapshot_history";
                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        byte[] timestampBytes = new byte[8];
                        reader.GetBytes(0, 0, timestampBytes, 0, 8);
                        m_lastSnapshotTimestamp = "0x" + BitConverter.ToString(timestampBytes).Replace("-", string.Empty);
                        break;
                    }
                }
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
        }

        struct SessionStruct
        {
            public int session_id;  ///< Идентификатор.
            public int runstatus;   ///< Статус.
        } ///< Сессия

        struct SessionHistoryStruct
        {
            public string comments; ///< Сообщение.
            public int error_id;    ///< Код ошибки.
            public string timestamp;///< timestamp.
        }   ///< Журанл сессии

        struct SnapshotHistoryStruct
        {
            public string comments; ///< Сообщение.
            public int runstatus;   ///< Статус.
            public int error_id;    ///< Код ошибки.
            public string timestamp;///< timestamp.
        }   ///< Журанл сессии Snapshot

        /** Запросить последние сообщения в журнале синхронизации.
         * @param sessionId: идентификатор сессии синхронизации.
         */
        private void requestMergeHistory(int sessionId)
        {
            SqlConnection sqlConnection = null;
            SqlCommand sqlCommand = null;
            try
            {
                string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";
                sqlConnection = new SqlConnection(sqlConnectionString);
                string sqlScript = "SELECT comments, error_id, timestamp FROM distribution.dbo.MSmerge_history WHERE session_id = " + sessionId.ToString() +
                    " AND agent_id = " + m_currentMergeAgentId.ToString() + " AND timestamp > " + m_lastMergeTimestamp + " ORDER BY timestamp;";
                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                sqlConnection.Open();
                List<SessionHistoryStruct> sessionHistoryStructList = new List<SessionHistoryStruct>();
                using (SqlDataReader reader = sqlCommand.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        SessionHistoryStruct itemSessionHistory;
                        itemSessionHistory.comments = reader.GetString(0);
                        itemSessionHistory.error_id = reader.GetInt32(1);
                        byte[] timestampBytes = new byte[8];
                        reader.GetBytes(2, 0, timestampBytes, 0, 8);
                        itemSessionHistory.timestamp = "0x" + BitConverter.ToString(timestampBytes).Replace("-", string.Empty);
                        m_lastMergeTimestamp = itemSessionHistory.timestamp;
                        sessionHistoryStructList.Add(itemSessionHistory);
                        m_clientServer.sendMessageToAll(itemSessionHistory.comments, itemSessionHistory.error_id);
                    }
                }
                sqlConnection.Close();

                foreach (SessionHistoryStruct item in sessionHistoryStructList)
                {
                    if (item.error_id > 0)
                    {
                        string sqlScriptForError = "SELECT error_code FROM distribution.dbo.MSrepl_errors WHERE id = " + item.error_id.ToString() + ";";
                        sqlCommand = new SqlCommand(sqlScriptForError, sqlConnection);
                        sqlConnection.Open();
                        using (SqlDataReader reader = sqlCommand.ExecuteReader())
                        {
                            while (reader.Read())
                            {
                                string error_code = reader.GetString(0);
                                //-2147198698 = The snapshot for this publication has become obsolete. The snapshot agent needs to be run again before the subscription can be synchronized.
                                //-2147201021 = The initial snapshot for publication *** is not yet available.Start the Snapshot Agent to generate the snapshot for this publication.
                                if (error_code == "-2147198698" || error_code == "-2147201021")
                                {
                                    m_snapshotNeeded = true;
                                    break;
                                }
                            }
                        }
                        sqlConnection.Close();
                        if (m_snapshotNeeded)
                        {
                            break;
                        }
                    }
                }
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(exception.Message);
            }
            finally
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Close();
                }
            }
        }

        /** Запросить  состояния job.
         * @param state: N/A.
         */
        public void onTimer(object state)
        {
            if (m_currentMergeAgentId > 0)
            {
                SqlConnection sqlConnection = null;
                SqlCommand sqlCommand = null;
                try
                {
                    string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";
                    sqlConnection = new SqlConnection(sqlConnectionString);
                    string sqlScript = "SELECT session_id, runstatus FROM distribution.dbo.MSmerge_sessions WHERE agent_id = " + m_currentMergeAgentId.ToString() +
                        " AND session_id > " + m_lastMergeSessionId + " ORDER BY session_id";

                    sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                    sqlConnection.Open();
                    List<SessionStruct> sessionStructList = new List<SessionStruct>();
                    using (SqlDataReader reader = sqlCommand.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            SessionStruct itemSession;
                            itemSession.session_id = reader.GetInt32(0);
                            itemSession.runstatus = reader.GetInt32(1);
                            sessionStructList.Add(itemSession);
                        }
                    }
                    sqlConnection.Close();

                    int index = 0;
                    foreach (SessionStruct item in sessionStructList)
                    {
                        if (item.runstatus == StartedSessionState || item.runstatus == IdleSessionState || item.runstatus == RetryingSessionState)
                        {

                        }
                        else if (item.runstatus == InProgressSessionState)
                        {
                            requestMergeHistory(item.session_id);
                        }
                        else if (item.runstatus == SucceededSessionState || item.runstatus == FailedSessionState)
                        {
                            if (m_lastMergeSessionId == item.session_id)
                            {
                                requestMergeHistory(item.session_id);
                                m_lastMergeTimestamp = "0x00";
                            }
                            else
                            {
                                m_lastMergeTimestamp = "0x00";
                                m_lastMergeSessionId = item.session_id;
                                requestMergeHistory(item.session_id);
                            }
                            bool error = false;
                            if (item.runstatus != SucceededSessionState)
                            {
                                error = true;
                            }
                            if (index == sessionStructList.Count - 1)
                            {
                                if (m_currentMergeAgentId == m_stoppedMergeAgentId)
                                {
                                    m_stoppedMergeAgentId = 0;
                                }
                                else
                                {
                                    m_clientServer.sendNextStepEndedManageToAll(m_currentDBName, error, m_snapshotNeeded, false);
                                    if (!m_snapshotNeeded)
                                    {
                                        m_currentDBName = "";
                                    }
                                    else
                                    {
                                        string errorMessage = "";
                                        if(startSnapshotJob(m_currentDBName, m_currentSnapshotAgentJobName, ref errorMessage) != StartJobResultType.StartJobSuccessType)
                                        {
                                            error = true;
                                            m_clientServer.sendNextStepEndedManageToAll(m_currentDBName, error, m_snapshotNeeded, false);
                                            m_currentDBName = "";
                                        }
                                    }
                                    m_currentMergeAgentId = 0;
                                }
                            }
                        }
                        index++;
                    }
                }
                catch (Exception exception)
                {
                    m_logWriter.logNormalMessage(exception.Message);
                }
                finally
                {
                    if (sqlConnection != null)
                    {
                        sqlConnection.Close();
                    }
                }
            }

            if (m_currentSnapshotAgentId > 0)
            {
                SqlConnection sqlConnection = null;
                SqlCommand sqlCommand = null;
                try
                {
                    string sqlConnectionString = "Data Source=" + m_publisherDBInstance + ";Initial Catalog=distribution;" + "Integrated Security=SSPI";
                    sqlConnection = new SqlConnection(sqlConnectionString);
                    string sqlScript = "SELECT comments, runstatus, error_id, timestamp FROM distribution.dbo.MSsnapshot_history WHERE agent_id = " + m_currentSnapshotAgentId.ToString() +
                        " AND timestamp > " + m_lastSnapshotTimestamp + " ORDER BY timestamp";

                    sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                    sqlConnection.Open();
                    List<SnapshotHistoryStruct> snapshotHistoryStructList = new List<SnapshotHistoryStruct>();
                    using (SqlDataReader reader = sqlCommand.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            SnapshotHistoryStruct itemSnapshot;
                            itemSnapshot.comments = reader.GetString(0);
                            itemSnapshot.runstatus = reader.GetInt32(1);
                            itemSnapshot.error_id = reader.GetInt32(2);
                            byte[] timestampBytes = new byte[8];
                            reader.GetBytes(3, 0, timestampBytes, 0, 8);
                            itemSnapshot.timestamp = "0x" + BitConverter.ToString(timestampBytes).Replace("-", string.Empty);
                            m_lastSnapshotTimestamp = itemSnapshot.timestamp;
                            snapshotHistoryStructList.Add(itemSnapshot);
                        }
                    }
                    sqlConnection.Close();

                    foreach (SnapshotHistoryStruct item in snapshotHistoryStructList)
                    {
                        m_clientServer.sendMessageToAll(item.comments, item.error_id);

                        if (item.runstatus == StartedSessionState || item.runstatus == IdleSessionState || item.runstatus == RetryingSessionState || item.runstatus == InProgressSessionState)
                        {

                        }
                        else if (item.runstatus == SucceededSessionState || item.runstatus == FailedSessionState)
                        {
                            bool error = false;
                            if (item.runstatus != SucceededSessionState)
                            {
                                error = true;
                            }
                            m_clientServer.sendNextStepEndedManageToAll(m_currentDBName, error, false, true);
                        }
                    }
                }
                catch (Exception exception)
                {
                    m_logWriter.logNormalMessage(exception.Message);
                }
                finally
                {
                    if (sqlConnection != null)
                    {
                        sqlConnection.Close();
                    }
                }
            }
        }
    }
}
