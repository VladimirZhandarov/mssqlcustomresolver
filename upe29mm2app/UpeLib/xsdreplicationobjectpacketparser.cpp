#include "xsdreplicationobjectpacketparser.h"

QString XsdReplicationObjectPacketParser::replicationObjectPacketTagName = "ReplicationObjectPacket";

QString XsdReplicationObjectPacketParser::usernameTagName = "username";
QString XsdReplicationObjectPacketParser::passwordTagName = "password";
QString XsdReplicationObjectPacketParser::dbNamesTagName = "dbNames";
QString XsdReplicationObjectPacketParser::replicationObjectDataSetTagName = "ReplicationObjectDataSet";
QString XsdReplicationObjectPacketParser::errorMessageTagName = "errorMessage";
QString XsdReplicationObjectPacketParser::resultTagName = "result";

QString XsdReplicationObjectPacketParser::dbNameTagName = "dbName";
QString XsdReplicationObjectPacketParser::tablesTagName = "tables";
QString XsdReplicationObjectPacketParser::storedProceduresTagName = "storedProcedures";
QString XsdReplicationObjectPacketParser::userFunctionsTagName = "userFunctions";
QString XsdReplicationObjectPacketParser::stringTagName = "string";

QString XsdReplicationObjectPacketParser::createPublicationTagName = "createPublication";
QString XsdReplicationObjectPacketParser::subscribersTagName = "subscribers";

static const char* ReplicationObjectResultEnumStrings[] =
{
	"UndefineReplicationObjectResult",
	"AcceptReplicationObjectResult",
	"RejectReplicationObjectResult",
	0
};

XsdReplicationObjectPacketParser::XsdReplicationObjectPacketParser(QObject* parent) : QObject(parent)
{

}

XsdReplicationObjectPacketParser::~XsdReplicationObjectPacketParser()
{

}

std::tr1::shared_ptr<ReplicationObjectPacketData> XsdReplicationObjectPacketParser::getPacket() const
{
	return m_packetData;
}

QByteArray XsdReplicationObjectPacketParser::createPacket(const std::tr1::shared_ptr<ReplicationObjectPacketData>& packetData)
{
	m_packetData = packetData;
	m_doc.clear();
	QDomElement replicationObjectPacketElement = m_doc.createElement(replicationObjectPacketTagName);
	QDomElement usernameElement = m_doc.createElement(usernameTagName);
	replicationObjectPacketElement.appendChild(usernameElement);

	QDomElement passwordElement = m_doc.createElement(passwordTagName);
	replicationObjectPacketElement.appendChild(passwordElement);

	QDomElement dbNamesElement = m_doc.createElement(dbNamesTagName);
	const QList<ReplicationObjectDataSet>& dbNames = m_packetData->getDBNames();
	for(int i = 0; i < dbNames.size(); i++)
	{
		QDomElement itemElement = m_doc.createElement(replicationObjectDataSetTagName);
		QDomElement dbNameElement = m_doc.createElement(dbNameTagName);
		QDomCharacterData data = m_doc.createTextNode(dbNames[i].getDBName());
		dbNameElement.appendChild(data);
		itemElement.appendChild(dbNameElement);
		dbNamesElement.appendChild(itemElement);
	}
	replicationObjectPacketElement.appendChild(dbNamesElement);
	
	QDomElement resultElement = m_doc.createElement(resultTagName);
	replicationObjectPacketElement.appendChild(resultElement);

	QDomElement createPublicationElement = m_doc.createElement(createPublicationTagName);
	replicationObjectPacketElement.appendChild(createPublicationElement);

	m_doc.appendChild(replicationObjectPacketElement);


	setUsername(m_packetData->getUsername());
	setPassword(m_packetData->getPassword());
	setResult(m_packetData->getResult());
	setCreatePublication(m_packetData->isCreatePublication());
	return docToByteArray();
}

bool XsdReplicationObjectPacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<ReplicationObjectPacketData>();

	m_doc.setContent(content);
	QDomElement replicationObjectPacketElement = m_doc.firstChildElement(replicationObjectPacketTagName);
	if(replicationObjectPacketElement.isNull())
	{
		return false;
	}

	QDomElement usernameElement = replicationObjectPacketElement.firstChildElement(usernameTagName);
	QString username = "";
	if(!usernameElement.isNull())
	{
		username = usernameElement.firstChild().toCharacterData().data();
	}

	QDomElement passwordElement = replicationObjectPacketElement.firstChildElement(passwordTagName);
	QString password = "";
	if(!passwordElement.isNull())
	{
		password = passwordElement.firstChild().toCharacterData().data();
	}
	
	QList<ReplicationObjectDataSet> dbNames;
	QDomElement dbNamesElement = replicationObjectPacketElement.firstChildElement(dbNamesTagName);
	if(!dbNamesElement.isNull())
	{
		for(QDomElement replicationObjectDataElement = dbNamesElement.firstChildElement(replicationObjectDataSetTagName); !replicationObjectDataElement.isNull();
			replicationObjectDataElement = replicationObjectDataElement.nextSiblingElement(replicationObjectDataSetTagName))
		{
			QDomElement dbNameElement = replicationObjectDataElement.firstChildElement(dbNameTagName);
			QString dbName = "";
			if(!dbNameElement.isNull())
			{
				dbName = dbNameElement.firstChild().toCharacterData().data();
			}
			QStringList tableList;
			QDomElement tablesElement = replicationObjectDataElement.firstChildElement(tablesTagName);
			if(!tablesElement.isNull())
			{
				for(QDomElement tableElement = tablesElement.firstChildElement(stringTagName); !tableElement.isNull();
					tableElement = tableElement.nextSiblingElement(stringTagName))
				{
					if(!tableElement.isNull())
					{
						QString tableName = tableElement.firstChild().toCharacterData().data();
						tableList.append(tableName);
					}
				}
			}

			QStringList storedProcedureList;
			QDomElement storedProceduresElement = replicationObjectDataElement.firstChildElement(storedProceduresTagName);
			if(!storedProceduresElement.isNull())
			{
				for(QDomElement storedProcedureElement = storedProceduresElement.firstChildElement(stringTagName); !storedProcedureElement.isNull();
					storedProcedureElement = storedProcedureElement.nextSiblingElement(stringTagName))
				{
					if(!storedProcedureElement.isNull())
					{
						QString storedProcedureName = storedProcedureElement.firstChild().toCharacterData().data();
						storedProcedureList.append(storedProcedureName);
					}
				}
			}

			QStringList userFunctionList;
			QDomElement userFunctionsElement = replicationObjectDataElement.firstChildElement(userFunctionsTagName);
			if(!userFunctionsElement.isNull())
			{
				for(QDomElement userFunctionElement = userFunctionsElement.firstChildElement(stringTagName); !userFunctionElement.isNull();
					userFunctionElement = userFunctionElement.nextSiblingElement(stringTagName))
				{
					if(!userFunctionElement.isNull())
					{
						QString userFunctionName = userFunctionElement.firstChild().toCharacterData().data();
						userFunctionList.append(userFunctionName);
					}
				}
			}
			dbNames.append(ReplicationObjectDataSet(dbName, tableList, storedProcedureList, userFunctionList));
		}
	}

	QString errorMessage = "";
	QDomElement errorMessageElement = replicationObjectPacketElement.firstChildElement(errorMessageTagName);
	if(!errorMessageElement.isNull())
	{
		errorMessage = errorMessageElement.firstChild().toCharacterData().data();
	}

	QDomElement resultElement = replicationObjectPacketElement.firstChildElement(resultTagName);
	if(resultElement.isNull())
	{
		return false;
	}
	QString resultString = resultElement.firstChild().toCharacterData().data();
	ReplicationObjectPacketData::ReplicationObjectResult resultType = ReplicationObjectPacketData::ReplicationObjectResult::UndefineReplicationObjectResult;
	int resultTypeIndex = resultType;
	while(ReplicationObjectResultEnumStrings[resultTypeIndex] != 0)
	{
		if(ReplicationObjectResultEnumStrings[resultTypeIndex] == resultString)
		{
			resultType = (ReplicationObjectPacketData::ReplicationObjectResult)resultTypeIndex;
			break;
		}
		resultTypeIndex++;
	}
	m_packetData = std::tr1::shared_ptr<ReplicationObjectPacketData>(new ReplicationObjectPacketData(username, password, resultType));
	m_packetData->setDBNames(dbNames);
	m_packetData->setErrorMessage(errorMessage);
	return true;
}

void XsdReplicationObjectPacketParser::setUsername(const QString& username)
{
	QDomElement replicationObjectPacketElement = m_doc.firstChildElement(replicationObjectPacketTagName);
	if(replicationObjectPacketElement.isNull())
	{
		return;
	}
	QDomElement usernameElement = replicationObjectPacketElement.firstChildElement(usernameTagName);
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

void XsdReplicationObjectPacketParser::setPassword(const QString& password)
{
	QDomElement replicationObjectPacketElement = m_doc.firstChildElement(replicationObjectPacketTagName);
	if(replicationObjectPacketElement.isNull())
	{
		return;
	}
	QDomElement passwordElement = replicationObjectPacketElement.firstChildElement(passwordTagName);
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

void XsdReplicationObjectPacketParser::setResult(ReplicationObjectPacketData::ReplicationObjectResult replicationObjectResult)
{
	QDomElement replicationObjectPacketElement = m_doc.firstChildElement(replicationObjectPacketTagName);
	if(replicationObjectPacketElement.isNull())
	{
		return;
	}
	QDomElement resultElement = replicationObjectPacketElement.firstChildElement(resultTagName);
	QDomCharacterData data = resultElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(ReplicationObjectResultEnumStrings[replicationObjectResult]);
	}
	else
	{
		data = m_doc.createTextNode(ReplicationObjectResultEnumStrings[replicationObjectResult]);
		resultElement.appendChild(data);
	}
}

void XsdReplicationObjectPacketParser::setCreatePublication(bool createPublication)
{

}

QByteArray XsdReplicationObjectPacketParser::docToByteArray() const
{
	QByteArray data = m_doc.toByteArray();
	data.replace('\0', "&#x0;");
	return data;
}
