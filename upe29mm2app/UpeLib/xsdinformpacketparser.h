#ifndef XSDINFORMPACKETPARSER_H
#define XSDINFORMPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "informpacketdata.h"

/**
* @class XsdInformPacketParser
* Parser ������ � ����������� � �������� �������������
*/
class UPELIB_EXPORT XsdInformPacketParser : public QObject
{
	Q_OBJECT
	static QString informPacketTagName;

	static QString errorCodeTagName;
	static QString messageTagName;

public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	XsdInformPacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdInformPacketParser();

	/**
	* �������� ����� � ����������� � �������� �������������
	* @return ����� � ����������� � �������� �������������
	*/
	std::tr1::shared_ptr<InformPacketData> getPacket() const;

	/**
	* ��������� ������
	* @param content ������
	* @return ������ ������� ��������
	*/
	bool parse(const QString& content);

private:
	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<InformPacketData> m_packetData;	///< ����� � ����������� � �������� �������������
};

#endif // XSDINFORMPACKETPARSER_H
