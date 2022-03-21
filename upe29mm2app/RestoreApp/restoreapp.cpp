#include "restoreapp.h"

#include <QtCore/QTimer>

#include <QtWidgets/QApplication>

#include <osqlsettingsreader.h>

#include "restorewizard.h"

RestoreApp::RestoreApp(QObject* parent) : QObject(parent)
{
	m_osqlSettingsReader = new OsqlSettingsReader("osqlSettings.xml", this);
	m_restoreWizard = new RestoreWizard(m_osqlSettingsReader);
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	m_timer->setSingleShot(true);
	m_timer->start();
}

RestoreApp::~RestoreApp()
{
	delete m_restoreWizard;
}

void RestoreApp::onTimeout()
{
	Q_ASSERT(m_restoreWizard);
	m_restoreWizard->exec();
	/*Q_ASSERT(m_osqlSettingsReader);
	RestoreSettingDialog dialog(m_osqlSettingsReader->getDBNameList(), m_osqlSettingsReader->getLastRestoreDir());
	if(dialog.exec() == QDialog::Accepted)
	{

		QString dir = dialog.getDirectory();
		if(dir.isEmpty())
		{
			QApplication::exit(0);
		}
		else
		{
			if(!dir.endsWith("/"))
			{
				dir += "/";
			}
		}
		m_osqlSettingsReader->setLastRestoreDir(dir);
		m_osqlSettingsReader->save();
		QString osqlPath = "";
		QString host = "";
		quint16 port = 0;
		QString ftpPath = "";
		if(dialog.isLocal())
		{
			osqlPath = m_osqlSettingsReader->getOsqlPath();
			if(!osqlPath.endsWith("/"))
			{
				osqlPath += "/";
			}
			if(!osqlPath.endsWith("OSQL.EXE"))
			{
				osqlPath += "OSQL.EXE";
			}
			QFile file(osqlPath);
			if(!file.exists())
			{
				QMessageBox::warning(0, tr("Backup copy progress"), tr("Could not open osql.exe.\r\n") +
					tr("Please set osqlPath in osqlsetting.xml"));
				return;
			}
			if(m_osqlSettingsReader->getDBServerInstanceName().isEmpty())
			{
				QMessageBox::warning(0, tr("Backup copy progress"), tr("DBServerInstanceName no setted.\r\n") +
					tr("Please set dbServerInstanceName in osqlsetting.xml"));
				return;
			}
		}
		else
		{
			host = m_osqlSettingsReader->getHostName();
			port = m_osqlSettingsReader->getPort();
			ftpPath = m_osqlSettingsReader->getFtpUrl();
		}
		QStringList dbNameList = dialog.getCheckedDatabases();
		m_progressDialog->exec(dir, host, port, osqlPath, dialog.isLocal(), m_osqlSettingsReader->getDBServerInstanceName(), ftpPath, dbNameList);
	}
	else
	{
		QApplication::exit(0);
	}*/
}
