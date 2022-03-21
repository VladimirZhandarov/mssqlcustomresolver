
namespace SinventCustomResolveService
{
    public class BackupDatabasePacket
    {
        public enum BackupDatabaseResult
        {
            UndefineBackupDatabaseResult = 0,   ///< Не определено
            AcceptBackupDatabaseResult,         ///< Ок
            RejectBackupDatabaseResult,         ///< Отмена
        }; ///< Ответ от сервиса клиенту

        public string username;
        public string password;
        public string dbName;                   ///< Имя базы данных
        public string message;                  ///< Сообщение
        public BackupDatabaseResult result;     ///< Ответ от сервиса клиенту
    }
}
