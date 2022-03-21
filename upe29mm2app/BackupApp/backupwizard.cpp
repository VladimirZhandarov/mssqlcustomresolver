#include "backupwizard.h"

#include <QtNetwork/QTcpSocket>

#include "backupsettingwizardpage.h"
#include "backupdatabasewizardpage.h"
#include "backupprogresswizardpage.h"

#include <osqlsettingsreader.h>
#include <rawpacketclientsocket.h>

#define DATABASE_PAGE_INDEX 1
#define PROGRESS_PAGE_INDEX 2

BackupWizard::BackupWizard(OsqlSettingsReader* osqlSettingsReader, QWidget* parent, Qt::WindowFlags f) : QWizard(parent, f),
	m_osqlSettingsReader(osqlSettingsReader)
{
	setWindowTitle(tr("UPE-29M/M2 backup database"));
	//setWindowTitle(tr("УПЭ-29M/M2 резервное копирование"));

	setWizardStyle(QWizard::ClassicStyle);

	QTcpSocket* tcpSocket = new QTcpSocket(this);
	m_rawPacketClientSocket = new RawPacketClientSocket(tcpSocket->socketDescriptor(), this);

	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(onCurrentIdChanged(int)));
	m_page1 = new BackupSettingWizardPage(m_osqlSettingsReader);
	addPage(m_page1);
	m_page2 = new BackupDatabaseWizardPage(m_osqlSettingsReader, m_rawPacketClientSocket);
	addPage(m_page2);
	m_page3 = new BackupProgressWizardPage(m_osqlSettingsReader, m_rawPacketClientSocket);
	connect(m_page3, SIGNAL(finishedSignal()), this, SLOT(onFinished()));
	addPage(m_page3);
}

BackupWizard::~BackupWizard()
{

}

void BackupWizard::onCurrentIdChanged(int id)
{
	if(id == DATABASE_PAGE_INDEX)
	{
		Q_ASSERT(m_page1 && m_page2 && m_osqlSettingsReader);
		m_osqlSettingsReader->setLastBackupDir(m_page1->getDirectory());
		m_osqlSettingsReader->save();
		connect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page2, SLOT(onDataReceived(int, QByteArray)));
		m_page2->init(m_page1->getUsername(), m_page1->getPassword(), m_page1->isLocalDataBase());
	}
	if(id == PROGRESS_PAGE_INDEX)
	{
		Q_ASSERT(m_page1 && m_page2 && m_page3);
		disconnect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page2, SLOT(onDataReceived(int, QByteArray)));
		connect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), m_page3, SLOT(onDataReceived(int, QByteArray)));
		m_page3->init(m_page1->getDirectory(), m_page1->getUsername(), m_page1->getPassword(), m_page1->isLocalDataBase(),
			m_page2->getDBNameList());
		QWizard::button(QWizard::BackButton)->setEnabled(false);
	}
}

void BackupWizard::onFinished()
{
	QWizard::button(QWizard::FinishButton)->setEnabled(true);
}
