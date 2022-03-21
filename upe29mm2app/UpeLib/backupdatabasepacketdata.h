#ifndef BACKUPDATABASEPACKETDATA_H
#define BACKUPDATABASEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT BackupDatabasePacketData : public QObject
{
	Q_OBJECT
public:
	enum BackupDatabaseResult
	{
		UndefineBackupDatabaseResult = 0,   ///< Не определено
		AcceptBackupDatabaseResult,         ///< Ок
		RejectBackupDatabaseResult,         ///< Отмена
	}; ///< Ответ от сервиса клиенту

	/**
	* Конструктор
	* @param result ответ от сервиса клиенту
	* @param parent родительский объект
	*/
	BackupDatabasePacketData(const QString& username, const QString& password, BackupDatabaseResult result, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~BackupDatabasePacketData();

	const QString& getUsername() const;
	const QString& getPassword() const;

	/**
	* Установить имя базы данных
	* @param dbName имя базы данных
	*/
	void setDBName(const QString& dbName);

	/**
	* Получить имя базы данных
	* @return имя базы данных
	*/
	const QString& getDBName() const;

	/**
	* Установить сообщение
	* @param errorMessage сообщение
	*/
	void setMessage(const QString& message);

	/**
	* Получить сообщение
	* @return сообщение
	*/
	const QString& getMessage() const;

	/**
	* Получить ответ от сервиса клиенту
	* @return ответ от сервиса клиенту
	*/
	BackupDatabaseResult getResult() const;

protected:
	QString m_username;
	QString m_password;
	QString m_dbName;				///< Имя базы данных
	QString m_message;				///< Сообщение
	BackupDatabaseResult m_result;	///< Ответ от сервиса клиенту
};

#endif // BACKUPDATABASEPACKETDATA_H
