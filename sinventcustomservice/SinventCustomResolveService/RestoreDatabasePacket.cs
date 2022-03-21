
namespace SinventCustomResolveService
{
    public class RestoreDatabasePacket
    {
        public enum RestoreDatabaseResult
        {
            UndefineRestoreDatabaseResult = 0,   ///< Не определено
            AcceptRestoreDatabaseResult,         ///< Ок
            RejectRestoreDatabaseResult,         ///< Отмена
        }; ///< Ответ от сервиса клиенту

        public string username;
        public string password;
        public string dbName;                   ///< Имя базы данных
        public string message;                  ///< Сообщение
        public RestoreDatabaseResult result;    ///< Ответ от сервиса клиенту
    }
}
