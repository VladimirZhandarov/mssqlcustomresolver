
namespace SinventCustomResolveService
{
    /** Настройки.
     */
    public class Settings
    {
        public int resolverPort;                ///< Порт для прослушки соединений от ресолвера
        public int clientPort;                  ///< Порт для прослушки соединений от клиента
        public int logLevel;                    ///< Уровень журналирования
        public string publisherDBInstance;      ///< Имя СУБД публикатора
        public string backupDirectory;          ///< Путь к каталогу backup и restore
    }
}
