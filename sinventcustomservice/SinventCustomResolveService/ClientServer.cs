using System.IO;
using System.Net.Sockets;
using System.Xml.Serialization;

namespace SinventCustomResolveService
{
    /** Сервер для получания соединений и взаимодействия по сети с клиентом.
     */
    public class ClientServer : BaseServer
    {
        public static string clientServerString = "ClientServer";

        private Socket m_mainClient = null;     ///< Сокет клиента РМО, который защел в режиме главного клиента.
        private Socket m_mobileClient = null;   ///< Сокет мобильного клиента или клиента РМО, который защел в режиме мобильного клиента.

        /** Конструктор.
         * @param businessLogic: интерфейс для сетевой части, чтобы вызывать фуенкции ядра.
         * @param port: порт для прослушивания соединений.
         * @param logWriter: журнал.
         */
        public ClientServer(IBusinessLogic businessLogic, int port, LogWriter logWriter) : base(businessLogic, clientServerString, port, logWriter)
        {

        }

        /** Установить сокет для клиента РМО, который защел в режиме главного клиента.
         * @param client: сокет.
         */
        public void setMainClient(Socket client)
        {
            m_mainClient = client;
        }

        /** Получить сокет для клиента РМО, который защел в режиме главного клиента.
         * @return Socket: сокет.
         */
        public Socket getMainClient()
        {
            return m_mainClient;
        }

        /** Установить сокет для мобильного клиента.
         * @param client: сокет.
         */
        public bool setMobileClient(Socket client)
        {
            if (m_mobileClient != null && client != null)
            {
                return false;
            }
            m_mobileClient = client;
            return true;
        }

        /** Клиент отсоединился.
         * @param client: сокет.
         */
        public void clientDisconnect(Socket client)
        {
            if (m_mainClient == client)
            {
                m_mainClient = null;
            }
            if (m_mobileClient == client)
            {
                m_mobileClient = null;
            }
        }

        /** Отправить пакет главному клиенту.
         * @param packetType: тип пакета (enum PacketType).
         * @param byteData: пакет.
         */
        public void sendToMainClient(int packetType, byte[] byteData)
        {
            if (m_mainClient != null)
            {
                send(m_mainClient, packetType, byteData);
            }
        }

        /** Отправить пакет всем клиентам.
         * @param packetType: тип пакета (enum PacketType).
         * @param byteData: пакет.
         */
        public override void sendToAll(int packetType, byte[] byteData)
        {
            if (m_mainClient != null)
            {
                send(m_mainClient, packetType, byteData);
            }
            if (m_mobileClient != null)
            {
                send(m_mobileClient, packetType, byteData);
            }
        }

        /** Отправить сообщение (информационный пакет) всем клиентам.
         * @param message: текст сообщения.
         * @param errorCode: код ошибки.
         *                   =0 - ошибки нет.
         */
        public void sendMessageToAll(string message, int errorCode)
        {
            InformPacket informPacket = new InformPacket();
            informPacket.message = message;
            informPacket.errorCode = errorCode;
            using (MemoryStream stream = new MemoryStream())
            {
                XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.InformPacket));
                serializer.Serialize(stream, informPacket);
                sendToAll((int)BaseServer.PacketType.INFORM_PACKET_TYPE, stream.ToArray());
            }
        }

        /** Отправить пакет управления процессом синхронизации всем клиентам с информацией об окончании шага синхронизации.
         * @param dbName: имя базы данных.
         * @param error: признак ошибки.
         *                   =false - ошибки нет.
         * @param snapshotNeeded: необходимо запустить создание snapshot 
         */
        public void sendNextStepEndedManageToAll(string dbName, bool error, bool snapshotNeeded, bool snapshotEnded)
        {
            ManagePacket managePacket = new ManagePacket();
            managePacket.manageType = ManagePacket.ManageType.NextStepEndedManageType;
            managePacket.dbName = dbName;
            if (snapshotNeeded)
            {
                managePacket.result = ManagePacket.ManageResult.SnapShotNeededManageResult;
            }
            else if(snapshotEnded)
            {
                managePacket.result = ManagePacket.ManageResult.SnapShotEndedManageResult;
            }
            else
            {
                if (!error)
                {
                    managePacket.result = ManagePacket.ManageResult.AcceptManageResult;
                }
                else
                {
                    managePacket.result = ManagePacket.ManageResult.RejectManageResult;
                }
            }
            using (MemoryStream stream = new MemoryStream())
            {
                XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolveService.ManagePacket));
                serializer.Serialize(stream, managePacket);
                sendToAll((int)BaseServer.PacketType.MANAGE_PACKET_TYPE, stream.ToArray());
            }
        }
    }
}
