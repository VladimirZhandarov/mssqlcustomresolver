#ifndef RESTOREAPP_H
#define RESTOREAPP_H

#include <QtCore/QObject>

class QTimer;

class OsqlSettingsReader;
class RestoreWizard;

/**
* ���������� ������������� �� ��������� �����
*/
class RestoreApp : public QObject
{
	Q_OBJECT
public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	RestoreApp(QObject* parent = 0);

	/**
	* ����������
	*/
	~RestoreApp();

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
	RestoreWizard* m_restoreWizard;				///< ������ ����������� �������� �������� ��������� �����
};

#endif // RESTOREAPP_H
