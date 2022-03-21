#ifndef RESOLVEPACKETDATA_H
#define RESOLVEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtCore/QVector>
#include <QtCore/QVariant>

#include "memory"

/**
* @class PrimaryNextValue
* Информация о следующих возможных значениях primary keys
*/
class UPELIB_EXPORT PrimaryNextValue
{
public:
	/**
	* Конструктор
	* @param fieldName имя поля
	* @param nextValue следующиее возможное значение
	*/
	PrimaryNextValue(const QString& fieldName = "", qint64 nextValue = 0);

	/**
	* Деструктор
	*/
	~PrimaryNextValue();

	/**
	* Получить имя поля
	* @return имя поля
	*/
	const QString& getFieldName() const;

	/**
	* Получить следующиее возможное значение
	* @return следующиее возможное значение
	*/
	qint64 getNextValue() const;

private:
	QString m_fieldName;	///< Имя поля
	qint64 m_nextValue;		///< Следующиее возможное значение
};

class FieldData;

/**
* @class PrimaryNextValue
* Информация о двоичных полях
*/
class UPELIB_EXPORT BinaryDataDescription
{
public:
	/**
	* Конструктор
	* @param fieldName имя поля
	* @param dataSize размер двоичных данных
	* @param equalData признак что данные в двух синхронизируемых полях идентичны
	* @param nullData признак =NULL
	*/
	BinaryDataDescription(const QString& fieldName = "", qint64 dataSize = 0, bool equalData = false, bool nullData = false);

	/**
	* Деструктор
	*/
	~BinaryDataDescription();

	/**
	* Получить имя поля
	* @return имя поля
	*/
	const QString& getFieldName() const;

	/**
	* Получить размер двоичных данных
	* @return размер двоичных данных
	*/
	qint64 getDataSize() const;

	/**
	* Получить признак что данные в двух синхронизируемых полях идентичны
	* @return признак что данные в двух синхронизируемых полях идентичны
	*/
	bool isEqualData() const;

	/**
	* Получить признак =NULL
	* @return признак =NULL
	*/
	bool isNullData() const;

private:
	QString m_fieldName;	///< Имя поля
	qint64 m_dataSize;		///< Размер двоичных данных
	bool m_equalData;		///< признак что данные в двух синхронизируемых полях идентичны
	bool m_nullData;		///< Признак = NULL
};

/**
* @class FieldMetaData
* Метаданные поля базы данных
*/
class UPELIB_EXPORT FieldMetaData : public QObject
{
	Q_OBJECT
public:
	enum XsdType
	{
		StringXsdType = 0,	///< Строка
		IntXsdType			///< Число
	};	///< Тип поля

		/**
		* Конструктор
		* @param name имя поля
		* @param type тип поля
		* @param minOccurs множественность
		* @param parent родительский объект
		*/
	FieldMetaData(const QString& name, XsdType type, int minOccurs, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~FieldMetaData();

	/**
	* Получить имя поля
	* @return имя поля
	*/
	const QString& getName() const;

	/**
	* Получить тип поля
	* @return тип поля
	*/
	XsdType getType() const;

	/**
	* Получить множественность
	* @return множественность
	*/
	int getMinOccurs() const;

protected:
	QString m_name;	///< Имя поля
	XsdType m_type;	///< Тип поля
	int m_minOccurs;	///< Множественность
};

/**
* @class ForeignKeyDataSet
* Информация о вторичном ключе
*/
class UPELIB_EXPORT ForeignKeyDataSet
{
public:
	/**
	* Конструктор
	* @param fieldName имя поля
	* @param foreignTable таблица, куда ссылается ключ
	* @param fieldMetaDataVector метаданные полей данных, куда ссылается ключ
	* @param dataVector поля, куда ссылается ключ
	* @param binaryDataDescriptions информация о двоичных полях, куда ссылается ключ
	*/
	ForeignKeyDataSet(const QString& fieldName = "", const QString& foreignTable = "",
		const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector = QVector<std::tr1::shared_ptr<FieldMetaData> >(),
		const QVector<QVariant>& dataVector = QVector<QVariant>(), const QVector<BinaryDataDescription>& binaryDataDescriptions =
		QVector<BinaryDataDescription>());

	/**
	* Деструктор
	*/
	~ForeignKeyDataSet();

	/**
	* Получить имя поля
	* @return имя поля
	*/
	const QString& getFieldName() const;

	/**
	* Получить таблицу, куда ссылается ключ
	* @return таблица, куда ссылается ключ
	*/
	const QString& getForeignTable() const;

	/**
	* Получить поля, куда ссылается ключ
	* @return поля, куда ссылается ключ
	*/
	const QVector<QVariant>& getDataVector() const;

	/**
	* Установить имя таблицы, куда ссылается ключ (для GUI)
	* @param tableViewName имя таблицы
	*/
	void setTableViewName(const QString& tableViewName);

	/**
	* Установить имя полей записи таблицы, куда ссылается ключ (для GUI)
	* @param fieldLabelVector имя полей таблицы
	*/
	void setFieldLabelVector(const QVector<std::tr1::shared_ptr<FieldData> >& fieldLabelVector);

	/**
	* Получить строку для tooltip
	* @return строка для tooltip
	*/
	QString getStringValue() const;

	/**
	* Получить значение rowguid записи таблицы, куда ссылается ключ
	* @return значение rowguid записи таблицы, куда ссылается ключ
	*/
	QString getRowGuid() const;

private:
	QString m_fieldName;		///< Имя поля
	QString m_foreignTable;		///< Имя таблицы, куда ссылается ключ
	const QVector<std::tr1::shared_ptr<FieldMetaData> > m_fieldMetaDataVector;	///< Метаданные полей данных, куда ссылается ключ
	QVector<QVariant> m_dataVector;	///< Поля, куда ссылается ключ
	QMap<QString, BinaryDataDescription> m_binaryDataDescriptionMap;	///< Информация о двоичных полях, куда ссылается ключ
	QString m_tableViewName;	///< Имя таблицы
	QVector<std::tr1::shared_ptr<FieldData> > m_fieldLabelVector; ///< Имя полей записи таблицы, куда ссылается ключ(для GUI)
};

/**
* @class ResolvePacketData
* Пакет разрешения конфликта синхронизации
*/
class UPELIB_EXPORT ResolvePacketData : public QObject
{
	Q_OBJECT
public:
	enum ResolveType
	{
		UndefineResolveType = -1,
		InsertResolveType = 0,
		InsertErrorResolveType,
		UpdateResolveType,
		UpdateErrorResolveType,
		DeleteResolveType,
		DeleteErrorResolveType,
		UpdateConflictsResolveType,
		UpdateDeleteConflictResolveType
	};	///< Тип конфликта

	enum SourceIdentifier
	{
		SourceIsUndefined = -1,
		// Сводка:
		//     Изменение данных возникло на издателе.
		SourceIsPublisher = 0,
		//
		// Сводка:
		//     Изменение данных возникло на подписчике.
		SourceIsSubscriber = 1
	};	///< Тип источника изменений

	enum ResolveResult
	{
		UndefineResolveResult = 0,
		AcceptResolveResult,
		RejectResolveResult,
		CustomResolveResult,
		Custom2ResolveResult
	};	///< Решение конфликта

	/**
	* Конструктор
	* @param resolveType тип конфликта
	* @param sourceIdentifier тип источника изменений
	* @param publisherName имя публикатора
	* @param subscriberName имя подписчика
	* @param publisherDBName имя базы данных публикатора
	* @param subscriberDBName имя базы данных подписчика
	* @param tableName имя таблицы
	* @param errorCode код ошибки
	* @param errorMessage сообщение ошибки
	* @param fieldMetaDataVector метаданные полей записи таблицы
	* @param sourceDataVector поля записи для сихронизации от инициатора
	* @param source2DataVector поля записи для сихронизации от получателя
	* @param result решение конфликта
	* @param parent родительский объект
	*/
	ResolvePacketData(ResolveType resolveType, SourceIdentifier sourceIdentifier, const QString&  publisherName, const QString&  subscriberName,
		const QString& publisherDBName, const QString& subscriberDBName, const QString& tableName, int errorCode, const QString& errorMessage,
		const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector, const QVector<QVariant>& sourceDataVector,
		const QVector<QVariant>& source2DataVector, ResolveResult result, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~ResolvePacketData();

	/**
	* Получить тип конфликта
	* @return тип конфликта
	*/
	ResolveType getResolveType() const;

	/**
	* Получить тип источника изменений
	* @return тип источника изменений
	*/
	SourceIdentifier getSourceIdentifier() const;

	/**
	* Получить имя публикатора
	* @return имя публикатора
	*/
	const QString& getPublisherName() const;

	/**
	* Получить имя подписчика
	* @return имя подписчика
	*/
	const QString& getSubscriberName() const;

	/**
	* Получить имя базы данных публикатора
	* @return
	*/
	const QString& getPublisherDBName() const;

	/**
	* Получить имя базы данных подписчика
	* @return имя базы данных подписчика
	*/
	const QString& getSubscriberDBName() const;

	/**
	* Получить имя таблицы
	* @return имя таблицы
	*/
	const QString& getTableName() const;

	/**
	* Получить код ошибки
	* @return код ошибки
	*/
	int getErrorCode() const;

	/**
	* Получить сообщение ошибки
	* @return сообщение ошибки
	*/
	const QString& getErrorMessage() const;

	/**
	* Получить метаданные полей записи таблицы
	* @return метаданные полей записи таблицы
	*/
	const QVector<std::tr1::shared_ptr<FieldMetaData> >& getFieldMetaDataVector() const;

	/**
	* Получить поля записи для сихронизации от инициатора
	* @return поля записи для сихронизации от инициатора
	*/
	const QVector<QVariant>& getSourceDataVector() const;

	/**
	* Получить поля записи для сихронизации от получателя
	* @return поля записи для сихронизации от получателя
	*/
	const QVector<QVariant>& getSource2DataVector() const;

	/**
	* Установить информацию о следующих возможных значениях primary keys для инициатора
	* @param sourcePrimaryNextValues следующие возможные значения primary keys для инициатора
	*/
	void setSourcePrimaryNextValues(const QVector<PrimaryNextValue>& sourcePrimaryNextValues);

	/**
	* Получить информацию о следующих возможных значениях primary keys для инициатора
	* @return следующие возможные значения primary keys для инициатора
	*/
	const QVector<PrimaryNextValue>& getSourcePrimaryNextValues() const;

	/**
	* Установить информацию о следующих возможных значениях primary keys для получателя
	* @param source2PrimaryNextValues следующие возможные значения primary keys для получателя
	*/
	void setSource2PrimaryNextValues(const QVector<PrimaryNextValue>& source2PrimaryNextValues);

	/**
	* Получить информацию о следующих возможных значениях primary keys для получателя
	* @return следующие возможные значения primary keys для получателя
	*/
	const QVector<PrimaryNextValue>& getSource2PrimaryNextValues() const;

	/**
	* Установить информацию о вторичных ключах для инициатора
	* @param sourceDataSetForeignKeys информация о вторичных ключах для инициатора
	*/
	void setSourceDataSetForeignKeys(const QVector<ForeignKeyDataSet>& sourceDataSetForeignKeys);

	/**
	* Получить информацию о вторичных ключах для инициатора
	* @return информация о вторичных ключах для инициатора
	*/
	const QVector<ForeignKeyDataSet>& getSourceDataSetForeignKeys() const;

	/**
	* Установить информацию о вторичных ключах для получателя
	* @param source2DataSetForeignKeys информация о вторичных ключах для получателя
	*/
	void setSource2DataSetForeignKeys(const QVector<ForeignKeyDataSet>& source2DataSetForeignKeys);

	/**
	* Получить информацию о вторичных ключах для получателя
	* @return информация о вторичных ключах для получателя
	*/
	const QVector<ForeignKeyDataSet>& getSource2DataSetForeignKeys() const;

	/**
	* Установить информацию о двоичных полях для инициатора
	* @param sourceBinaryDataDescriptions информация о двоичных полях для инициатора
	*/
	void setSourceBinaryDataDescriptions(const QVector<BinaryDataDescription>& sourceBinaryDataDescriptions);

	/**
	* Получить информацию о двоичных полях для получателя
	* @return информация о двоичных полях для получателя
	*/
	BinaryDataDescription getSourceBinaryDataDescription(const QString& name) const;

	/**
	* Установить информацию о двоичных полях для получателя
	* @param source2BinaryDataDescriptions информация о двоичных полях для получателя
	*/
	void setSource2BinaryDataDescriptions(const QVector<BinaryDataDescription>& source2BinaryDataDescriptions);

	/**
	* Получить информацию о двоичных полях для получателя
	* @return информация о двоичных полях для получателя
	*/
	BinaryDataDescription getSource2BinaryDataDescription(const QString& name) const;

	/**
	* Получить максимальное значение для первичного ключа по имени поля
	* @param fieldName имя поля для первичного ключа
	* @return максимальное значение для первичного ключа
	*/
	qint64 getMaxSourcesPrimaryNextValue(const QString& fieldName);

	/**
	* Получить решение конфликта
	* @return решение конфликта
	*/
	ResolveResult getResult() const;

	/**
	* Получить строку tooltip для вторичного ключа по имени поля для инициатора
	* @param fieldName имя поля для вторичного ключа
	* @return строка tooltip
	*/
	QString getSourceToolTipString(const QString& fieldName) const;

	/**
	* Получить строку tooltip для вторичного ключа по имени поля для получателя
	* @param fieldName имя поля для вторичного ключа
	* @return строка tooltip
	*/
	QString getSource2ToolTipString(const QString& fieldName) const;

protected:
	ResolveType m_resolveType;				///< Тип конфликта
	SourceIdentifier m_sourceIdentifier;	///< Источника изменений
	QString m_publisherName;	///< Имя публикатора
	QString m_subscriberName;	///< Имя подписчика
	QString m_publisherDBName;	///< Имя базы данных публикатора
	QString m_subscriberDBName;	///< Имя базы данных подписчика
	QString m_tableName;		///< Имя таблицы
	int m_errorCode;			///< Код ошибки
	QString m_errorMessage;		///< Сообщение ошибки
	QVector<std::tr1::shared_ptr<FieldMetaData> > m_fieldMetaDataVector;	///< Метаданные полей записи таблицы
	QVector<QVariant> m_sourceDataVector;	///< Поля записи для сихронизации от инициатора
	QVector<QVariant> m_source2DataVector;	///< Поля записи для сихронизации от получателя
	QVector<PrimaryNextValue> m_sourcePrimaryNextValues;	///< Информацию о следующих возможных значениях primary keys для инициатора
	QVector<PrimaryNextValue> m_source2PrimaryNextValues;	///< Информацию о следующих возможных значениях primary keys для получателя
	QVector<ForeignKeyDataSet> m_sourceDataSetForeignKeys;	///< Информация о вторичных ключах для инициатора
	QVector<ForeignKeyDataSet> m_source2DataSetForeignKeys;	///< Информация о вторичных ключах для получателя
	QMap<QString, BinaryDataDescription> m_sourceBinaryDataDescriptionMap;	///< Информация о двоичных полях для инициатора
	QMap<QString, BinaryDataDescription> m_source2BinaryDataDescriptionMap;	///< Информация о двоичных полях для получателя

	ResolveResult m_result;		///< Решение конфликта
};

#endif // RESOLVEPACKETDATA_H
