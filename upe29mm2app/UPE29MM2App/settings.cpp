#include "settings.h"

QString Settings::synchButtonGroup = "synchButton";
QString Settings::backupButtonGroup = "backupButton";
QString Settings::restoreButtonGroup = "restoreButton";
QString Settings::startButtonGroup = "startButton";
QString Settings::exitButtonGroup = "exitButton";

QString Settings::showKey = "show";
QString Settings::applicationPathKey = "applicationPath";
QString Settings::originalPictureFilePathKey = "originalPictureFilePath";
QString Settings::pressedPictureFilePathKey = "pressedPictureFilePath";

Settings::Settings(const QFileInfo& fileInfo, QObject* parent) : QSettings(fileInfo.absoluteFilePath(), QSettings::IniFormat, parent)
{
	m_typeMap.insert(SYNCH_BUTTON, synchButtonGroup);
	m_typeMap.insert(BACKUP_BUTTON, backupButtonGroup);
	m_typeMap.insert(RESTORE_BUTTON, restoreButtonGroup);
	m_typeMap.insert(START_BUTTON, startButtonGroup);
	m_typeMap.insert(EXIT_BUTTON, exitButtonGroup);

	if(!fileInfo.exists())
	{
		setDefaultValues();
	}
}

Settings::~Settings()
{

}

bool Settings::isShow(ButtonType buttonType)
{
	bool result = true;
	QString groupName = m_typeMap.value(buttonType);
	if(!groupName.isEmpty())
	{
		beginGroup(groupName);
		result = value(showKey).toBool();
		endGroup();
	}
	return result;
}

QString Settings::getApplicationPath(ButtonType buttonType)
{
	QString result = "";
	QString groupName = m_typeMap.value(buttonType);
	if(!groupName.isEmpty())
	{
		beginGroup(groupName);
		result = value(applicationPathKey).toString();
		endGroup();
	}
	return result;
}

QString Settings::getOriginalPictureFilePath(ButtonType buttonType)
{
	QString result = "";
	QString groupName = m_typeMap.value(buttonType);
	if(!groupName.isEmpty())
	{
		beginGroup(groupName);
		result = value(originalPictureFilePathKey).toString();
		endGroup();
	}
	return result;
}

QString Settings::getPressedPictureFilePath(ButtonType buttonType)
{
	QString result = "";
	QString groupName = m_typeMap.value(buttonType);
	if(!groupName.isEmpty())
	{
		beginGroup(groupName);
		result = value(pressedPictureFilePathKey).toString();
		endGroup();
	}
	return result;
}

void Settings::setDefaultValues()
{
	setValues(SYNCH_BUTTON, false, "", "", "");
	setValues(BACKUP_BUTTON, true, "backupApp.exe", "", "");
	setValues(RESTORE_BUTTON, true, "restoreApp.exe", "", "");
	setValues(START_BUTTON, true, "SinventCustomResolverClientApp.exe", "", "");
	setValues(EXIT_BUTTON, true, "", "", "");
}

void Settings::setValues(ButtonType buttonType, bool show, const QString& applicationPath, const QString& originalPictureFilePath,
	const QString& pressedPictureFilePath)
{
	QString groupName = m_typeMap.value(buttonType);
	if(!groupName.isEmpty())
	{
		beginGroup(groupName);
		setValue(showKey, show);
		setValue(applicationPathKey, applicationPath);
		setValue(originalPictureFilePathKey, originalPictureFilePath);
		setValue(pressedPictureFilePathKey, pressedPictureFilePath);
		endGroup();
	}
}
