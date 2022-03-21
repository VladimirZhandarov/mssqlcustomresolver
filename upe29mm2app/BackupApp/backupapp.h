#ifndef BACKUPAPP_H
#define BACKUPAPP_H

#include <QtCore/QObject>

class QTimer;

class OsqlSettingsReader;
class BackupWizard;

/**
* Приложение создания резервной копии
*/
class BackupApp : public QObject
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	BackupApp(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~BackupApp();

	/**
	* Выполнить application
	*/
	void exec();

protected slots:
	/**
	* Запустить процесс создания резервной копии
	*/
	void onTimeout();

private:
	OsqlSettingsReader* m_osqlSettingsReader;	///< Reader списка баз данных для создания резервной копии
	QTimer* m_timer;							///< Таймер запуска процесса создания резервной копии
	BackupWizard* m_backupWizard;				///< Диалог отображения процесса создания резервной копии
};

#endif // BACKUPAPP_H
