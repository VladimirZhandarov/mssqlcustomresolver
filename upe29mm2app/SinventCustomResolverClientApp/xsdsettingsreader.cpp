#include "xsdsettingsreader.h"

#include <QtCore/QFile>

QString XsdSettingsReader::settingsTagName = "Settings";
QString XsdSettingsReader::hostNameTagName = "hostName";
QString XsdSettingsReader::portTagName = "port";
QString XsdSettingsReader::mainClientTagName = "mainClient";
QString XsdSettingsReader::ownResolveTagName = "ownResolve";
QString XsdSettingsReader::dbServerInstanceNameTagName = "DBServerInstanceName";
QString XsdSettingsReader::dbNamesTagName = "DBNames";
QString XsdSettingsReader::dbNameTagName = "DBName";
QString XsdSettingsReader::logLevelTagName = "logLevel";
QString XsdSettingsReader::applicationFontTagName = "applicationFont";
QString XsdSettingsReader::dataTableFontTagName = "dataTableFont";
QString XsdSettingsReader::logFontTagName = "logFont";
QString XsdSettingsReader::fontNameAttrName = "name";
QString XsdSettingsReader::fontSizeAttrName = "size";
QString XsdSettingsReader::fontWeightAttrName = "weight";

XsdSettingsReader::XsdSettingsReader(const QString& filename, QObject* parent) : QObject(parent), m_filename(filename)
{
	m_hostName = "localhost";
	m_port = 55001;
	m_mainClient = false;
	m_ownResolve = true;
	m_dbServerInstanceName = "";
	m_dbNameList.append("UPE_MM2_Administration2_EN");
	m_dbNameList.append("UPE_MM2_Catalog_EN");
	m_dbNameList.append("UPE_MM2_Documents2_EN");
	m_dbNameList.append("UPE_MM2_Formular_EN");
	m_dbNameList.append("UPE_MM2_ItemsAccount_EN");
	m_dbNameList.append("UPE_MM2_JPS_EN");
	m_dbNameList.append("UPE_MM2_Organization_EN");
	m_dbNameList.append("UPE_MM2_Planning_EN");
	m_dbNameList.append("UPE_MM2_Reglament2_EN");

	m_logLevel = 0;

	m_applicationFont = QFont("MS Shel Dlg 2", 12, QFont::Bold);
	m_dataTableFont = QFont("MS Shel Dlg 2", 12, QFont::Bold);
	m_logFont = QFont("MS Shel Dlg 2", 12, QFont::Bold);

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

	QDomElement mainClientDomElement = settingsDomElement.firstChildElement(mainClientTagName);
	QDomCharacterData mainClientChData = mainClientDomElement.firstChild().toCharacterData();
	if(!mainClientChData.isNull())
	{
		m_mainClient = (mainClientChData.toText().data().toLower() == "true");
	}
	else
	{
		m_mainClient = false;
	}

	QDomElement ownResolveDomElement = settingsDomElement.firstChildElement(ownResolveTagName);
	QDomCharacterData ownResolveChData = ownResolveDomElement.firstChild().toCharacterData();
	if(!ownResolveChData.isNull())
	{
		m_ownResolve = (ownResolveChData.toText().data().toLower() != "false");
	}
	else
	{
		m_ownResolve = true;
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

	QDomElement logLevelDomElement = settingsDomElement.firstChildElement(logLevelTagName);
	QDomCharacterData logLevelChData = logLevelDomElement.firstChild().toCharacterData();
	if(!logLevelChData.isNull())
	{
		m_logLevel = portChData.toText().data().toInt();
	}
	else
	{
		m_logLevel = 0;
	}

	QDomElement applicationFontDomElement = settingsDomElement.firstChildElement(applicationFontTagName);
	if(!applicationFontDomElement.isNull())
	{
		m_applicationFont = QFont(applicationFontDomElement.attribute(fontNameAttrName), applicationFontDomElement.attribute(fontSizeAttrName).toInt(),
			applicationFontDomElement.attribute(fontWeightAttrName).toInt());
	}

	QDomElement dataTableFontDomElement = settingsDomElement.firstChildElement(dataTableFontTagName);
	if(!dataTableFontDomElement.isNull())
	{
		m_dataTableFont = QFont(dataTableFontDomElement.attribute(fontNameAttrName), dataTableFontDomElement.attribute(fontSizeAttrName).toInt(),
			dataTableFontDomElement.attribute(fontWeightAttrName).toInt());
	}

	QDomElement logFontDomElement = settingsDomElement.firstChildElement(logFontTagName);
	if(!logFontDomElement.isNull())
	{
		m_logFont = QFont(logFontDomElement.attribute(fontNameAttrName), logFontDomElement.attribute(fontSizeAttrName).toInt(),
			logFontDomElement.attribute(fontWeightAttrName).toInt());
	}
}

XsdSettingsReader::~XsdSettingsReader()
{

}

bool XsdSettingsReader::isNull() const
{
	return m_doc.isNull();
}

const QString& XsdSettingsReader::getHostName() const
{
	return m_hostName;
}

quint16 XsdSettingsReader::getPort() const
{
	return m_port;
}

bool XsdSettingsReader::isMainClient() const
{
	return m_mainClient;
}

void XsdSettingsReader::setOwnResolve(bool ownResolve)
{
	m_ownResolve = ownResolve;
}

bool XsdSettingsReader::isOwnResolve() const
{
	return m_ownResolve;
}

const QString& XsdSettingsReader::getDBServerInstanceName() const
{
	return m_dbServerInstanceName;
}

const QStringList& XsdSettingsReader::getDBNameList() const
{
	return m_dbNameList;
}

int XsdSettingsReader::getLogLevel() const
{
	return m_logLevel;
}

QFont XsdSettingsReader::getApplicationFont() const
{
	return m_applicationFont;
}

QFont XsdSettingsReader::getDataTableFont() const
{
	return m_dataTableFont;
}

QFont XsdSettingsReader::getLogFont() const
{
	return m_logFont;
}

void XsdSettingsReader::save()
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

	QString mainClientValue = "false";
	if(m_mainClient)
	{
		mainClientValue = "true";
	}
	QDomCharacterData mainClientChData = m_doc.createTextNode(mainClientValue);
	QDomElement mainClientDomElement = m_doc.createElement(mainClientTagName);
	mainClientDomElement.appendChild(mainClientChData);
	settingsDomElement.appendChild(mainClientDomElement);

	QString ownResolveValue = "false";
	if(m_ownResolve)
	{
		ownResolveValue = "true";
	}
	QDomCharacterData ownResolveChData = m_doc.createTextNode(ownResolveValue);
	QDomElement ownResolveDomElement = m_doc.createElement(ownResolveTagName);
	ownResolveDomElement.appendChild(ownResolveChData);
	settingsDomElement.appendChild(ownResolveDomElement);

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

	QDomCharacterData logLevelChData = m_doc.createTextNode(QString::number(m_logLevel));
	QDomElement logLevelDomElement = m_doc.createElement(logLevelTagName);
	logLevelDomElement.appendChild(logLevelChData);
	settingsDomElement.appendChild(logLevelDomElement);

	QDomElement applicationFontDomElement = m_doc.createElement(applicationFontTagName);
	applicationFontDomElement.setAttribute(fontNameAttrName, m_applicationFont.family());
	applicationFontDomElement.setAttribute(fontSizeAttrName, m_applicationFont.pointSize());
	settingsDomElement.appendChild(applicationFontDomElement);

	QDomElement dataTableFontDomElement = m_doc.createElement(dataTableFontTagName);
	dataTableFontDomElement.setAttribute(fontNameAttrName, m_dataTableFont.family());
	dataTableFontDomElement.setAttribute(fontSizeAttrName, m_dataTableFont.pointSize());
	settingsDomElement.appendChild(dataTableFontDomElement);

	QDomElement logFontDomElement = m_doc.createElement(logFontTagName);
	logFontDomElement.setAttribute(fontNameAttrName, m_logFont.family());
	logFontDomElement.setAttribute(fontSizeAttrName, m_logFont.pointSize());
	settingsDomElement.appendChild(logFontDomElement);

	m_doc.appendChild(settingsDomElement);
	QFile file(m_filename);
	if(file.open(QIODevice::WriteOnly))
	{
		file.write(m_doc.toByteArray());
		file.flush();
		file.close();
	}
}
