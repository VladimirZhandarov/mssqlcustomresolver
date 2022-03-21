using System;
using System.Net.Sockets;
using System.IO;
using System.Xml.Serialization;

namespace SinventCustomResolver
{
    /** Класс отвечающий за сетевое взаимодействие.
     */
    class Network
    {
        private string m_hostName;      ///< Адрес сервиса
        private int m_port;             ///< Порт сервиса
        private Socket m_socket = null; ///< Соект

        private int resolvePacketTypeId = 1;            ///< Идкентификатор для пакетов с данными для разрешения конфликта
        private int informPacketTypeId = 2;             ///< Идкентификатор для информационных пакетов
        private int dbConnectErrorPacketTypeId = 3;     ///< Идкентификатор для пакетов с ошибкой соединения с СУБД

        /** Конструктор.
         * @param hostName: адрес сервиса.
         * @param port: порт сервиса.
         */
        public Network(string hostName, int port)
        {
            m_hostName = hostName;
            m_port = port;
        }

        /** Установить соединение.
         * @param errorMeassage: возвращается сообщенире об ошибке.
         */
        public Socket connectSocket(ref string errorMeassage)
        {
            if (m_socket != null)
            {
                m_socket.Close();
                m_socket = null;
            }
            try
            {
                Socket tempSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                tempSocket.Connect(m_hostName, m_port);
                if (tempSocket.Connected)
                {
                    m_socket = tempSocket;
                }
            }
            catch (Exception exception)
            {
                errorMeassage = exception.Message;
            }
            return m_socket;
        }

        /** Отправить пакет с данными для разрешения конфликта.
         * @param packet: пакет с данными для разрешения конфликта.
         */
        public void sendPacket(SinventCustomResolver.ResolvePacket packet)
        {
            using (MemoryStream stream = new MemoryStream())
            {
                XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolver.ResolvePacket));
                serializer.Serialize(stream, packet);
                m_socket.Send(BitConverter.GetBytes(stream.Length + sizeof(int)));
                m_socket.Send(BitConverter.GetBytes(resolvePacketTypeId));
                m_socket.Send(stream.GetBuffer());
            }
        }

        /** Отправить информационный пакет.
         * @param packet: информационный пакет.
         */
        public void sendPacket(SinventCustomResolver.InformPacket packet)
        {
            using (MemoryStream stream = new MemoryStream())
            {
                XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolver.InformPacket));
                serializer.Serialize(stream, packet);
                m_socket.Send(BitConverter.GetBytes(stream.Length + sizeof(int)));
                m_socket.Send(BitConverter.GetBytes(informPacketTypeId));
                m_socket.Send(stream.GetBuffer());
            }
        }

        /** Отправить пакет с ошибкой соединения с СУБД.
         * @param packet: пакет с ошибкой соединения с СУБД.
         */
        public void sendPacket(SinventCustomResolver.DBConnectErrorPacket packet)
        {
            using (MemoryStream stream = new MemoryStream())
            {
                XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolver.DBConnectErrorPacket));
                serializer.Serialize(stream, packet);
                m_socket.Send(BitConverter.GetBytes(stream.Length + sizeof(int)));
                m_socket.Send(BitConverter.GetBytes(dbConnectErrorPacketTypeId));
                m_socket.Send(stream.GetBuffer());
            }
        }

        /** Получить пакет с ответом разрешения конфликта.
         * @return packet: пакет с ответом разрешения конфликта.
         */
        public SinventCustomResolver.ResolvePacket recvPacket()
        {
            int bytes = 0;
            byte[] bytesReceived = new byte[sizeof(long)];
            byte[] bytesPacketType = new byte[sizeof(int)];
            bytes = m_socket.Receive(bytesReceived, bytesReceived.Length, 0);
            if (bytes <= 0)
            {
                throw new SinventCustomResolver.ResolveException("Нет ответа от сервиса");
            }
            bytes = m_socket.Receive(bytesPacketType, bytesPacketType.Length, 0);
            if (bytes <= 0)
            {
                throw new SinventCustomResolver.ResolveException("Нет ответа от сервиса");
            }
            long packetSize = BitConverter.ToInt64(bytesReceived, 0);
            int packetType = BitConverter.ToInt32(bytesPacketType, 0);
            byte[] packetByte = new byte[packetSize + 1 - sizeof(int)];
            int totalBytes = 0;
            do
            {
                bytes = m_socket.Receive(packetByte, totalBytes, packetByte.Length - totalBytes, SocketFlags.None);
                if (bytes <= 0)
                {
                    throw new SinventCustomResolver.ResolveException("Нет ответа от сервиса");
                }
                totalBytes += bytes;
            } while (totalBytes < packetByte.Length - 1);

            using (MemoryStream stream = new MemoryStream(packetByte))
            {
                if (packetType == resolvePacketTypeId)
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(SinventCustomResolver.ResolvePacket));
                    SinventCustomResolver.ResolvePacket resultPacket = (SinventCustomResolver.ResolvePacket)serializer.Deserialize(stream);
                    return resultPacket;
                }
            }
            return new SinventCustomResolver.ResolvePacket();
        }
    }
}
