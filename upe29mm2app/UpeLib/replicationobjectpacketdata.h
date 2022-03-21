#ifndef REPLICATIONOBJECTPACKETDATA_H
#define REPLICATIONOBJECTPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtCore/QVector>
#include <QtCore/QVariant>

#include "memory"

class UPELIB_EXPORT ReplicationObjectDataSet
{
public:
	/**
	* Конструктор
	* @param dbName
	* @param tableList
	* @param storedProcedureList
	* @param userFunctionList
	*/
	ReplicationObjectDataSet(const QString& dbName = "", const QStringList& tableList = QStringList(), const QStringList& storedProcedureList = QStringList(),
		const QStringList& userFunctionList = QStringList());

	/**
	* Деструктор
	*/
	~ReplicationObjectDataSet();

	const QString& getDBName() const;

	const QStringList& getTableList() const;
	const QStringList& getStoredProcedureList() const;
	const QStringList& getUserFunctionList() const;

private:
	QString m_dbName;
	QStringList m_tableList;
	QStringList m_storedProcedureList;
	QStringList m_userFunctionList;
};

class UPELIB_EXPORT ReplicationObjectPacketData : public QObject
{
	Q_OBJECT
public:
	enum ReplicationObjectResult
	{
		UndefineReplicationObjectResult = 0,	///< Не определено
		AcceptReplicationObjectResult,			///< Ок
		RejectReplicationObjectResult			///< Отмена
	};	///< Ответ от сервиса клиенту

	/**
	* Конструктор
	* @param username
	* @param password
	* @param result ответ от сервиса клиенту
	* @param parent родительский объект
	*/
	ReplicationObjectPacketData(const QString& username, const QString& password, ReplicationObjectResult result, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~ReplicationObjectPacketData();

	const QString& getUsername() const;
	const QString& getPassword() const;

	void setDBNames(const QList<ReplicationObjectDataSet>& dbNames);
	const QList<ReplicationObjectDataSet>& getDBNames() const;

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
	ReplicationObjectResult getResult() const;

	bool isCreatePublication() const;

	void setSubscriberList(QStringList subscriberList);

protected:
	QString m_username;
	QString m_password;
	QList<ReplicationObjectDataSet> m_dbNames;
	QString m_errorMessage;				///< Сообщение ошибки
	ReplicationObjectResult m_result;	///< Ответ от сервиса клиенту

	bool m_createPublication;
	QStringList m_subscriberList;
};

#endif // REPLICATIONOBJECTPACKETDATA_H
