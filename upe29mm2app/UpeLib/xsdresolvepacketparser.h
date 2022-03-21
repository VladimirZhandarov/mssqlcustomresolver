#ifndef XSDRESOLVEPACKETPARSER_H
#define XSDRESOLVEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "resolvepacketdata.h"

/**
* @class XsdResolvePacketParser
* Parser пакета разрешения конфликта синхронизации
*/
class UPELIB_EXPORT XsdResolvePacketParser : public QObject
{
	Q_OBJECT
	static QString resolvePacketTagName;

	static QString resolveTypeTagName;
	static QString sourceIdentifierTagName;
	static QString publisherNameTagName;
	static QString subscriberNameTagName;
	static QString publisherDBNameTagName;
	static QString subscriberDBNameTagName;

	static QString errorCodeTagName;
	static QString errorMessageTagName;
	static QString sourceDataSetTagName;
	static QString source2DataSetTagName;
	static QString sourcePrimaryNextValuesTagName;
	static QString source2PrimaryNextValuesTagName;
	static QString sourceDataSetForeignKeysTagName;
	static QString source2DataSetForeignKeysTagName;
	static QString binaryDataDescriptionsTagName;
	static QString sourceBinaryDataDescriptionsTagName;
	static QString source2BinaryDataDescriptionsTagName;

	static QString resultTagName;

	static QString primaryNextValueTagName;
	static QString foreignKeyDataSetTagName;
	static QString binaryDataDescriptionTagName;
	static QString fieldNameTagName;
	static QString nextValueTagName;
	static QString dataSetTagName;
	static QString dataSizeTagName;
	static QString equalDataTagName;
	static QString nullDataTagName;

	static QString xsSchemaTagName;
	static QString xsElementTagName;
	static QString xsComplexTypeTagName;
	static QString xsChoiceTagName;
	static QString xsSequenceTagName;

	static QString idAttrName;
	static QString nameAttrName;
	static QString typeAttrName;
	static QString minOccursAttrName;

	static QString diffgrDiffgramTagName;
	static QString diffgrIdAttrName;
	static QString rowguidAttrName;

public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	XsdResolvePacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdResolvePacketParser();

	/**
	* Получить пакет разрешения конфликта синхронизации (запрос на разрешение конфликта)
	* @return пакет разрешения конфликта синхронизации
	*/
	std::tr1::shared_ptr<ResolvePacketData> getPacket() const;

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

	/**
	* Разрешить конфликт (Принять)
	* @return пакет
	*/
	QByteArray setAcceptResult();

	/**
	* Разрешить конфликт (Отменить)
	* @return пакет
	*/
	QByteArray setRejectResult();

	/**
	* Разрешить конфликт ("Пользовательское решение 1")
	* @return пакет
	*/
	QByteArray setCustomResult();

	/**
	* Разрешить конфликт ("Пользовательское решение 2")
	* @return пакет
	*/
	QByteArray setCustom2Result();

private:
	/**
	* Разобрать xml-элемент и получить метаданные полей записи таблицы
	* @param dataSetElement xml-элемент
	* @return метаданные полей записи таблицы
	*/
	QVector<std::tr1::shared_ptr<FieldMetaData> > createFieldMetaDataVector(QDomElement& dataSetElement) const;

	/**
	* Разобрать xml-элемент и получить имя таблицы
	* @param dataSetElement xml-элемент
	* @return имя таблицы
	*/
	QString getTableTagName(QDomElement& dataSetElement) const;

	/**
	* Разобрать xml-элемент и получить данные записи таблицы
	* @param dataSetElement xml-элемент
	* @param tableTagName имя xml-элемент для записи таблицы
	* @param fieldMetaDataVector метаданные полей записи таблицы
	* @return данные полей записи таблицы
	*/
	QVector<QVariant> createDataVector(QDomElement& sourceDataSetElement, const QString& tableTagName,
		const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector) const;

	/**
	* Установить решение конфликта
	* @param resolveResult решение конфликта
	*/
	void setResult(ResolvePacketData::ResolveResult resolveResult);

	/**
	* Преобразовать документ xml в последовательность байт
	* @return последовательность байт
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<ResolvePacketData> m_packetData;	///< Пакет разрешения конфликта синхронизации
};

#endif // XSDRESOLVEPACKETPARSER_H
