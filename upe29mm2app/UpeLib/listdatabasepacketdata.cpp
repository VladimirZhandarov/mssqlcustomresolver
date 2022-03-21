#include "listdatabasepacketdata.h"

ListDatabasePacketData::ListDatabasePacketData(const QString& username, const QString& password, ListDatabaseResult result, QObject* parent) : QObject(parent),
	m_username(username), m_password(password), m_result(result)
{

}

ListDatabasePacketData::~ListDatabasePacketData()
{

}

const QString& ListDatabasePacketData::getUsername() const
{
	return m_username;
}

const QString& ListDatabasePacketData::getPassword() const
{
	return m_password;
}

void ListDatabasePacketData::setDBNames(const QStringList& dbNames)
{
	m_dbNames = dbNames;
}

const QStringList& ListDatabasePacketData::getDBNames() const
{
	return m_dbNames;
}

void ListDatabasePacketData::setErrorMessage(const QString& errorMessage)
{
	m_errorMessage = errorMessage;
}

const QString& ListDatabasePacketData::getErrorMessage() const
{
	return m_errorMessage;
}

ListDatabasePacketData::ListDatabaseResult ListDatabasePacketData::getResult() const
{
	return m_result;
}
