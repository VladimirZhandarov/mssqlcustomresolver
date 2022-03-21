
namespace SinventCustomResolveService
{
    /** Пакет управления.
     */
    public class ManagePacket
    {
        public enum ManageType
        {
            UndefineManageType = -1,    ///< Не определено
            StartManageType = 0,        ///< Начать синхронизацию
            StopManageType,             ///< Остановить синхронизацию
            NextStepManageType,         ///< Начать следующий шаг синхронизации
            NextStepEndedManageType,    ///< Шаг синхронизации окончен
            StartMainClientType,        ///< Начать синхронизацию как главный клиент
            StopMainClientType         ///< Остановить синхронизацию как главный клиент
        }; ///< Подтип пакета

        public enum ManageResult
        {
            UndefineManageResult = 0,   ///< Не определено
            AcceptManageResult,         ///< Ок
            RejectManageResult,         ///< Отмена
            SnapShotNeededManageResult, ///< Нужен snapshot
            SnapShotEndedManageResult   ///< Snapshot сделан
        }; ///< Ответ от сервиса клиенту

        public enum ErrorCode
        {
            NOT_SET_SERVER_NAME = 1,        ///< Не установлено имя сервера
            NOT_SET_PUBLISHER_SERVER_NAME,  ///< Не установлено имя публикатора
            NOT_START_MAIN_RESOLVE_APP,     ///< Главный клиент не запущен
            RESOLVE_PROCESS_EXIST,          ///< Процесс синхронизации идет
            DATABASE_NOT_FOUND,             ///< База данных не найдена
            START_JOB_EXCEPTION             ///< Ошибка начала job msssql
        };  ///< Код ошибки

        public ManageType manageType;   ///< Подтип пакета
        public bool mainClient;         ///< Признак главного клиента
        public bool ownResolve;         ///< Признак разрешения конфликта на мобильном клиенте
        public string dbServerInstanceName; ///< Имя СУБД
        public string dbName;               ///< Имя базы данных
        public string language;             ///< Язык
        public int errorCode;               ///< Код ошибки
        public string errorMessage;         ///< Сообщение ошибки
        public ManageResult result;         ///< Ответ от сервиса клиенту
    }
}
