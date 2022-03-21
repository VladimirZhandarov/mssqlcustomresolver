#ifndef XSDRESTOREDATABASEPACKETPARSER_H
#define XSDRESTOREDATABASEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "restoredatabasepacketdata.h"

class UPELIB_EXPORT XsdRestoreDatabasePacketParser : public QObject
{
	Q_OBJECT
	static QString restoreDatabasePacketTagName;

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
	XsdRestoreDatabasePacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdRestoreDatabasePacketParser();

	std::tr1::shared_ptr<RestoreDatabasePacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<RestoreDatabasePacketData>& packetData);

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
	
	void setResult(RestoreDatabasePacketData::RestoreDatabaseResult restoreDatabaseResult);

	/**
	* ������������� �������� xml � ������������������ ����
	* @return ������������������ ����
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<RestoreDatabasePacketData> m_packetData;
};

#endif // XSDRESTOREDATABASEPACKETPARSER_H
