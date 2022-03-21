#ifndef RESTOREWIZARD_H
#define RESTOREWIZARD_H

#include <QtWidgets/QWizard>

class OsqlSettingsReader;
class RawPacketClientSocket;

class RestoreSettingWizardPage;
class RestoreProgressWizardPage;
class ReplicationObjectWizardPage;
class ReplicationSubscriberWizardPage;
class ReplicationProgressWizardPage;

class RestoreWizard : public QWizard
{
	Q_OBJECT
public:
	RestoreWizard(OsqlSettingsReader* osqlSettingsReader, QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
	~RestoreWizard();

protected slots:
	void onCurrentIdChanged(int id);
	void onFinishedPage2();

private:
	OsqlSettingsReader* m_osqlSettingsReader;
	RawPacketClientSocket* m_rawPacketClientSocket;

	RestoreSettingWizardPage* m_page1;
	RestoreProgressWizardPage* m_page2;
	ReplicationObjectWizardPage* m_page3;
	ReplicationSubscriberWizardPage* m_page4;
	ReplicationProgressWizardPage* m_page5;
};

#endif // RESTOREPWIZARD_H