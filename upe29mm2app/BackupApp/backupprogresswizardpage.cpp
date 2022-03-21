#include "backupprogresswizardpage.h"

#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>

#include <QtWidgets/QMessageBox>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <osqlsettingsreader.h>
#include <xsdbackupdatabasepacketparser.h>
#include <rawpacketclientsocket.h>

#include <ftpclient.h>

BackupProgressWizardPage::BackupProgressWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent) :
	QWizardPage(parent), m_rawPacketClientSocket(rawPacketClientSocket)
{
	ui.setupUi(this);
	m_ftpClient = 0;

	Q_ASSERT(osqlSettingsReader);
	m_dbServerInstanceName = osqlSettingsReader->getDBServerInstanceName();
	m_dbNameList = osqlSettingsReader->getDBNameList();
	m_host = osqlSettingsReader->getHostName();
	m_port = osqlSettingsReader->getPort();
	m_ftpUrl = QString("ftp://%1:21").arg(osqlSettingsReader->getHostName());

	m_backupDatabasePacketParser = std::tr1::shared_ptr<XsdBackupDatabasePacketParser>(new XsdBackupDatabasePacketParser());

	showLogPB_toggled(ui.showLogPB->isChecked());

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	m_timer->setSingleShot(true);
}

BackupProgressWizardPage::~BackupProgressWizardPage()
{

}

void BackupProgressWizardPage::init(const QString& dir, const QString& username, const QString& password, bool localDataBase, const QStringList& dbNameList)
{
	m_dir = dir;
	if(!m_dir.endsWith("/"))
	{
		m_dir += "/";
	}
	m_username = username;
	m_password = password;
	m_local = localDataBase;
	m_dbNameList = dbNameList;
	m_step = 0;
	if(!m_dbNameList.isEmpty())
	{
		if(!m_local)
		{
			if(m_ftpUrl.isEmpty())
			{
				QMessageBox::warning(0, tr("Backup copy progress"), tr("ftpUrl no setted.\r\n") + tr("Please set ftpUrl in osqlsetting.xml"));
				return ;
			}
			m_ftpClient = new FtpClient(dir);
			connect(m_ftpClient, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(onFtpDataTransferProgress(qint64, qint64)));
			connect(m_ftpClient, SIGNAL(commandFinished(QString)), this, SLOT(onFtpCommandFinished(QString)));
		}
		ui.backupProgress->setRange(0, dbNameList.size());
		emit completeChanged();
		startProgress(m_step);
	}
	m_copyIndex = 0;
	ui.copyL->setText(tr("Copy 0/%1:").arg(m_dbNameList.size()));
}

bool BackupProgressWizardPage::isComplete() const
{
	return (m_step >= m_dbNameList.size());
}

void BackupProgressWizardPage::showLogPB_toggled(bool toggled)
{
	if(toggled)
	{
		ui.logTE->show();
		ui.verticalSpacer->changeSize(20, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
		ui.showLogPB->setText(tr("<< Show less info"));
		//ui.showLogPB->setText(tr("<< Кратко"));
	}
	else
	{
		ui.logTE->hide();
		ui.verticalSpacer->changeSize(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
		ui.showLogPB->setText(tr("Show more info >>"));
		//ui.showLogPB->setText(tr("Подробнее >>"));
	}
}

void BackupProgressWizardPage::onErrorMessage(QString errorMessage)
{
	QMessageBox::warning(this, tr(""), errorMessage);
}

void BackupProgressWizardPage::onDataReceived(int packetType, QByteArray recvArray)
{
	Q_ASSERT(m_backupDatabasePacketParser.get());
	if(packetType == RawPacketClientSocket::BACKUP_DATABASE_PACKET_TYPE)
	{
		if(m_backupDatabasePacketParser->parse(QString(recvArray)))
		{
			std::tr1::shared_ptr<BackupDatabasePacketData> backupDatabasePacketData = m_backupDatabasePacketParser->getPacket();
			if(backupDatabasePacketData.get())
			{
				m_step++;
				ui.backupProgress->setValue(m_step);

				if(backupDatabasePacketData->getResult() != BackupDatabasePacketData::AcceptBackupDatabaseResult)
				{
					QMessageBox messageBox;
					messageBox.setWindowTitle("Backup copy progress");
					messageBox.setText(tr("The backuping process was ended with error on database \"%1\"").arg(m_currentDbName));
					messageBox.setInformativeText(backupDatabasePacketData->getMessage());
					QMessageBox::StandardButtons buttons;
					if(m_step < m_dbNameList.size())
					{
						buttons = QMessageBox::Save | QMessageBox::Cancel;
						messageBox.setStandardButtons(buttons);
						messageBox.setButtonText(QMessageBox::Save, tr("Next database"));
						messageBox.setButtonText(QMessageBox::Cancel, tr("End process"));
						messageBox.setDefaultButton(QMessageBox::Save);
					}
					else
					{
						buttons = QMessageBox::Ok;
						messageBox.setStandardButtons(buttons);
					}
					int ret = messageBox.exec();
					if(ret == QMessageBox::Cancel || ret == QMessageBox::Ok)
					{
						close();
					}
				}
				else
				{
					if(m_ftpClient)
					{
						if(m_ftpClient->connectToFtp(m_ftpUrl))
						{
							m_ftpClient->loadFile(m_currentFilename);
						}
					}
				}
				if(m_step < m_dbNameList.size())
				{
					startProgress(m_step);
				}
				else
				{
					//emit finishedSignal();
					//QMessageBox::information(0, tr("Backup copy progress"), tr("The backuping process was completed successfully"));
					//QMessageBox::information(0, tr("Резервное копирование"), tr("Резервное копирование успешно завершено"));
				}
			}
		}
	}
}

void BackupProgressWizardPage::onFtpDataTransferProgress(qint64 done, qint64 total)
{
	ui.copyProgress->setMaximum(total);
	ui.copyProgress->setValue(done);
}

void BackupProgressWizardPage::onFtpCommandFinished(QString errorMsg)
{
	m_copyIndex++;
	ui.copyL->setText(tr("Copy %2/%1:").arg(m_dbNameList.size()).arg(m_copyIndex));
	if(m_dbNameList.size() == m_copyIndex)
	{
		ui.copyProgress->setMaximum(100);
		ui.copyProgress->setValue(100);

		emit finishedSignal();
		QMessageBox::information(0, tr("Backup copy progress"), tr("The backuping process was completed successfully"));
	}
}

void BackupProgressWizardPage::onTimeout()
{
	if(m_step < m_dbNameList.size())
	{
		startProgress(m_step);
	}
	else
	{
		emit finishedSignal();
		QMessageBox::information(0, tr("Backup copy progress"), tr("The backuping process was completed successfully"));
	}
}

void BackupProgressWizardPage::startProgress(int step)
{
	if(step < m_dbNameList.size())
	{
		m_currentDbName = m_dbNameList[step];
		m_currentFilename = m_currentDbName + ".bak";
		m_currentFilePath = m_dir + m_currentFilename;
		if(m_local)
		{
			ui.copyL->hide();
			ui.copyProgress->hide();

			QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
			if(m_username.isEmpty() || m_password.isEmpty())
			{
				db.setDatabaseName("DRIVER={SQL Server};Server=" + m_dbServerInstanceName + ";Trusted_Connection=yes;");
			}
			else
			{
				db.setDatabaseName("DRIVER={SQL Server};Server=" + m_dbServerInstanceName + ";");
				db.setUserName(m_username);
				db.setPassword(m_password);
			}
			QStringList notUsedatabases;
			notUsedatabases << "master";
			notUsedatabases << "model";
			notUsedatabases << "msdb";
			notUsedatabases << "tempdb";
			notUsedatabases << "Resource";
			notUsedatabases << "distribution";
			notUsedatabases << "reportserver";
			notUsedatabases << "reportservertempdb";

			if(db.open())
			{
				QSqlQuery query;
				bool res = query.exec(QString("BACKUP DATABASE %1 TO DISK = \'%2\'").arg(m_currentDbName).arg(m_currentFilePath));
				if(!res)
				{
					QSqlError error = query.lastError();
					ui.logTE->append(QString("Database %1 NOT backuped to file %2").arg(m_currentDbName).arg(m_currentFilePath));
					ui.logTE->append(QString("Error: ").arg(error.text()));
				}
				else
				{
					ui.logTE->append(QString("Database %1 backuped to file %2").arg(m_currentDbName).arg(m_currentFilePath));
				}
			}
			else
			{
				QMessageBox::warning(this, tr("Backup copy progress"), tr("Could not open connection with local sql server\r\nError:") +
					db.lastError().text());
			}
			m_step++;
			ui.backupProgress->setValue(m_step);
			Q_ASSERT(m_timer);
			m_timer->start(100);
		}
		else
		{
			if(m_rawPacketClientSocket)
			{
				if(!m_rawPacketClientSocket->isConnected())
				{
					m_rawPacketClientSocket->tcpConnect(m_host, m_port);
				}
				if(!m_rawPacketClientSocket->isConnected())
				{
					QMessageBox::warning(this, tr("Backup copy progress"), tr("Could not open connection with resolver service\r\n") +
						tr("with address %1:%2").arg(m_host).arg(m_port));
					return;
				}
				else
				{
					Q_ASSERT(m_backupDatabasePacketParser.get());
					std::tr1::shared_ptr<BackupDatabasePacketData> packetData = std::tr1::shared_ptr<BackupDatabasePacketData>(new BackupDatabasePacketData(
						m_username, m_password, BackupDatabasePacketData::UndefineBackupDatabaseResult));
					packetData->setDBName(m_currentDbName);
					QByteArray data = m_backupDatabasePacketParser->createPacket(packetData);
					m_rawPacketClientSocket->sendBackupPacket(data);
				}
			}
		}
	}
}
