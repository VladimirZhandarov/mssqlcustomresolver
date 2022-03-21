#include "informpacketdata.h"

InformPacketData::InformPacketData(int errorCode, const QString& message, QObject* parent) : QObject(parent), m_errorCode(errorCode), m_message(message)
{

}

InformPacketData::~InformPacketData()
{

}

int InformPacketData::getErrorCode() const
{
	return m_errorCode;
}

const QString& InformPacketData::getMessage() const
{
	return m_message;
}
