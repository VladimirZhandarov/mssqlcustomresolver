using Microsoft.SqlServer.Replication.BusinessLogicSupport;
using System.Collections.Generic;
using System.Data;

namespace SinventCustomResolver
{
    /** Информация первичном ключе.
     */
    public class PrimaryNextValue
    {
        public string fieldName;    ///< Имя поля  
        public long nextValue;      ///< Следующее свободное значение первичного ключа
    }

    /** Метаданные бинарных данных.
     */
    public class BinaryDataDescription
    {
        public string fieldName;    ///< Имя поля
        public int dataSize;        ///< Размер данных
        public bool equalData;      ///< Данные эквиваленты (нет конфликта)
        public bool nullData;       ///< Данные = NULL
    }

    /** Информация вторичном ключе.
     */
    public class ForeignKeyDataSet
    {
        public string fieldName;    ///< Имя поля
        public DataSet dataSet;     ///< Набор данных (другая таблица)
        public List<BinaryDataDescription> binaryDataDescriptions;
    }

    /** Пакет с данными для разранения конфликта репликации.
     */
    public class ResolvePacket
    {
        public enum ResolveType
        {
            InsertResolveType = 0,
            InsertErrorResolveType,
            UpdateResolveType,
            UpdateErrorResolveType,
            DeleteResolveType,
            DeleteErrorResolveType,
            UpdateConflictsResolveType,
            UpdateDeleteConflictResolveType
        }; ///< Тип конфликта.

        public enum ResolveResult
        {
            UndefineResolveResult = 0,
            AcceptResolveResult,
            RejectResolveResult,
            CustomResolveResult,
            Custom2ResolveResult
        }; ///< Тип решения конфликта.

        public ResolveType resolveType;             ///< Тип конфликта.
        public SourceIdentifier sourceIdentifier;   ///< Источник данных (публиукатор или подпичик).
        public string publisherName;                ///< Имя СУБД публикатора.
        public string subscriberName;               ///< Имя СУБД подпичика.
        public string publisherDBName;              ///< Имя БД публикатора.
        public string subscriberDBName;             ///< Имя БД подпичика.
        public int errorCode;                       ///< Код ошибки.
        public string errorMessage;                 ///< Сообщение об ошибке.
        public DataSet sourceDataSet;               ///< Первый набор данных (от источника).
        public DataSet source2DataSet;              ///< Второй набор данных (от получателя).

        public List<PrimaryNextValue> sourcePrimaryNextValues;  ///< Набор следующих свободных значений первичных ключей для sourceDataSet.
        public List<PrimaryNextValue> source2PrimaryNextValues; ///< Набор следующих свободных значений первичных ключей для source2DataSet.

        public List<ForeignKeyDataSet> sourceDataSetForeignKeys;    ///< Значения для вторичных ключей sourceDataSet.
        public List<ForeignKeyDataSet> source2DataSetForeignKeys;   ///< Значения для вторичных ключей source2DataSet.

        public List<BinaryDataDescription> sourceBinaryDataDescriptions;    ///< Описание для двоичных данных, которые не передаются (так как клиент не умеет отображать binary data) для sourceDataSet.
        public List<BinaryDataDescription> source2BinaryDataDescriptions;   ///< Описание для двоичных данных, которые не передаются (так как клиент не умеет отображать binary data) для source2DataSet.

        public ResolveResult result = ResolveResult.UndefineResolveResult;   ///< Тип решения конфликта.
    }
}
