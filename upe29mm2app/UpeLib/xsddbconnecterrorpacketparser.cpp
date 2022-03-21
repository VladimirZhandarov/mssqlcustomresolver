#include "xsddbconnecterrorpacketparser.h"

QString XsdDBConnectErrorPacketParser::dbConnectErrorPacketTagName = "DBConnectErrorPacket";

QString XsdDBConnectErrorPacketParser::publisherTagName = "publisher";
QString XsdDBConnectErrorPacketParser::serverNameTagName = "serverName";
QString XsdDBConnectErrorPacketParser::dbNameTagName = "dbName";
QString XsdDBConnectErrorPacketParser::tableNameTagName = "tableName";
QString XsdDBConnectErrorPacketParser::userNameTagName = "userName";


XsdDBConnectErrorPacketParser::XsdDBConnectErrorPacketParser(QObject* parent) : QObject(parent)
{

}

XsdDBConnectErrorPacketParser::~XsdDBConnectErrorPacketParser()
{

}

std::tr1::shared_ptr<DBConnectErrorPacketData> XsdDBConnectErrorPacketParser::getPacket() const
{
	return m_packetData;
}

bool XsdDBConnectErrorPacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<DBConnectErrorPacketData>();

	m_doc.setContent(content);
	QDomElement dbConnectErrorPacketElement = m_doc.firstChildElement(dbConnectErrorPacketTagName);
	if(dbConnectErrorPacketElement.isNull())
	{
		return false;
	}

	QDomElement publisherElement = dbConnectErrorPacketElement.firstChildElement(publisherTagName);
	if(publisherElement.isNull())
	{
		return false;
	}

	QString publisherString = publisherElement.firstChild().toCharacterData().data();
	bool publisher = false;
	if(publisherString.toLower() == "true")
	{
		publisher = true;
	}

	QString serverName = "";
	QDomElement serverNameElement = dbConnectErrorPacketElement.firstChildElement(serverNameTagName);
	if(!serverNameElement.isNull())
	{
		serverName = serverNameElement.firstChild().toCharacterData().data();
	}

	QString dbName = "";
	QDomElement dbNameElement = dbConnectErrorPacketElement.firstChildElement(dbNameTagName);
	if(!dbNameElement.isNull())
	{
		dbName = dbNameElement.firstChild().toCharacterData().data();
	}

	QString tableName = "";
	QDomElement tableNameElement = dbConnectErrorPacketElement.firstChildElement(tableNameTagName);
	if(!tableNameElement.isNull())
	{
		tableName = tableNameElement.firstChild().toCharacterData().data();
	}

	QString userName = "";
	QDomElement userNameElement = dbConnectErrorPacketElement.firstChildElement(userNameTagName);
	if(!userNameElement.isNull())
	{
		userName = userNameElement.firstChild().toCharacterData().data();
	}

	m_packetData = std::tr1::shared_ptr<DBConnectErrorPacketData>(new DBConnectErrorPacketData(publisher, serverName, dbName, tableName, userName));
	return true;
}
