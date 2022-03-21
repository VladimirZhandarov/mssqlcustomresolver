#ifndef LISTDATABASEPACKETDATA_H
#define LISTDATABASEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT ListDatabasePacketData : public QObject
{
	Q_OBJECT
public:
	enum ListDatabaseResult
	{
		UndefineListDatabaseResult = 0,	///< �� ����������
		AcceptListDatabaseResult,		///< ��
		RejectListDatabaseResult		///< ������
	};	///< ����� �� ������� �������

	/**
	* �����������
	* @param username
	* @param password
	* @param result ����� �� ������� �������
	* @param parent ������������ ������
	*/
	ListDatabasePacketData(const QString& username, const QString& password, ListDatabaseResult result, QObject* parent = 0);

	/**
	* ����������
	*/
	~ListDatabasePacketData();

	const QString& getUsername() const;
	const QString& getPassword() const;

	void setDBNames(const QStringList& dbNames);
	const QStringList& getDBNames() const;

	/**
	* ���������� ��������� ������
	* @param errorMessage ��������� ������
	*/
	void setErrorMessage(const QString& errorMessage);

	/**
	* �������� ��������� ������
	* @return ��������� ������
	*/
	const QString& getErrorMessage() const;

	/**
	* �������� ����� �� ������� �������
	* @return ����� �� ������� �������
	*/
	ListDatabaseResult getResult() const;

protected:
	QString m_username;
	QString m_password;
	QStringList m_dbNames;		///< ����� ��� ������
	QString m_errorMessage;			///< ��������� ������
	ListDatabaseResult m_result;	///< ����� �� ������� �������
};

#endif // LISTDATABASEPACKETDATA_H
