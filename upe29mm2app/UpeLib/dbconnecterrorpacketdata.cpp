#include "dbconnecterrorpacketdata.h"

DBConnectErrorPacketData::DBConnectErrorPacketData(bool publisher, const QString& serverName, const QString& dbName, const QString& tableName,
	const QString& userName, QObject* parent) : QObject(parent), m_publisher(publisher), m_serverName(serverName), m_dbName(dbName), m_tableName(tableName),
	m_userName(userName)
{

}

DBConnectErrorPacketData::~DBConnectErrorPacketData()
{

}

bool DBConnectErrorPacketData::isPulisher() const
{
	return m_publisher;
}

const QString& DBConnectErrorPacketData::getServerName() const
{
	return m_serverName;
}

const QString& DBConnectErrorPacketData::getDBName() const
{
	return m_dbName;
}

const QString& DBConnectErrorPacketData::getTableName() const
{
	return m_tableName;
}

const QString& DBConnectErrorPacketData::getUserName() const
{
	return m_userName;
}