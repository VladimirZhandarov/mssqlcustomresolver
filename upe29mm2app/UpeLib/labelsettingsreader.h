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
* �������� ����
*/
class UPELIB_EXPORT FieldData
{
public:
	/**
	* �����������
	* @param fieldName ��� ����
	* @param viewName ��� ���� (��� ����������� � gui)
	* @param bold ������� �����
	*/
	FieldData(const QString& fieldName, const QString& viewName, bool bold);

	/**
	* ����������
	*/
	~FieldData();

	/**
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getFieldName() const;

	/**
	* �������� ��� ���� (��� ����������� � gui)
	* @return ��� ���� (��� ����������� � gui)
	*/
	const QString& getViewName() const;

	/**
	* �������� ������� �������� ������
	* @return ������� �������� ������
	*/
	bool isBold() const;

private:
	QString m_fieldName;	///< ��� ����
	QString m_viewName;		///< ��� ���� (��� ����������� � gui)
	bool m_bold;			///< ������� �������� ������
};

/**
* �������� �������
*/
class UPELIB_EXPORT TableData
{
public:
	/**
	* �����������
	* @param viewName ��� ������� (��� ����������� � gui)
	* @param fieldVector map �������� �����
	*/
	TableData(const QString& viewName, const QVector<std::tr1::shared_ptr<FieldData> >& fieldVector);

	/**
	* ����������
	*/
	~TableData();

	/**
	* �������� ��� ������� (��� ����������� � gui)
	* @return ��� ������� (��� ����������� � gui)
	*/
	const QString& getViewName() const;

	/**
	* �������� map �������� �����
	* @return map �������� �����
	*/
	const QVector<std::tr1::shared_ptr<FieldData> >& getFieldVector() const;

private:
	QString m_viewName;	///< ��� ������� (��� ����������� � gui)
	QVector<std::tr1::shared_ptr<FieldData> > m_fieldVector;	///< map �������� �����
};

/**
* �������� ���� ������
*/
class UPELIB_EXPORT DbData
{
public:
	/**
	* �����������
	* @param tableMap map �������� ������
	*/
	DbData(const QMap<QString, std::tr1::shared_ptr<TableData> >& tableMap);

	/**
	* ����������
	*/
	~DbData();

	/**
	* �������� map �������� ������
	* @return map �������� ������
	*/
	const QMap<QString, std::tr1::shared_ptr<TableData> >& getTableMap() const;

private:
	QMap<QString, std::tr1::shared_ptr<TableData> > m_tableMap;	///< map �������� ������
};

/*
<?xml version="1.0"?>
	<tableLabel xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
		<dbName name="DBName">
			<table name="Table1" viewName="�������1">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="������������" show=true/>
			</table>
			<table name="Table2" viewName="�������2">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="������������" show=true/>
			</table>
		</dbName>
		<dbName name="DBName2">
			<table name="Table1" viewName="�������1">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="������������" show=true/>
			</table>
			<table name="Table2" viewName="�������2">
				<field name="id" viewName="" bold="false"/>
				<field name="name" viewName="������������" show=true/>
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
	* �����������
	* @param filename ��� ����� (������ ����)
	* @param parent ������������ ������
	*/
	LabelSettingsReader(const QString& filename, QObject* parent = 0);

	/**
	* ����������
	*/
	~LabelSettingsReader();

	/**
	* �������� map �������� ��� ������
	* @return map �������� ��� ������
	*/
	const QMap<QString, std::tr1::shared_ptr<DbData> >& getDbMap() const;

private:
	QDomDocument m_doc;	///< �������� xml

	QMap<QString, std::tr1::shared_ptr<DbData> > m_dbMap;	///< map �������� ��� ������
};

#endif // LABELSETTINGSREADER_H
