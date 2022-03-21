#include "restoreprogresswizardpage.h"

#include <QtCore/QFile>
#include <QtCore/QTextCodec>

#include <QtWidgets/QMessageBox>

#include <assert.h>

#include <ftpclient.h>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <osqlsettingsreader.h>
#include <rawpacketclientsocket.h>
#include <xsdrestoredatabasepacketparser.h>

RestoreProgressWizardPage::RestoreProgressWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent) :
	QWizardPage(parent), m_rawPacketClientSocket(rawPacketClientSocket)
{
	ui.setupUi(this);

	m_ftpClient = 0;

	Q_ASSERT(osqlSettingsReader);
	m_dbServerInstanceName = osqlSettingsReader->getDBServerInstanceName();
	m_host = osqlSettingsReader->getHostName();
	m_port = osqlSettingsReader->getPort();
	m_ftpUrl = QString("ftp://%1:21").arg(osqlSettingsReader->getHostName());

	m_restoreDatabasePacketParser = std::tr1::shared_ptr<XsdRestoreDatabasePacketParser>(new XsdRestoreDatabasePacketParser());

	showLogPB_toggled(ui.showLogPB->isChecked());
	
	/*ui.logTE->setPlainText(tr("Восстановление базы данных %1").arg(m_currentDbName));

	ui.restoreL->setText(tr("Восстановление:"));
	ui.copyL->setText(tr("Копирование 0/7:"));*/
}

RestoreProgressWizardPage::~RestoreProgressWizardPage()
{

}

void RestoreProgressWizardPage::init(const QString& dir, const QString& username, const QString& password, bool localDataBase,
	const QStringList& dbNameList)
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
				QMessageBox::warning(0, tr("Restore copy progress"), tr("ftpUrl no setted.\r\n") + tr("Please set ftpUrl in osqlsetting.xml"));
				return;
			}
			m_ftpClient = new FtpClient(dir);
			connect(m_ftpClient, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(onFtpDataTransferProgress(qint64, qint64)));
			connect(m_ftpClient, SIGNAL(commandFinished(QString)), this, SLOT(onFtpCommandFinished(QString)));
		}
		ui.restoreProgress->setRange(0, dbNameList.size());
		emit completeChanged();
		startProgress(m_step);
	}
	m_copyIndex = 0;
	ui.copyL->setText(tr("Copy 0/%1:").arg(m_dbNameList.size()));
}

bool RestoreProgressWizardPage::isComplete() const
{
	return (m_step >= m_dbNameList.size());
}

void RestoreProgressWizardPage::showLogPB_toggled(bool toggled)
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

void RestoreProgressWizardPage::onErrorMessage(QString errorMessage)
{
	QMessageBox::warning(this, tr(""), errorMessage);
}

void RestoreProgressWizardPage::onDataReceived(int packetType, QByteArray recvArray)
{
	Q_ASSERT(m_restoreDatabasePacketParser.get());
	if(packetType == RawPacketClientSocket::RESTORE_DATABASE_PACKET_TYPE)
	{
		if(m_restoreDatabasePacketParser->parse(QString(recvArray)))
		{
			std::tr1::shared_ptr<RestoreDatabasePacketData> restoreDatabasePacketData = m_restoreDatabasePacketParser->getPacket();
			if(restoreDatabasePacketData.get())
			{
				m_step++;
				ui.restoreProgress->setValue(m_step);

				if(restoreDatabasePacketData->getResult() != RestoreDatabasePacketData::AcceptRestoreDatabaseResult)
				{
					QMessageBox messageBox;
					messageBox.setWindowTitle("Restore database progress");
					messageBox.setText(tr("The restore process was ended with error on database \"%1\"").arg(m_currentServerDbName));
					messageBox.setInformativeText(restoreDatabasePacketData->getMessage());
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
				if(m_step < m_dbNameList.size())
				{
					startServerProgress(m_step);
				}
				else
				{
					if(m_local)
					{
						QMessageBox::information(0, tr("Restore database progress"), tr("The restoring process was completed successfully"));
						//QMessageBox::information(0, tr("Резервное копирование"), tr("Резервное копирование успешно завершено"));
					}
					else
					{
						emit finishedSignal();
					}
				}
			}
		}
	}
}

void RestoreProgressWizardPage::onFtpDataTransferProgress(qint64 done, qint64 total)
{
	ui.copyProgress->setMaximum(total);
	ui.copyProgress->setValue(done);
}

void RestoreProgressWizardPage::onFtpCommandFinished(QString errorMsg)
{
	m_copyIndex++;
	ui.copyL->setText(tr("Copy %2/%1:").arg(m_dbNameList.size()).arg(m_copyIndex));
	if(m_dbNameList.size() == m_copyIndex)
	{
		ui.copyProgress->setMaximum(100);
		ui.copyProgress->setValue(100);
		startServerProgress(m_step);
	}
	else
	{
		startProgress(m_copyIndex);
	}
}

void RestoreProgressWizardPage::startServerProgress(int step)
{
	if(step < m_dbNameList.size())
	{
		m_currentServerDbName = m_dbNameList[step];
		if(!m_local)
		{
			if(m_rawPacketClientSocket)
			{
				if(!m_rawPacketClientSocket->isConnected())
				{
					m_rawPacketClientSocket->tcpConnect(m_host, m_port);
				}
				if(!m_rawPacketClientSocket->isConnected())
				{
					QMessageBox::warning(this, tr("Restore database progress"), tr("Could not open connection with resolver service\r\n") +
						tr("with address %1:%2").arg(m_host).arg(m_port));
					return;
				}
				else
				{
					Q_ASSERT(m_restoreDatabasePacketParser.get());
					std::tr1::shared_ptr<RestoreDatabasePacketData> packetData = std::tr1::shared_ptr<RestoreDatabasePacketData>(new RestoreDatabasePacketData(
						m_username, m_password, RestoreDatabasePacketData::UndefineRestoreDatabaseResult));
					packetData->setDBName(m_currentServerDbName);
					QByteArray data = m_restoreDatabasePacketParser->createPacket(packetData);
					m_rawPacketClientSocket->sendRestorePacket(data);
				}
			}
		}
	}
}

void RestoreProgressWizardPage::startProgress(int step)
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

			if(db.open())
			{
				QSqlQuery query;
				bool res = query.exec(QString("RESTORE DATABASE %1 FROM DISK = \'%2\'").arg(m_currentDbName).arg(m_currentFilePath));
				while(query.next())
				{
					/*					QString name = query.value(0).toString();
					if(!notUsedatabases.contains(name.toLower()) && !name.toLower().startsWith("reportserver$"))
					{
					databases.append(name);
					}*/
				}
			}
			else
			{
				QMessageBox::warning(this, tr("Restore database progress"), tr("Could not open connection with local sql server\r\nError:") +
					db.lastError().text());
			}
		}
		else
		{
			if(m_ftpClient)
			{
				if(m_ftpClient->connectToFtp(m_ftpUrl))
				{
					m_ftpClient->uploadFile(m_currentFilename);
				}
			}
		}
	}
}
