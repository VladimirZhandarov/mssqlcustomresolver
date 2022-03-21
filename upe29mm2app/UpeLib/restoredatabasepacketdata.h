#ifndef RESTOREDATABASEPACKETDATA_H
#define RESTOREDATABASEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT RestoreDatabasePacketData : public QObject
{
	Q_OBJECT
public:
	enum RestoreDatabaseResult
	{
		UndefineRestoreDatabaseResult = 0,   ///< �� ����������
		AcceptRestoreDatabaseResult,         ///< ��
		RejectRestoreDatabaseResult,         ///< ������
	}; ///< ����� �� ������� �������

	/**
	* �����������
	* @param result ����� �� ������� �������
	* @param parent ������������ ������
	*/
	RestoreDatabasePacketData(const QString& username, const QString& password, RestoreDatabaseResult result, QObject* parent = 0);

	/**
	* ����������
	*/
	~RestoreDatabasePacketData();

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
	RestoreDatabaseResult getResult() const;

protected:
	QString m_username;
	QString m_password;
	QString m_dbName;				///< ��� ���� ������
	QString m_message;				///< ���������
	RestoreDatabaseResult m_result;	///< ����� �� ������� �������
};

#endif // RESTOREDATABASEPACKETDATA_H
