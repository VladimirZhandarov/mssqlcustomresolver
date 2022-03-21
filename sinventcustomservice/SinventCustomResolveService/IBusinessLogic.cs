using System.Net.Sockets;

namespace SinventCustomResolveService
{
    /** Интерфейс для сетевой части, чтобы вызывать фуенкции ядра.
     */
    public interface IBusinessLogic
    {
        /** Обработка полученного пакета.
         * @param client: сокет.
         * @param serverName: имя сервера (для того чтобы отличать ClientServer и ResolverServer).
         * @param size: размер пакета
         * @param type: тип пакета (enum PacketType).
         * @param packetData: пакет
         */
        byte[] onPacket(Socket client, string serverName, long size, int type, byte[] packetData);

        /** Клиент отсоединился.
         * @param client: сокет.
         * @param serverName: имя сервера (для того чтобы отличать ClientServer и ResolverServer).
         */
        void clientDisconnect(Socket client, string serverName);
    }
}
