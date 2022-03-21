
using System.Collections.Generic;

namespace SinventCustomResolveService
{
    public class ListServerPacket
    {
        public enum ListServerResult
        {
            UndefineListServerResult = 0,   ///< Не определено
            AcceptListServerResult,         ///< Ок
            RejectListServerResult,         ///< Отмена
        }; ///< Ответ от сервиса клиенту

        public string username;
        public string password;
        public List<string> serverNames;      ///< Имя базы данных
        public string errorMessage;           ///< Сообщение об ошибке
        public ListServerResult result;       ///< Ответ от сервиса клиенту
    }
}
