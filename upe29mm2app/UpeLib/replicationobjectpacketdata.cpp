#include "replicationobjectpacketdata.h"

ReplicationObjectDataSet::ReplicationObjectDataSet(const QString& dbName, const QStringList& tableList, const QStringList& storedProcedureList,
	const QStringList& userFunctionList) : m_dbName(dbName), m_tableList(tableList), m_storedProcedureList(storedProcedureList),
	m_userFunctionList(userFunctionList)
{

}

ReplicationObjectDataSet::~ReplicationObjectDataSet()
{

}

const QString& ReplicationObjectDataSet::getDBName() const
{
	return m_dbName;
}

const QStringList& ReplicationObjectDataSet::getTableList() const
{
	return m_tableList;
}

const QStringList& ReplicationObjectDataSet::getStoredProcedureList() const
{
	return m_storedProcedureList;
}

const QStringList& ReplicationObjectDataSet::getUserFunctionList() const
{
	return m_userFunctionList;
}

ReplicationObjectPacketData::ReplicationObjectPacketData(const QString& username, const QString& password, ReplicationObjectResult result, QObject* parent) :
	QObject(parent), m_username(username), m_password(password), m_result(result), m_createPublication(false)
{

}

ReplicationObjectPacketData::~ReplicationObjectPacketData()
{

}

const QString& ReplicationObjectPacketData::getUsername() const
{
	return m_username;
}

const QString& ReplicationObjectPacketData::getPassword() const
{
	return m_password;
}

void ReplicationObjectPacketData::setDBNames(const QList<ReplicationObjectDataSet>& dbNames)
{
	m_dbNames = dbNames;
}

const QList<ReplicationObjectDataSet>& ReplicationObjectPacketData::getDBNames() const
{
	return m_dbNames;
}

void ReplicationObjectPacketData::setErrorMessage(const QString& errorMessage)
{
	m_errorMessage = errorMessage;
}

const QString& ReplicationObjectPacketData::getErrorMessage() const
{
	return m_errorMessage;
}

ReplicationObjectPacketData::ReplicationObjectResult ReplicationObjectPacketData::getResult() const
{
	return m_result;
}

bool ReplicationObjectPacketData::isCreatePublication() const
{
	return m_createPublication;
}

void ReplicationObjectPacketData::setSubscriberList(QStringList subscriberList)
{
	m_createPublication = true;
	m_subscriberList = subscriberList;
}
