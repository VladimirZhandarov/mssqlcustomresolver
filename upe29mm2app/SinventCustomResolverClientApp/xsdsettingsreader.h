#ifndef XSDSETTINGSREADER_H
#define XSDSETTINGSREADER_H

#include <QtCore/QObject>

#include <QtGui/QFont>

#include <QtXml/QDomDocument>

/*
<?xml version="1.0"?>
	<Settings xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
		<hostName>localhost</hostName>
		<port>55001</port>
		<mainClient>false</mainClient>
		<ownResolve>true</ownResolve>
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
		<logLevel>0</logLevel>
		<applicationFont name="Arial" size = "12"/>
		<dataTableFont name="Arial" size = "12"/>
		<logFont name="Arial" size = "12"/>
	</Settings>
*/
class XsdSettingsReader : public QObject
{
	Q_OBJECT
private:
	static QString settingsTagName;
	static QString hostNameTagName;
	static QString portTagName;
	static QString mainClientTagName;
	static QString ownResolveTagName;
	static QString dbServerInstanceNameTagName;
	static QString dbNamesTagName;
	static QString dbNameTagName;
	static QString logLevelTagName;
	static QString applicationFontTagName;
	static QString dataTableFontTagName;
	static QString logFontTagName;
	static QString fontNameAttrName;
	static QString fontSizeAttrName;
	static QString fontWeightAttrName;

public:
	/**
	* Конструктор
	* @param filename имя файла (полный путь)
	* @param parent родительский объект
	*/
	XsdSettingsReader(const QString& filename, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdSettingsReader();

	/**
	* Получить признак отсутвия данных настоек
	* @return признак отсутвия данных настоек
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
	* Получить имя локальной СУБД
	* @return имя локальной СУБД
	*/
	const QString& getDBServerInstanceName() const;

	/**
	* Получить имена баз данных
	* @return имена баз данных
	*/
	const QStringList& getDBNameList() const;

	/**
	* Получить уровень журналирования
	* @return уровень журналирования
	*/
	int getLogLevel() const;

	/**
	* Получить шрифт для приложения
	* @return шрифт для приложения
	*/
	QFont getApplicationFont() const;

	/**
	* Получить шрифт для таблицы
	* @return шрифт для таблицы
	*/
	QFont getDataTableFont() const;

	/**
	* Получить шрифт для окна журнала
	* @return шрифт для окна журнала
	*/
	QFont getLogFont() const;

	/**
	* Сохранить настройки
	*/
	void save();

private:
	QString m_filename;	///< Имя файла настроек
	QDomDocument m_doc;	///< Документ xml

	QString m_hostName;	///< Имя хоста или IP-адрес сервиса разрешения конфликтов
	quint16 m_port;		///< Порт сервиса разрешения конфликтов
	bool m_mainClient;	///< Признак главного клиента
	bool m_ownResolve;	///< Признак разрешения конфликта на мобильном клиенте
	QString m_dbServerInstanceName;	///< Имя локальной СУБД
	QStringList m_dbNameList;		///< Имена баз данных
	int m_logLevel;					///< Уровень журналирования
	QFont m_applicationFont;		///< Шрифт для приложения
	QFont m_dataTableFont;			///< Шрифт для таблицы
	QFont m_logFont;				///< Шрифт для окна журнала
};

#endif // XSDSETTINGSREADER_H
