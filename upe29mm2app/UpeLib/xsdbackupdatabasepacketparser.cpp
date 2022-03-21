#include "xsdbackupdatabasepacketparser.h"

QString XsdBackupDatabasePacketParser::backupDatabasePacketTagName = "BackupDatabasePacket";

QString XsdBackupDatabasePacketParser::usernameTagName = "username";
QString XsdBackupDatabasePacketParser::passwordTagName = "password";
QString XsdBackupDatabasePacketParser::dbNameTagName = "dbName";
QString XsdBackupDatabasePacketParser::messageTagName = "message";
QString XsdBackupDatabasePacketParser::resultTagName = "result";

static const char* BackupDatabaseResultEnumStrings[] =
{
	"UndefineBackupDatabaseResult",
	"AcceptBackupDatabaseResult",
	"RejectBackupDatabaseResult",
	0
};

XsdBackupDatabasePacketParser::XsdBackupDatabasePacketParser(QObject* parent) : QObject(parent)
{

}

XsdBackupDatabasePacketParser::~XsdBackupDatabasePacketParser()
{

}

std::tr1::shared_ptr<BackupDatabasePacketData> XsdBackupDatabasePacketParser::getPacket() const
{
	return m_packetData;
}

QByteArray XsdBackupDatabasePacketParser::createPacket(const std::tr1::shared_ptr<BackupDatabasePacketData>& packetData)
{
	m_packetData = packetData;
	m_doc.clear();
	QDomElement backupDatabasePacketElement = m_doc.createElement(backupDatabasePacketTagName);
	QDomElement usernameElement = m_doc.createElement(usernameTagName);
	backupDatabasePacketElement.appendChild(usernameElement);

	QDomElement passwordElement = m_doc.createElement(passwordTagName);
	backupDatabasePacketElement.appendChild(passwordElement);

	QDomElement dbNameElement = m_doc.createElement(dbNameTagName);
	backupDatabasePacketElement.appendChild(dbNameElement);

	QDomElement resultElement = m_doc.createElement(resultTagName);
	backupDatabasePacketElement.appendChild(resultElement);
	m_doc.appendChild(backupDatabasePacketElement);

	setUsername(m_packetData->getUsername());
	setPassword(m_packetData->getPassword());
	setDBName(m_packetData->getDBName());
	setResult(m_packetData->getResult());
	return docToByteArray();
}

bool XsdBackupDatabasePacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<BackupDatabasePacketData>();

	m_doc.setContent(content);
	QDomElement backupDatabasePacketElement = m_doc.firstChildElement(backupDatabasePacketTagName);
	if(backupDatabasePacketElement.isNull())
	{
		return false;
	}

	QDomElement usernameElement = backupDatabasePacketElement.firstChildElement(usernameTagName);
	QString username = "";
	if(!usernameElement.isNull())
	{
		username = usernameElement.firstChild().toCharacterData().data();
	}

	QDomElement passwordElement = backupDatabasePacketElement.firstChildElement(passwordTagName);
	QString password = "";
	if(!passwordElement.isNull())
	{
		password = passwordElement.firstChild().toCharacterData().data();
	}


	QDomElement dbNameElement = backupDatabasePacketElement.firstChildElement(dbNameTagName);
	QString dbName = "";
	if(!dbNameElement.isNull())
	{
		dbName = dbNameElement.firstChild().toCharacterData().data();
	}

	QDomElement messageElement = backupDatabasePacketElement.firstChildElement(messageTagName);
	QString message = "";
	if(!messageElement.isNull())
	{
		message = messageElement.firstChild().toCharacterData().data();
	}

	QDomElement resultElement = backupDatabasePacketElement.firstChildElement(resultTagName);
	if(resultElement.isNull())
	{
		return false;
	}
	QString resultString = resultElement.firstChild().toCharacterData().data();
	BackupDatabasePacketData::BackupDatabaseResult resultType = BackupDatabasePacketData::BackupDatabaseResult::UndefineBackupDatabaseResult;
	int resultTypeIndex = resultType;
	while (BackupDatabaseResultEnumStrings[resultTypeIndex] != 0)
	{
		if(BackupDatabaseResultEnumStrings[resultTypeIndex] == resultString)
		{
			resultType = (BackupDatabasePacketData::BackupDatabaseResult)resultTypeIndex;
			break;
		}
		resultTypeIndex++;
	}
	m_packetData = std::tr1::shared_ptr<BackupDatabasePacketData>(new BackupDatabasePacketData(username, password, resultType));
	m_packetData->setDBName(dbName);
	m_packetData->setMessage(message);
	return true;
}

void XsdBackupDatabasePacketParser::setUsername(const QString& username)
{
	QDomElement backupDatabasePacketElement = m_doc.createElement(backupDatabasePacketTagName);
	if(backupDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement usernameElement = backupDatabasePacketElement.firstChildElement(usernameTagName);
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

void XsdBackupDatabasePacketParser::setPassword(const QString& password)
{
	QDomElement backupDatabasePacketElement = m_doc.createElement(backupDatabasePacketTagName);
	if(backupDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement passwordElement = backupDatabasePacketElement.firstChildElement(passwordTagName);
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

void XsdBackupDatabasePacketParser::setDBName(const QString& dbName)
{
	QDomElement backupDatabasePacketElement = m_doc.firstChildElement(backupDatabasePacketTagName);
	if(backupDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement dbNameElement = backupDatabasePacketElement.firstChildElement(dbNameTagName);
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

void XsdBackupDatabasePacketParser::setResult(BackupDatabasePacketData::BackupDatabaseResult backupDatabaseResult)
{
	QDomElement backupDatabasePacketElement = m_doc.firstChildElement(backupDatabasePacketTagName);
	if(backupDatabasePacketElement.isNull())
	{
		return;
	}
	QDomElement resultElement = backupDatabasePacketElement.firstChildElement(resultTagName);
	QDomCharacterData data = resultElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(BackupDatabaseResultEnumStrings[backupDatabaseResult]);
	}
	else
	{
		data = m_doc.createTextNode(BackupDatabaseResultEnumStrings[backupDatabaseResult]);
		resultElement.appendChild(data);
	}
}

QByteArray XsdBackupDatabasePacketParser::docToByteArray() const
{
	QByteArray data = m_doc.toByteArray();
	data.replace('\0', "&#x0;");
	return data;
}
