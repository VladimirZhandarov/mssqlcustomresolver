#ifndef OSQLSETTINGSREADER_H
#define OSQLSETTINGSREADER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

/*
<?xml version="1.0"?>
	<Settings xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
		<hostName>localhost</hostName>
		<port>55001</port>
		<DBServerInstanceName>Mobile111\MSSQLSERVER</DBServerInstanceName>
		<DBNames>
			<DBName>UPE_MM2_Administration2_RU</DBName>
			<DBName>UPE_MM2_Catalog_RU</DBName>
			<DBName>UPE_MM2_Documents2_RU</DBName>
			<DBName>UPE_MM2_Formular_RU</DBName>
			<DBName>UPE_MM2_ItemsAccount_RU</DBName>
			<DBName>UPE_MM2_JPS_RU</DBName>
			<DBName>UPE_MM2_Organization_RU</DBName>
			<DBName>UPE_MM2_Planning_RU</DBName>
			<DBName>UPE_MM2_Reglament2_RU</DBName>
		</DBNames>
		<lastBackupDir>C:</lastBackupDir>
	</Settings>
*/
class UPELIB_EXPORT OsqlSettingsReader : public QObject
{
	Q_OBJECT
private:
	static QString settingsTagName;

	static QString hostNameTagName;
	static QString portTagName;
	static QString dbServerInstanceNameTagName;
	static QString dbNamesTagName;
	static QString dbNameTagName;
	static QString lastBackupDirTagName;
	static QString lastBackupUseUsernameTagName;
	static QString lastRestoreDirTagName;
	static QString lastRestoreUseUsernameTagName;

public:
	/**
	* Конструктор
	* @param filename имя файла (полный путь)
	* @param parent родительский объект
	*/
	OsqlSettingsReader(const QString& filename, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~OsqlSettingsReader();

	/**
	* Документ пуст
	* @return признак пустого документа
	*/
	bool isNull() const;

	/**
	* Получить имя хоста или IP-адрес сервиса разрешения конфликтов
	* @return имя хоста или IP-адрес сервиса разрешения конфликтов
	*/
	const QString& getHostName() const;

	/**
	* Получить порт сервиса разрешения конфликтов
	* @return порт сервиса разрешения конфликтов
	*/
	quint16 getPort() const;

	/**
	* Получить имя СУБД
	* @return имя СУБД
	*/
	const QString& getDBServerInstanceName() const;

	/**
	* Получить список баз данных
	* @return список баз данных
	*/
	const QStringList& getDBNameList() const;

	void setLastBackupDir(const QString& lastBackupDir);
	QString getLastBackupDir() const;

	void setLastBackupUseUsername(bool lastBackupUseUsername);
	bool getLastBackupUseUsername() const;

	void setLastRestoreDir(const QString& lastRestoreDir);
	QString getLastRestoreDir() const;

	void setLastRestoreUseUsername(bool lastRestoreUseUsername);
	bool getLastRestoreUseUsername() const;

	/**
	* Сохранить файл конфигурации
	*/
	void save();

private:
	QString m_filename;	///< Имя файла конфигурации
	QDomDocument m_doc;	///< Прочитанный из xml-файла документ

	QString m_hostName;	///< Имя хоста или IP-адрес сервиса разрешения конфликтов
	quint16 m_port;		///< Порт сервиса разрешения конфликтов
	QString m_dbServerInstanceName;	///< Имя СУБД
	QStringList m_dbNameList;		///< Список баз данных
	QString m_lastBackupDir;
	bool m_lastBackupUseUsername;
	QString m_lastRestoreDir;
	bool m_lastRestoreUseUsername;
	int m_logLevel;					///< Уровень журналирования
};

#endif // OSQLSETTINGSREADER_H
