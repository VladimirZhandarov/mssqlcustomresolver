using System;
using System.Data;
using Microsoft.SqlServer.Replication.BusinessLogicSupport;
using System.IO;
using System.Xml.Serialization;
using System.Data.SqlClient;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Runtime.Serialization.Formatters.Binary;

/** Реализует пользовательскую бизнес-логику, вызываемую во время работы процесса синхронизации репликации слиянием. */
public class BaseBusinessLogicHandler : Microsoft.SqlServer.Replication.BusinessLogicSupport.BusinessLogicModule
{
    protected string rowguidString; ///< строка="rowguid"
    protected string bigintString;  ///< строка="bigint"
    protected string intString;     ///< строка="int"
    protected string smallintString;///< строка="smallint"
    protected string tinyintString; ///< строка="tinyint"

    protected string tableName;     ///< Имя таблицы

    private string publisherName;   ///< Имя СУБД публикатора
    private string subscriberName;  ///< Имя СУБД подписчика
    private string publisherDBName; ///< Имя БД публикатора
    private string subscriberDBName;///< Имя БД подписчика

    ///Параметры из xml файла настроек sinventresolver.xml
    private bool autoInsertPublisher;   ///< Производить неконфликтные добавления строк от публикатора без участия оператора (автоматически)
    private bool autoInsertSubscriber;  ///< Производить неконфликтные добавления строк от подписчика без участия оператора (автоматически)
    private bool autoUpdatePublisher;   ///< Производить неконфликтные изменения строк от публикатора без участия оператора (автоматически)
    private bool autoUpdateSubscriber;  ///< Производить неконфликтные изменения строк от подписчика без участия оператора (автоматически)
    private bool autoDeletePublisher;   ///< Производить неконфликтные удаления строк от публикатора без участия оператора (автоматически)
    private bool autoDeleteSubscriber;  ///< Производить неконфликтные удаления строк от подписчика без участия оператора (автоматически)
    private string publisherDBUser;     ///< Имя пользователя для присоединения к СУБД публиктора
    private string publisherDBPassword; ///< Пароль пользователя для присоединения к СУБД публиктора
    private string subscriberDBUser;    ///< Имя пользователя для присоединения к СУБД подписчика
    private string subscriberDBPassword;///< Пароль пользователя для присоединения к СУБД подписчика
    private string logFilePath;         ///< Имя файла журнала
    private Dictionary<string, SinventCustomResolver.DBNameDictonary> dbSetting;
    private static readonly object logFileLocker = new object(); ///< Mutex для добавления записей в файл журнала

    private SinventCustomResolver.Network network;  ///< Класс отвечающий за сетевое взаимодействие

    /** Конструктор.
     */
    public BaseBusinessLogicHandler()
    {
        rowguidString = "rowguid";
        bigintString = "bigint";
        intString = "int";
        smallintString = "smallint";
        tinyintString = "tinyint";

        string xmlFile = "sinventresolver.xml";
        if (File.Exists(xmlFile))
        {
            using (Stream stream = new FileStream(xmlFile, FileMode.Open))
            {
                XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolver.Settings));
                SinventCustomResolver.Settings xmlData = (SinventCustomResolver.Settings)serializer.Deserialize(stream);
                autoInsertPublisher = xmlData.autoInsertPublisher;
                autoInsertSubscriber = xmlData.autoInsertSubscriber;
                autoUpdatePublisher = xmlData.autoUpdatePublisher;
                autoUpdateSubscriber = xmlData.autoUpdateSubscriber;
                autoDeletePublisher = xmlData.autoDeletePublisher;
                autoDeleteSubscriber = xmlData.autoDeleteSubscriber;
                publisherDBUser = xmlData.publisherDBUser;
                publisherDBPassword = xmlData.publisherDBPassword;
                subscriberDBUser = xmlData.subscriberDBUser;
                subscriberDBPassword = xmlData.subscriberDBPassword;
                logFilePath = xmlData.logFilePath;
                List<SinventCustomResolver.DBName> dbNameList = xmlData.dbName;
                dbSetting = new Dictionary<string, SinventCustomResolver.DBNameDictonary>();
                foreach (SinventCustomResolver.DBName dbName in dbNameList)
                {
                    List<SinventCustomResolver.Table> tableList = dbName.table;
                    Dictionary<string, SinventCustomResolver.Table> tableDictionary = new Dictionary<string, SinventCustomResolver.Table>();
                    foreach (SinventCustomResolver.Table table in tableList)
                    {
                        tableDictionary.Add(table.name, table);
                    }
                    SinventCustomResolver.DBNameDictonary dbNameDictonary = new SinventCustomResolver.DBNameDictonary();
                    dbNameDictonary.name = dbName.name;
                    dbNameDictonary.tableDictionary = tableDictionary;
                    dbSetting.Add(dbNameDictonary.name, dbNameDictonary);
                }
                network = new SinventCustomResolver.Network(xmlData.hostName, xmlData.port);
            }
        }
        else
        {
            string hostName = "localhost";
            int port = 55000;
            autoInsertPublisher = true;
            autoInsertSubscriber = true;
            autoUpdatePublisher = true;
            autoUpdateSubscriber = true;
            autoDeletePublisher = true;
            autoDeleteSubscriber = true;
            publisherDBUser = "";
            publisherDBPassword = "";
            subscriberDBUser = "upedb";
            subscriberDBPassword = "upem29";

            logFilePath = "sinventresolverlog.txt";

            SinventCustomResolver.Table admSystemEventAccountTable = new SinventCustomResolver.Table();
            admSystemEventAccountTable.name = "admSystemEventAccount";
            admSystemEventAccountTable.autoInsertError = 1;

            SinventCustomResolver.DBName administrationDB = new SinventCustomResolver.DBName();
            administrationDB.name = "UPE_MM2_Administration2";
            administrationDB.table = new List<SinventCustomResolver.Table>();
            administrationDB.table.Add(admSystemEventAccountTable);

            List<SinventCustomResolver.DBName> dbNameSource = new List<SinventCustomResolver.DBName>();
            dbNameSource.Add(administrationDB);

            SinventCustomResolver.Settings xmlData = new SinventCustomResolver.Settings();
            xmlData.hostName = hostName;
            xmlData.port = port;
            xmlData.autoInsertPublisher = autoInsertPublisher;
            xmlData.autoInsertSubscriber = autoInsertSubscriber;
            xmlData.autoUpdatePublisher = autoUpdatePublisher;
            xmlData.autoUpdateSubscriber = autoUpdateSubscriber;
            xmlData.autoDeletePublisher = autoDeletePublisher;
            xmlData.autoDeleteSubscriber = autoDeleteSubscriber;
            xmlData.publisherDBUser = publisherDBUser;
            xmlData.publisherDBPassword = publisherDBPassword;
            xmlData.subscriberDBUser = subscriberDBUser;
            xmlData.subscriberDBPassword = subscriberDBPassword;
            xmlData.logFilePath = logFilePath;
            xmlData.dbName = dbNameSource;

            List<SinventCustomResolver.DBName> dbNameList = xmlData.dbName;
            dbSetting = new Dictionary<string, SinventCustomResolver.DBNameDictonary>();
            foreach (SinventCustomResolver.DBName dbName in dbNameList)
            {
                List<SinventCustomResolver.Table> tableList = dbName.table;
                Dictionary<string, SinventCustomResolver.Table> tableDictionary = new Dictionary<string, SinventCustomResolver.Table>();
                foreach (SinventCustomResolver.Table table in tableList)
                {
                    tableDictionary.Add(table.name, table);
                }
                SinventCustomResolver.DBNameDictonary dbNameDictonary = new SinventCustomResolver.DBNameDictonary();
                dbNameDictonary.name = dbName.name;
                dbNameDictonary.tableDictionary = tableDictionary;
                dbSetting.Add(dbNameDictonary.name, dbNameDictonary);
            }

            using (Stream writer = new FileStream(xmlFile, FileMode.Create))
            {
                XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolver.Settings));
                serializer.Serialize(writer, xmlData);
            }
            network = new SinventCustomResolver.Network(hostName, port);
        }
    }

    /** Отправляет информацию о синхронизируемой статье сопоставителю бизнес-логики при запуске агента слияния.
     * @param publisher: имя издателя.
     * @param subscriber: имя подписчика.
     * @param distributor: имя распространителя.
     * @param publisherDB: имя базы данных публикации.
     * @param ubscriberDB: имя базы данных подписки.
     * @param articleName: имя опубликованной статьи.
	 */
    public override void Initialize(string publisher, string subscriber, string distributor, string publisherDB, string subscriberDB, string articleName)
    {
        publisherName = publisher;
        subscriberName = subscriber;
        publisherDBName = publisherDB;
        subscriberDBName = subscriberDB;

        if (!testPublisherDBConnect())
        {
            if (!String.IsNullOrEmpty(publisherDBUser))
            {
                addMessage("cannot connect user " + publisherDBUser + " to " + publisherName + " database name is " + publisherDBName + " with table " +
                    tableName);
            }
            else
            {
                addMessage("cannot connect Windows user to " + publisherName + " database name is " + publisherDBName + " with table " + tableName);
            }
            SinventCustomResolver.DBConnectErrorPacket errorPacket = new SinventCustomResolver.DBConnectErrorPacket();
            errorPacket.publisher = true;
            errorPacket.serverName = publisherName;
            errorPacket.dbName = publisherDBName;
            errorPacket.tableName = tableName;
            errorPacket.userName = publisherDBUser;
            string socketErrorMeassage = "";
            Socket socket = network.connectSocket(ref socketErrorMeassage);
            if (socket == null)
            {
                throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
            }
            network.sendPacket(errorPacket);
        }
        else
        {
            addMessage("connected user " + publisherDBUser + " to " + publisherName + " database name is " + publisherDBName + " with table " + tableName);
        }

        if (!testSubscriberDBConnect())
        {
            if (!String.IsNullOrEmpty(subscriberDBUser))
            {
                addMessage("cannot connect user " + subscriberDBUser + " to " + subscriberName + " database name is " + subscriberDBName + " with table " +
                    tableName);
            }
            else
            {
                addMessage("cannot connect Windows user to " + subscriberName + " database name is " + subscriberDBName + " with table " + tableName);
            }

            SinventCustomResolver.DBConnectErrorPacket errorPacket = new SinventCustomResolver.DBConnectErrorPacket();
            errorPacket.publisher = false;
            errorPacket.serverName = subscriberName;
            errorPacket.dbName = subscriberDBName;
            errorPacket.tableName = tableName;
            errorPacket.userName = subscriberDBUser;
            string socketErrorMeassage = "";
            Socket socket = network.connectSocket(ref socketErrorMeassage);
            if (socket == null)
            {
                throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
            }
            network.sendPacket(errorPacket);
        }
        else
        {
            addMessage("connected user " + subscriberDBUser + " to " + subscriberName + " database name is " + subscriberDBName + " with table " + tableName);
        }
    }

    /** Проверить соедиенение с СУБД публикатора.
     */
    private bool testPublisherDBConnect()
    {
        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        bool result = false;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsPublisher));
            string sqlScript = "SELECT TOP (1) * FROM " + tableName;

            sqlCommand = new SqlCommand(sqlScript, sqlConnection);
            sqlConnection.Open();
            using (SqlDataReader reader = sqlCommand.ExecuteReader())
            {
                result = true;
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
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

    /** Проверить соедиенение с СУБД подпичиска. */
    private bool testSubscriberDBConnect()
    {
        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        bool result = false;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsSubscriber));
            string sqlScript = "SELECT TOP (1) * FROM " + tableName;

            sqlCommand = new SqlCommand(sqlScript, sqlConnection);
            sqlConnection.Open();
            using (SqlDataReader reader = sqlCommand.ExecuteReader())
            {
                result = true;
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
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

    /** Обязательное свойство, которое возвращает сведения о типе изменений, обрабатываемых модулем бизнес-логики.
     *  return Microsoft.SqlServer.Replication.BusinessLogicSupport.ChangeStates перечисляет типы изменений, обрабатываемых модулем бизнес-логики.
     */
    public override ChangeStates HandledChangeStates
    {
        get
        {
            return ChangeStates.PublisherInserts | ChangeStates.PublisherUpdates | ChangeStates.PublisherDeletes |
                ChangeStates.SubscriberInserts | ChangeStates.SubscriberUpdates | ChangeStates.SubscriberDeletes |
                ChangeStates.UpdateConflicts | ChangeStates.PublisherUpdateSubscriberDeleteConflicts |
                ChangeStates.SubscriberUpdatePublisherDeleteConflicts |
                ChangeStates.PublisherInsertErrors | ChangeStates.SubscriberInsertErrors |
                ChangeStates.PublisherUpdateErrors | ChangeStates.SubscriberUpdateErrors |
                ChangeStates.PublisherDeleteErrors | ChangeStates.SubscriberDeleteErrors;
        }
    }

    /** Добавить сообщение в журнал.
     * @param message: текст сообщения
     */
    private void addMessage(string message)
    {
        lock (logFileLocker)
        {
            try
            {
                if (!String.IsNullOrEmpty(logFilePath))
                {
                    StreamWriter logStreamWriter;
                    logStreamWriter = File.AppendText(logFilePath);
                    logStreamWriter.WriteLine(DateTime.Now.ToString("[MM\\/dd\\/yyyy h\\:mm tt] ") + "Table:" + tableName + " ; " + message);
                    logStreamWriter.Close();
                }
            }
            catch (Exception exception)
            {
                throw new SinventCustomResolver.ResolveException(exception.Message);
            }
        }
    }

    /** Получить список названий полей для таблицы, которые являются первичными ключами.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, запращивать информацию у издателя или подписчика.
     * @param packet: объект "Пакет", который передается по сети (заполняются возможные следующие значения первичных ключей).
     * @return List<string> список названий полей для таблицы, которые являются первичными ключами. 
     */
    List<string> getPrimaryKeys(SourceIdentifier source, ref SinventCustomResolver.ResolvePacket packet)
    {
        addMessage("getPrimaryKeys start");

        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        List<string> result = new List<string>();
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsPublisher));
            string sqlScript = "SELECT Col.Column_Name from INFORMATION_SCHEMA.TABLE_CONSTRAINTS Tab, INFORMATION_SCHEMA.CONSTRAINT_COLUMN_USAGE Col " +
                "WHERE Col.Constraint_Name = Tab.Constraint_Name AND Col.Table_Name = Tab.Table_Name AND Constraint_Type = 'PRIMARY KEY' AND " +
                "Col.Table_Name = \'" + tableName + "\'";

            sqlCommand = new SqlCommand(sqlScript, sqlConnection);
            sqlConnection.Open();
            using (SqlDataReader reader = sqlCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    result.Add(reader.GetString(0));
                }
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }
        finally
        {
            if (sqlConnection != null)
            {
                sqlConnection.Close();
            }
        }

        List<SinventCustomResolver.PrimaryNextValue> sourcePrimaryNextValues = new List<SinventCustomResolver.PrimaryNextValue>();
        List<SinventCustomResolver.PrimaryNextValue> source2PrimaryNextValues = new List<SinventCustomResolver.PrimaryNextValue>();
        foreach (string id in result)
        {
            if (source == SourceIdentifier.SourceIsPublisher)
            {
                sourcePrimaryNextValues.Add(getPrimaryNextValue(SourceIdentifier.SourceIsPublisher, id));
                source2PrimaryNextValues.Add(getPrimaryNextValue(SourceIdentifier.SourceIsSubscriber, id));
            }
            else 
            {
                sourcePrimaryNextValues.Add(getPrimaryNextValue(SourceIdentifier.SourceIsSubscriber, id));
                source2PrimaryNextValues.Add(getPrimaryNextValue(SourceIdentifier.SourceIsPublisher, id));
            }
        }
        packet.sourcePrimaryNextValues = sourcePrimaryNextValues;
        packet.source2PrimaryNextValues = source2PrimaryNextValues;

        addMessage("getPrimaryKeys end");

        return result;
    }

    /** Получить максимальное значение для первичного ключа.
     * @param columnName: имя поля первичного ключа.
     * @param packet: объект "Пакет", который передается по сети (берутся возможные следующие значения первичных ключей).
     * @return long максимальное значение для первичного ключа.
     */
    long getMaxSourcesPrimaryNextValue(string columnName, SinventCustomResolver.ResolvePacket packet)
    {
        long result = -1;
        foreach (SinventCustomResolver.PrimaryNextValue item in packet.sourcePrimaryNextValues)
        {
            if (item.fieldName == columnName)
            {
                result = item.nextValue;
            }
        }
        long result2 = -1;
        foreach (SinventCustomResolver.PrimaryNextValue item in packet.source2PrimaryNextValues)
        {
            if (item.fieldName == columnName)
            {
                result2 = item.nextValue;
            }
        }
        if (result > result2)
        {
            return result;
        }
        return result2;
    }

    /** Создать строку соединения с СУБД.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, установить соединения надо с издателем или подписчиком.
     * @return string строка соединения с СУБД.
     */
    string createSqlConnectionString(SourceIdentifier source)
    {
        string user = publisherDBUser;
        string password = publisherDBPassword;
        string name = publisherName;
        string dbName = publisherDBName;
        if (source == SourceIdentifier.SourceIsSubscriber)
        {
            user = subscriberDBUser;
            password = subscriberDBPassword;
            name = subscriberName;
            dbName = subscriberDBName;
        }
        if (string.IsNullOrEmpty(user) || string.IsNullOrEmpty(password))
        {
            return "Data Source=" + name + ";Initial Catalog=" + dbName + ";" + "Integrated Security=SSPI";
        }
        else
        {
            return "Data Source=" + name + ";Initial Catalog=" + dbName + ";" + "User Id=" + user + ";Password=" + password;
        }
    }

    /** Получить следующее значение для первичного ключа.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, запращивать информацию у издателя или подписчика.
     * @param columnName: имя поля первичного ключа.
     * @return SinventCustomResolver.PrimaryNextValue следующее значение для первичного ключа (имя + значение).
     */
    SinventCustomResolver.PrimaryNextValue getPrimaryNextValue(SourceIdentifier source, string columnName)
    {
        SinventCustomResolver.PrimaryNextValue result = new SinventCustomResolver.PrimaryNextValue();
        result.fieldName = columnName;
        string intType = getIntTypePrimaryKey(columnName);
        if (String.IsNullOrEmpty(intType))
        {
            return result;
        }
        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(source));
            string sqlScript = "SELECT MAX(" + columnName + ") FROM " + tableName;
            sqlCommand = new SqlCommand(sqlScript, sqlConnection);
            sqlConnection.Open();
            using (SqlDataReader reader = sqlCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    if (intType == bigintString)
                    {
                        result.nextValue = reader.GetInt64(0) + 1;
                    }
                    else if (intType == intString)
                    {
                        result.nextValue = reader.GetInt32(0) + 1;
                    }
                    else if (intType == smallintString)
                    {
                        result.nextValue = reader.GetInt16(0) + 1;
                    }
                    else if (intType == tinyintString)
                    {
                        result.nextValue = reader.GetByte(0) + 1;
                    }
                    break;
                }
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
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

    /** Получить имя типа данных для первичного ключа (только целочисленный).
     * @param columnName: имя поля первичного ключа.
     * @return string имя типа данных для первичного ключа.
     *                Если = "", то не целочисленный первичный ключ.
     */
    string getIntTypePrimaryKey(string columnName)
    {
        if (String.IsNullOrEmpty(columnName))
        {
            return "";
        }
        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        string result = "";
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsPublisher));
            string sqlScript = "SELECT DATA_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = \'" + tableName + "\' AND  COLUMN_NAME = \'" + columnName +
                "\'";

            sqlCommand = new SqlCommand(sqlScript, sqlConnection);
            sqlConnection.Open();
            using (SqlDataReader reader = sqlCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    string value = reader.GetString(0);
                    if (value == bigintString || value == intString || value == smallintString || value == tinyintString)
                    {
                        result = value;
                        break;
                    }
                }
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
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

    /** Получить информацию о строках в других таблицах, куда ссылаются вторичные ключи текущей таблицы.
     * @param dataSet: набор данных.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, запращивать информацию у издателя или подписчика.
     * @return List<SinventCustomResolver.ForeignKeyDataSet> информацию о строках в других таблицах, куда ссылаются вторичные ключи текущей таблицы
     *        (имя поля вторичного ключа + набор данных).
     */
    public List<SinventCustomResolver.ForeignKeyDataSet> getForeignKeysValue(DataSet dataSet, SourceIdentifier source)
    {
        addMessage("getForeignKeysValue start");

        List<SinventCustomResolver.ForeignKeyDataSet> result = new List<SinventCustomResolver.ForeignKeyDataSet>();
        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        SqlCommand sqlCommand2 = null;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(source));
            string sqlScript = "SELECT COL_NAME(fc.parent_object_id, fc.parent_column_id) AS ColumnName, OBJECT_NAME (f.referenced_object_id) AS " +
                "ReferenceTableName, COL_NAME(fc.referenced_object_id, fc.referenced_column_id) AS ReferenceColumnName FROM sys.foreign_keys AS f " +
                "INNER JOIN sys.foreign_key_columns AS fc ON f.OBJECT_ID = fc.constraint_object_id WHERE OBJECT_NAME(f.parent_object_id) = \'" + tableName +
                "\'";

            sqlCommand = new SqlCommand(sqlScript, sqlConnection);
            sqlConnection.Open();
            List<SinventCustomResolver.ForeignKeyRes> foreignKeyResList = new List<SinventCustomResolver.ForeignKeyRes>();
            using (SqlDataReader reader = sqlCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    SinventCustomResolver.ForeignKeyRes foreignKeyRes = new SinventCustomResolver.ForeignKeyRes();
                    foreignKeyRes.fieldNameData = reader.GetString(0);
                    foreignKeyRes.tableNameData = reader.GetString(1);
                    foreignKeyRes.idNameData = reader.GetString(2);
                    foreignKeyResList.Add(foreignKeyRes);
                }
            }
            foreach (SinventCustomResolver.ForeignKeyRes foreignKeyRes in foreignKeyResList)
            {
                string idValue = dataSet.Tables[0].Rows[0][foreignKeyRes.fieldNameData].ToString();
                if (!System.String.IsNullOrEmpty(idValue))
                {
                    string sqlScript2 = "SELECT * FROM " + foreignKeyRes.tableNameData + " WHERE " + foreignKeyRes.idNameData + " = " + idValue;
                    sqlCommand2 = new SqlCommand(sqlScript2, sqlConnection);
                    DataSet refDataSet = new DataSet();
                    SqlDataAdapter dataAdapter = new SqlDataAdapter(sqlCommand2);
                    dataAdapter.Fill(refDataSet);
                    refDataSet.Tables[0].TableName = foreignKeyRes.tableNameData;
                    SinventCustomResolver.ForeignKeyDataSet foreignKeyDataSet = new SinventCustomResolver.ForeignKeyDataSet();
                    foreignKeyDataSet.fieldName = foreignKeyRes.fieldNameData;

                    foreach (DataColumn col in refDataSet.Tables[0].Columns)
                    {
                        if (col.DataType.Name == "Byte[]")
                        {
                            SinventCustomResolver.BinaryDataDescription binaryDataDescription = new SinventCustomResolver.BinaryDataDescription();
                            binaryDataDescription.fieldName = col.ColumnName;
                            if (refDataSet.Tables[0].Rows[0].IsNull(col.ColumnName))
                            {
                                binaryDataDescription.nullData = true;
                                binaryDataDescription.dataSize = 0;
                            }
                            else
                            {
                                binaryDataDescription.nullData = false;
                                binaryDataDescription.dataSize = objectToByteArray(refDataSet.Tables[0].Rows[0][col.ColumnName]).Length;
                            }
                            refDataSet.Tables[0].Rows[0][col.ColumnName] = null;
                            foreignKeyDataSet.binaryDataDescriptions.Add(binaryDataDescription);
                        }
                    }
                    foreignKeyDataSet.dataSet = refDataSet;
                    result.Add(foreignKeyDataSet);
                }
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }
        finally
        {
            if (sqlConnection != null)
            {
                sqlConnection.Close();
            }
        }

        addMessage("getForeignKeysValue end");

        return result;
    }

    /** Заполнить данные в "Пакете" о вторичных ключах.
     * @param packet: объект "Пакет", который передается по сети.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, запращивать информацию у издателя или подписчика.
     */
    void updateForeignKeys(ref SinventCustomResolver.ResolvePacket packet, SourceIdentifier source)
    {
        packet.sourceDataSetForeignKeys = getForeignKeysValue(packet.sourceDataSet, source);
        SourceIdentifier otherSource = SourceIdentifier.SourceIsPublisher;
        if (source == SourceIdentifier.SourceIsPublisher)
        {
            otherSource = SourceIdentifier.SourceIsSubscriber;
        }
        if (packet.source2DataSet != null)
        {
            packet.source2DataSetForeignKeys = getForeignKeysValue(packet.source2DataSet, otherSource);
        }
    }

    /** Преобразовать объект в массив.
     * @param obj объект
     * @return byte[] массив
     */
    byte[] objectToByteArray(object obj)
    {
        if (obj == null)
        {
            return null;
        }
        BinaryFormatter bf = new BinaryFormatter();
        using (MemoryStream ms = new MemoryStream())
        {
            bf.Serialize(ms, obj);
            return ms.ToArray();
        }
    }

    /** Удалить бинарные данные в "Пакете" оставить только метаданные (длинна, NULL, совпадение).
     * @param packet: объект "Пакет", который передается по сети.
     * @return List<string>: список имен полей, которые были изменены.
     */
    List<string> deleteNUpdateBinaryData(ref SinventCustomResolver.ResolvePacket packet)
    {
        List<string> result = new List<string>();
        Dictionary<string, bool> equalMap = new Dictionary<string, bool>();
        packet.sourceBinaryDataDescriptions = new List<SinventCustomResolver.BinaryDataDescription>();
        foreach (DataColumn col in packet.sourceDataSet.Tables[0].Columns)
        {
            if (col.DataType.Name == "Byte[]")
            {
                SinventCustomResolver.BinaryDataDescription binaryDataDescription = new SinventCustomResolver.BinaryDataDescription();
                binaryDataDescription.fieldName = col.ColumnName;
                result.Add(col.ColumnName);
                if (packet.source2DataSet != null)
                {
                    if (packet.sourceDataSet.Tables[0].Rows[0].IsNull(col.ColumnName) && packet.source2DataSet.Tables[0].Rows[0].IsNull(col.ColumnName))
                    {
                        binaryDataDescription.equalData = true;
                    }
                    else
                    {
                        binaryDataDescription.equalData = true;
                        byte[] b1 = objectToByteArray(packet.sourceDataSet.Tables[0].Rows[0][col.ColumnName]);
                        byte[] b2 = objectToByteArray(packet.source2DataSet.Tables[0].Rows[0][col.ColumnName]);
                        if (b1.Length != b2.Length)
                        {
                            binaryDataDescription.equalData = false;
                        }
                        else
                        {
                            for (int i = 0; i < b1.Length; i++)
                            {
                                if (b1[i] != b2[i])
                                {
                                    binaryDataDescription.equalData = false;
                                }
                            }
                        }
                    }
                }
                else
                {
                    binaryDataDescription.equalData = true;
                }
                equalMap.Add(binaryDataDescription.fieldName, binaryDataDescription.equalData);
                if (packet.sourceDataSet.Tables[0].Rows[0].IsNull(col.ColumnName))
                {
                    binaryDataDescription.nullData = true;
                    binaryDataDescription.dataSize = 0;
                }
                else
                {
                    binaryDataDescription.nullData = false;
                    binaryDataDescription.dataSize = objectToByteArray(packet.sourceDataSet.Tables[0].Rows[0][col.ColumnName]).Length - 28;
                }
                packet.sourceBinaryDataDescriptions.Add(binaryDataDescription);
            }
        }
        if (packet.source2DataSet != null)
        {
            packet.source2BinaryDataDescriptions = new List<SinventCustomResolver.BinaryDataDescription>();
            List<SinventCustomResolver.BinaryDataDescription> source2BinaryDataDescriptions = new List<SinventCustomResolver.BinaryDataDescription>();
            foreach (DataColumn col in packet.source2DataSet.Tables[0].Columns)
            {
                if (col.DataType.Name == "Byte[]")
                {
                    SinventCustomResolver.BinaryDataDescription binaryDataDescription = new SinventCustomResolver.BinaryDataDescription();
                    binaryDataDescription.fieldName = col.ColumnName;
                    if (equalMap.ContainsKey(binaryDataDescription.fieldName))
                    {
                        binaryDataDescription.equalData = equalMap[binaryDataDescription.fieldName];
                    }
                    else
                    {
                        binaryDataDescription.equalData = false;
                    }
                    if (packet.source2DataSet.Tables[0].Rows[0].IsNull(col.ColumnName))
                    {
                        binaryDataDescription.nullData = true;
                        binaryDataDescription.dataSize = 0;
                    }
                    else
                    {
                        binaryDataDescription.nullData = false;
                        binaryDataDescription.dataSize = objectToByteArray(packet.source2DataSet.Tables[0].Rows[0][col.ColumnName]).Length - 28;
                    }
                    packet.source2DataSet.Tables[0].Rows[0][col.ColumnName] = null;
                    packet.source2BinaryDataDescriptions.Add(binaryDataDescription);
                }
            }
        }
        foreach (string columnName in result)
        {
            packet.sourceDataSet.Tables[0].Columns.Remove(columnName);
            packet.sourceDataSet.Tables[0].Columns.Add(columnName);
            packet.source2DataSet.Tables[0].Columns.Remove(columnName);
            packet.source2DataSet.Tables[0].Columns.Add(columnName);
        }
        return result;
    }

    /** Получить метаданные о вторичных ключах текущей таблицы.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, запращивать информацию у издателя или подписчика.
     * @return List<SinventCustomResolver.ForeignData> метаданные о вторичных ключах текущей таблицы.
     */
    private List<SinventCustomResolver.ForeignData> getForeignKeysData(SourceIdentifier source)
    {
        addMessage("getForeignKeysData start");

        List<SinventCustomResolver.ForeignData> result = new List<SinventCustomResolver.ForeignData>();
        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(source));
            string sqlScript = "SELECT COL_NAME(fc.parent_object_id, fc.parent_column_id) AS ColumnName, OBJECT_NAME (f.parent_object_id) AS " +
                "ParentTableName, COL_NAME(fc.referenced_object_id, fc.referenced_column_id) AS ReferenceColumnName, update_referential_action_desc, " +
                "delete_referential_action_desc FROM sys.foreign_keys AS f INNER JOIN sys.foreign_key_columns AS fc ON f.OBJECT_ID = " +
                "fc.constraint_object_id WHERE OBJECT_NAME(f.referenced_object_id) = \'" + tableName + "\'";

            sqlCommand = new SqlCommand(sqlScript, sqlConnection);
            sqlConnection.Open();
            List<SinventCustomResolver.ForeignKeyRes> foreignKeyResList = new List<SinventCustomResolver.ForeignKeyRes>();
            using (SqlDataReader reader = sqlCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    SinventCustomResolver.ForeignData foreignData = new SinventCustomResolver.ForeignData();
                    foreignData.columnName = reader.GetString(0);
                    foreignData.tableName = reader.GetString(1);
                    foreignData.referenceColumnName = reader.GetString(2);
                    foreignData.updateAction = reader.GetString(3);
                    foreignData.deleteAction = reader.GetString(4);
                    result.Add(foreignData);
                }
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }
        finally
        {
            if (sqlConnection != null)
            {
                sqlConnection.Close();
            }
        }

        addMessage("getForeignKeysData end");

        return result;
    }

    /** Запросить набор данных по первичным ключам (используется при конфликте добавления данных, чтобы узнать с чем конфликтуют новые данные).
     * @param sqlConnection: открытое соединение для запроса данных.
     * @param sqlTransaction: открытая транзакция для запроса данных
     * @param primaryKeys: список имен полей первичных ключей.
     * @param dataSet: набор данных (берутся значения для первичных ключей из неё).
     * @return DataSet набор данных, отвечающий значениям первичных ключей.
     */
    DataSet getData(SqlConnection sqlConnection, SqlTransaction sqlTransaction, List<string> primaryKeys, DataSet dataSet)
    {
        addMessage("getData start");

        DataSet result = new DataSet();
        if (primaryKeys.Count == 0)
        {
            return result;
        }
        SqlCommand sqlCommand = null;
        try
        {
            string sqlScriptWhere = "";
            bool firstId = true;
            foreach (string id in primaryKeys)
            {
                string idValue = dataSet.Tables[0].Rows[0][id].ToString();
                if (!System.String.IsNullOrEmpty(idValue))
                {
                    if (firstId)
                    {
                        firstId = false;
                    }
                    else
                    {
                        sqlScriptWhere += " AND ";
                    }
                    if (!String.IsNullOrEmpty(getIntTypePrimaryKey(id)))
                    {
                        sqlScriptWhere += id + " = " + idValue;
                    }
                    else
                    {
                        sqlScriptWhere += id + " = \'" + idValue + "\'";
                    }
                }
            }
            if (!String.IsNullOrEmpty(sqlScriptWhere))
            {
                string sqlScript = "SELECT * FROM " + tableName + " WHERE " + sqlScriptWhere;
                sqlCommand = new SqlCommand(sqlScript, sqlConnection, sqlTransaction);
                SqlDataAdapter dataAdapter = new SqlDataAdapter(sqlCommand);
                dataAdapter.Fill(result);
                result.Tables[0].TableName = tableName;
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }

        addMessage("getData end");

        return result;
    }

    /** Запросить набор данных по первичным ключам (используется при конфликте добавления данных, чтобы узнать с чем конфликтуют новые данные).
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, запращивать информацию у издателя или подписчика.
     * @param primaryKeys: список имен полей первичных ключей.
     * @param dataSet: набор данных (берутся значения для первичных ключей из неё).
     * @return DataSet набор данных, отвечающий значениям первичных ключей.
     */
    DataSet getData(SourceIdentifier source, List<string> primaryKeys, DataSet dataSet)
    {
        addMessage("getData start");

        DataSet result = new DataSet();
        if (primaryKeys.Count == 0)
        {
            return result;
        }
        SqlConnection sqlConnection = null;
        SqlCommand sqlCommand = null;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(source));
            string sqlScriptWhere = "";
            bool firstId = true;
            foreach (string id in primaryKeys)
            {
                string idValue = dataSet.Tables[0].Rows[0][id].ToString();
                if (!System.String.IsNullOrEmpty(idValue))
                {
                    if (firstId)
                    {
                        firstId = false;
                    }
                    else
                    {
                        sqlScriptWhere += " AND ";
                    }
                    if (!String.IsNullOrEmpty(getIntTypePrimaryKey(id)))
                    {
                        sqlScriptWhere += id + " = " + idValue;
                    }
                    else
                    {
                        sqlScriptWhere += id + " = \'" + idValue + "\'";
                    }
                }
            }
            if (!String.IsNullOrEmpty(sqlScriptWhere))
            {
                string sqlScript = "SELECT * FROM " + tableName + " WHERE " + sqlScriptWhere;
                sqlCommand = new SqlCommand(sqlScript, sqlConnection);
                SqlDataAdapter dataAdapter = new SqlDataAdapter(sqlCommand);
                dataAdapter.Fill(result);
                result.Tables[0].TableName = tableName;
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }
        finally
        {
            if (sqlConnection != null)
            {
                sqlConnection.Close();
            }
        }

        addMessage("getData end");

        return result;
    }

    /** Удалить записи по первичным ключам.
     * @param sqlTransaction: открытая транзакция для запроса данных
     * @param primaryKeys: список имен полей первичных ключей.
     * @param primaryKeys: список имен полей первичных ключей.
     * @param binaryCols: список имен полей с бинарными данными.
     * @param keyValues: значения полей первичных ключей.
     * @param sqlTransaction: транзакция используемая для СУБД
     * @return bool =TRUE - данные удалены.
 */
    bool deleteData(SqlConnection sqlConnection, SqlTransaction sqlTransaction, List<string> primaryKeys, List<string> binaryCols, List<string> keyValues)
    {
        bool result = true;
        if (primaryKeys.Count == 0)
        {
            return false;
        }

        addMessage("deleteData start");

        SqlCommand sqlCommand = null;
        try
        {
            string sqlScriptWhere = "";
            bool firstId = true;
            int index = 0;
            foreach (string id in primaryKeys)
            {
                string idValue = keyValues[index];
                if (!System.String.IsNullOrEmpty(idValue))
                {
                    if (firstId)
                    {
                        firstId = false;
                    }
                    else
                    {
                        sqlScriptWhere += " AND ";
                    }
                    if (!String.IsNullOrEmpty(getIntTypePrimaryKey(id)))
                    {
                        sqlScriptWhere += id + " = " + idValue;
                    }
                    else
                    {
                        sqlScriptWhere += id + " = \'" + idValue + "\'";
                    }
                }
                index++;
            }
            if (!String.IsNullOrEmpty(sqlScriptWhere))
            {
                string sqlScript = "DELETE FROM " + tableName + " WHERE " + sqlScriptWhere;
                sqlCommand = new SqlCommand(sqlScript, sqlConnection, sqlTransaction);
                if (sqlCommand.ExecuteNonQuery() == 0)
                {
                    result = false;
                }
                addMessage("delete " + sqlScript + "\n with result = " + result);
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }

        addMessage("deleteData end");

        return result;
    }

    /** Запросить набор данных по первичным ключам (используется при конфликте добавления данных, чтобы узнать с чем конфликтуют новые данные).
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, новые данные пришли от издателя или подписчика.
     * @param primaryKeys: список имен полей первичных ключей.
     * @param dataSet: набор данных.
     * @return DataSet набор данных, отвечающий значениям первичных ключей.
     */
    DataSet getDataForInsertError(SourceIdentifier source, List<string> primaryKeys, DataSet dataSet)
    {
        if (source == SourceIdentifier.SourceIsSubscriber)
        {
            return getData(SourceIdentifier.SourceIsPublisher, primaryKeys, dataSet);
        }
        else
        {
            return getData(SourceIdentifier.SourceIsSubscriber, primaryKeys, dataSet);
        }
    }

    /** Разрешение конфликта при конфликтном добавлении данных (Оставляем оба варианта - один перемещаем).
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, новые данные пришли от издателя или подписчика.
     * @param primaryKeys: список имен полей первичных ключей.
     * @param binaryCols: список имен полей с бинарными данными.
     * @param dataSet: набор данных.
     * @param oldIdList: список старых значений для первичных ключей.
     * @return bool =TRUE - данные изменены.
     */
    bool setDataForInsertError(SourceIdentifier source, List<string> primaryKeys, List<string> binaryCols, DataSet dataSet, List<string> oldIdList)
    {
        bool result = true;
        SqlConnection sqlConnection = null;

        string idStrings = "";
        foreach (string id in primaryKeys)
        {
            idStrings += dataSet.Tables[0].Rows[0][id].ToString() + " ";
        }
        List<SinventCustomResolver.ForeignData> foreignDataList = getForeignKeysData(source);
        DataSet insertedData = null;
        SqlTransaction sqlTransaction = null;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(source));
            sqlConnection.Open();
            long milliseconds = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            sqlTransaction = sqlConnection.BeginTransaction("InsTrans" + milliseconds.ToString());
            if (oldIdList.Count > 0)
            {
                if (source == SourceIdentifier.SourceIsPublisher)
                {
                    addMessage("insert (fix update) to publisher ids = " + idStrings);
                }
                else
                {
                    addMessage("insert (fix update) to subscriber ids = " + idStrings);
                }
                using (SqlBulkCopy bulkCopy = new SqlBulkCopy(sqlConnection, SqlBulkCopyOptions.Default, sqlTransaction))
                {
                    bulkCopy.DestinationTableName = tableName;
                    DataTable dataTable = dataSet.Tables[0];
                    dataTable.Columns.Remove(rowguidString);
                    bulkCopy.WriteToServer(dataTable);
                }
                insertedData = getData(sqlConnection, sqlTransaction, primaryKeys, dataSet);
                foreach (SinventCustomResolver.ForeignData foreignData in foreignDataList)
                {
                    int idIndex = primaryKeys.IndexOf(foreignData.referenceColumnName);
                    string oldIdValue = "";
                    if (idIndex >= 0 && idIndex < oldIdList.Count)
                    {
                        oldIdValue = oldIdList[idIndex];
                    }
                    string updateFkString = "UPDATE " + foreignData.tableName;
                    string updateFkSetString = " SET " + foreignData.columnName + " = ";
                    string updateFkWhereString = " WHERE " + foreignData.columnName + " = ";
                    string idValue = dataSet.Tables[0].Rows[0][foreignData.referenceColumnName].ToString();
                    if (String.IsNullOrEmpty(idValue) || String.IsNullOrEmpty(oldIdValue))
                    {
                        addMessage("no update " + foreignData.tableName + "(idValue = " + idValue + ", oldIdValue = " + oldIdValue + ")");
                        continue;
                    }
                    if (!String.IsNullOrEmpty(getIntTypePrimaryKey(foreignData.referenceColumnName)))
                    {
                        updateFkSetString += idValue;
                        updateFkWhereString += oldIdValue;
                    }
                    else
                    {
                        if (!binaryCols.Contains(foreignData.referenceColumnName))
                        {
                            updateFkSetString += "\'" + idValue + "\'";
                            updateFkWhereString += "\'" + oldIdValue + "\'";
                        }
                        else
                        {
                            updateFkSetString += idValue;
                            updateFkWhereString += oldIdValue;
                        }
                    }
                    SqlCommand updateCommand = new SqlCommand(updateFkString + updateFkSetString + updateFkWhereString + "; ", sqlConnection, sqlTransaction);
                    if (updateCommand.ExecuteNonQuery() != 1)
                    {
                        result = false;
                    }
                    addMessage("update fk " + updateFkString + updateFkSetString + updateFkWhereString + "\n with result = " + result);
                }
                if (!deleteData(sqlConnection, sqlTransaction, primaryKeys, binaryCols, oldIdList))
                {
                    result = false;
                }
            }
            sqlTransaction.Commit();
        }
        catch (Exception exception)
        {
            result = false;
            addMessage(exception.Message);
            if (sqlTransaction != null)
            {
                try
                {
                    sqlTransaction.Rollback();
                }
                catch (Exception exception2)
                {
                    addMessage(exception2.Message);
                }
            }
        }
        finally
        {
            if (sqlConnection != null)
            {
                sqlConnection.Close();
            }
        }

        DataTable dataForInsert = dataSet.Tables[0];
        if (insertedData != null)
        {
            dataForInsert = insertedData.Tables[0];
        }
        try
        {
            if (source == SourceIdentifier.SourceIsSubscriber)
            {
                addMessage("insert to publisher ids = " + idStrings);
                sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsPublisher));
            }
            else
            {
                addMessage("insert to subscriber ids = " + idStrings);
                sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsSubscriber));
            }
            sqlConnection.Open();
            using (SqlBulkCopy bulkCopy = new SqlBulkCopy(sqlConnection))
            {
                bulkCopy.DestinationTableName = tableName;
                bulkCopy.WriteToServer(dataForInsert);
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.ToString());
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

    /** Разрешение конфликта при конфликтном добавлении данных.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, новые данные пришли от издателя или подписчика.
     * @param primaryKeys: список имен полей первичных ключей.
     * @param binaryCols: список имен полей с бинарными данными.
     * @param dataSet: набор данных (для удаления).
     * @param dataSet2: набор данных (используем только его).
     * @param oldIdList: список старых значений для первичных ключей.
     * @return bool =TRUE - данные изменены.
     */
    bool setDataForInsertErrorOnlyOne(SourceIdentifier source, List<string> primaryKeys, List<string> binaryCols, DataSet dataSet, DataSet dataSet2, List<string> oldIdList)
    {
        bool result = true;
        SqlConnection sqlConnection = null;

        string idStrings = "";
        List<string> tempIdList = new List<string>();
        foreach (string id in primaryKeys)
        {
            idStrings += dataSet.Tables[0].Rows[0][id].ToString() + " ";
            tempIdList.Add(dataSet.Tables[0].Rows[0][id].ToString());
        }
        List<SinventCustomResolver.ForeignData> foreignDataList = getForeignKeysData(source);
        DataSet insertedData = null;
        DataSet insertedData2 = null;
        SqlTransaction sqlTransaction = null;
        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(source));
            sqlConnection.Open();
            long milliseconds = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            sqlTransaction = sqlConnection.BeginTransaction("InsTrans" + milliseconds.ToString());
            if (oldIdList.Count > 0)
            {
                if (source == SourceIdentifier.SourceIsPublisher)
                {
                    addMessage("insert only one (fix update) to publisher ids = " + idStrings);
                }
                else
                {
                    addMessage("insert only one (fix update) to subscriber ids = " + idStrings);
                }
                using (SqlBulkCopy bulkCopy = new SqlBulkCopy(sqlConnection, SqlBulkCopyOptions.Default, sqlTransaction))
                {
                    bulkCopy.DestinationTableName = tableName;
                    DataTable dataTable = dataSet.Tables[0];
                    dataTable.Columns.Remove(rowguidString);
                    bulkCopy.WriteToServer(dataTable);
                }
                insertedData = getData(sqlConnection, sqlTransaction, primaryKeys, dataSet);
                foreach (SinventCustomResolver.ForeignData foreignData in foreignDataList)
                {
                    int idIndex = primaryKeys.IndexOf(foreignData.referenceColumnName);
                    string oldIdValue = "";
                    if (idIndex >= 0 && idIndex < oldIdList.Count)
                    {
                        oldIdValue = oldIdList[idIndex];
                    }
                    string updateFkString = "UPDATE " + foreignData.tableName;
                    string updateFkSetString = " SET " + foreignData.columnName + " = ";
                    string updateFkWhereString = " WHERE " + foreignData.columnName + " = ";
                    string idValue = dataSet.Tables[0].Rows[0][foreignData.referenceColumnName].ToString();
                    if (String.IsNullOrEmpty(idValue) || String.IsNullOrEmpty(oldIdValue))
                    {
                        addMessage("no update " + foreignData.tableName + "(idValue = " + idValue + ", oldIdValue = " + oldIdValue + ")");
                        continue;
                    }
                    if (!String.IsNullOrEmpty(getIntTypePrimaryKey(foreignData.referenceColumnName)))
                    {
                        updateFkSetString += idValue;
                        updateFkWhereString += oldIdValue;
                    }
                    else
                    {
                        if (!binaryCols.Contains(foreignData.referenceColumnName))
                        {
                            updateFkSetString += "\'" + idValue + "\'";
                            updateFkWhereString += "\'" + oldIdValue + "\'";
                        }
                        else
                        {
                            updateFkSetString += idValue;
                            updateFkWhereString += oldIdValue;
                        }
                    }
                    SqlCommand updateCommand = new SqlCommand(updateFkString + updateFkSetString + updateFkWhereString + "; ", sqlConnection, sqlTransaction);
                    if (updateCommand.ExecuteNonQuery() != 1)
                    {
                        result = false;
                    }
                    addMessage("update fk " + updateFkString + updateFkSetString + updateFkWhereString + "\n with result = " + result);
                }
                if (!deleteData(sqlConnection, sqlTransaction, primaryKeys, binaryCols, oldIdList))
                {
                    result = false;
                }
                using (SqlBulkCopy bulkCopy2 = new SqlBulkCopy(sqlConnection, SqlBulkCopyOptions.Default, sqlTransaction))
                {
                    bulkCopy2.DestinationTableName = tableName;
                    DataTable dataTable = dataSet2.Tables[0];
                    bulkCopy2.WriteToServer(dataTable);
                }
                insertedData2 = getData(sqlConnection, sqlTransaction, primaryKeys, dataSet2);
                foreach (SinventCustomResolver.ForeignData foreignData in foreignDataList)
                {
                    int idIndex = primaryKeys.IndexOf(foreignData.referenceColumnName);
                    string tempIdValue = "";
                    if (idIndex >= 0 && idIndex < tempIdList.Count)
                    {
                        tempIdValue = tempIdList[idIndex];
                    }
                    string updateFkString = "UPDATE " + foreignData.tableName;
                    string updateFkSetString = " SET " + foreignData.columnName + " = ";
                    string updateFkWhereString = " WHERE " + foreignData.columnName + " = ";
                    string idValue = dataSet2.Tables[0].Rows[0][foreignData.referenceColumnName].ToString();
                    if (String.IsNullOrEmpty(idValue) || String.IsNullOrEmpty(tempIdValue))
                    {
                        addMessage("no update " + foreignData.tableName + "(idValue = " + idValue + ", tempIdValue = " + tempIdValue + ")");
                        continue;
                    }
                    if (!String.IsNullOrEmpty(getIntTypePrimaryKey(foreignData.referenceColumnName)))
                    {
                        updateFkSetString += idValue;
                        updateFkWhereString += tempIdValue;
                    }
                    else
                    {
                        if (!binaryCols.Contains(foreignData.referenceColumnName))
                        {
                            updateFkSetString += "\'" + idValue + "\'";
                            updateFkWhereString += "\'" + tempIdValue + "\'";
                        }
                        else
                        {
                            updateFkSetString += idValue;
                            updateFkWhereString += tempIdValue;
                        }
                    }
                    SqlCommand updateCommand = new SqlCommand(updateFkString + updateFkSetString + updateFkWhereString + "; ", sqlConnection, sqlTransaction);
                    if (updateCommand.ExecuteNonQuery() != 1)
                    {
                        result = false;
                    }
                    addMessage("update fk " + updateFkString + updateFkSetString + updateFkWhereString + "\n with result = " + result);
                }
                if (!deleteData(sqlConnection, sqlTransaction, primaryKeys, binaryCols, tempIdList))
                {
                    result = false;
                }
            }
            sqlTransaction.Commit();
        }
        catch (Exception exception)
        {
            result = false;
            addMessage(exception.Message);
            if (sqlTransaction != null)
            {
                try
                {
                    sqlTransaction.Rollback();
                }
                catch (Exception exception2)
                {
                    addMessage(exception2.Message);
                }
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

    /** Разрешение конфликта при конфликтном изменении данных.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, новые данные пришли от издателя или подписчика.
     * @param primaryKeys: список имен полей первичных ключей.
     * @param binaryCols: список имен полей с бинарными данными.
     * @param dataSet: набор данных.
     * @return bool =TRUE - данные изменены.
     */
    bool setDataForUpdateError(SourceIdentifier source, List<string> primaryKeys, List<string> binaryCols, DataSet dataSet)
    {
        bool result = false;
        SqlConnection sqlConnection = null;

        string idStrings = "";
        foreach (string id in primaryKeys)
        {
            idStrings += dataSet.Tables[0].Rows[0][id].ToString() + " ";
        }
        try
        {
            if (source == SourceIdentifier.SourceIsSubscriber)
            {
                addMessage("insert to publisher ids = " + idStrings);
                sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsPublisher));
            }
            else
            {
                addMessage("insert to subscriber ids = " + idStrings);
                sqlConnection = new SqlConnection(createSqlConnectionString(SourceIdentifier.SourceIsSubscriber));
            }
            sqlConnection.Open();
            using (SqlBulkCopy bulkCopy = new SqlBulkCopy(sqlConnection))
            {
                bulkCopy.DestinationTableName = tableName;
                bulkCopy.WriteToServer(dataSet.Tables[0]);
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }
        finally
        {
            if (sqlConnection != null)
            {
                sqlConnection.Close();
            }
        }

        try
        {
            sqlConnection = new SqlConnection(createSqlConnectionString(source));
            if (source == SourceIdentifier.SourceIsPublisher)
            {
                addMessage("update to publisher ids = " + idStrings);
            }
            else
            {
                addMessage("update to subscriber ids = " + idStrings);
            }
            sqlConnection.Open();
            bool firstId = true;
            string updateString = "UPDATE " + dataSet.Tables[0].TableName + " SET ";
            foreach (string id in primaryKeys)
            {
                string idValue = dataSet.Tables[0].Rows[0][id].ToString();
                if (!System.String.IsNullOrEmpty(idValue))
                {
                    if (firstId)
                    {
                        firstId = false;
                    }
                    else
                    {
                        updateString += ", ";
                    }
                    if (!String.IsNullOrEmpty(getIntTypePrimaryKey(id)))
                    {
                        updateString += id + " = " + idValue;
                    }
                    else
                    {
                        updateString += id + " = \'" + idValue + "\'";
                    }
                }
            }
            updateString = updateString + " WHERE " + rowguidString + "=\'" + dataSet.Tables[0].Rows[0][rowguidString].ToString() + "\';";
            SqlCommand updateCommand = new SqlCommand(updateString, sqlConnection);
            if (updateCommand.ExecuteNonQuery() == 1)
            {
                result = true;
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
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

    /** Создать пакет для пкередачи по сети.
     * @param resolveType: тип конфликта.
     * @param source: Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает, новые данные пришли от издателя или подписчика.
     * @param binaryCols: список имен полей с бинарными данными.
     * @param dataSet: набор данных.
     * @return SinventCustomResolver.ResolvePacket пакет для пкередачи по сети.
     */
    SinventCustomResolver.ResolvePacket createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType resolveType, SourceIdentifier source)
    {
        SinventCustomResolver.ResolvePacket packet = new SinventCustomResolver.ResolvePacket();
        packet.resolveType = resolveType;
        packet.sourceIdentifier = source;
        packet.publisherName = publisherName;
        packet.subscriberName = subscriberName;
        packet.publisherDBName = publisherDBName;
        packet.subscriberDBName = subscriberDBName;
        return packet;
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при возникновении ошибки в момент загрузки или передачи инструкции INSERT.
    //
    // Параметры:
    //   insertSource:
    //     Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает,
    //     является ли источник вставки, вызвавшей ошибку, издателем или подписчиком.
    //
    //   insertedDataSet:
    //     Набор данных, содержащий идентификатор GUID вставленной строки.
    //
    //   errorLogType:
    //     Выходной параметр Microsoft.SqlServer.Replication.BusinessLogicSupport.ErrorLogType,
    //     управляющий регистрацией ошибки при репликации. Этот параметр используется, только
    //     если возвращается значение Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError.AcceptCustomErrorBehavior.
    //
    //   customErrorMessage:
    //     Выходной параметр, используемый для возвращения настраиваемого сообщения об ошибке.
    //     Этот параметр используется, только если возвращается значение Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError.AcceptCustomErrorBehavior.
    //
    //   errorCode:
    //     Код ошибки, соответствующий произошедшей базовой ошибке репликации.
    //
    //   errorMessage:
    //     Сообщение об ошибке, соответствующее произошедшей базовой ошибке репликации.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемым
    //     обработчиком бизнес-логики.
    //
    //   historyLogMessage:
    //     Сообщение журнала, передаваемое настраиваемым обработчиком бизнес-логики. Сообщение
    //     журнала записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError,
    //     которое будет выполняться процессом после выполнения пользовательского обработчика
    //     бизнес-логики.
    public override ActionOnDataError InsertErrorHandler(SourceIdentifier insertSource, DataSet insertedDataSet, ref ErrorLogType errorLogType,
        ref string customErrorMessage, int errorCode, string errorMessage, ref int historyLogLevel, ref string historyLogMessage)
    {
        addMessage("InsertErrorHandler start");

        int autoInsertError = 0;
        if (dbSetting != null && dbSetting.ContainsKey(publisherDBName))
        {
            SinventCustomResolver.DBNameDictonary dbName = dbSetting[publisherDBName];
            if (dbName != null && dbName.tableDictionary.ContainsKey(tableName))
            {
                SinventCustomResolver.Table table = dbName.tableDictionary[tableName];
                if (table != null)
                {
                    autoInsertError = table.autoInsertError;
                }
            }
        }
        if (autoInsertError > 0)
        {
            SinventCustomResolver.InformPacket informPacket = new SinventCustomResolver.InformPacket();
            informPacket.message = "insert row in table " + tableName + ". Auto resolve insert error conflict";
            string informSocketErrorMeassage = "";
            Socket informSocket = network.connectSocket(ref informSocketErrorMeassage);
            if (informSocket != null)
            {
                network.sendPacket(informPacket);
            }
        }

        SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.InsertErrorResolveType, insertSource);
        packet.errorCode = errorCode;
        packet.errorMessage = errorMessage;

        List<string> primaryKeys = getPrimaryKeys(insertSource, ref packet);
        DataSet conflictedDataSetWithSchema = getDataForInsertError(insertSource, primaryKeys, insertedDataSet);
        DataSet insertedDataSetWithSchemaForSend = conflictedDataSetWithSchema.Clone();
        DataSet insertedDataSetWithSchemaForUse = conflictedDataSetWithSchema.Clone();
        foreach (DataRow row in insertedDataSet.Tables[0].Rows)
        {
            insertedDataSetWithSchemaForSend.Tables[0].ImportRow(row);
            insertedDataSetWithSchemaForUse.Tables[0].ImportRow(row);
        }
        DataSet conflictedDataSetWithSchemaForSend = conflictedDataSetWithSchema.Clone();
        foreach (DataRow row in conflictedDataSetWithSchema.Tables[0].Rows)
        {
            conflictedDataSetWithSchemaForSend.Tables[0].ImportRow(row);
        }

        packet.sourceDataSet = insertedDataSetWithSchemaForSend;
        packet.source2DataSet = conflictedDataSetWithSchemaForSend;
        updateForeignKeys(ref packet, insertSource);
        List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
        SinventCustomResolver.ResolvePacket resultPacket = packet;

        if (autoInsertError == 0)
        {
            string socketErrorMeassage = "";
            Socket socket = network.connectSocket(ref socketErrorMeassage);
            if (socket == null)
            {
                    throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
            }
            network.sendPacket(packet);
            resultPacket = network.recvPacket();
        }
        else if (autoInsertError == 1) //Подписчика в конец
        {
            if (insertSource == SourceIdentifier.SourceIsSubscriber)
            {
                resultPacket.result = SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult;
            }
            else
            {
                resultPacket.result = SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult;
            }
        }
        else if (autoInsertError == 2) //Публикатоора в конец
        {
            if (insertSource == SourceIdentifier.SourceIsPublisher)
            {
                resultPacket.result = SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult;
            }
            else
            {
                resultPacket.result = SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult;
            }
        }

        SourceIdentifier nonInsertSource = SourceIdentifier.SourceIsPublisher;
        if (insertSource == SourceIdentifier.SourceIsPublisher)
        {
            nonInsertSource = SourceIdentifier.SourceIsSubscriber;
        }
        if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
        //Переместить данные инициатора в конец
        {
            List<string> oldIdList = new List<string>();
            foreach (string fieldName in primaryKeys)
            {
                long id = getMaxSourcesPrimaryNextValue(fieldName, packet);
                oldIdList.Add(insertedDataSetWithSchemaForUse.Tables[0].Rows[0][fieldName].ToString());
                if (id != -1)
                {
                    insertedDataSetWithSchemaForUse.Tables[0].Rows[0][fieldName] = id;
                }
            }
            setDataForInsertError(insertSource, primaryKeys, binaryCols, insertedDataSetWithSchemaForUse, oldIdList);
            setDataForInsertError(nonInsertSource, primaryKeys, binaryCols, conflictedDataSetWithSchema, new List<string>());
            addMessage("InsertErrorHandler end AcceptResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult)
        //Переместить данные получателя в конец
        {
            List<string> oldIdList = new List<string>();
            foreach (string fieldName in primaryKeys)
            {
                long id = getMaxSourcesPrimaryNextValue(fieldName, packet);
                oldIdList.Add(conflictedDataSetWithSchema.Tables[0].Rows[0][fieldName].ToString());
                if (id != -1)
                {
                    conflictedDataSetWithSchema.Tables[0].Rows[0][fieldName] = id;
                }
            }
            setDataForInsertError(nonInsertSource, primaryKeys, binaryCols, conflictedDataSetWithSchema, oldIdList);
            setDataForInsertError(insertSource, primaryKeys, binaryCols, insertedDataSetWithSchemaForUse, new List<string>());
            addMessage("InsertErrorHandler end RejectResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.CustomResolveResult)
        //Оставить данные инициатора
        {
            List<string> oldIdList = new List<string>();
            foreach (string fieldName in primaryKeys)
            {
                long id = getMaxSourcesPrimaryNextValue(fieldName, packet);
                oldIdList.Add(conflictedDataSetWithSchema.Tables[0].Rows[0][fieldName].ToString());
                if (id != -1)
                {
                    conflictedDataSetWithSchema.Tables[0].Rows[0][fieldName] = id;
                }
            }
            setDataForInsertErrorOnlyOne(nonInsertSource, primaryKeys, binaryCols, conflictedDataSetWithSchema, insertedDataSetWithSchemaForUse, oldIdList);
            addMessage("InsertErrorHandler end CustomResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.Custom2ResolveResult)
        //Оставить данные получателя
        {
            List<string> oldIdList = new List<string>();
            foreach (string fieldName in primaryKeys)
            {
                long id = getMaxSourcesPrimaryNextValue(fieldName, packet);
                oldIdList.Add(insertedDataSetWithSchemaForUse.Tables[0].Rows[0][fieldName].ToString());
                if (id != -1)
                {
                    insertedDataSetWithSchemaForUse.Tables[0].Rows[0][fieldName] = id;
                }
            }
            setDataForInsertErrorOnlyOne(insertSource, primaryKeys, binaryCols, insertedDataSetWithSchemaForUse, conflictedDataSetWithSchema, oldIdList);
            addMessage("InsertErrorHandler end Custom2ResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else
        {
            throw new SinventCustomResolver.ResolveException("InsertErrorHandler - Answer from service in N/A");
        }
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при загрузке или передаче инструкций INSERT.
    //
    // Параметры:
    //   insertSource:
    //     Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier определяет,
    //     где была инициирована вставка — в издателе или в подписчике.
    //
    //   insertedDataSet:
    //     Набор данных, содержащий идентификатор GUID вставленной строки.
    //
    //   customDataSet:
    //     Набор данных, содержащий измененную строку, возвращаемую настраиваемой бизнес-логикой.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемой
    //     бизнес-логикой.
    //
    //   historyLogMessage:
    //     Сообщение журнала, передаваемое настраиваемой бизнес-логикой. Сообщение журнала
    //     записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataChange,
    //     которое будет выполняться процессом после выполнения настраиваемой бизнес-логики.
    public override ActionOnDataChange InsertHandler(SourceIdentifier insertSource, DataSet insertedDataSet, ref DataSet customDataSet,
        ref int historyLogLevel, ref string historyLogMessage)
    {
        addMessage("InsertHandler start");
        bool autoResolve = false;
        if (insertSource == SourceIdentifier.SourceIsPublisher)
        {
            if (autoInsertPublisher)
            {
                autoResolve = true;
            }
        }
        else
        {
            if (autoInsertSubscriber)
            {
                autoResolve = true;
            }
        }
        if (autoResolve)
        {
            SinventCustomResolver.InformPacket informPacket = new SinventCustomResolver.InformPacket();
            informPacket.message = "insert row in table " + tableName;
            string informSocketErrorMeassage = "";
            Socket informSocket = network.connectSocket(ref informSocketErrorMeassage);
            if (informSocket != null)
            {
                network.sendPacket(informPacket);
            }
            addMessage("UpdateHandler auto resolve");
            return base.InsertHandler(insertSource, insertedDataSet, ref customDataSet, ref historyLogLevel, ref historyLogMessage);
        }

        string socketErrorMeassage = "";
        Socket socket = network.connectSocket(ref socketErrorMeassage);
        if (socket == null)
        {
            throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
        }

        SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.InsertResolveType, insertSource);
        insertedDataSet.Tables[0].TableName = tableName;

        packet.sourceDataSet = insertedDataSet;
        updateForeignKeys(ref packet, insertSource);
        List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
        network.sendPacket(packet);
        SinventCustomResolver.ResolvePacket resultPacket = network.recvPacket();

        if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
        {
            addMessage("InsertHandler end AcceptData");
            return ActionOnDataChange.AcceptData;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult)
        {
            addMessage("InsertHandler end RejectData");
            return ActionOnDataChange.RejectData;
        }
        else
        {
            throw new SinventCustomResolver.ResolveException("InsertHandler - Answer from service in N/A");
        }
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при возникновении ошибки в момент загрузки или передачи инструкции UPDATE.
    //
    // Параметры:
    //   updateSource:
    //     Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает,
    //     является ли источник обновления, вызвавшего ошибку, издателем или подписчиком.
    //
    //   updatedDataSet:
    //     Набор данных, представляющий инструкцию UPDATE, вызвавшую ошибку.
    //
    //   errorLogType:
    //     Выходной параметр Microsoft.SqlServer.Replication.BusinessLogicSupport.ErrorLogType,
    //     управляющий регистрацией ошибки при репликации. Этот параметр используется, только
    //     если возвращается значение Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError.AcceptCustomErrorBehavior.
    //
    //   customErrorMessage:
    //     Выходной параметр, используемый для возвращения настраиваемого сообщения об ошибке.
    //     Этот параметр используется, только если возвращается значение Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError.AcceptCustomErrorBehavior.
    //
    //   errorCode:
    //     Код ошибки, соответствующий произошедшей базовой ошибке репликации.
    //
    //   errorMessage:
    //     Сообщение об ошибке, соответствующее произошедшей базовой ошибке репликации.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемым
    //     обработчиком бизнес-логики.
    //
    //   historyLogMessage:
    //     Сообщение журнала, передаваемое настраиваемым обработчиком бизнес-логики. Сообщение
    //     журнала записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError,
    //     которое будет выполняться процессом после выполнения пользовательского обработчика
    //     бизнес-логики.
    public override ActionOnDataError UpdateErrorHandler(SourceIdentifier updateSource, DataSet updatedDataSet, ref ErrorLogType errorLogType,
        ref string customErrorMessage, int errorCode, string errorMessage, ref int historyLogLevel, ref string historyLogMessage)
    {
        addMessage("UpdateErrorHandler start");

        string socketErrorMeassage = "";
        Socket socket = network.connectSocket(ref socketErrorMeassage);
        if (socket == null)
        {
            throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
        }

        SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.UpdateErrorResolveType, updateSource);
        packet.errorCode = errorCode;
        packet.errorMessage = errorMessage;

        List<string> primaryKeys = getPrimaryKeys(updateSource, ref packet);
        DataSet conflictedDataSetWithSchema = getDataForInsertError(updateSource, primaryKeys, updatedDataSet);

        DataSet updatedDataSetWithSchemaForSend = conflictedDataSetWithSchema.Clone();
        DataSet updatedDataSetWithSchemaForUse = conflictedDataSetWithSchema.Clone();
        foreach (DataRow row in updatedDataSet.Tables[0].Rows)
        {
            updatedDataSetWithSchemaForSend.Tables[0].ImportRow(row);
            updatedDataSetWithSchemaForUse.Tables[0].ImportRow(row);
        }
        DataSet conflictedDataSetWithSchemaForSend = conflictedDataSetWithSchema.Clone();
        foreach (DataRow row in conflictedDataSetWithSchema.Tables[0].Rows)
        {
            conflictedDataSetWithSchemaForSend.Tables[0].ImportRow(row);
        }

        packet.sourceDataSet = updatedDataSetWithSchemaForSend;
        packet.source2DataSet = conflictedDataSetWithSchemaForSend;
        updateForeignKeys(ref packet, updateSource);
        List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
        network.sendPacket(packet);

        SinventCustomResolver.ResolvePacket resultPacket = network.recvPacket();
        SourceIdentifier nonInsertSource = SourceIdentifier.SourceIsPublisher;
        if (updateSource == SourceIdentifier.SourceIsPublisher)
        {
            nonInsertSource = SourceIdentifier.SourceIsSubscriber;
        }
        if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
        //Переместить данные инициатора в конец
        {
            foreach (string fieldName in primaryKeys)
            {
                long id = getMaxSourcesPrimaryNextValue(fieldName, packet);
                if (id != -1)
                {
                    updatedDataSetWithSchemaForUse.Tables[0].Rows[0][fieldName] = id;
                }
            }
            setDataForUpdateError(updateSource, primaryKeys, binaryCols, updatedDataSetWithSchemaForUse);
            setDataForUpdateError(nonInsertSource, primaryKeys, binaryCols, conflictedDataSetWithSchema);
            addMessage("UpdateErrorHandler end AcceptResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult)
        //Переместить данные получателя в конец
        {
            foreach (string fieldName in primaryKeys)
            {
                long id = getMaxSourcesPrimaryNextValue(fieldName, packet);
                if (id != -1)
                {
                    conflictedDataSetWithSchema.Tables[0].Rows[0][fieldName] = id;
                }
            }
            setDataForUpdateError(nonInsertSource, primaryKeys, binaryCols, conflictedDataSetWithSchema);
            setDataForUpdateError(updateSource, primaryKeys, binaryCols, updatedDataSetWithSchemaForUse);
            addMessage("UpdateErrorHandler end RejectResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.CustomResolveResult)
        //Оставить данные инициатора
        {
            addMessage("UpdateErrorHandler end CustomResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.Custom2ResolveResult)
        //Оставить данные получателя
        {
            addMessage("UpdateErrorHandler end Custom2ResolveResult");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else
        {
            throw new SinventCustomResolver.ResolveException("UpdateErrorHandler - Answer from service in N/A");
        }
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при загрузке или передаче инструкций UPDATE.
    //
    // Параметры:
    //   updateSource:
    //     Указывает, где было инициировано обновление — в издателе или в подписчике.
    //
    //   updatedDataSet:
    //     Набор данных, представляющий обновляемые данные.
    //
    //   customDataSet:
    //     Набор данных, реализованный в настраиваемой бизнес-логике.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемой
    //     бизнес-логикой.
    //
    //   historyLogMessage:
    //     Сообщение журнала, представленное настраиваемой бизнес-логикой. Сообщение журнала
    //     записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataChange,
    //     которое будет выполняться процессом после выполнения настраиваемой бизнес-логики.
    public override ActionOnDataChange UpdateHandler(SourceIdentifier updateSource, DataSet updatedDataSet, ref DataSet customDataSet, ref int historyLogLevel,
        ref string historyLogMessage)
    {
        addMessage("UpdateHandler start");

        bool autoResolve = false;
        if (updateSource == SourceIdentifier.SourceIsPublisher)
        {
            if (autoUpdatePublisher)
            {
                autoResolve = true;
                
            }
        }
        else
        {
            if (autoUpdateSubscriber)
            {
                autoResolve = true;
            }
        }
        if (autoResolve)
        {
            SinventCustomResolver.InformPacket informPacket = new SinventCustomResolver.InformPacket();
            informPacket.message = "update row in table " + tableName;
            string inrfoermSocketErrorMeassage = "";
            Socket informSocket = network.connectSocket(ref inrfoermSocketErrorMeassage);
            if (informSocket != null)
            {
                network.sendPacket(informPacket);
            }
            addMessage("UpdateHandler auto resolve");
            return base.UpdateHandler(updateSource, updatedDataSet, ref customDataSet, ref historyLogLevel, ref historyLogMessage);
        }

        string socketErrorMeassage = "";
        Socket socket = network.connectSocket(ref socketErrorMeassage);
        if (socket == null)
        {
            throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
        }

        SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.UpdateResolveType, updateSource);
        updatedDataSet.Tables[0].TableName = tableName;
        packet.sourceDataSet = updatedDataSet;
        updateForeignKeys(ref packet, updateSource);
        List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
        network.sendPacket(packet);
        SinventCustomResolver.ResolvePacket resultPacket = network.recvPacket();
        if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
        {
            addMessage("UpdateHandler end AcceptData");
            return ActionOnDataChange.AcceptData;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult)
        {
            addMessage("UpdateHandler end RejectData");
            return ActionOnDataChange.RejectData;
        }
        else
        {
            throw new SinventCustomResolver.ResolveException("UpdateHandler - Answer from service in N/A");
        }
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при возникновении ошибки в момент загрузки или передачи инструкции DELETE.
    //
    // Параметры:
    //   deleteSource:
    //     Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает,
    //     является ли источник удаления, вызвавшего ошибку, издателем или подписчиком.
    //
    //   deletedDataSet:
    //     Набор данных, содержащий идентификатор GUID удаленной строки.
    //
    //   errorLogType:
    //     Выходной параметр Microsoft.SqlServer.Replication.BusinessLogicSupport.ErrorLogType,
    //     управляющий регистрацией ошибки при репликации. Этот параметр используется, только
    //     если возвращается значение Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError.AcceptCustomErrorBehavior.
    //
    //   customErrorMessage:
    //     Выходной параметр, используемый для возвращения настраиваемого сообщения об ошибке.
    //     Этот параметр используется, только если возвращается значение Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError.AcceptCustomErrorBehavior.
    //
    //   errorCode:
    //     Код ошибки, соответствующий произошедшей базовой ошибке репликации.
    //
    //   errorMessage:
    //     Сообщение об ошибке, соответствующее произошедшей базовой ошибке репликации.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемым
    //     обработчиком бизнес-логики.
    //
    //   historyLogMessage:
    //     Сообщение журнала, передаваемое настраиваемым обработчиком бизнес-логики. Сообщение
    //     журнала записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataError,
    //     которое будет выполняться процессом после выполнения пользовательского обработчика
    //     бизнес-логики.
    public override ActionOnDataError DeleteErrorHandler(SourceIdentifier deleteSource, DataSet deletedDataSet, ref ErrorLogType errorLogType,
        ref string customErrorMessage, int errorCode, string errorMessage, ref int historyLogLevel, ref string historyLogMessage)
    {
        addMessage("DeleteErrorHandler start");

        string socketErrorMeassage = "";
        Socket socket = network.connectSocket(ref socketErrorMeassage);
        if (socket == null)
        {
            throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
        }

        SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.DeleteErrorResolveType, deleteSource);
        packet.errorCode = errorCode;
        packet.errorMessage = errorMessage;
        deletedDataSet.Tables[0].TableName = tableName;
        packet.sourceDataSet = deletedDataSet;
        updateForeignKeys(ref packet, deleteSource);
        List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
        network.sendPacket(packet);
        SinventCustomResolver.ResolvePacket resultPacket = network.recvPacket();

        if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
        {
            addMessage("DeleteErrorHandler end AcceptDefaultErrorBehavior");
            return ActionOnDataError.AcceptDefaultErrorBehavior;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.CustomResolveResult)
        {
            addMessage("DeleteErrorHandler end AcceptCustomErrorBehavior");
            return ActionOnDataError.AcceptCustomErrorBehavior;
        }
        else
        {
            throw new SinventCustomResolver.ResolveException("DeleteErrorHandler - Answer from service in N/A");
        }
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при загрузке или передаче инструкций DELETE.
    //
    // Параметры:
    //   deleteSource:
    //     Указывает, где было инициировано удаление — в издателе или в подписчике.
    //
    //   deletedDataSet:
    //     Набор данных, содержащий идентификатор GUID удаленной строки.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемой
    //     бизнес-логикой.
    //
    //   historyLogMessage:
    //     Сообщение журнала, передаваемое настраиваемой бизнес-логикой. Сообщение журнала
    //     записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnDataDelete,
    //     которое будет выполняться процессом после выполнения настраиваемой бизнес-логики.
    public override ActionOnDataDelete DeleteHandler(SourceIdentifier deleteSource, DataSet deletedDataSet, ref int historyLogLevel,
        ref string historyLogMessage)
    {
        addMessage("DeleteHandler start");

        bool autoResolve = false;
        if (deleteSource == SourceIdentifier.SourceIsPublisher)
        {
            if (autoDeletePublisher)
            {
                autoResolve = true;
            }
        }
        else
        {
            if (autoDeleteSubscriber)
            {
                autoResolve = true;
            }
        }
        if (autoResolve)
        {
            SinventCustomResolver.InformPacket informPacket = new SinventCustomResolver.InformPacket();
            informPacket.message = "delete row in table " + tableName;
            string informSocketErrorMeassage = "";
            Socket informSocket = network.connectSocket(ref informSocketErrorMeassage);
            if (informSocket != null)
            {
                network.sendPacket(informPacket);
            }
            addMessage("DeleteHandler auto resolve");
            return base.DeleteHandler(deleteSource, deletedDataSet, ref historyLogLevel, ref historyLogMessage);
        }

        string socketErrorMeassage = "";
        Socket socket = network.connectSocket(ref socketErrorMeassage);
        if (socket == null)
        {
            throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
        }

        SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.DeleteResolveType, deleteSource);
        deletedDataSet.Tables[0].TableName = tableName;
        packet.sourceDataSet = deletedDataSet;
        updateForeignKeys(ref packet, deleteSource);
        List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
        network.sendPacket(packet);
        SinventCustomResolver.ResolvePacket resultPacket = network.recvPacket();

        if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
        {
            addMessage("DeleteHandler end AcceptDelete");
            return ActionOnDataDelete.AcceptDelete;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult)
        {
            addMessage("DeleteHandler end RejectDelete");
            return ActionOnDataDelete.RejectDelete;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.CustomResolveResult)
        {
            addMessage("DeleteHandler end ConvertDeleteToUpdate");
            return ActionOnDataDelete.ConvertDeleteToUpdate;
        }
        else
        {
            throw new SinventCustomResolver.ResolveException("DeleteHandler - Answer from service in N/A");
        }
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при возникновении конфликта инструкции UPDATE на издателе и подписчике.
    //
    // Параметры:
    //   publisherDataSet:
    //     Набор данных, представляющий данные издателя.
    //
    //   subscriberDataSet:
    //     Набор данных, представляющий данные подписчика.
    //
    //   customDataSet:
    //     Набор данных, реализованный в настраиваемой бизнес-логике.
    //
    //   conflictLogType:
    //     Тип Microsoft.SqlServer.Replication.BusinessLogicSupport.ConflictLogType, запрашиваемый
    //     настраиваемым обработчиком бизнес-логики.
    //
    //   customConflictMessage:
    //     Сообщение конфликта, возвращаемое настраиваемым обработчиком бизнес-логики.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемым
    //     обработчиком бизнес-логики.
    //
    //   historyLogMessage:
    //     Сообщение журнала, передаваемое настраиваемым обработчиком бизнес-логики. Сообщение
    //     журнала записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnUpdateConflict,
    //     которое будет выполняться процессом после выполнения пользовательского обработчика
    //     бизнес-логики.
    public override ActionOnUpdateConflict UpdateConflictsHandler(DataSet publisherDataSet, DataSet subscriberDataSet, ref DataSet customDataSet,
        ref ConflictLogType conflictLogType, ref string customConflictMessage, ref int historyLogLevel, ref string historyLogMessage)
    {
        addMessage("UpdateConflictsHandler start");
        try
        {
            string socketErrorMeassage = "";
            Socket socket = network.connectSocket(ref socketErrorMeassage);
            if (socket == null)
            {
                throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
            }

            SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.UpdateConflictsResolveType,
                SourceIdentifier.SourceIsPublisher);
            publisherDataSet.Tables[0].TableName = tableName;
            subscriberDataSet.Tables[0].TableName = tableName;
            packet.sourceDataSet = publisherDataSet;
            packet.source2DataSet = subscriberDataSet;
            updateForeignKeys(ref packet, SourceIdentifier.SourceIsPublisher);

            List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
            network.sendPacket(packet);
            SinventCustomResolver.ResolvePacket resultPacket = network.recvPacket();
            if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
            {
                addMessage("UpdateConflictsHandler end AcceptPublisherData");
                return ActionOnUpdateConflict.AcceptPublisherData;
            }
            else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult)
            {
                addMessage("UpdateConflictsHandler end AcceptSubscriberData");
                return ActionOnUpdateConflict.AcceptSubscriberData;
            }
        }
        catch (Exception exception)
        {
            addMessage(exception.Message);
        }
        throw new SinventCustomResolver.ResolveException("UpdateConflictsHandler - Answer from service in N/A");
    }

    // Сводка:
    //     Необязательный метод, который реализует пользовательскую бизнес-логику и вызывается
    //     при возникновении конфликта инструкции DELETE на издателе и подписчике.
    //
    // Параметры:
    //   updateSource:
    //     Microsoft.SqlServer.Replication.BusinessLogicSupport.SourceIdentifier указывает,
    //     является ли источник обновления издателем или подписчиком.
    //
    //   sourceDataSet:
    //     Набор данных, содержащий идентификатор GUID, представляющий строку в конфликте.
    //
    //   customDataSet:
    //     Набор данных, реализованный в настраиваемой бизнес-логике.
    //
    //   conflictLogType:
    //     Тип Microsoft.SqlServer.Replication.BusinessLogicSupport.ConflictLogType, запрашиваемый
    //     настраиваемым обработчиком бизнес-логики.
    //
    //   customConflictMessage:
    //     Сообщение конфликта, возвращаемое настраиваемым обработчиком бизнес-логики.
    //
    //   historyLogLevel:
    //     Уровень ведения журнала настраиваемого сообщения, возвращаемого настраиваемым
    //     обработчиком бизнес-логики.
    //
    //   historyLogMessage:
    //     Сообщение журнала, передаваемое настраиваемым обработчиком бизнес-логики. Сообщение
    //     журнала записывается в базу данных распространителя.
    //
    // Возврат:
    //     Действие Microsoft.SqlServer.Replication.BusinessLogicSupport.ActionOnUpdateDeleteConflict,
    //     которое будет выполняться процессом после выполнения пользовательского обработчика
    //     бизнес-логики.
    public override ActionOnUpdateDeleteConflict UpdateDeleteConflictHandler(SourceIdentifier updateSource, DataSet sourceDataSet, ref DataSet customDataSet,
        ref ConflictLogType conflictLogType, ref string customConflictMessage, ref int historyLogLevel, ref string historyLogMessage)
    {
        addMessage("UpdateDeleteConflictHandler start");

        string socketErrorMeassage = "";
        Socket socket = network.connectSocket(ref socketErrorMeassage);
        if (socket == null)
        {
            throw new SinventCustomResolver.ResolveException("No connection with service: " + socketErrorMeassage);
        }

        SinventCustomResolver.ResolvePacket packet = createResolvePacket(SinventCustomResolver.ResolvePacket.ResolveType.UpdateDeleteConflictResolveType,
            updateSource);
        sourceDataSet.Tables[0].TableName = tableName;
        packet.sourceDataSet = sourceDataSet;
        updateForeignKeys(ref packet, updateSource);
        List<string> binaryCols = deleteNUpdateBinaryData(ref packet);
        network.sendPacket(packet);
        SinventCustomResolver.ResolvePacket resultPacket = network.recvPacket();

        if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.AcceptResolveResult)
        {
            addMessage("UpdateDeleteConflictHandler end AcceptUpdate");
            return ActionOnUpdateDeleteConflict.AcceptUpdate;
        }
        else if (resultPacket.result == SinventCustomResolver.ResolvePacket.ResolveResult.RejectResolveResult)
        {
            addMessage("UpdateDeleteConflictHandler end AcceptDelete");
            return ActionOnUpdateDeleteConflict.AcceptDelete;
        }
        else
        {
            throw new SinventCustomResolver.ResolveException("UpdateDeleteConflictHandler - Answer from service in N/A");
        }
    }
}
