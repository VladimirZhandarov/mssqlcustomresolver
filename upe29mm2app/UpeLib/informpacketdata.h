#ifndef INFORMPACKETDATA_H
#define INFORMPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

/**
* @class InformPacketData
* ����� � ����������� � �������� �������������
*/
class UPELIB_EXPORT InformPacketData : public QObject
{
	Q_OBJECT
public:
	/**
	* �����������
	* @param errorCode ��� ������
	* @param message ���������
	* @param parent ������������ ������
	*/
	InformPacketData(int errorCode, const QString& message, QObject* parent = 0);

	/**
	* ����������
	*/
	~InformPacketData();

	/**
	* �������� ��� ������
	* @return ��� ������
	*/
	int getErrorCode() const;

	/**
	* �������� ���������
	* @return ���������
	*/
	const QString& getMessage() const;

protected:
	int m_errorCode;	///< ��� ������
	QString m_message;	///< ���������
};

#endif // INFORMPACKETDATA_H
