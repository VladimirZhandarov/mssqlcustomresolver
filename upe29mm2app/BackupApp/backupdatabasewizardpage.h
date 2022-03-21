#ifndef BACKUPDATABASEWIZARDPAGE_H
#define BACKUPDATABASEWIZARDPAGE_H

#include <QtWidgets/QWizardPage>

#include "ui_backupdatabasewizardpage.h"

#include "memory"

class OsqlSettingsReader;
class RawPacketClientSocket;
class XsdListDatabasePacketParser;

class BackupDatabaseWizardPage : public QWizardPage
{
	Q_OBJECT
	static QStringList notUsedatabases;

public:
	BackupDatabaseWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent = 0);
	~BackupDatabaseWizardPage();

	void init(const QString& username, const QString& password, bool localDataBase);

	virtual bool isComplete() const;

	QStringList getDBNameList() const;

protected slots:
	void onDatabasesCheckStateChanged();

	void onDataReceived(int packetType, QByteArray recvArray);

private:
	Ui::BackupDatabaseWizardPage ui;

	RawPacketClientSocket* m_rawPacketClientSocket;

	QString m_dbServerInstanceName;
	QStringList m_checkedDatabases;
	QStringList m_systemDatabases;
	QString m_host;
	quint16 m_port;

	std::tr1::shared_ptr<XsdListDatabasePacketParser> m_listDatabasePacketParser;
};

#endif // BACKUPDATABASEWIZARDPAGE_H