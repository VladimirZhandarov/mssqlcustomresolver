#include "replicationprogresswizardpage.h"

#include <QtCore/QFile>
#include <QtCore/QTextCodec>

#include <QtWidgets/QMessageBox>

#include <assert.h>

#include <ftpclient.h>
#include <rawpacketclientsocket.h>
#include <xsdbackupdatabasepacketparser.h>

#include <osqlsettingsreader.h>

ReplicationProgressWizardPage::ReplicationProgressWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket,
	QWidget* parent) : QWizardPage(parent), m_rawPacketClientSocket(rawPacketClientSocket)
{
	ui.setupUi(this);
	connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(process_finished(int, QProcess::ExitStatus)));

	m_ftpClient = 0;

	m_backupDatabasePacketParser = std::tr1::shared_ptr<XsdBackupDatabasePacketParser>(new XsdBackupDatabasePacketParser());
	QTcpSocket* tcpSocket = new QTcpSocket(this);
	m_rawPacketClientSocket = new RawPacketClientSocket(tcpSocket->socketDescriptor(), this);
	connect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), this, SLOT(onDataReceived(int, QByteArray)));

	showLogPB_toggled(ui.showLogPB->isChecked());

	//ui.createPublicationL->setText(tr("Создание публикаций:"));
}

ReplicationProgressWizardPage::~ReplicationProgressWizardPage()
{

}

void ReplicationProgressWizardPage::init()
{

}

bool ReplicationProgressWizardPage::isComplete() const
{
	return true;
}

/*int ProgressDialog::exec(const QString& dir, const QString& host, quint16 port, const QString& osqlPath, bool local, const QString& dbServerInstanceName,
	const QString& ftpUrl, const QStringList& dbNameList)
{
	m_dir = dir;
	m_host = host;
	m_port = port;
	m_osqlPath = osqlPath;
	m_local = local;
	m_dbServerInstanceName = dbServerInstanceName;
	m_ftpUrl = ftpUrl;
	m_dbNameList = dbNameList;
	m_step = 0;
	if(!m_dbNameList.isEmpty())
	{
		if(!m_local)
		{
			if(m_ftpUrl.isEmpty())
			{
				QMessageBox::warning(0, tr("Backup copy progress"), tr("ftpUrl no setted.\r\n") + tr("Please set ftpUrl in osqlsetting.xml"));
				return QDialog::Rejected;
			}
			m_ftpClient = new FtpClient(dir);
		}
		ui.backupProgress->setRange(0, dbNameList.size());
		ui.copyProgress->setRange(0, dbNameList.size());
		startProgress(m_step);
		return QDialog::exec();
	}
	return QDialog::Rejected;
}*/

/*void ProgressDialog::closePB_clicked()
{
	if(m_step < m_dbNameList.size())
	{
		reject();
	}
	else
	{
		accept();
	}
}*/

void ReplicationProgressWizardPage::showLogPB_toggled(bool toggled)
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

void ReplicationProgressWizardPage::onErrorMessage(QString errorMessage)
{
	QMessageBox::warning(this, tr(""), errorMessage);
}

void ReplicationProgressWizardPage::process_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
	m_step++;
	if(exitCode != 0 || !QFile(m_currentFilename).exists())
	{
		QByteArray processOut = m_process.readAll();
		QTextCodec* codec = QTextCodec::codecForName("IBM 866");
		QString errorMessage = "";
		if(codec)
		{
			errorMessage = codec->toUnicode(processOut);
		}
		QMessageBox messageBox;
		messageBox.setWindowTitle(tr("Backup copy progress"));
		messageBox.setText(tr("The backuping process was ended with error on database \"%1\"").arg(m_currentDbName));
		messageBox.setInformativeText(errorMessage);
		QMessageBox::StandardButtons buttons;
		if(m_step < m_dbNameList.size())
		{
			buttons = QMessageBox::Save | QMessageBox::Cancel;
			messageBox.setStandardButtons(buttons);
			messageBox.setButtonText(QMessageBox::Save, tr("Next database"));
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
			return;
		}
	}
	ui.createPublicationProgress->setValue(m_step);
	if(m_step < m_dbNameList.size())
	{
		startProgress(m_step);
	}
	else
	{
		close();
		QMessageBox::information(0, tr("Backup copy progress"), tr("The backuping process was completed successfully"));
		//QMessageBox::information(0, tr("Резервное копирование"), tr("Резервное копирование успешно завершено"));
	}
}

void ReplicationProgressWizardPage::onDataReceived(int packetType, QByteArray recvArray)
{
	if(packetType == RawPacketClientSocket::LIST_DATABASE_PACKET_TYPE)
	{
		processListDatabasePacket(recvArray);
	}
	else if(packetType == RawPacketClientSocket::BACKUP_DATABASE_PACKET_TYPE)
	{
		processBackupDatabasePacket(recvArray);
	}
}

void ReplicationProgressWizardPage::startProgress(int step)
{
	if(step < m_dbNameList.size())
	{
		m_currentDbName = m_dbNameList[step];
		m_currentFilename = m_dir + m_currentDbName + ".bak";
		if(m_local)
		{
			ui.createPublicationL->hide();
			ui.createPublicationProgress->hide();

			QFile::remove(m_dir + m_currentDbName + ".bak");
			m_process.start("\"" + m_osqlPath + "\"" + " -E -S " + m_dbServerInstanceName + " -Q \"BACKUP DATABASE " + m_currentDbName + " TO DISK = \'" +
				m_currentFilename + "\'\"");
			//ui.messageL->setText("Backup " + m_currentDbName + " database");
		}
		else
		{
			Q_ASSERT(m_rawPacketClientSocket);
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
					"", "", BackupDatabasePacketData::UndefineBackupDatabaseResult));
				packetData->setDBName(m_currentDbName);
				QByteArray data = m_backupDatabasePacketParser->createPacket(packetData);
				sendBackupPacket(data);
			}
		}
	}
}

void ReplicationProgressWizardPage::sendBackupPacket(const QByteArray& data)
{
	RawPacketClientSocket::PacketType packetType = RawPacketClientSocket::MANAGE_PACKET_TYPE;
	QByteArray array;
	qint64 dataLen = data.size() + sizeof(int);
	array.append((char*)&dataLen, sizeof(qint64));
	array.append((char*)&packetType, sizeof(int));
	array.append(data);
	Q_ASSERT(m_rawPacketClientSocket);
	m_rawPacketClientSocket->send(array);
}

void ReplicationProgressWizardPage::processListDatabasePacket(QByteArray recvArray)
{

}

void ReplicationProgressWizardPage::processBackupDatabasePacket(QByteArray recvArray)
{
	Q_ASSERT(m_backupDatabasePacketParser.get());
	if(m_backupDatabasePacketParser->parse(QString(recvArray)))
	{
		std::tr1::shared_ptr<BackupDatabasePacketData> managePacketData = m_backupDatabasePacketParser->getPacket();
		if(managePacketData.get())
		{
			m_step++;
			ui.createPublicationProgress->setValue(m_step);

			if(managePacketData->getResult() != BackupDatabasePacketData::AcceptBackupDatabaseResult)
			{
				QMessageBox messageBox;
				messageBox.setWindowTitle("Backup copy progress");
				messageBox.setText(tr("The backuping process was ended with error on database \"%1\"").arg(m_currentDbName));
				messageBox.setInformativeText(managePacketData->getMessage());
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
				close();
				QMessageBox::information(0, tr("Backup copy progress"), tr("The backuping process was completed successfully"));
				//QMessageBox::information(0, tr("Резервное копирование"), tr("Резервное копирование успешно завершено"));
			}
		}
	}
}
