#ifndef RESTOREAPP_H
#define RESTOREAPP_H

#include <QtCore/QObject>

class QTimer;

class OsqlSettingsReader;
class RestoreWizard;

/**
* Приложение востановления из резервной копии
*/
class RestoreApp : public QObject
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	RestoreApp(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~RestoreApp();

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
	RestoreWizard* m_restoreWizard;				///< Диалог отображения процесса создания резервной копии
};

#endif // RESTOREAPP_H
