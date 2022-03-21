#ifndef RESTOREDATABASEPACKETDATA_H
#define RESTOREDATABASEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT RestoreDatabasePacketData : public QObject
{
	Q_OBJECT
public:
	enum RestoreDatabaseResult
	{
		UndefineRestoreDatabaseResult = 0,   ///< Не определено
		AcceptRestoreDatabaseResult,         ///< Ок
		RejectRestoreDatabaseResult,         ///< Отмена
	}; ///< Ответ от сервиса клиенту

	/**
	* Конструктор
	* @param result ответ от сервиса клиенту
	* @param parent родительский объект
	*/
	RestoreDatabasePacketData(const QString& username, const QString& password, RestoreDatabaseResult result, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~RestoreDatabasePacketData();

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
	RestoreDatabaseResult getResult() const;

protected:
	QString m_username;
	QString m_password;
	QString m_dbName;				///< Имя базы данных
	QString m_message;				///< Сообщение
	RestoreDatabaseResult m_result;	///< Ответ от сервиса клиенту
};

#endif // RESTOREDATABASEPACKETDATA_H
