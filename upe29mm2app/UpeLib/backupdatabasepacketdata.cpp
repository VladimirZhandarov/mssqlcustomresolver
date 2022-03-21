#include "backupdatabasepacketdata.h"

BackupDatabasePacketData::BackupDatabasePacketData(const QString& username, const QString& password, BackupDatabaseResult result, QObject* parent) :
	QObject(parent), m_username(username), m_password(password), m_result(result)
{

}

BackupDatabasePacketData::~BackupDatabasePacketData()
{

}

const QString& BackupDatabasePacketData::getUsername() const
{
	return m_username;
}

const QString& BackupDatabasePacketData::getPassword() const
{
	return m_password;
}

void BackupDatabasePacketData::setDBName(const QString& dbName)
{
	m_dbName = dbName;
}

const QString& BackupDatabasePacketData::getDBName() const
{
	return m_dbName;
}

void BackupDatabasePacketData::setMessage(const QString& message)
{
	m_message = message;
}

const QString& BackupDatabasePacketData::getMessage() const
{
	return m_message;
}

BackupDatabasePacketData::BackupDatabaseResult BackupDatabasePacketData::getResult() const
{
	return m_result;
}
