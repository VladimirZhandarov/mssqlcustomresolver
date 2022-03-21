#include "restoredatabasepacketdata.h"

RestoreDatabasePacketData::RestoreDatabasePacketData(const QString& username, const QString& password, RestoreDatabaseResult result, QObject* parent) :
	QObject(parent), m_username(username), m_password(password), m_result(result)
{

}

RestoreDatabasePacketData::~RestoreDatabasePacketData()
{

}

const QString& RestoreDatabasePacketData::getUsername() const
{
	return m_username;
}

const QString& RestoreDatabasePacketData::getPassword() const
{
	return m_password;
}

void RestoreDatabasePacketData::setDBName(const QString& dbName)
{
	m_dbName = dbName;
}

const QString& RestoreDatabasePacketData::getDBName() const
{
	return m_dbName;
}

void RestoreDatabasePacketData::setMessage(const QString& message)
{
	m_message = message;
}

const QString& RestoreDatabasePacketData::getMessage() const
{
	return m_message;
}

RestoreDatabasePacketData::RestoreDatabaseResult RestoreDatabasePacketData::getResult() const
{
	return m_result;
}
