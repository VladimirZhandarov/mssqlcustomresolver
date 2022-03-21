#ifndef XSDDBCONNECTERRORPACKETPARSER_H
#define XSDDBCONNECTERRORPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "dbconnecterrorpacketdata.h"

/**
* @class XsdDBConnectErrorPacketParser
* Parser ������ � ������� ���������� � ����� ������
*/
class UPELIB_EXPORT  XsdDBConnectErrorPacketParser : public QObject
{
	Q_OBJECT
	static QString dbConnectErrorPacketTagName;

	static QString publisherTagName;
	static QString serverNameTagName;
	static QString dbNameTagName;
	static QString tableNameTagName;
	static QString userNameTagName;

public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	XsdDBConnectErrorPacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdDBConnectErrorPacketParser();

	/**
	* �������� ����� � ������� ���������� � ����� ������
	* @return ����� � ������� ���������� � ����� ������
	*/
	std::tr1::shared_ptr<DBConnectErrorPacketData> getPacket() const;

	/**
	* ��������� ������
	* @param content ������
	* @return ������ ������� ��������
	*/
	bool parse(const QString& content);

private:
	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<DBConnectErrorPacketData> m_packetData;	///< ����� � ������� ���������� � ����� ������
};

#endif // XSDDBCONNECTERRORPACKETPARSER_H
