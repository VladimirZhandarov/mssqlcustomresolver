#ifndef XSDBACKUPDATABASEPACKETPARSER_H
#define XSDBACKUPDATABASEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "backupdatabasepacketdata.h"

class UPELIB_EXPORT XsdBackupDatabasePacketParser : public QObject
{
	Q_OBJECT
	static QString backupDatabasePacketTagName;

	static QString usernameTagName;
	static QString passwordTagName;
	static QString dbNameTagName;
	static QString messageTagName;
	static QString resultTagName;

public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	XsdBackupDatabasePacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdBackupDatabasePacketParser();

	std::tr1::shared_ptr<BackupDatabasePacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<BackupDatabasePacketData>& packetData);

	/**
	* ��������� ������
	* @param content ������
	* @return ������ ������� ��������
	*/
	bool parse(const QString& content);

private:
	void setUsername(const QString& username);
	void setPassword(const QString& password);

	/**
	* ���������� ��� ���� ������
	* @param dbName ��� ���� ������
	*/
	void setDBName(const QString& dbName);
	
	void setResult(BackupDatabasePacketData::BackupDatabaseResult backupDatabaseResult);

	/**
	* ������������� �������� xml � ������������������ ����
	* @return ������������������ ����
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<BackupDatabasePacketData> m_packetData;
};

#endif // XSDBACKUPDATABASEPACKETPARSER_H
