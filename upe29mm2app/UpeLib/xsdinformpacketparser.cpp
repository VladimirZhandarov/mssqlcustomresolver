#include "xsdinformpacketparser.h"

QString XsdInformPacketParser::informPacketTagName = "InformPacket";

QString XsdInformPacketParser::errorCodeTagName = "errorCode";
QString XsdInformPacketParser::messageTagName = "message";

XsdInformPacketParser::XsdInformPacketParser(QObject* parent) : QObject(parent)
{

}

XsdInformPacketParser::~XsdInformPacketParser()
{

}

std::tr1::shared_ptr<InformPacketData> XsdInformPacketParser::getPacket() const
{
	return m_packetData;
}

bool XsdInformPacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<InformPacketData>();

	m_doc.setContent(content);
	QDomElement informPacketElement = m_doc.firstChildElement(informPacketTagName);
	if(informPacketElement.isNull())
	{
		return false;
	}

	int errorCode = 0;
	QDomElement errorCodeElement = informPacketElement.firstChildElement(errorCodeTagName);
	if(!errorCodeElement.isNull())
	{
		errorCode = errorCodeElement.firstChild().toCharacterData().data().toInt();
	}

	QString message = "";
	QDomElement messageElement = informPacketElement.firstChildElement(messageTagName);
	if(!messageElement.isNull())
	{
		message = messageElement.firstChild().toCharacterData().data();
	}
	m_packetData = std::tr1::shared_ptr<InformPacketData>(new InformPacketData(errorCode, message));
	return true;
}
