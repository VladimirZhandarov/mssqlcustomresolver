#include "backupdatabasewizardpage.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <databasetreemodel.h>

#include <xsdlistdatabasepacketparser.h>
#include <rawpacketclientsocket.h>

#include <osqlsettingsreader.h>

#include <QtWidgets/QMessageBox>

QStringList BackupDatabaseWizardPage::notUsedatabases = QStringList();

BackupDatabaseWizardPage::BackupDatabaseWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent) :
	QWizardPage(parent), m_rawPacketClientSocket(rawPacketClientSocket)
{
	if(notUsedatabases.isEmpty())
	{
		notUsedatabases << "master";
		notUsedatabases << "model";
		notUsedatabases << "msdb";
		notUsedatabases << "tempdb";
		notUsedatabases << "Resource";
		notUsedatabases << "distribution";
		notUsedatabases << "reportserver";
		notUsedatabases << "reportservertempdb";
	}
	m_systemDatabases.clear();

	ui.setupUi(this);
	ui.databasesTV->setHeaderHidden(true);

	Q_ASSERT(osqlSettingsReader);
	m_dbServerInstanceName = osqlSettingsReader->getDBServerInstanceName();
	m_checkedDatabases = osqlSettingsReader->getDBNameList();
	m_host = osqlSettingsReader->getHostName();
	m_port = osqlSettingsReader->getPort();

	//ui.databasesL->setText(tr("Базы данных:"));
	//ui.withSystemDatabasesChB->setText(tr("С системными базами данных"));

	DataBaseTreeModel* dataBaseTreeModel = new DataBaseTreeModel();
	ui.databasesTV->setModel(dataBaseTreeModel);
	connect(dataBaseTreeModel, SIGNAL(checkStateChanged()), this, SLOT(onDatabasesCheckStateChanged()));

	m_listDatabasePacketParser = std::tr1::shared_ptr<XsdListDatabasePacketParser>(new XsdListDatabasePacketParser());
}

BackupDatabaseWizardPage::~BackupDatabaseWizardPage()
{

}

void BackupDatabaseWizardPage::init(const QString& username, const QString& password, bool localDataBase)
{
	if(localDataBase)
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
		if(username.isEmpty() || password.isEmpty())
		{
			db.setDatabaseName("DRIVER={SQL Server};Server=" + m_dbServerInstanceName  + ";Trusted_Connection=yes;");
		}
		else
		{
			db.setDatabaseName("DRIVER={SQL Server};Server=" + m_dbServerInstanceName + ";");
			db.setUserName(username);
			db.setPassword(password);
		}

		if(db.open())
		{
			QSqlQuery query;
			query.exec("SELECT NAME FROM sys.sysdatabases");
			QStringList databases;
			while(query.next())
			{
				QString name = query.value(0).toString();
				if(!notUsedatabases.contains(name.toLower()) && !name.toLower().startsWith("reportserver$"))
				{
					databases.append(name);
				}
				else
				{
					m_systemDatabases.append(name);
				}
			}
			DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
			if(dataBaseTreeModel)
			{
				dataBaseTreeModel->setDatabases(databases, m_checkedDatabases);
				onDatabasesCheckStateChanged();
			}
		}
		else
		{
			QMessageBox::warning(this, tr("Backup copy progress"), tr("Could not open connection with local sql server\r\nError:") +
				db.lastError().text());
		}
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
			Q_ASSERT(m_listDatabasePacketParser.get());
			std::tr1::shared_ptr<ListDatabasePacketData> packetData = std::tr1::shared_ptr<ListDatabasePacketData>(new ListDatabasePacketData(
				username, password, ListDatabasePacketData::UndefineListDatabaseResult));
			QByteArray data = m_listDatabasePacketParser->createPacket(packetData);
			m_rawPacketClientSocket->sendListDatabasePacket(data);
		}
	}
}

bool BackupDatabaseWizardPage::isComplete() const
{
	DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
	if(dataBaseTreeModel)
	{
		return !dataBaseTreeModel->getCheckedDatabases().isEmpty();
	}
	else
	{
		return true;
	}
}

QStringList BackupDatabaseWizardPage::getDBNameList() const
{
	QStringList result;
	DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
	if(dataBaseTreeModel)
	{
		result = dataBaseTreeModel->getCheckedDatabases();
	}
	return result;
}

void BackupDatabaseWizardPage::onDatabasesCheckStateChanged()
{
	emit completeChanged();
}

void BackupDatabaseWizardPage::onDataReceived(int packetType, QByteArray recvArray)
{
	Q_ASSERT(m_listDatabasePacketParser.get());
	if(packetType == RawPacketClientSocket::LIST_DATABASE_PACKET_TYPE)
	{
		if(m_listDatabasePacketParser->parse(QString(recvArray)))
		{
			std::tr1::shared_ptr<ListDatabasePacketData> listDatabasePacketData = m_listDatabasePacketParser->getPacket();
			if(listDatabasePacketData.get())
			{
				QStringList allDatabases = listDatabasePacketData->getDBNames();
				QStringList databases;
				for(int i = 0; i < allDatabases.size(); i++)
				{
					QString name = allDatabases[i];
					if(!notUsedatabases.contains(name.toLower()) && !name.toLower().startsWith("reportserver$"))
					{
						databases.append(name);
					}
					else
					{
						m_systemDatabases.append(name);
					}
				}
				DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
				if(dataBaseTreeModel)
				{
					dataBaseTreeModel->setDatabases(databases, m_checkedDatabases);
					onDatabasesCheckStateChanged();
				}
			}
		}
	}
}
