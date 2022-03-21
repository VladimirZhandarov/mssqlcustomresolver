#ifndef DBCONNECTERRORPACKETDATA_H
#define DBCONNECTERRORPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

/**
* @class DBConnectErrorPacketData
* Пакет с ошибкой соединения с базой данных
*/
class UPELIB_EXPORT DBConnectErrorPacketData : public QObject
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param publisher признак публикатора
	* @param serverName имя СУБД
	* @param dbName имя базы данных
	* @param tableName имя таблицы
	* @param userName имя пользователя
	* @param parent родительский объект
	*/
	DBConnectErrorPacketData(bool publisher, const QString& serverName, const QString& dbName, const QString& tableName, const QString& userName,
		QObject* parent = 0);

	/**
	* Деструктор
	*/
	~DBConnectErrorPacketData();

	/**
	* Получить признак публикатора
	* @return признак публикатора
	*/
	bool isPulisher() const;

	/**
	* Получить имя СУБД
	* @return имя СУБД
	*/
	const QString& getServerName() const;

	/**
	* Получить имя базы данных
	* @return имя базы данных
	*/
	const QString& getDBName() const;

	/**
	* Получить имя таблицы
	* @return имя таблицы
	*/
	const QString& getTableName() const;

	/**
	* Получить имя пользователя
	* @return имя пользователя
	*/
	const QString& getUserName() const;

protected:
	bool m_publisher;		///< Признак публикатора
	QString m_serverName;	///< Имя СУБД
	QString m_dbName;		///< Имя базы данных
	QString m_tableName;	///< Имя таблицы
	QString m_userName;		///< Имя пользователя
};

#endif // DBCONNECTERRORPACKETDATA_H
