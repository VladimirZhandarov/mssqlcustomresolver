#ifndef LABELSETTINGSREADER_H
#define LABELSETTINGSREADER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtCore/QMap>
#include <QtCore/QVector>

#include <QtGui/QFont>

#include <QtXml/QDomDocument>

#include "memory"

/**
* Описание поля
*/
class UPELIB_EXPORT FieldData
{
public:
	/**
	* Конструктор
	* @param fieldName имя поля
	* @param viewName имя поля (для отображения в gui)
	* @param bold широкий шрифт
	*/
	FieldData(const QString& fieldName, const QString& viewName, bool bold);

	/**
	* Деструктор
	*/
	~FieldData();

	/**
	* Получить имя поля
	* @return имя поля
	*/
	const QString& getFieldName() const;

	/**
	* Получить имя поля (для отображения в gui)
	* @return имя поля (для отображения в gui)
	*/
	const QString& getViewName() const;

	/**
	* Получить признак широкого шрифта
	* @return признак широкого шрифта
	*/
	bool isBold() const;

private:
	QString m_fieldName;	///< Имя поля
	QString m_viewName;		///< Имя поля (для отображения в gui)
	bool m_bold;			///< Признак широкого шрифта
};

/**
* Описание таблицы
*/
class UPELIB_EXPORT TableData
{
public:
	/**
	* Конструктор
	* @param viewName имя таблицы (для отображения в gui)
	* @param fieldVector map описаний полей
	*/
	TableData(const QString& viewName, const QVector<std::tr1::shared_ptr<FieldData> >& fieldVector);

	/**
	* Деструктор
	*/
	~TableData();

	/**
	* Получить имя таблицы (для отображения в gui)
	* @return имя таблицы (для отображения в gui)
	*/
	const QString& getViewName() const;

	/**
	* Получить map описаний полей
	* @return map описаний полей
	*/
	const QVector<std::tr1::shared_ptr<FieldData> >& getFieldVector() const;

private:
	QString m_viewName;	///< Имя таблицы (для отображения в gui)
	QVector<std::tr1::shared_ptr<FieldData> > m_fieldVector;	///< map описаний полей
};

/**
* Описание базы данных
*/
class UPELIB_EXPORT DbData
{
public:
	/**
	* Конструктор
	* @param tableMap map описаний таблиц
	*/
	DbData(const QMap<QString, std::tr1::shared_ptr<TableData> >& tableMap);

	/**
	* Деструктор
	*/
	~DbData();

	/**
	* Получить map описаний таблиц
	* @return map описаний таблиц
	*/
	const QMap<QString, std::tr1::shared_ptr<TableData> >& getTableMap() const;

private:
	QMap<QString, std::tr1::shared_ptr<TableData> > m_tableMap;	///< map описаний таблиц
};

/*
<?xml version="1.0"?>
	<tableLabel xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
		<dbName name="DBName">
			<table name="Table1" viewName="Таблица1">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="Наименование" show=true/>
			</table>
			<table name="Table2" viewName="Таблица2">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="Наименование" show=true/>
			</table>
		</dbName>
		<dbName name="DBName2">
			<table name="Table1" viewName="Таблица1">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="Наименование" show=true/>
			</table>
			<table name="Table2" viewName="Таблица2">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="Наименование" show=true/>
			</table>
		</dbName>
	</tableLabel>
*/
class UPELIB_EXPORT LabelSettingsReader : public QObject
{
	Q_OBJECT
private:
	static QString tableLabelTagName;
	static QString dbNameTagName;
	static QString tableTagName;
	static QString fieldTagName;
	static QString nameAttrName;
	static QString viewNameAttrName;
	static QString boldAttrName;

public:
	/**
	* Конструктор
	* @param filename имя файла (полный путь)
	* @param parent родительский объект
	*/
	LabelSettingsReader(const QString& filename, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~LabelSettingsReader();

	/**
	* Получить map описаний баз данных
	* @return map описаний баз данных
	*/
	const QMap<QString, std::tr1::shared_ptr<DbData> >& getDbMap() const;

private:
	QDomDocument m_doc;	///< Документ xml

	QMap<QString, std::tr1::shared_ptr<DbData> > m_dbMap;	///< map описаний баз данных
};

#endif // LABELSETTINGSREADER_H
