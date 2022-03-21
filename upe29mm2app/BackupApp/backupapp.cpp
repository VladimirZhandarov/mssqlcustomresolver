#include "backupapp.h"

#include <QtCore/QTimer>
#include <QtCore/QFile>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <osqlsettingsreader.h>

#include "backupwizard.h"

BackupApp::BackupApp(QObject* parent) : QObject(parent)
{
	m_osqlSettingsReader = new OsqlSettingsReader("osqlSettings.xml", this);
	m_backupWizard = new BackupWizard(m_osqlSettingsReader);
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	m_timer->setSingleShot(true);
	m_timer->start();
}

BackupApp::~BackupApp()
{
	delete m_backupWizard;
}

void BackupApp::onTimeout()
{
	Q_ASSERT(m_backupWizard);
	m_backupWizard->exec();
	//BackupSettingDialog dialog(m_osqlSettingsReader->getDBNameList(), m_osqlSettingsReader->getLastBackupDir());
	/*if(dialog.exec() == QDialog::Accepted)
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
		m_osqlSettingsReader->setLastBackupDir(dir);
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
		//m_progressDialog->exec(dir, host, port, osqlPath, dialog.isLocal(), m_osqlSettingsReader->getDBServerInstanceName(), ftpPath, dbNameList);
	}
	else
	{
		QApplication::exit(0);
	}*/
}
