#ifndef XSDMANAGEPACKETPARSER_H
#define XSDMANAGEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "managepacketdata.h"

/**
* @class XsdManagePacketParser
* Parser ������ ����������
*/
class UPELIB_EXPORT XsdManagePacketParser : public QObject
{
	Q_OBJECT
	static QString managePacketTagName;

	static QString manageTypeTagName;
	static QString mainClientTagName;
	static QString ownResolveTagName;
	static QString dbServerInstanceNameTagName;
	static QString dbNameTagName;
	static QString languageTagName;
	static QString errorCodeTagName;
	static QString errorMessageTagName;
	static QString resultTagName;

public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	XsdManagePacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdManagePacketParser();

	/**
	* �������� ����� ����������
	* @return ����� ����������
	*/
	std::tr1::shared_ptr<ManagePacketData> getPacket() const;

	/**
	* ������� ����� ����������
	* @param packetData ����� ����������
	* @return �����
	*/
	QByteArray createPacket(const std::tr1::shared_ptr<ManagePacketData>& packetData);

	/**
	* ��������� ������
	* @param content ������
	* @return ������ ������� ��������
	*/
	bool parse(const QString& content);

	/**
	* ���������� ����� �� ������� ������� - ��
	* @return �����
	*/
	QByteArray setAcceptResult();

	/**
	* ���������� ����� �� ������� ������� - ������
	* @return �����
	*/
	QByteArray setRejectResult();

private:
	/**
	* ���������� ������ ������
	* @param manageType ������ ������
	*/
	void setType(ManagePacketData::ManageType manageType);

	/**
	* ���������� ������� �������� �������
	* @param mainClient ������� �������� �������
	*/
	void setMainClient(bool mainClient);

	/**
	* ���������� ������� ���������� ��������� �� ��������� �������
	* @param ownResolve ������� ���������� ��������� �� ��������� �������
	*/
	void setOwnResolve(bool ownResolve);

	/**
	* ���������� ��� ����
	* @param dbServerInstanceName ��� ����
	*/
	void setDBServerInstanceName(const QString& dbServerInstanceName);

	/**
	* ���������� ��� ���� ������
	* @param dbName ��� ���� ������
	*/
	void setDBName(const QString& dbName);

	/**
	* ���������� ����
	* @param language ����
	*/
	void setLanguage(const QString& language);

	/**
	* ���������� ����� �� ������� �������
	* @param manageResult ����� �� ������� �������
	*/
	void setResult(ManagePacketData::ManageResult manageResult);

	/**
	* ������������� �������� xml � ������������������ ����
	* @return ������������������ ����
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<ManagePacketData> m_packetData;	///< ����� ���������� ��������� �������������
};

#endif // XSDMANAGEPACKETPARSER_H
