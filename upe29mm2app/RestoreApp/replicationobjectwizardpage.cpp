#include "replicationobjectwizardpage.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include <databasetreemodel.h>

#include <rawpacketclientsocket.h>
#include <xsdreplicationobjectpacketparser.h>

#include <osqlsettingsreader.h>

ReplicationObjectWizardPage::ReplicationObjectWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket,
	QWidget* parent) : QWizardPage(parent), m_rawPacketClientSocket(rawPacketClientSocket), m_initedData(false)
{
	ui.setupUi(this);

	Q_ASSERT(osqlSettingsReader);
	m_host = osqlSettingsReader->getHostName();
	m_port = osqlSettingsReader->getPort();

	m_replicationObjectPacketParser = std::tr1::shared_ptr<XsdReplicationObjectPacketParser>(new XsdReplicationObjectPacketParser());

	DataBaseTreeModel* replicationObjectTreeModel = new DataBaseTreeModel();
	ui.replicationObjectsTV->setModel(replicationObjectTreeModel);
	ui.replicationObjectsTV->setHeaderHidden(true);

	connect(replicationObjectTreeModel, SIGNAL(checkStateChanged()), this, SLOT(onReplicationObjectCheckStateChanged()));

	//ui.replicationObjectsL->setText(tr("Объекты для публикации:"));
}

ReplicationObjectWizardPage::~ReplicationObjectWizardPage()
{

}

void ReplicationObjectWizardPage::init(const QString& username, const QString& password, const QStringList& dbNameList)
{
	if(m_initedData)
	{
		return;
	}
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
			Q_ASSERT(m_replicationObjectPacketParser.get());
			std::tr1::shared_ptr<ReplicationObjectPacketData> packetData = std::tr1::shared_ptr<ReplicationObjectPacketData>(new ReplicationObjectPacketData(
				username, password, ReplicationObjectPacketData::UndefineReplicationObjectResult));
			QList<ReplicationObjectDataSet> dbNames;
			for(int i = 0; i < dbNameList.size(); i++)
			{
				dbNames.append(ReplicationObjectDataSet(dbNameList[i]));
			}
			packetData->setDBNames(dbNames);
			QByteArray data = m_replicationObjectPacketParser->createPacket(packetData);
			m_rawPacketClientSocket->sendReplicationObjectPacket(data);
		}
	}
	m_initedData = true;
}

bool ReplicationObjectWizardPage::isComplete() const
{
	DataBaseTreeModel* replicationObjectTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.replicationObjectsTV->model());
	if(replicationObjectTreeModel)
	{
		return !replicationObjectTreeModel->getCheckedDatabases().isEmpty();
	}
	else
	{
		return true;
	}
}

void ReplicationObjectWizardPage::onReplicationObjectCheckStateChanged()
{
	emit completeChanged();
}

void ReplicationObjectWizardPage::onDataReceived(int packetType, QByteArray recvArray)
{
	Q_ASSERT(m_replicationObjectPacketParser.get());
	if(packetType == RawPacketClientSocket::REPLICATION_OBJECT_PACKET_TYPE)
	{
		DataBaseTreeModel* replicationObjectTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.replicationObjectsTV->model());
		Q_ASSERT(replicationObjectTreeModel);
		if(m_replicationObjectPacketParser->parse(QString(recvArray)))
		{
			std::tr1::shared_ptr<ReplicationObjectPacketData> replicationObjectPacketData = m_replicationObjectPacketParser->getPacket();
			if(replicationObjectPacketData.get())
			{
				const QList<ReplicationObjectDataSet>& dbNames = replicationObjectPacketData->getDBNames();
				QStringList databases;
				QStringList checkedDatabases;
				for(int i = 0; i < dbNames.size(); i++)
				{
					databases.append(dbNames[i].getDBName());
					checkedDatabases.append(dbNames[i].getDBName());
				}
				replicationObjectTreeModel->setDatabases(databases, checkedDatabases);
				for(int i = 0; i < dbNames.size(); i++)
				{
					QString dbName = dbNames[i].getDBName();
					if(!dbNames[i].getTableList().isEmpty())
					{
						replicationObjectTreeModel->setTables(dbName, dbNames[i].getTableList());
					}
					if(!dbNames[i].getStoredProcedureList().isEmpty())
					{
						replicationObjectTreeModel->setStoredProcedures(dbName, dbNames[i].getStoredProcedureList());
					}
					if(!dbNames[i].getUserFunctionList().isEmpty())
					{
						replicationObjectTreeModel->setUserFunctions(dbName, dbNames[i].getUserFunctionList());
					}
				}
			}
		}
	}
	emit completeChanged();
}
