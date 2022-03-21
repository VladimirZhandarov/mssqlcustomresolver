#ifndef LISTSERVERPACKETDATA_H
#define LISTSERVERPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

class UPELIB_EXPORT ListServerPacketData : public QObject
{
	Q_OBJECT
public:
	enum ListServerResult
	{
		UndefineListServerResult = 0,	///< �� ����������
		AcceptListServerResult,		///< ��
		RejectListServerResult		///< ������
	};	///< ����� �� ������� �������

	/**
	* �����������
	* @param result ����� �� ������� �������
	* @param parent ������������ ������
	*/
	ListServerPacketData(ListServerResult result, QObject* parent = 0);

	/**
	* ����������
	*/
	~ListServerPacketData();

	void setServerNames(const QStringList& serverNames);
	const QStringList& getServerNames() const;

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
	ListServerResult getResult() const;

protected:
	QStringList m_serverNames;
	QString m_errorMessage;			///< ��������� ������
	ListServerResult m_result;	///< ����� �� ������� �������
};

#endif // LISTSERVERPACKETDATA_H
