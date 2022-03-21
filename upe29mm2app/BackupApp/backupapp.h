#ifndef BACKUPAPP_H
#define BACKUPAPP_H

#include <QtCore/QObject>

class QTimer;

class OsqlSettingsReader;
class BackupWizard;

/**
* ���������� �������� ��������� �����
*/
class BackupApp : public QObject
{
	Q_OBJECT
public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	BackupApp(QObject* parent = 0);

	/**
	* ����������
	*/
	~BackupApp();

	/**
	* ��������� application
	*/
	void exec();

protected slots:
	/**
	* ��������� ������� �������� ��������� �����
	*/
	void onTimeout();

private:
	OsqlSettingsReader* m_osqlSettingsReader;	///< Reader ������ ��� ������ ��� �������� ��������� �����
	QTimer* m_timer;							///< ������ ������� �������� �������� ��������� �����
	BackupWizard* m_backupWizard;				///< ������ ����������� �������� �������� ��������� �����
};

#endif // BACKUPAPP_H
