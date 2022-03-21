
namespace SinventCustomResolver
{
    /** Пакет с ошибкой соедиенения с СУБД.
     */
    public class DBConnectErrorPacket
    {
        public bool publisher = true;   ///< Публикатор - TRUE, Подпичик - FALSE
        public string serverName = null;///< Имя СУБД
        public string dbName = null;    ///< Имя БД
        public string tableName = null; ///< Имя таблицы
        public string userName = null;  ///< Имя пользователя
    }
}
