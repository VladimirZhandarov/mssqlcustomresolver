
using System.Collections.Generic;

namespace SinventCustomResolveService
{
    public class ListDatabasePacket
    {
        public enum ListDatabaseResult
        {
            UndefineListDatabaseResult = 0,   ///< Не определено
            AcceptListDatabaseResult,         ///< Ок
            RejectListDatabaseResult,         ///< Отмена
        }; ///< Ответ от сервиса клиенту

        public string username;
        public string password;
        public List<string> dbNames;            ///< Имя базы данных
        public string errorMessage;             ///< Сообщение об ошибке
        public ListDatabaseResult result;       ///< Ответ от сервиса клиенту
    }
}
