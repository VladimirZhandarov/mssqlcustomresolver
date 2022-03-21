#ifndef XSDLISTSERVERPACKETPARSER_H
#define XSDLISTSERVERPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "listserverpacketdata.h"

class UPELIB_EXPORT XsdListServerPacketParser : public QObject
{
	Q_OBJECT
	static QString listServerPacketTagName;

	static QString serverNamesTagName;
	static QString stringTagName;
	static QString errorMessageTagName;
	static QString resultTagName;

public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	XsdListServerPacketParser(QObject* parent = 0);

	/**
	* ����������
	*/
	~XsdListServerPacketParser();

	std::tr1::shared_ptr<ListServerPacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<ListServerPacketData>& packetData);

	/**
	* ��������� ������
	* @param content ������
	* @return ������ ������� ��������
	*/
	bool parse(const QString& content);

private:
	void setResult(ListServerPacketData::ListServerResult listServerResult);

	/**
	* ������������� �������� xml � ������������������ ����
	* @return ������������������ ����
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< �������� xml

	std::tr1::shared_ptr<ListServerPacketData> m_packetData;
};

#endif // XSDLISTSERVERPACKETPARSER_H
