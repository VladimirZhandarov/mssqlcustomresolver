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
	* �����������
	* @param filename ��� ����� (������ ����)
	* @param parent ������������ ������
	*/
	XsdSettingsReader(const QString& filename, QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdSettingsReader();

	/**
	* �������� ������� �������� ������ �������
	* @return ������� �������� ������ �������
	*/
	bool isNull() const;

	/**
	* �������� ��� ����� ��� IP-����� ������� ���������� ����������
	* @return ��� ����� ��� IP-����� ������� ���������� ����������
	*/
	const QString& getHostName() const;

	/**
	* �������� ���� ������� ���������� ����������
	* @return ���� ������� ���������� ����������
	*/
	quint16 getPort() const;

	/**
	* �������� ������� �������� �������
	* @return ������� �������� �������
	*/
	bool isMainClient() const;
	
	/**
	* ���������� ������� ���������� ��������� �� ��������� �������
	* @param ownResolve ������� ���������� ��������� �� ��������� �������
	*/
	void setOwnResolve(bool ownResolve);

	/**
	* �������� ������� ���������� ��������� �� ��������� �������
	* @return ������� ���������� ��������� �� ��������� �������
	*/
	bool isOwnResolve() const;

	/**
	* �������� ��� ��������� ����
	* @return ��� ��������� ����
	*/
	const QString& getDBServerInstanceName() const;

	/**
	* �������� ����� ��� ������
	* @return ����� ��� ������
	*/
	const QStringList& getDBNameList() const;

	/**
	* �������� ������� ��������������
	* @return ������� ��������������
	*/
	int getLogLevel() const;

	/**
	* �������� ����� ��� ����������
	* @return ����� ��� ����������
	*/
	QFont getApplicationFont() const;

	/**
	* �������� ����� ��� �������
	* @return ����� ��� �������
	*/
	QFont getDataTableFont() const;

	/**
	* �������� ����� ��� ���� �������
	* @return ����� ��� ���� �������
	*/
	QFont getLogFont() const;

	/**
	* ��������� ���������
	*/
	void save();

private:
	QString m_filename;	///< ��� ����� ��������
	QDomDocument m_doc;	///< �������� xml

	QString m_hostName;	///< ��� ����� ��� IP-����� ������� ���������� ����������
	quint16 m_port;		///< ���� ������� ���������� ����������
	bool m_mainClient;	///< ������� �������� �������
	bool m_ownResolve;	///< ������� ���������� ��������� �� ��������� �������
	QString m_dbServerInstanceName;	///< ��� ��������� ����
	QStringList m_dbNameList;		///< ����� ��� ������
	int m_logLevel;					///< ������� ��������������
	QFont m_applicationFont;		///< ����� ��� ����������
	QFont m_dataTableFont;			///< ����� ��� �������
	QFont m_logFont;				///< ����� ��� ���� �������
};

#endif // XSDSETTINGSREADER_H
