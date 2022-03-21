using System.Collections.Generic;

namespace SinventCustomResolver
{
    /** Настройки таблицы.
     */
    public class Table
    {
        public string name;         ///< Имя таблицы.
        public int autoInsertError = 0; ///< =0 - разрешает пользователь
                                    ///< =1 - автоматичеки insert подписчика в конец.
                                    ///< =2 - автоматичеки insert публикатора в конец.
    }

    public class DBNameDictonary
    {
        public string name;         ///< Имя БД
        public Dictionary<string, Table> tableDictionary;
    }

    /** Настройки БД.
     */
    public class DBName
    {
        public string name;         ///< Имя БД
        public List<Table> table;
    }

    /** Настройки.
     */
    public class Settings
    {
        public string hostName;             ///< Имя хоста сервиса (IP-адрес)
        public int port;                    ///< Порт сервиса
        public bool autoInsertPublisher;    ///< Производить неконфликтные добавления строк от публикатора без участия оператора (автоматически)
        public bool autoInsertSubscriber;   ///< Производить неконфликтные добавления строк от подписчика без участия оператора (автоматически)
        public bool autoUpdatePublisher;    ///< Производить неконфликтные изменения строк от публикатора без участия оператора (автоматически)
        public bool autoUpdateSubscriber;   ///< Производить неконфликтные изменения строк от подписчика без участия оператора (автоматически)
        public bool autoDeletePublisher;    ///< Производить неконфликтные удаления строк от публикатора без участия оператора (автоматически)
        public bool autoDeleteSubscriber;   ///< Производить неконфликтные удаления строк от подписчика без участия оператора (автоматически)
        public string publisherDBUser;      ///< Имя пользователя для присоединения к СУБД публиктора
        public string publisherDBPassword;  ///< Пароль пользователя для присоединения к СУБД публиктора
        public string subscriberDBUser;     ///< Имя пользователя для присоединения к СУБД подписчика
        public string subscriberDBPassword; ///< Пароль пользователя для присоединения к СУБД подписчика
        public string logFilePath;          ///< Имя файла журнала
        public List<DBName> dbName;         ///< Список БД
    }
}
