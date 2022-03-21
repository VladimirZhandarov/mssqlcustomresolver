using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace SinventCustomResolveService
{
    /** Объект-состояние для сокета
      */
    public class StateObject
    {
        public Socket workSocket = null;
        public const int bufferSize = 1024;
        public byte[] buffer = new byte[bufferSize];
        public MemoryStream stream = new MemoryStream();
        public int receiveStep = 0;
        public long packetSize = 0;
        public int packetType = 0;
    }

    /** Сервер для получания соединений и взаимодействия по сети.
     */
    public class BaseServer
    {
        public enum PacketType
        {
            RESOLVE_PACKET_TYPE = 1,        ///< Пакет разрешения конфликтов
            INFORM_PACKET_TYPE,             ///< Информационный пакет
            DB_CONNECT_ERROR_PACKET_TYPE,   ///< Пакет ошибки соединения
            MANAGE_PACKET_TYPE,             ///< Пакет управления процессом синхронизации
            LIST_DATABASE_PACKET_TYPE,
            BACKUP_DATABASE_PACKET_TYPE,
            RESTORE_DATABASE_PACKET_TYPE,
            REPLICATION_OBJECT_PACKET_TYPE,
            CREATE_PUBLICATION_PACKET_TYPE,
            LIST_SERVER_PACKET_TYPE,
            CREATE_SUBSCRIBER_PACKET_TYPE

        };  ///< Тип пакета

        private IBusinessLogic m_businessLogic; ///< Интерфейс для сетевой части, чтобы вызывать фуенкции ядра.
        protected string m_serverName;          ///< Имя сервера (для того чтобы отличать ClientServer и ResolverServer).
        private int m_port;                     ///< Порт для прослушивания соединений.
        private Thread m_listenThread;          ///< Поток для прослушивания соединений.
        private bool m_workFlag;                ///< Признак работы сервера
        protected LogWriter m_logWriter;        ///< Журнал

        private readonly object sendSyncLock = new object();    ///< Mutex на отправку пакета.
        public ManualResetEvent m_allDone = new ManualResetEvent(false);

        /** Конструктор.
         * @param businessLogic: интерфейс для сетевой части, чтобы вызывать фуенкции ядра.
         * @param serverName: имя сервера (для того чтобы отличать ClientServer и ResolverServer).
         * @param port: порт для прослушивания соединений.
         * @param logWriter: журнал.
         */
        public BaseServer(IBusinessLogic businessLogic, string serverName, int port, LogWriter logWriter)
        {
            m_businessLogic = businessLogic;
            m_serverName = serverName;
            m_port = port;
            m_logWriter = logWriter;
            m_workFlag = true;
        }

        ~BaseServer()
        {

        }

        /** Получить имя сервера (для того чтобы отличать ClientServer и ResolverServer).
        * @return string: имя сервера (для того чтобы отличать ClientServer и ResolverServer).
        */
        public string getServerName()
        {
            return m_serverName;
        }

        /** Слущать серверный сокет.
        */
        public void listen()
        {
            try
            {
                IPEndPoint localEndPoint = new IPEndPoint(0, m_port);
                Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                listener.Bind(localEndPoint);
                listener.Listen(100);
                m_logWriter.logNormalMessage(m_serverName + " listen on 0.0.0.0:" + m_port.ToString());
                while (m_workFlag)
                {
                    m_allDone.Reset();
                    listener.BeginAccept(new AsyncCallback(this.acceptCallback), listener);
                    m_allDone.WaitOne();
                }
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(m_serverName + "::listen exception: " + exception.ToString());
            }
        }

        /** Начать работу m_listenThread.
        */
        public void start()
        {
            try
            {
                m_listenThread = new Thread(new ThreadStart(listen));
                m_listenThread.Start();
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(m_serverName + "::start exception: " + exception.ToString());
            }
        }

        /** Окончить работу m_listenThread.
        */
        public void stop()
        {
            try
            {
                m_workFlag = false;
                m_allDone.Set();
                m_listenThread.Join();
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(m_serverName + "::stop exception: " + exception.ToString());
            }
        }

        /** Callback на входящее соединение.
         * @param ar: объект входящего соединения (Представляет состояние асинхронной операции).
         */
        public void acceptCallback(IAsyncResult ar)
        {
            m_allDone.Set();
            try
            {
                Socket listener = (Socket)ar.AsyncState;
                Socket handler = listener.EndAccept(ar);
                StateObject state = new StateObject();
                state.workSocket = handler;
                handler.BeginReceive(state.buffer, 0, StateObject.bufferSize, 0, new AsyncCallback(this.receiveCallback), state);
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(m_serverName + "::acceptCallback exception: " + exception.ToString());
            }
        }

        /** Получить входящий пакет.
         * @param client: сокет.
         */
        public void receive(Socket client)
        {
            try
            {
                StateObject state = new StateObject();
                state.workSocket = client;
                client.BeginReceive(state.buffer, 0, StateObject.bufferSize, 0, new AsyncCallback(this.receiveCallback), state);
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(m_serverName + "::receive exception: " + exception.ToString());
            }
        }

        /** Callback на входящие данные в сокете.
         * @param ar: объект входящие данные в сокете (Представляет состояние асинхронной операции).
         */
        public void receiveCallback(IAsyncResult ar)
        {
            try
            {
                StateObject state = (StateObject)ar.AsyncState;
                Socket client = state.workSocket;
                int bytesRead = client.EndReceive(ar);
                if (bytesRead > 0)
                {
                    int index = 0;
                    if (state.receiveStep == 0 && bytesRead >= sizeof(long))
                    {
                        state.packetSize = BitConverter.ToInt64(state.buffer, 0);
                        state.receiveStep = 1;
                        index = sizeof(long);
                    }
                    if (state.receiveStep == 1 && bytesRead >= index + sizeof(int))
                    {
                        state.packetType = BitConverter.ToInt32(state.buffer, index);
                        state.receiveStep = 2;
                        index += sizeof(int);
                    }
                    state.stream.Write(state.buffer, index, bytesRead - index);
                    if (state.stream.Length + sizeof(int) >= state.packetSize)
                    {
                        state.receiveStep = 0;
                        byte[] resultPacket = m_businessLogic.onPacket(client, m_serverName, state.packetSize, state.packetType, state.stream.ToArray());
                        if (resultPacket != null)
                        {
                            send(state.workSocket, state.packetType, resultPacket);
                        }
                        state.stream = new MemoryStream();
                    }
                    client.BeginReceive(state.buffer, 0, StateObject.bufferSize, 0, new AsyncCallback(this.receiveCallback), state);
                }
                else
                {
                   m_businessLogic.clientDisconnect(client, m_serverName);
                }
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(m_serverName + "::receiveCallback exception: " + exception.ToString());
            }
        }

        /** Отправить пакет всем клиентам.
         * @param packetType: тип пакета (enum PacketType).
         * @param byteData: пакет.
         */
        public virtual void sendToAll(int packetType, byte[] byteData)
        {

        }

        /** Отправить пакет одному клиенту.
         * @param client: cокет клиента.
         * @param packetType: тип пакета (enum PacketType).
         * @param byteData: пакет.
         */
        public void send(Socket client, int packetType, byte[] byteData)
        {
            lock (sendSyncLock)
            {
                try
                {
                    using (MemoryStream stream = new MemoryStream(byteData))
                    {
                        client.Send(BitConverter.GetBytes(stream.Length + sizeof(int)));
                        client.Send(BitConverter.GetBytes(packetType));
                        client.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(this.sendCallback), client);
                    }
                }
                catch (Exception exception)
                {
                    m_logWriter.logNormalMessage(m_serverName + "::send exception: " + exception.ToString());
                }
            }
        }

        /** Callback на отправку данных в сокет.
         * @param ar: объект исходящих данных в сокете (Представляет состояние асинхронной операции).
         */
        public void sendCallback(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                int bytesSent = client.EndSend(ar);
            }
            catch (Exception exception)
            {
                m_logWriter.logNormalMessage(m_serverName + "::sendCallback exception: " + exception.ToString());
            }
        }
    }
}
