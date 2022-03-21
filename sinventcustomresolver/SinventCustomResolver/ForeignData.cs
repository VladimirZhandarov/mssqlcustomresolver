
namespace SinventCustomResolver
{
    /** Метаданные вторичного ключа.
     */
    class ForeignData
    {
        public string columnName = "";          ///< Имя поля в первичной таблице
        public string tableName = "";           ///< Имя  первичной таблицы
        public string referenceColumnName = ""; ///< Имя поля вторичного ключа в текущей таблице
        public string updateAction;             ///< Действие при UPDATE
        public string deleteAction;             ///< Действие при DELETE
    }
}
