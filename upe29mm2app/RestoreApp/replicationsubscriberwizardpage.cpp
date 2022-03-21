#include "replicationsubscriberwizardpage.h"

#include <QtWidgets/QMessageBox>

#include <rawpacketclientsocket.h>
#include <xsdlistserverpacketparser.h>

#include <osqlsettingsreader.h>

ReplicationSubscriberWizardPage::ReplicationSubscriberWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket,
	QWidget* parent) : QWizardPage(parent), m_rawPacketClientSocket(rawPacketClientSocket), m_initedData(false)
{
	ui.setupUi(this);

	Q_ASSERT(osqlSettingsReader);
	m_host = osqlSettingsReader->getHostName();
	m_port = osqlSettingsReader->getPort();

	m_listServerPacketParser = std::tr1::shared_ptr<XsdListServerPacketParser>(new XsdListServerPacketParser());

	connect(ui.subscribersLW, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		this, SLOT(onCurrentItemChanged(QListWidgetItem*, QListWidgetItem*)));
	connect(ui.newSubscriberLE, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

	/*ui.subscribersL->setText(tr("Подписчики:"));
	ui.newSubscriberL->setText(tr("Новый подписчик:"));
	ui.removeSubscriberPB->setText(tr("Удалить"));
	ui.addSubscriberPB->setText(tr("Добавить"));*/
}

ReplicationSubscriberWizardPage::~ReplicationSubscriberWizardPage()
{

}

void ReplicationSubscriberWizardPage::init()
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
			Q_ASSERT(m_listServerPacketParser.get());
			std::tr1::shared_ptr<ListServerPacketData> packetData = std::tr1::shared_ptr<ListServerPacketData>(new ListServerPacketData(
				ListServerPacketData::UndefineListServerResult));
			QByteArray data = m_listServerPacketParser->createPacket(packetData);
			m_rawPacketClientSocket->sendListServerPacket(data);
			ui.subscribersL->setText(tr("Subscribers (Waiting for server instance list):"));
			ui.removeSubscriberPB->setEnabled(false);
			ui.newSubscriberL->setEnabled(false);
			ui.newSubscriberLE->setEnabled(false);
			ui.addSubscriberPB->setEnabled(false);
		}
	}
	m_initedData = true;
}

bool ReplicationSubscriberWizardPage::isComplete() const
{
	return true;
}

void ReplicationSubscriberWizardPage::onDataReceived(int packetType, QByteArray recvArray)
{
	Q_ASSERT(m_listServerPacketParser.get());
	if(packetType == RawPacketClientSocket::LIST_SERVER_PACKET_TYPE)
	{
		if(m_listServerPacketParser->parse(QString(recvArray)))
		{
			std::tr1::shared_ptr<ListServerPacketData> listServerPacketData = m_listServerPacketParser->getPacket();
			if(listServerPacketData.get())
			{
				QStringList serverNames = listServerPacketData->getServerNames();
				ui.subscribersLW->addItems(serverNames);
			}
		}
		ui.subscribersL->setText(tr("Subscribers (Received server instance list):"));
		ui.newSubscriberL->setEnabled(true);
		ui.newSubscriberLE->setEnabled(true);
		onCurrentItemChanged(ui.subscribersLW->currentItem(), 0);
		onTextChanged(ui.newSubscriberLE->text());
	}
}

void ReplicationSubscriberWizardPage::onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
	if(current)
	{
		ui.removeSubscriberPB->setEnabled(true);
	}
	else
	{
		ui.removeSubscriberPB->setEnabled(false);
	}
}

void ReplicationSubscriberWizardPage::onTextChanged(QString text)
{
	if(text.trimmed().isEmpty())
	{
		ui.addSubscriberPB->setEnabled(false);
	}
	else
	{
		ui.addSubscriberPB->setEnabled(true);
	}
}

void ReplicationSubscriberWizardPage::addSubscriberPB_clicked()
{
	ui.subscribersLW->addItem(ui.newSubscriberLE->text().trimmed());
}

void ReplicationSubscriberWizardPage::removeSubscriberPB_clicked()
{
	int row = ui.subscribersLW->currentRow();
	if(row >= 0 && row < ui.subscribersLW->count())
	{
		QListWidgetItem* item = ui.subscribersLW->takeItem(row);
		delete item;
	}
}
