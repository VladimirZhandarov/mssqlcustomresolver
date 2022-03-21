#ifndef LISTDATABASEPACKETDATA_H
#define LISTDATABASEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT ListDatabasePacketData : public QObject
{
	Q_OBJECT
public:
	enum ListDatabaseResult
	{
		UndefineListDatabaseResult = 0,	///< Не определено
		AcceptListDatabaseResult,		///< Ок
		RejectListDatabaseResult		///< Отмена
	};	///< Ответ от сервиса клиенту

	/**
	* Конструктор
	* @param username
	* @param password
	* @param result ответ от сервиса клиенту
	* @param parent родительский объект
	*/
	ListDatabasePacketData(const QString& username, const QString& password, ListDatabaseResult result, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~ListDatabasePacketData();

	const QString& getUsername() const;
	const QString& getPassword() const;

	void setDBNames(const QStringList& dbNames);
	const QStringList& getDBNames() const;

	/**
	* Установить сообщение ошибки
	* @param errorMessage сообщение ошибки
	*/
	void setErrorMessage(const QString& errorMessage);

	/**
	* Получить сообщение ошибки
	* @return сообщение ошибки
	*/
	const QString& getErrorMessage() const;

	/**
	* Получить ответ от сервиса клиенту
	* @return ответ от сервиса клиенту
	*/
	ListDatabaseResult getResult() const;

protected:
	QString m_username;
	QString m_password;
	QStringList m_dbNames;		///< Имена баз данных
	QString m_errorMessage;			///< Сообщение ошибки
	ListDatabaseResult m_result;	///< Ответ от сервиса клиенту
};

#endif // LISTDATABASEPACKETDATA_H
