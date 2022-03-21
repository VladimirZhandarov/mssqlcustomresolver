#include "xsdmanagepacketparser.h"

QString XsdManagePacketParser::managePacketTagName = "ManagePacket";

QString XsdManagePacketParser::manageTypeTagName = "manageType";
QString XsdManagePacketParser::mainClientTagName = "mainClient";
QString XsdManagePacketParser::ownResolveTagName = "ownResolve";
QString XsdManagePacketParser::dbServerInstanceNameTagName = "dbServerInstanceName";
QString XsdManagePacketParser::dbNameTagName = "dbName";
QString XsdManagePacketParser::languageTagName = "language";
QString XsdManagePacketParser::errorCodeTagName = "errorCode";
QString XsdManagePacketParser::errorMessageTagName = "errorMessage";
QString XsdManagePacketParser::resultTagName = "result";

static const char* ManageTypeEnumStrings[] =
{
	"StartManageType",
	"StopManageType",
	"NextStepManageType",
	"NextStepEndedManageType",
	"StartMainClientType",
	"StopMainClientType",
	"BackupDatabase",
	"RestoreDatabase",
	0
};

static const char* ManageResultEnumStrings[] =
{
	"UndefineManageResult",
	"AcceptManageResult",
	"RejectManageResult",
	"SnapShotNeededManageResult",
	"SnapShotEndedManageResult",
	0
};

XsdManagePacketParser::XsdManagePacketParser(QObject* parent) : QObject(parent)
{

}

XsdManagePacketParser::~XsdManagePacketParser()
{

}

std::tr1::shared_ptr<ManagePacketData> XsdManagePacketParser::getPacket() const
{
	return m_packetData;
}

QByteArray XsdManagePacketParser::createPacket(const std::tr1::shared_ptr<ManagePacketData>& packetData)
{
	m_packetData = packetData;
	m_doc.clear();
	QDomElement managePacketElement = m_doc.createElement(managePacketTagName);
	QDomElement manageTypeElement = m_doc.createElement(manageTypeTagName);
	managePacketElement.appendChild(manageTypeElement);

	QDomElement mainClientElement = m_doc.createElement(mainClientTagName);
	managePacketElement.appendChild(mainClientElement);

	QDomElement ownResolveElement = m_doc.createElement(ownResolveTagName);
	managePacketElement.appendChild(ownResolveElement);

	QDomElement dbServerInstanceNameElement = m_doc.createElement(dbServerInstanceNameTagName);
	managePacketElement.appendChild(dbServerInstanceNameElement);

	QDomElement dbNameElement = m_doc.createElement(dbNameTagName);
	managePacketElement.appendChild(dbNameElement);

	QDomElement languageElement = m_doc.createElement(languageTagName);
	managePacketElement.appendChild(languageElement);

	QDomElement resultElement = m_doc.createElement(resultTagName);
	managePacketElement.appendChild(resultElement);
	m_doc.appendChild(managePacketElement);

	setType(m_packetData->getManageType());
	setMainClient(m_packetData->isMainClient());
	setOwnResolve(m_packetData->isOwnResolve());
	setDBServerInstanceName(m_packetData->getDBServerInstanceName());
	setDBName(m_packetData->getDBName());
	setResult(m_packetData->getResult());

	return docToByteArray();
}

bool XsdManagePacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<ManagePacketData>();

	m_doc.setContent(content);
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return false;
	}

	QDomElement manageTypeElement = managePacketElement.firstChildElement(manageTypeTagName);
	if(manageTypeElement.isNull())
	{
		return false;
	}
	QString resolveTypeString = manageTypeElement.firstChild().toCharacterData().data();
	ManagePacketData::ManageType manageType = ManagePacketData::ManageType::UndefineManageType;
	int manageTypeIndex = manageType + 1;
	while(ManageTypeEnumStrings[manageTypeIndex] != 0)
	{
		if(ManageTypeEnumStrings[manageTypeIndex] == resolveTypeString)
		{
			manageType = (ManagePacketData::ManageType)manageTypeIndex;
			break;
		}
		manageTypeIndex++;
	}

	QDomElement mainClientElement = managePacketElement.firstChildElement(mainClientTagName);
	if(mainClientElement.isNull())
	{
		return false;
	}
	QString mainClientString = mainClientElement.firstChild().toCharacterData().data();
	bool mainClientValue = false;
	if(mainClientString.toLower() == "true")
	{
		mainClientValue = true;
	}

	QDomElement ownResolveElement = managePacketElement.firstChildElement(ownResolveTagName);
	if(ownResolveElement.isNull())
	{
		return false;
	}
	QString ownResolveString = ownResolveElement.firstChild().toCharacterData().data();
	bool ownResolveValue = true;
	if(ownResolveString.toLower() == "false")
	{
		ownResolveValue = false;
	}

	QDomElement dbServerInstanceNameElement = managePacketElement.firstChildElement(dbServerInstanceNameTagName);
	QString dbServerInstanceName = "";
	if(!dbServerInstanceNameElement.isNull())
	{
		dbServerInstanceName = dbServerInstanceNameElement.firstChild().toCharacterData().data();
	}

	QDomElement dbNameElement = managePacketElement.firstChildElement(dbNameTagName);
	QString dbName = "";
	if(!dbNameElement.isNull())
	{
		dbName = dbNameElement.firstChild().toCharacterData().data();
	}

	QDomElement languageElement = managePacketElement.firstChildElement(languageTagName);
	QString language = "";
	if(!languageElement.isNull())
	{
		language = languageElement.firstChild().toCharacterData().data();
	}
	
	int errorCode = 0;
	QDomElement errorCodeElement = managePacketElement.firstChildElement(errorCodeTagName);
	if(!errorCodeElement.isNull())
	{
		errorCode = errorCodeElement.firstChild().toCharacterData().data().toInt();
	}

	QString errorMessage = "";
	QDomElement errorMessageElement = managePacketElement.firstChildElement(errorMessageTagName);
	if(!errorMessageElement.isNull())
	{
		errorMessage = errorMessageElement.firstChild().toCharacterData().data();
	}

	QDomElement resultElement = managePacketElement.firstChildElement(resultTagName);
	if(resultElement.isNull())
	{
		return false;
	}
	QString resultString = resultElement.firstChild().toCharacterData().data();
	ManagePacketData::ManageResult resultType = ManagePacketData::ManageResult::UndefineManageResult;
	int resultTypeIndex = resultType;
	while(ManageResultEnumStrings[resultTypeIndex] != 0)
	{
		if(ManageResultEnumStrings[resultTypeIndex] == resultString)
		{
			resultType = (ManagePacketData::ManageResult)resultTypeIndex;
			break;
		}
		resultTypeIndex++;
	}
	m_packetData = std::tr1::shared_ptr<ManagePacketData>(new ManagePacketData(manageType, dbServerInstanceName, resultType));
	m_packetData->setMainClient(mainClientValue);
	m_packetData->setOwnResolve(ownResolveValue);
	m_packetData->setDBName(dbName);
	m_packetData->setLanguage(language);
	m_packetData->setErrorCode(errorCode);
	m_packetData->setErrorMessage(errorMessage);
	return true;
}

QByteArray XsdManagePacketParser::setAcceptResult()
{
	setResult(ManagePacketData::ManageResult::AcceptManageResult);
	return docToByteArray();
}

QByteArray XsdManagePacketParser::setRejectResult()
{
	setResult(ManagePacketData::ManageResult::RejectManageResult);
	return docToByteArray();
}

void XsdManagePacketParser::setType(ManagePacketData::ManageType manageType)
{
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return;
	}
	QDomElement manageTypeElement = managePacketElement.firstChildElement(manageTypeTagName);
	QDomCharacterData data = manageTypeElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(ManageTypeEnumStrings[manageType]);
	}
	else
	{
		data = m_doc.createTextNode(ManageTypeEnumStrings[manageType]);
		manageTypeElement.appendChild(data);
	}
}

void XsdManagePacketParser::setMainClient(bool mainClient)
{
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return;
	}
	QDomElement mainClientElement = managePacketElement.firstChildElement(mainClientTagName);
	QDomCharacterData data = mainClientElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		if(mainClient)
		{
			data.setData("true");
		}
		else
		{
			data.setData("false");
		}
	}
	else
	{
		if(mainClient)
		{
			data = m_doc.createTextNode("true");
		}
		else
		{
			data = m_doc.createTextNode("false");
		}
		mainClientElement.appendChild(data);
	}
}

void XsdManagePacketParser::setOwnResolve(bool ownResolve)
{
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return;
	}
	QDomElement ownResolveElement = managePacketElement.firstChildElement(ownResolveTagName);
	QDomCharacterData data = ownResolveElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		if(ownResolve)
		{
			data.setData("true");
		}
		else
		{
			data.setData("false");
		}
	}
	else
	{
		if(ownResolve)
		{
			data = m_doc.createTextNode("true");
		}
		else
		{
			data = m_doc.createTextNode("false");
		}
		ownResolveElement.appendChild(data);
	}
}

void XsdManagePacketParser::setDBServerInstanceName(const QString& dbServerInstanceName)
{
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return;
	}
	QDomElement dbServerInstanceNameElement = managePacketElement.firstChildElement(dbServerInstanceNameTagName);
	QDomCharacterData data = dbServerInstanceNameElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(dbServerInstanceName);
	}
	else
	{
		data = m_doc.createTextNode(dbServerInstanceName);
		dbServerInstanceNameElement.appendChild(data);
	}
}

void XsdManagePacketParser::setDBName(const QString& dbName)
{
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return;
	}
	QDomElement dbNameElement = managePacketElement.firstChildElement(dbNameTagName);
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

void XsdManagePacketParser::setLanguage(const QString& language)
{
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return;
	}
	QDomElement languageElement = managePacketElement.firstChildElement(languageTagName);
	QDomCharacterData data = languageElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(language);
	}
	else
	{
		data = m_doc.createTextNode(language);
		languageElement.appendChild(data);
	}
}

void XsdManagePacketParser::setResult(ManagePacketData::ManageResult manageResult)
{
	QDomElement managePacketElement = m_doc.firstChildElement(managePacketTagName);
	if(managePacketElement.isNull())
	{
		return;
	}
	QDomElement resultElement = managePacketElement.firstChildElement(resultTagName);
	QDomCharacterData data = resultElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(ManageResultEnumStrings[manageResult]);
	}
	else
	{
		data = m_doc.createTextNode(ManageResultEnumStrings[manageResult]);
		resultElement.appendChild(data);
	}
}

QByteArray XsdManagePacketParser::docToByteArray() const
{
	QByteArray data = m_doc.toByteArray();
	data.replace('\0', "&#x0;");
	return data;
}
