#ifndef REPLICATIONSUBSCRIBERWIZARDPAGE_H
#define REPLICATIONSUBSCRIBERWIZARDPAGE_H

#include <QtWidgets/QWizardPage>

#include "ui_replicationsubscriberwizardpage.h"

#include "memory"

class RawPacketClientSocket;
class XsdListServerPacketParser;

class OsqlSettingsReader;

class ReplicationSubscriberWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	ReplicationSubscriberWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent = 0);
	~ReplicationSubscriberWizardPage();

	void init();

	virtual bool isComplete() const;

protected slots:
	void onDataReceived(int packetType, QByteArray recvArray);
	void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void onTextChanged(QString text);

	void addSubscriberPB_clicked();
	void removeSubscriberPB_clicked();

private:
	Ui::ReplicationSubscriberWizardPage ui;

	RawPacketClientSocket* m_rawPacketClientSocket;
	std::tr1::shared_ptr<XsdListServerPacketParser> m_listServerPacketParser;

	QString m_host;
	quint16 m_port;
	bool m_initedData;
};

#endif // REPLICATIONSUBSCRIBERWIZARDPAGE_H