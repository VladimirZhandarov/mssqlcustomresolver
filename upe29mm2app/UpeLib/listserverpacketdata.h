#ifndef LISTSERVERPACKETDATA_H
#define LISTSERVERPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT ListServerPacketData : public QObject
{
	Q_OBJECT
public:
	enum ListServerResult
	{
		UndefineListServerResult = 0,	///< Не определено
		AcceptListServerResult,		///< Ок
		RejectListServerResult		///< Отмена
	};	///< Ответ от сервиса клиенту

	/**
	* Конструктор
	* @param result ответ от сервиса клиенту
	* @param parent родительский объект
	*/
	ListServerPacketData(ListServerResult result, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~ListServerPacketData();

	void setServerNames(const QStringList& serverNames);
	const QStringList& getServerNames() const;

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
	ListServerResult getResult() const;

protected:
	QStringList m_serverNames;
	QString m_errorMessage;			///< Сообщение ошибки
	ListServerResult m_result;	///< Ответ от сервиса клиенту
};

#endif // LISTSERVERPACKETDATA_H
