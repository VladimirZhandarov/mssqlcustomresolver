#include "xsdlistdatabasepacketparser.h"

QString XsdListDatabasePacketParser::listDatabasePacketTagName = "ListDatabasePacket";

QString XsdListDatabasePacketParser::usernameTagName = "username";
QString XsdListDatabasePacketParser::passwordTagName = "password";
QString XsdListDatabasePacketParser::dbNamesTagName = "dbNames";
QString XsdListDatabasePacketParser::stringTagName = "string";
QString XsdListDatabasePacketParser::errorMessageTagName = "errorMessage";
QString XsdListDatabasePacketParser::resultTagName = "result";

static const char* ListDatabaseResultEnumStrings[] =
{
	"UndefineListDatabaseResult",
	"AcceptListDatabaseResult",
	"RejectListDatabaseResult",
	0
};

XsdListDatabasePacketParser::XsdListDatabasePacketParser(QObject* parent) : QObject(parent)
{

}

XsdListDatabasePacketParser::~XsdListDatabasePacketParser()
{

}

std::tr1::shared_ptr<ListDatabasePacketData> XsdListDatabasePacketParser::getPacket() const
{
	return m_packetData;
}

QByteArray XsdListDatabasePacketParser::createPacket(const std::tr1::shared_ptr<ListDatabasePacketData>& packetData)
{
	m_packetData = packetData;
	m_doc.clear();
	QDomElement listDatabasePacketElement = m_doc.createElement(listDatabasePacketTagName);
	QDomElement usernameElement = m_doc.createElement(usernameTagName);
	listDatabasePacketElement.appendChild(usernameElement);

	QDomElement passwordElement = m_doc.createElement(passwordTagName);
	listDatabasePacketElement.appendChild(passwordElement);

	QDomElement resultElement = m_doc.createElement(resultTagName);
	listDatabasePacketElement.appendChild(resultElement);
	m_doc.appendChild(listDatabasePacketElement);

	setUsername(m_packetData->getUsername());
	setPassword(m_packetData->getPassword());
	setResult(m_packetData->getResult());
	return docToByteArray();
}

bool XsdListDatabasePacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<ListDatabasePacketData>();

	m_doc.setContent(content);
	QDomElement listDatabasePacketElement = m_doc.firstChildElement(listDatabasePacketTagName);
	if(listDatabasePacketElement.isNull())
	{
		return false;
	}

	QDomElement usernameElement = listDatabasePacketElement.firstChildElement(usernameTagName);
	QString username = "";
	if(!usernameElement.isNull())
	{
		username = usernameElement.firstChild().toCharacterData().data();
	}

	QDomElement passwordElement = listDatabasePacketElement.firstChildElement(passwordTagName);
	QString password = "";
	if(!passwordElement.isNull())
	{
		password = passwordElement.firstChild().toCharacterData().data();
	}

	QStringList dbNames;
	QDomElement dbNamesElement = listDatabasePacketElement.firstChildElement(dbNamesTagName);
	if(!dbNamesElement.isNull())
	{
		for(QDomElement stringElement = dbNamesElement.firstChildElement(stringTagName); !stringElement.isNull();
			stringElement = stringElement.nextSiblingElement(stringTagName))
		{
			QString dbName = stringElement.firstChild().toCharacterData().data();
			if(!dbName.isEmpty())
			{
				dbNames.append(dbName);
			}
		}
	}

	QString errorMessage = "";
	QDomElement errorMessageElement = listDatabasePacketElement.firstChildElement(errorMessageTagName);
	if(!errorMessageElement.isNull())
	{
		errorMessage = errorMessageElement.firstChild().toCharacterData().data();
	}

	QDomElement resultElement = listDatabasePacketElement.firstChildElement(resultTagName);
	if(resultElement.isNull())
	{
		return false;
	}
	QString resultString = resultElement.firstChild().toCharacterData().data();
	ListDatabasePacketData::ListDatabaseResult resultType = ListDatabasePacketData::ListDatabaseResult::UndefineListDatabaseResult;
	int resultTypeIndex = resultType;
	while(ListDatabaseResultEnumStrings[resultTypeIndex] != 0)
	{
		if(ListDatabaseResultEnumStrings[resultTypeIndex] == resultString)
		{
			resultType = (ListDatabasePacketData::ListDatabaseResult)resultTypeIndex;
			break;
		}
		resultTypeIndex++;
	}
	m_packetData = std::tr1::shared_ptr<ListDatabasePacketData>(new ListDatabasePacketData(username, password, resultType));
	m_packetData->setDBNames(dbNames);
	m_packetData->setErrorMessage(errorMessage);
	return true;
}

void XsdListDatabasePacketParser::setUsername(const QString& username)
{
	QDomElement listDatabasePacketlement = m_doc.firstChildElement(listDatabasePacketTagName);
	if(listDatabasePacketlement.isNull())
	{
		return;
	}
	QDomElement usernameElement = listDatabasePacketlement.firstChildElement(usernameTagName);
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

void XsdListDatabasePacketParser::setPassword(const QString& password)
{
	QDomElement listDatabasePacketlement = m_doc.firstChildElement(listDatabasePacketTagName);
	if(listDatabasePacketlement.isNull())
	{
		return;
	}
	QDomElement passwordElement = listDatabasePacketlement.firstChildElement(passwordTagName);
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

void XsdListDatabasePacketParser::setResult(ListDatabasePacketData::ListDatabaseResult listDatabaseResult)
{
	QDomElement listDatabasePacketlement = m_doc.firstChildElement(listDatabasePacketTagName);
	if(listDatabasePacketlement.isNull())
	{
		return;
	}
	QDomElement resultElement = listDatabasePacketlement.firstChildElement(resultTagName);
	QDomCharacterData data = resultElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(ListDatabaseResultEnumStrings[listDatabaseResult]);
	}
	else
	{
		data = m_doc.createTextNode(ListDatabaseResultEnumStrings[listDatabaseResult]);
		resultElement.appendChild(data);
	}
}

QByteArray XsdListDatabasePacketParser::docToByteArray() const
{
	QByteArray data = m_doc.toByteArray();
	data.replace('\0', "&#x0;");
	return data;
}
