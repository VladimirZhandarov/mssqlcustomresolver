#ifndef MANAGEPACKETDATA_H
#define MANAGEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

/**
* @class ManagePacketData
* ����� ���������� ��������� �������������
*/
class UPELIB_EXPORT ManagePacketData : public QObject
{
	Q_OBJECT
public:
	enum ManageType
	{
		UndefineManageType = -1,	///< �� ����������
		StartManageType = 0,		///< ������ �������������
		StopManageType,				///< ���������� �������������
		NextStepManageType,			///< ������ ��������� ��� �������������
		NextStepEndedManageType,	///< ��� ������������� �������
		StartMainClientType,		///< ������ ������������� ��� ������� ������
		StopMainClientType			///< ���������� ������������� ��� ������� ������
	};	///< ������ ������

	enum ManageResult
	{
		UndefineManageResult = 0,	///< �� ����������
		AcceptManageResult,			///< ��
		RejectManageResult,			///< ������
		SnapShotNeededManageResult,	///< ����� snapshot
		SnapShotEndedManageResult	///< Snapshot ������
	};	///< ����� �� ������� �������

	/**
	* �����������
	* @param manageType ������ ������
	* @param dbServerInstanceName ��� ����
	* @param result ����� �� ������� �������
	* @param parent ������������ ������
	*/
	ManagePacketData(ManageType manageType, const QString& dbServerInstanceName, ManageResult result, QObject* parent = 0);

	/**
	* ����������
	*/
	~ManagePacketData();

	/**
	* �������� ������ ������
	* @return ������ ������
	*/
	ManageType getManageType() const;

	/**
	* ���������� ������� �������� �������
	* @param mainClient ������� �������� �������
	*/
	void setMainClient(bool mainClient);

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
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getDBServerInstanceName() const;

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
	* ���������� ����
	* @param language ����
	*/
	void setLanguage(const QString& language);

	/**
	* �������� ����
	* @return ����
	*/
	const QString& getLanguage() const;

	/**
	* ���������� ��� ������
	* @param erroeCode ��� ������
	*/
	void setErrorCode(int erroeCode);

	/**
	* �������� ��� ������
	* @return ��� ������
	*/
	int getErrorCode() const;

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
	ManageResult getResult() const;

protected:
	ManageType m_manageType;			///< ������ ������
	bool m_mainClient;					///< ������� �������� �������
	bool m_ownResolve;					///< ������� ���������� ��������� �� ��������� �������
	QString m_dbServerInstanceName;		///< ��� ����
	QString m_dbName;					///< ��� ���� ������
	QString m_language;					///< ����
	int m_errorCode;					///< ��� ������
	QString m_errorMessage;				///< ��������� ������
	ManageResult m_result;				///< ����� �� ������� �������
};

#endif // MANAGEPACKETDATA_H
