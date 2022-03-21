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
	* Конструктор
	* @param parent родительский объект
	*/
	XsdListServerPacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdListServerPacketParser();

	std::tr1::shared_ptr<ListServerPacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<ListServerPacketData>& packetData);

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

private:
	void setResult(ListServerPacketData::ListServerResult listServerResult);

	/**
	* Преобразовать документ xml в последовательность байт
	* @return последовательность байт
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<ListServerPacketData> m_packetData;
};

#endif // XSDLISTSERVERPACKETPARSER_H
