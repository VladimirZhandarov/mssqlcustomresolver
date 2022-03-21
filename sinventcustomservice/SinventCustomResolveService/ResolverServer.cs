using System.Net.Sockets;

namespace SinventCustomResolveService
{
    /** Сервер для получания соединений от resolver`а.
     */
    public class ResolverServer : BaseServer
    {
        public static string resolverServerString = "ResolverServer";

        private Socket m_client = null; ///< Сокет для взаимодействия с resolver

        /** Конструктор.
         * @param businessLogic: интерфейс для сетевой части, чтобы вызывать фуенкции ядра.
         * @param port: порт для прослушивания соединений.
         * @param logWriter: журнал.
         */
        public ResolverServer(IBusinessLogic businessLogic, int port, LogWriter logWriter) : base(businessLogic, resolverServerString, port, logWriter)
        {

        }

        /** Установить сокет для взаимодействия с resolver.
         * @param client: сокет для взаимодействия с resolver.
         */
        public void setClient(Socket client)
        {
            m_client = client;
        }

        /** Отправить пакет всем клиентам.
         * @param packetType: тип пакета (enum PacketType).
         * @param byteData: пакет.
         */
        public override void sendToAll(int packetType, byte[] byteData)
        {
            if (m_client != null)
            {
                send(m_client, packetType, byteData);
            }
        }
    }
}

