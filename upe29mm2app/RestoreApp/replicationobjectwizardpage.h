#ifndef REPLICATIONOBJECTWIZARDPAGE_H
#define REPLICATIONOBJECTWIZARDPAGE_H

#include <QtWidgets/QWizardPage>

#include "ui_replicationobjectwizardpage.h"

#include "memory"

class RawPacketClientSocket;
class XsdReplicationObjectPacketParser;

class OsqlSettingsReader;

class ReplicationObjectWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	ReplicationObjectWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent = 0);
	~ReplicationObjectWizardPage();

	void init(const QString& username, const QString& password, const QStringList& dbNameList);

	virtual bool isComplete() const;

protected slots:
	void onReplicationObjectCheckStateChanged();

	void onDataReceived(int packetType, QByteArray recvArray);

signals:
	void pushButtonChanged();

private:
	Ui::ReplicationObjectWizardPage ui;

	RawPacketClientSocket* m_rawPacketClientSocket;
	std::tr1::shared_ptr<XsdReplicationObjectPacketParser> m_replicationObjectPacketParser;

	QString m_host;
	quint16 m_port;
	bool m_initedData;
};

#endif // REPLICATIONOBJECTWIZARDPAGE_H