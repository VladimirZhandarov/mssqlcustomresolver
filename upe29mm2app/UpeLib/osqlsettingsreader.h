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
	* �����������
	* @param filename ��� ����� (������ ����)
	* @param parent ������������ ������
	*/
	OsqlSettingsReader(const QString& filename, QObject* parent = 0);

	/**
	* ����������
	*/
	~OsqlSettingsReader();

	/**
	* �������� ����
	* @return ������� ������� ���������
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
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getDBServerInstanceName() const;

	/**
	* �������� ������ ��� ������
	* @return ������ ��� ������
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
	* ��������� ���� ������������
	*/
	void save();

private:
	QString m_filename;	///< ��� ����� ������������
	QDomDocument m_doc;	///< ����������� �� xml-����� ��������

	QString m_hostName;	///< ��� ����� ��� IP-����� ������� ���������� ����������
	quint16 m_port;		///< ���� ������� ���������� ����������
	QString m_dbServerInstanceName;	///< ��� ����
	QStringList m_dbNameList;		///< ������ ��� ������
	QString m_lastBackupDir;
	bool m_lastBackupUseUsername;
	QString m_lastRestoreDir;
	bool m_lastRestoreUseUsername;
	int m_logLevel;					///< ������� ��������������
};

#endif // OSQLSETTINGSREADER_H
