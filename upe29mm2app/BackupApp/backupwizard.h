#ifndef BACKUPWIZARD_H
#define BACKUPWIZARD_H

#include <QtWidgets/QWizard>

class OsqlSettingsReader;
class RawPacketClientSocket;

class BackupSettingWizardPage;
class BackupDatabaseWizardPage;
class BackupProgressWizardPage;

class BackupWizard : public QWizard
{
	Q_OBJECT
public:
	BackupWizard(OsqlSettingsReader* osqlSettingsReader, QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
	~BackupWizard();

protected slots:
	void onCurrentIdChanged(int id);
	void onFinished();

private:
	OsqlSettingsReader* m_osqlSettingsReader;
	RawPacketClientSocket* m_rawPacketClientSocket;

	BackupSettingWizardPage* m_page1;
	BackupDatabaseWizardPage* m_page2;
	BackupProgressWizardPage* m_page3;
};

#endif // BACKUPWIZARD_H