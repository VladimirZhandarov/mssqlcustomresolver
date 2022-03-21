#ifndef XSDDBCONNECTERRORPACKETPARSER_H
#define XSDDBCONNECTERRORPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "dbconnecterrorpacketdata.h"

/**
* @class XsdDBConnectErrorPacketParser
* Parser пакета с ошибкой соединения с базой данных
*/
class UPELIB_EXPORT  XsdDBConnectErrorPacketParser : public QObject
{
	Q_OBJECT
	static QString dbConnectErrorPacketTagName;

	static QString publisherTagName;
	static QString serverNameTagName;
	static QString dbNameTagName;
	static QString tableNameTagName;
	static QString userNameTagName;

public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	XsdDBConnectErrorPacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdDBConnectErrorPacketParser();

	/**
	* Получить пакет с ошибкой соединения с базой данных
	* @return пакет с ошибкой соединения с базой данных
	*/
	std::tr1::shared_ptr<DBConnectErrorPacketData> getPacket() const;

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

private:
	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<DBConnectErrorPacketData> m_packetData;	///< Пакет с ошибкой соединения с базой данных
};

#endif // XSDDBCONNECTERRORPACKETPARSER_H
