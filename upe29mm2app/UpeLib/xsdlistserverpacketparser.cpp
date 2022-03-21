#include "xsdlistserverpacketparser.h"

QString XsdListServerPacketParser::listServerPacketTagName = "ListServerPacket";

QString XsdListServerPacketParser::serverNamesTagName = "serverNames";
QString XsdListServerPacketParser::stringTagName = "string";
QString XsdListServerPacketParser::errorMessageTagName = "errorMessage";
QString XsdListServerPacketParser::resultTagName = "result";

static const char* ListServerResultEnumStrings[] =
{
	"UndefineListServerResult",
	"AcceptListServerResult",
	"RejectListServerResult",
	0
};

XsdListServerPacketParser::XsdListServerPacketParser(QObject* parent) : QObject(parent)
{

}

XsdListServerPacketParser::~XsdListServerPacketParser()
{

}

std::tr1::shared_ptr<ListServerPacketData> XsdListServerPacketParser::getPacket() const
{
	return m_packetData;
}

QByteArray XsdListServerPacketParser::createPacket(const std::tr1::shared_ptr<ListServerPacketData>& packetData)
{
	m_packetData = packetData;
	m_doc.clear();
	QDomElement listServerPacketElement = m_doc.createElement(listServerPacketTagName);

	QDomElement resultElement = m_doc.createElement(resultTagName);
	listServerPacketElement.appendChild(resultElement);
	m_doc.appendChild(listServerPacketElement);

	setResult(m_packetData->getResult());
	return docToByteArray();
}

bool XsdListServerPacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<ListServerPacketData>();

	m_doc.setContent(content);
	QDomElement listServerPacketElement = m_doc.firstChildElement(listServerPacketTagName);
	if(listServerPacketElement.isNull())
	{
		return false;
	}

	QStringList serverNames;
	QDomElement serverNamesElement = listServerPacketElement.firstChildElement(serverNamesTagName);
	if(!serverNamesElement.isNull())
	{
		for(QDomElement stringElement = serverNamesElement.firstChildElement(stringTagName); !stringElement.isNull();
			stringElement = stringElement.nextSiblingElement(stringTagName))
		{
			QString serverName = stringElement.firstChild().toCharacterData().data();
			if(!serverName.isEmpty())
			{
				serverNames.append(serverName);
			}
		}
	}

	QString errorMessage = "";
	QDomElement errorMessageElement = listServerPacketElement.firstChildElement(errorMessageTagName);
	if(!errorMessageElement.isNull())
	{
		errorMessage = errorMessageElement.firstChild().toCharacterData().data();
	}

	QDomElement resultElement = listServerPacketElement.firstChildElement(resultTagName);
	if(resultElement.isNull())
	{
		return false;
	}
	QString resultString = resultElement.firstChild().toCharacterData().data();
	ListServerPacketData::ListServerResult resultType = ListServerPacketData::ListServerResult::UndefineListServerResult;
	int resultTypeIndex = resultType;
	while(ListServerResultEnumStrings[resultTypeIndex] != 0)
	{
		if(ListServerResultEnumStrings[resultTypeIndex] == resultString)
		{
			resultType = (ListServerPacketData::ListServerResult)resultTypeIndex;
			break;
		}
		resultTypeIndex++;
	}
	m_packetData = std::tr1::shared_ptr<ListServerPacketData>(new ListServerPacketData(resultType));
	m_packetData->setServerNames(serverNames);
	m_packetData->setErrorMessage(errorMessage);
	return true;
}

void XsdListServerPacketParser::setResult(ListServerPacketData::ListServerResult listServerResult)
{
	QDomElement listServerPacketlement = m_doc.firstChildElement(listServerPacketTagName);
	if(listServerPacketlement.isNull())
	{
		return;
	}
	QDomElement resultElement = listServerPacketlement.firstChildElement(resultTagName);
	QDomCharacterData data = resultElement.firstChild().toCharacterData();
	if(!data.isNull())
	{
		data.setData(ListServerResultEnumStrings[listServerResult]);
	}
	else
	{
		data = m_doc.createTextNode(ListServerResultEnumStrings[listServerResult]);
		resultElement.appendChild(data);
	}
}

QByteArray XsdListServerPacketParser::docToByteArray() const
{
	QByteArray data = m_doc.toByteArray();
	data.replace('\0', "&#x0;");
	return data;
}
