#ifndef MANAGEPACKETDATA_H
#define MANAGEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

/**
* @class ManagePacketData
* Пакет управления процессом синхронизации
*/
class UPELIB_EXPORT ManagePacketData : public QObject
{
	Q_OBJECT
public:
	enum ManageType
	{
		UndefineManageType = -1,	///< Не определено
		StartManageType = 0,		///< Начать синхронизацию
		StopManageType,				///< Остановить синхронизацию
		NextStepManageType,			///< Начать следующий шаг синхронизации
		NextStepEndedManageType,	///< Шаг синхронизации окончен
		StartMainClientType,		///< Начать синхронизацию как главный клиент
		StopMainClientType			///< Остановить синхронизацию как главный клиент
	};	///< Подтип пакета

	enum ManageResult
	{
		UndefineManageResult = 0,	///< Не определено
		AcceptManageResult,			///< Ок
		RejectManageResult,			///< Отмена
		SnapShotNeededManageResult,	///< Нужен snapshot
		SnapShotEndedManageResult	///< Snapshot сделан
	};	///< Ответ от сервиса клиенту

	/**
	* Конструктор
	* @param manageType подтип пакета
	* @param dbServerInstanceName имя СУБД
	* @param result ответ от сервиса клиенту
	* @param parent родительский объект
	*/
	ManagePacketData(ManageType manageType, const QString& dbServerInstanceName, ManageResult result, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~ManagePacketData();

	/**
	* Получить подтип пакета
	* @return подтип пакета
	*/
	ManageType getManageType() const;

	/**
	* Установить признак главного клиента
	* @param mainClient признак главного клиента
	*/
	void setMainClient(bool mainClient);

	/**
	* Получить признак главного клиента
	* @return признак главного клиента
	*/
	bool isMainClient() const;

	/**
	* Установить признак разрешения конфликта на мобильном клиенте
	* @param ownResolve признак разрешения конфликта на мобильном клиенте
	*/
	void setOwnResolve(bool ownResolve);

	/**
	* Получить признак разрешения конфликта на мобильном клиенте
	* @return признак разрешения конфликта на мобильном клиенте
	*/
	bool isOwnResolve() const;

	/**
	* Получить имя СУБД
	* @return имя СУБД
	*/
	const QString& getDBServerInstanceName() const;

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
	* Установить язык
	* @param language язык
	*/
	void setLanguage(const QString& language);

	/**
	* Получить язык
	* @return язык
	*/
	const QString& getLanguage() const;

	/**
	* Установить код ошибки
	* @param erroeCode код ошибки
	*/
	void setErrorCode(int erroeCode);

	/**
	* Получить код ошибки
	* @return код ошибки
	*/
	int getErrorCode() const;

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
	ManageResult getResult() const;

protected:
	ManageType m_manageType;			///< Подтип пакета
	bool m_mainClient;					///< Признак главного клиента
	bool m_ownResolve;					///< Признак разрешения конфликта на мобильном клиенте
	QString m_dbServerInstanceName;		///< Имя СУБД
	QString m_dbName;					///< Имя базы данных
	QString m_language;					///< Язык
	int m_errorCode;					///< Код ошибки
	QString m_errorMessage;				///< Сообщение ошибки
	ManageResult m_result;				///< Ответ от сервиса клиенту
};

#endif // MANAGEPACKETDATA_H
