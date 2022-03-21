#include "managepacketdata.h"

#include <QtCore/QLocale>

ManagePacketData::ManagePacketData(ManageType manageType, const QString& dbServerInstanceName, ManageResult result, QObject* parent) : QObject(parent),
	m_manageType(manageType), m_mainClient(false), m_ownResolve(true), m_dbServerInstanceName(dbServerInstanceName), m_errorCode(0), m_result(result)
{
	m_language = QLocale::system().name();
}

ManagePacketData::~ManagePacketData()
{

}

ManagePacketData::ManageType ManagePacketData::getManageType() const
{
	return m_manageType;
}

void ManagePacketData::setMainClient(bool mainClient)
{
	m_mainClient = mainClient;
}

bool ManagePacketData::isMainClient() const
{
	return m_mainClient;
}

void ManagePacketData::setOwnResolve(bool ownResolve)
{
	m_ownResolve = ownResolve;
}

bool ManagePacketData::isOwnResolve() const
{
	return m_ownResolve;
}

const QString& ManagePacketData::getDBServerInstanceName() const
{
	return m_dbServerInstanceName;
}

void ManagePacketData::setDBName(const QString& dbName)
{
	m_dbName = dbName;
}

const QString& ManagePacketData::getDBName() const
{
	return m_dbName;
}

void ManagePacketData::setLanguage(const QString& language)
{
	m_language = language;
}

const QString& ManagePacketData::getLanguage() const
{
	return m_language;
}

void ManagePacketData::setErrorCode(int errorCode)
{
	m_errorCode = errorCode;
}

int ManagePacketData::getErrorCode() const
{
	return m_errorCode;
}

void ManagePacketData::setErrorMessage(const QString& errorMessage)
{
	m_errorMessage = errorMessage;
}

const QString& ManagePacketData::getErrorMessage() const
{
	return m_errorMessage;
}

ManagePacketData::ManageResult ManagePacketData::getResult() const
{
	return m_result;
}
