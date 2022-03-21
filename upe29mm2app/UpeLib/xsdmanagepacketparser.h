#ifndef XSDMANAGEPACKETPARSER_H
#define XSDMANAGEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "managepacketdata.h"

/**
* @class XsdManagePacketParser
* Parser пакета управления
*/
class UPELIB_EXPORT XsdManagePacketParser : public QObject
{
	Q_OBJECT
	static QString managePacketTagName;

	static QString manageTypeTagName;
	static QString mainClientTagName;
	static QString ownResolveTagName;
	static QString dbServerInstanceNameTagName;
	static QString dbNameTagName;
	static QString languageTagName;
	static QString errorCodeTagName;
	static QString errorMessageTagName;
	static QString resultTagName;

public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	XsdManagePacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdManagePacketParser();

	/**
	* Получить пакет управления
	* @return пакет управления
	*/
	std::tr1::shared_ptr<ManagePacketData> getPacket() const;

	/**
	* Создать пакет управления
	* @param packetData пакет управления
	* @return пакет
	*/
	QByteArray createPacket(const std::tr1::shared_ptr<ManagePacketData>& packetData);

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

	/**
	* Установить ответ от сервиса клиенту - ок
	* @return пакет
	*/
	QByteArray setAcceptResult();

	/**
	* Установить ответ от сервиса клиенту - отмена
	* @return пакет
	*/
	QByteArray setRejectResult();

private:
	/**
	* Установить подтип пакета
	* @param manageType подтип пакета
	*/
	void setType(ManagePacketData::ManageType manageType);

	/**
	* Установить признак главного клиента
	* @param mainClient признак главного клиента
	*/
	void setMainClient(bool mainClient);

	/**
	* Установить признак разрешения конфликта на мобильном клиенте
	* @param ownResolve признак разрешения конфликта на мобильном клиенте
	*/
	void setOwnResolve(bool ownResolve);

	/**
	* Установить имя СУБД
	* @param dbServerInstanceName имя СУБД
	*/
	void setDBServerInstanceName(const QString& dbServerInstanceName);

	/**
	* Установить имя базы данных
	* @param dbName имя базы данных
	*/
	void setDBName(const QString& dbName);

	/**
	* Установить язык
	* @param language язык
	*/
	void setLanguage(const QString& language);

	/**
	* Установить ответ от сервиса клиенту
	* @param manageResult ответ от сервиса клиенту
	*/
	void setResult(ManagePacketData::ManageResult manageResult);

	/**
	* Преобразовать документ xml в последовательность байт
	* @return последовательность байт
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<ManagePacketData> m_packetData;	///< Пакет управления процессом синхронизации
};

#endif // XSDMANAGEPACKETPARSER_H
