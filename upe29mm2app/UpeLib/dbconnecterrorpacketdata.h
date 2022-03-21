#ifndef DBCONNECTERRORPACKETDATA_H
#define DBCONNECTERRORPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

/**
* @class DBConnectErrorPacketData
* ����� � ������� ���������� � ����� ������
*/
class UPELIB_EXPORT DBConnectErrorPacketData : public QObject
{
	Q_OBJECT
public:
	/**
	* �����������
	* @param publisher ������� �����������
	* @param serverName ��� ����
	* @param dbName ��� ���� ������
	* @param tableName ��� �������
	* @param userName ��� ������������
	* @param parent ������������ ������
	*/
	DBConnectErrorPacketData(bool publisher, const QString& serverName, const QString& dbName, const QString& tableName, const QString& userName,
		QObject* parent = 0);

	/**
	* ����������
	*/
	~DBConnectErrorPacketData();

	/**
	* �������� ������� �����������
	* @return ������� �����������
	*/
	bool isPulisher() const;

	/**
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getServerName() const;

	/**
	* �������� ��� ���� ������
	* @return ��� ���� ������
	*/
	const QString& getDBName() const;

	/**
	* �������� ��� �������
	* @return ��� �������
	*/
	const QString& getTableName() const;

	/**
	* �������� ��� ������������
	* @return ��� ������������
	*/
	const QString& getUserName() const;

protected:
	bool m_publisher;		///< ������� �����������
	QString m_serverName;	///< ��� ����
	QString m_dbName;		///< ��� ���� ������
	QString m_tableName;	///< ��� �������
	QString m_userName;		///< ��� ������������
};

#endif // DBCONNECTERRORPACKETDATA_H
