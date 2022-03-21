#include "restorewizard.h"

#include <QtNetwork/QTcpSocket>

#include "restoresettingwizardpage.h"
#include "restoreprogresswizardpage.h"
#include "replicationobjectwizardpage.h"
#include "replicationsubscriberwizardpage.h"
#include "replicationprogresswizardpage.h"

#include <osqlsettingsreader.h>
#include <rawpacketclientsocket.h>

#define START_PAGE_INDEX 0
#define PROGRESS_PAGE_INDEX 1
#define PUBLICATION_PAGE_INDEX 2
#define SUBSCRIPTION_PAGE_INDEX 3
#define PROGRESS2_PAGE_INDEX 4

RestoreWizard::RestoreWizard(OsqlSettingsReader* osqlSettingsReader, QWidget* parent, Qt::WindowFlags f) : QWizard(parent, f),
	m_osqlSettingsReader(osqlSettingsReader)
{
	setWindowTitle(tr("UPE-29M/M2 restore database"));
	//setWindowTitle(tr("УПЭ-29M/M2 восстановление баз данных"));

	setWizardStyle(QWizard::ClassicStyle);

	QTcpSocket* tcpSocket = new QTcpSocket(this);
	m_rawPacketClientSocket = new RawPacketClientSocket(tcpSocket->socketDescriptor(), this);

	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(onCurrentIdChanged(int)));
	m_page1 = new RestoreSettingWizardPage(m_osqlSettingsReader);
	addPage(m_page1);
	m_page2 = new RestoreProgressWizardPage(m_osqlSettingsReader, m_rawPacketClientSocket);
	connect(m_page2, SIGNAL(finishedSignal()), this, SLOT(onFinishedPage2()));
	addPage(m_page2);
	m_page3 = new ReplicationObjectWizardPage(m_osqlSettingsReader, m_rawPacketClientSocket);
	addPage(m_page3);
	m_page4 = new ReplicationSubscriberWizardPage(m_osqlSettingsReader, m_rawPacketClientSocket);
	addPage(m_page4);
	m_page5 = new ReplicationProgressWizardPage(m_osqlSettingsReader, m_rawPacketClientSocket);
	addPage(m_page5);
}

RestoreWizard::~RestoreWizard()
{

}

void RestoreWizard::RestoreWizard::onCurrentIdChanged(int id)
{
	if(id == PROGRESS_PAGE_INDEX)
	{
		Q_ASSERT(m_page1 && m_page2);
		connect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page2, SLOT(onDataReceived(int, QByteArray)));
		m_page2->init(m_page1->getDirectory(), m_page1->getUsername(), m_page1->getPassword(), m_page1->isLocalDataBase(), m_page1->getDBNameList());
		QWizard::button(QWizard::BackButton)->setEnabled(false);
		if(m_page1->isLocalDataBase())
		{
			QWizard::button(QWizard::NextButton)->setVisible(false);
			QWizard::button(QWizard::FinishButton)->setVisible(true);
		}
	}
	else if(id == PUBLICATION_PAGE_INDEX)
	{
		removePage(START_PAGE_INDEX);
		removePage(PROGRESS_PAGE_INDEX);
		Q_ASSERT(m_page2 && m_page3);
		disconnect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page2, SLOT(onDataReceived(int, QByteArray)));
		connect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page3, SLOT(onDataReceived(int, QByteArray)));
		m_page3->init(m_page1->getUsername(), m_page1->getPassword(), m_page1->getDBNameList());
		QWizard::button(QWizard::BackButton)->setEnabled(false);
	}
	else if(id == SUBSCRIPTION_PAGE_INDEX)
	{
		Q_ASSERT(m_page3 && m_page4);
		disconnect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page3, SLOT(onDataReceived(int, QByteArray)));
		connect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page4, SLOT(onDataReceived(int, QByteArray)));
		m_page4->init();
	}
	else if(id == PROGRESS2_PAGE_INDEX)
	{
		Q_ASSERT(m_page3 && m_page4 && m_page5);
		m_page5->init();
		QWizard::button(QWizard::BackButton)->setEnabled(false);
	}
}

void RestoreWizard::onFinishedPage2()
{
	Q_ASSERT(m_page1);
	if(m_page1->isLocalDataBase())
	{
		QWizard::button(QWizard::FinishButton)->setEnabled(true);
	}
	else
	{
		QWizard::button(QWizard::NextButton)->setEnabled(true);
	}
}
