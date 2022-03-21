#ifndef XSDINFORMPACKETPARSER_H
#define XSDINFORMPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "informpacketdata.h"

/**
* @class XsdInformPacketParser
* Parser пакета с информацией о процессе синхронизации
*/
class UPELIB_EXPORT XsdInformPacketParser : public QObject
{
	Q_OBJECT
	static QString informPacketTagName;

	static QString errorCodeTagName;
	static QString messageTagName;

public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	XsdInformPacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdInformPacketParser();

	/**
	* Получить пакет с информацией о процессе синхронизации
	* @return пакет с информацией о процессе синхронизации
	*/
	std::tr1::shared_ptr<InformPacketData> getPacket() const;

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

private:
	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<InformPacketData> m_packetData;	///< Пакет с информацией о процессе синхронизации
};

#endif // XSDINFORMPACKETPARSER_H
