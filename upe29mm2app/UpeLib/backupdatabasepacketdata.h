#ifndef BACKUPDATABASEPACKETDATA_H
#define BACKUPDATABASEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT BackupDatabasePacketData : public QObject
{
	Q_OBJECT
public:
	enum BackupDatabaseResult
	{
		UndefineBackupDatabaseResult = 0,   ///< �� ����������
		AcceptBackupDatabaseResult,         ///< ��
		RejectBackupDatabaseResult,         ///< ������
	}; ///< ����� �� ������� �������

	/**
	* �����������
	* @param result ����� �� ������� �������
	* @param parent ������������ ������
	*/
	BackupDatabasePacketData(const QString& username, const QString& password, BackupDatabaseResult result, QObject* parent = 0);

	/**
	* ����������
	*/
	~BackupDatabasePacketData();

	const QString& getUsername() const;
	const QString& getPassword() const;

	/**
	* ���������� ��� ���� ������
	* @param dbName ��� ���� ������
	*/
	void setDBName(const QString& dbName);

	/**
	* �������� ��� ���� ������
	* @return ��� ���� ������
	*/
	const QString& getDBName() const;

	/**
	* ���������� ���������
	* @param errorMessage ���������
	*/
	void setMessage(const QString& message);

	/**
	* �������� ���������
	* @return ���������
	*/
	const QString& getMessage() const;

	/**
	* �������� ����� �� ������� �������
	* @return ����� �� ������� �������
	*/
	BackupDatabaseResult getResult() const;

protected:
	QString m_username;
	QString m_password;
	QString m_dbName;				///< ��� ���� ������
	QString m_message;				///< ���������
	BackupDatabaseResult m_result;	///< ����� �� ������� �������
};

#endif // BACKUPDATABASEPACKETDATA_H
