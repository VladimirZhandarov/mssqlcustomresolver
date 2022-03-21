#ifndef XSDRESOLVEPACKETPARSER_H
#define XSDRESOLVEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "resolvepacketdata.h"

/**
* @class XsdResolvePacketParser
* Parser ������ ���������� ��������� �������������
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
	* �����������
	* @param parent ������������ ������
	*/
	XsdResolvePacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdResolvePacketParser();

	/**
	* �������� ����� ���������� ��������� ������������� (������ �� ���������� ���������)
	* @return ����� ���������� ��������� �������������
	*/
	std::tr1::shared_ptr<ResolvePacketData> getPacket() const;

	/**
	* ��������� ������
	* @param content ������
	* @return ������ ������� ��������
	*/
	bool parse(const QString& content);

	/**
	* ��������� �������� (�������)
	* @return �����
	*/
	QByteArray setAcceptResult();

	/**
	* ��������� �������� (��������)
	* @return �����
	*/
	QByteArray setRejectResult();

	/**
	* ��������� �������� ("���������������� ������� 1")
	* @return �����
	*/
	QByteArray setCustomResult();

	/**
	* ��������� �������� ("���������������� ������� 2")
	* @return �����
	*/
	QByteArray setCustom2Result();

private:
	/**
	* ��������� xml-������� � �������� ���������� ����� ������ �������
	* @param dataSetElement xml-�������
	* @return ���������� ����� ������ �������
	*/
	QVector<std::tr1::shared_ptr<FieldMetaData> > createFieldMetaDataVector(QDomElement& dataSetElement) const;

	/**
	* ��������� xml-������� � �������� ��� �������
	* @param dataSetElement xml-�������
	* @return ��� �������
	*/
	QString getTableTagName(QDomElement& dataSetElement) const;

	/**
	* ��������� xml-������� � �������� ������ ������ �������
	* @param dataSetElement xml-�������
	* @param tableTagName ��� xml-������� ��� ������ �������
	* @param fieldMetaDataVector ���������� ����� ������ �������
	* @return ������ ����� ������ �������
	*/
	QVector<QVariant> createDataVector(QDomElement& sourceDataSetElement, const QString& tableTagName,
		const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector) const;

	/**
	* ���������� ������� ���������
	* @param resolveResult ������� ���������
	*/
	void setResult(ResolvePacketData::ResolveResult resolveResult);

	/**
	* ������������� �������� xml � ������������������ ����
	* @return ������������������ ����
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<ResolvePacketData> m_packetData;	///< ����� ���������� ��������� �������������
};

#endif // XSDRESOLVEPACKETPARSER_H
