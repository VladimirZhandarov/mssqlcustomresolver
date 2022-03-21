#include "osqlsettingsreader.h"

#include <QtCore/QFile>

QString OsqlSettingsReader::settingsTagName = "Settings";
QString OsqlSettingsReader::hostNameTagName = "hostName";
QString OsqlSettingsReader::portTagName = "port";
QString OsqlSettingsReader::dbServerInstanceNameTagName = "DBServerInstanceName";
QString OsqlSettingsReader::dbNamesTagName = "DBNames";
QString OsqlSettingsReader::dbNameTagName = "DBName";
QString OsqlSettingsReader::lastBackupDirTagName = "lastBackupDir";
QString OsqlSettingsReader::lastBackupUseUsernameTagName = "lastBackupUseUsername";
QString OsqlSettingsReader::lastRestoreDirTagName = "lastRestoreDir";
QString OsqlSettingsReader::lastRestoreUseUsernameTagName = "lastRestoreUseUsername";

OsqlSettingsReader::OsqlSettingsReader(const QString& filename, QObject* parent) : QObject(parent), m_filename(filename)
{
	m_hostName = "localhost";
	m_port = 55001;
	m_dbServerInstanceName = "";
	m_dbNameList.append("UPE_MM2_Administration2");
	m_dbNameList.append("UPE_MM2_Catalog");
	m_dbNameList.append("UPE_MM2_Documents2");
	m_dbNameList.append("UPE_MM2_Formular");
	m_dbNameList.append("UPE_MM2_ItemsAccount");
	m_dbNameList.append("UPE_MM2_JPS");
	m_dbNameList.append("UPE_MM2_Organization");
	m_dbNameList.append("UPE_MM2_Planning");
	m_dbNameList.append("UPE_MM2_Reglament2");

	QFile file(m_filename);
	if(file.open(QIODevice::ReadOnly))
	{
		m_doc.setContent(&file);
	}
	else
	{
		save();
		return;
	}

	QDomElement settingsDomElement = m_doc.firstChildElement(settingsTagName);
	if(settingsDomElement.isNull())
	{
		return;
	}

	QDomElement hostNameDomElement = settingsDomElement.firstChildElement(hostNameTagName);
	QDomCharacterData hostNameChData = hostNameDomElement.firstChild().toCharacterData();
	if(!hostNameChData.isNull())
	{
		m_hostName = hostNameChData.toText().data();
	}
	else
	{
		m_hostName = "";
	}

	QDomElement portDomElement = settingsDomElement.firstChildElement(portTagName);
	QDomCharacterData portChData = portDomElement.firstChild().toCharacterData();
	if(!portChData.isNull())
	{
		m_port = portChData.toText().data().toUShort();
	}
	else
	{
		m_port = 0;
	}

	QDomElement dbServerInstanceNameDomElement = settingsDomElement.firstChildElement(dbServerInstanceNameTagName);
	QDomCharacterData dbServerInstanceNameChData = dbServerInstanceNameDomElement.firstChild().toCharacterData();
	if(!dbServerInstanceNameChData.isNull())
	{
		m_dbServerInstanceName = dbServerInstanceNameChData.toText().data();
	}
	else
	{
		m_dbServerInstanceName = "";
	}

	m_dbNameList.clear();
	QDomElement dbNamesDomElement = settingsDomElement.firstChildElement(dbNamesTagName);
	for(QDomElement dbNameDomElement = dbNamesDomElement.firstChildElement(dbNameTagName); !dbNameDomElement.isNull();
		dbNameDomElement = dbNameDomElement.nextSiblingElement(dbNameTagName))
	{
		QDomCharacterData dbNameChData = dbNameDomElement.firstChild().toCharacterData();
		if(!dbNameChData.isNull())
		{
			m_dbNameList.append(dbNameChData.toText().data());
		}
	}

	QDomElement lastBackupDirDomElement = settingsDomElement.firstChildElement(lastBackupDirTagName);
	QDomCharacterData lastBackupDirChData = lastBackupDirDomElement.firstChild().toCharacterData();
	if(!lastBackupDirChData.isNull())
	{
		m_lastBackupDir = lastBackupDirChData.toText().data();
	}
	else
	{
		m_lastBackupDir = "";
	}

	QDomElement lastRestoreDirDomElement = settingsDomElement.firstChildElement(lastRestoreDirTagName);
	QDomCharacterData lastRestoreDirChData = lastRestoreDirDomElement.firstChild().toCharacterData();
	if(!lastRestoreDirChData.isNull())
	{
		m_lastRestoreDir = lastRestoreDirChData.toText().data();
	}
	else
	{
		m_lastRestoreDir = "";
	}
}

OsqlSettingsReader::~OsqlSettingsReader()
{

}

bool OsqlSettingsReader::isNull() const
{
	return m_doc.isNull();
}

const QString& OsqlSettingsReader::getHostName() const
{
	return m_hostName;
}

quint16 OsqlSettingsReader::getPort() const
{
	return m_port;
}

const QString& OsqlSettingsReader::getDBServerInstanceName() const
{
	return m_dbServerInstanceName;
}

const QStringList& OsqlSettingsReader::getDBNameList() const
{
	return m_dbNameList;
}

void OsqlSettingsReader::setLastBackupDir(const QString& lastBackupDir)
{
	m_lastBackupDir = lastBackupDir;
}

QString OsqlSettingsReader::getLastBackupDir() const
{
	return m_lastBackupDir;
}

void OsqlSettingsReader::setLastBackupUseUsername(bool lastBackupUseUsername)
{
	m_lastBackupUseUsername = lastBackupUseUsername;
}

bool OsqlSettingsReader::getLastBackupUseUsername() const
{
	return m_lastBackupUseUsername;
}

void OsqlSettingsReader::setLastRestoreDir(const QString& lastRestoreDir)
{
	m_lastRestoreDir = lastRestoreDir;
}

QString OsqlSettingsReader::getLastRestoreDir() const
{
	return m_lastRestoreDir;
}

void OsqlSettingsReader::setLastRestoreUseUsername(bool lastRestoreUseUsername)
{
	m_lastRestoreUseUsername = lastRestoreUseUsername;
}

bool OsqlSettingsReader::getLastRestoreUseUsername() const
{
	return m_lastRestoreUseUsername;
}

void OsqlSettingsReader::save()
{
	m_doc.clear();

	QDomElement settingsDomElement = m_doc.createElement(settingsTagName);

	QDomCharacterData hostNameChData = m_doc.createTextNode(m_hostName);
	QDomElement hostNameDomElement = m_doc.createElement(hostNameTagName);
	hostNameDomElement.appendChild(hostNameChData);
	settingsDomElement.appendChild(hostNameDomElement);

	QDomCharacterData portChData = m_doc.createTextNode(QString::number(m_port));
	QDomElement portDomElement = m_doc.createElement(portTagName);
	portDomElement.appendChild(portChData);
	settingsDomElement.appendChild(portDomElement);

	QDomCharacterData dbServerInstanceNameChData = m_doc.createTextNode(m_dbServerInstanceName);
	QDomElement dbServerInstanceNameDomElement = m_doc.createElement(dbServerInstanceNameTagName);
	dbServerInstanceNameDomElement.appendChild(dbServerInstanceNameChData);
	settingsDomElement.appendChild(dbServerInstanceNameDomElement);

	QDomElement dbNamesDomElement = m_doc.createElement(dbNamesTagName);
	for(int i = 0; i < m_dbNameList.size(); i++)
	{
		QDomCharacterData dbNameChData = m_doc.createTextNode(m_dbNameList[i]);
		QDomElement dbNameDomElement = m_doc.createElement(dbNameTagName);
		dbNameDomElement.appendChild(dbNameChData);
		dbNamesDomElement.appendChild(dbNameDomElement);
	}
	settingsDomElement.appendChild(dbNamesDomElement);

	QDomCharacterData lastBackupDirChData = m_doc.createTextNode(m_lastBackupDir);
	QDomElement lastBackupDirDomElement = m_doc.createElement(lastBackupDirTagName);
	lastBackupDirDomElement.appendChild(lastBackupDirChData);
	settingsDomElement.appendChild(lastBackupDirDomElement);

	QDomCharacterData lastBackupUseUsernameChData;
	if(m_lastBackupUseUsername)
	{
		lastBackupUseUsernameChData = m_doc.createTextNode("True");
	}
	else
	{
		lastBackupUseUsernameChData  = m_doc.createTextNode("False");
	}
	QDomElement lastBackupUseUsernameDomElement = m_doc.createElement(lastBackupUseUsernameTagName);
	lastBackupUseUsernameDomElement.appendChild(lastBackupUseUsernameChData);
	settingsDomElement.appendChild(lastBackupUseUsernameDomElement);

	QDomCharacterData lastRestoreDirChData = m_doc.createTextNode(m_lastRestoreDir);
	QDomElement lastRestoreDirDomElement = m_doc.createElement(lastRestoreDirTagName);
	lastRestoreDirDomElement.appendChild(lastRestoreDirChData);
	settingsDomElement.appendChild(lastRestoreDirDomElement);

	QDomCharacterData lastRestoreUseUsernameChData;
	if(m_lastRestoreUseUsername)
	{
		lastRestoreUseUsernameChData = m_doc.createTextNode("True");
	}
	else
	{
		lastRestoreUseUsernameChData = m_doc.createTextNode("False");
	}
	QDomElement lastRestoreUseUsernameDomElement = m_doc.createElement(lastRestoreUseUsernameTagName);
	lastRestoreUseUsernameDomElement.appendChild(lastRestoreUseUsernameChData);
	settingsDomElement.appendChild(lastRestoreUseUsernameDomElement);
	
	m_doc.appendChild(settingsDomElement);
	QFile file(m_filename);
	if(file.open(QIODevice::WriteOnly))
	{
		file.write(m_doc.toByteArray());
		file.flush();
		file.close();
	}
}
