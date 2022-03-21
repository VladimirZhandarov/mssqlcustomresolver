#ifndef XSDREPLICATIONOBJECTPACKETPARSER_H
#define XSDREPLICATIONOBJECTPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "replicationobjectpacketdata.h"

class UPELIB_EXPORT XsdReplicationObjectPacketParser : public QObject
{
	Q_OBJECT
	static QString replicationObjectPacketTagName;

	static QString usernameTagName;
	static QString passwordTagName;
	static QString dbNamesTagName;
	static QString replicationObjectDataSetTagName;
	static QString errorMessageTagName;
	static QString resultTagName;

	static QString dbNameTagName;
	static QString tablesTagName;
	static QString storedProceduresTagName;
	static QString userFunctionsTagName;
	static QString stringTagName;

	static QString createPublicationTagName;
	static QString subscribersTagName;

public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	XsdReplicationObjectPacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdReplicationObjectPacketParser();

	std::tr1::shared_ptr<ReplicationObjectPacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<ReplicationObjectPacketData>& packetData);

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

private:
	void setUsername(const QString& username);
	void setPassword(const QString& password);
	void setResult(ReplicationObjectPacketData::ReplicationObjectResult replicationObjectResult);
	void setCreatePublication(bool createPublication);

	/**
	* Преобразовать документ xml в последовательность байт
	* @return последовательность байт
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<ReplicationObjectPacketData> m_packetData;
};

#endif // XSDREPLICATIONOBJECTPACKETPARSER_H
