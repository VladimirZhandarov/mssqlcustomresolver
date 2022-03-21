#include "listserverpacketdata.h"

ListServerPacketData::ListServerPacketData(ListServerResult result, QObject* parent) : QObject(parent), m_result(result)
{

}

ListServerPacketData::~ListServerPacketData()
{

}

void ListServerPacketData::setServerNames(const QStringList& serverNames)
{
	m_serverNames = serverNames;
}

const QStringList& ListServerPacketData::getServerNames() const
{
	return m_serverNames;
}

void ListServerPacketData::setErrorMessage(const QString& errorMessage)
{
	m_errorMessage = errorMessage;
}

const QString& ListServerPacketData::getErrorMessage() const
{
	return m_errorMessage;
}

ListServerPacketData::ListServerResult ListServerPacketData::getResult() const
{
	return m_result;
}
