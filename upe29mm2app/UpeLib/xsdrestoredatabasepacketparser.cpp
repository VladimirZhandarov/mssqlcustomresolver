#include "xsdrestoredatabasepacketparser.h"

QString XsdRestoreDatabasePacketParser::restoreDatabasePacketTagName = "RestoreDatabasePacket";

QString XsdRestoreDatabasePacketParser::usernameTagName = "username";
QString XsdRestoreDatabasePacketParser::passwordTagName = "password";
QString XsdRestoreDatabasePacketParser::dbNameTagName = "dbName";
QString XsdRestoreDatabasePacketParser::messageTagName = "message";
QString XsdRestoreDatabasePacketParser::resultTagName = "result";

static const char* RestoreDatabaseResultEnumStrings[] =
{
	"UndefineRestoreDatabaseResult",
	"AcceptRestoreDatabaseResult",
	"RejectRestoreDatabaseResult",
	0
};

XsdRestoreDatabasePacketParser::XsdRestoreDatabasePacketParser(QObject* parent) : QObject(parent)
{

}

XsdRestoreDatabasePacketParser::~XsdRestoreDatabasePacketParser()
{

}

std::tr1::shared_ptr<RestoreDatabasePacketData> XsdRestoreDatabasePacketParser::getPacket() const
{
	return m_packetData;
}

QByteArray XsdRestoreDatabasePacketParser::createPacket(const std::tr1::shared_ptr<RestoreDatabasePacketData>& packetData)
{
	m_packetData = packetData;
	m_doc.clear();
	QDomElement restoreDatabasePacketElement = m_doc.createElement(restoreDatabasePacketTagName);
	QDomElement usernameElement = m_doc.createElement(usernameTagName);
	restoreDatabasePacketElement.appendChild(usernameElement);

	QDomElement passwordElement = m_doc.createElement(passwordTagName);
	restoreDatabasePacketElement.appendChild(passwordElement);

	QDomElement dbNameElement = m_doc.createElement(dbNameTagName);
	restoreDatabasePacketElement.appendChild(dbNameElement);

	QDomElement resultElement = m_doc.createElement(resultTagName);
	restoreDatabasePacketElement.appendChild(resultElement);
	m_doc.appendChild(restoreDatabasePacketElement);

	setUsername(m_packetData->getUsername());
	setPassword(m_packetData->getPassword());
	setDBName(m_packetData->getDBName());
	setResult(m_packetData->getResult());
	return docToByteArray();
}

bool XsdRestoreDatabasePacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<RestoreDatabasePacketData>();

	m_doc.setContent(content);
	QDomElement restoreDatabasePacketElement = m_doc.firstChildElement(restoreDatabasePacketTagName);
	if(restoreDatabasePacketElement.isNull())
	{
		return false;
	}

	QDomElement usernameElement = restoreDatabasePacketElement.firstChildElement(usernameTagName);
	QString username = "";
	if(!usernameElement.isNull())
	{
		username = usernameElement.firstChild().toCharacterData().data();
	}

	QDomElement passwordElement = restoreDatabasePacketElement.firstChildElement(passwordTagName);
	QString password = "";
	if(!passwordElement.isNull())
	{
		password = passwordElement.firstChild().toCharacterData().data();
	}


	QDomElement dbNameElement = restoreDatabasePacketElement.firstChildElement(dbNameTagName);
	QString dbName = "";
	if(!dbNameElement.isNull())
	{
		dbName = dbNameElement.firstChild().toCharacterData().data();
	}

	QDomElement messageElement = restoreDatabasePacketElement.firstChildElement(messageTagName);
	QString message = "";
	if(!messageElement.isNull())
	{
		message = messageElement.firstChild().toCharacterData().data();
	}

	QDomElement resultElement = restoreDatabasePacketElement.firstChildElement(resultTagName);
	if(resultElement.isNull())
	{
		return false;
	}
	QString resultString = resultElement.firstChild().toCharacterData().data();
	RestoreDatabasePacketData::RestoreDatabaseResult resultType = RestoreDatabasePacketData::RestoreDatabaseResult::UndefineRestoreDatabaseResult;
	int resultTypeIndex = resultType;
	while (RestoreDatabaseResultEnumStrings[resultTypeIndex] != 0)
	{
		if(RestoreDatabaseResultEnumStrings[resultTypeIndex] == resultString)
		{
			resultType = (RestoreDatabasePacketData::RestoreDatabaseResult)resultTypeIndex;
			break;
		}
		resultTypeIndex++;
	}
	m_packetData = std::tr1::shared_ptr<RestoreDatabasePacketData>(new RestoreDatabasePacketData(username, password, resultType));
	m_packetData->setDBName(dbName);
	m_packetData->setMessage(message);
	return true;
}

void XsdRestoreDatabasePacketParser::setUsername(const QString& username)
{
	QDomElement restoreDatabasePacketElement = m_doc.firstChildElement(restoreDatabasePacketTagName);
	if(restoreDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement usernameElement = restoreDatabasePacketElement.firstChildElement(usernameTagName);
	QDomCharacterData data = usernameElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(username);
	}
	else
	{
		data = m_doc.createTextNode(username);
		usernameElement.appendChild(data);
	}
}

void XsdRestoreDatabasePacketParser::setPassword(const QString& password)
{
	QDomElement restoreDatabasePacketElement = m_doc.firstChildElement(restoreDatabasePacketTagName);
	if(restoreDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement passwordElement = restoreDatabasePacketElement.firstChildElement(passwordTagName);
	QDomCharacterData data = passwordElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(password);
	}
	else
	{
		data = m_doc.createTextNode(password);
		passwordElement.appendChild(data);
	}
}

void XsdRestoreDatabasePacketParser::setDBName(const QString& dbName)
{
	QDomElement restoreDatabasePacketElement = m_doc.firstChildElement(restoreDatabasePacketTagName);
	if(restoreDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement dbNameElement = restoreDatabasePacketElement.firstChildElement(dbNameTagName);
	QDomCharacterData data = dbNameElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(dbName);
	}
	else
	{
		data = m_doc.createTextNode(dbName);
		dbNameElement.appendChild(data);
	}
}

void XsdRestoreDatabasePacketParser::setResult(RestoreDatabasePacketData::RestoreDatabaseResult restoreDatabaseResult)
{
	QDomElement restoreDatabasePacketElement = m_doc.firstChildElement(restoreDatabasePacketTagName);
	if(restoreDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement resultElement = restoreDatabasePacketElement.firstChildElement(resultTagName);
	QDomCharacterData data = resultElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(RestoreDatabaseResultEnumStrings[restoreDatabaseResult]);
	}
	else
	{
		data = m_doc.createTextNode(RestoreDatabaseResultEnumStrings[restoreDatabaseResult]);
		resultElement.appendChild(data);
	}
}

QByteArray XsdRestoreDatabasePacketParser::docToByteArray() const
{
	QByteArray data = m_doc.toByteArray();
	data.replace('\0', "&#x0;");
	return data;
}
