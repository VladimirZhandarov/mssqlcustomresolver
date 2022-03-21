
namespace SinventCustomResolver
{
    /** Метаданные вторичного ключа (для оперативной обработки).
     */
    class ForeignKeyRes
    {
        public string fieldNameData; ///< Имя поля в первичной таблице
        public string tableNameData; ///< Имя  первичной таблицы
        public string idNameData;    ///< Имя поля вторичного ключа в текущей таблице
    }
}
