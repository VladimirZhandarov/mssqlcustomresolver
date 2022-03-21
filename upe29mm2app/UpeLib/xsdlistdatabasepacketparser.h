#ifndef XSDLISTDATABASEPACKETPARSER_H
#define XSDLISTDATABASEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "listdatabasepacketdata.h"

class UPELIB_EXPORT XsdListDatabasePacketParser : public QObject
{
	Q_OBJECT
	static QString listDatabasePacketTagName;

	static QString usernameTagName;
	static QString passwordTagName;
	static QString dbNamesTagName;
	static QString stringTagName;
	static QString errorMessageTagName;
	static QString resultTagName;

public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	XsdListDatabasePacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdListDatabasePacketParser();

	std::tr1::shared_ptr<ListDatabasePacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<ListDatabasePacketData>& packetData);

	/**
	* ��������� ������
	* @param content ������
	* @return ������ ������� ��������
	*/
	bool parse(const QString& content);

private:
	void setUsername(const QString& username);
	void setPassword(const QString& password);
	void setResult(ListDatabasePacketData::ListDatabaseResult listDatabaseResult);

	/**
	* ������������� �������� xml � ������������������ ����
	* @return ������������������ ����
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<ListDatabasePacketData> m_packetData;
};

#endif // XSDLISTDATABASEPACKETPARSER_H
