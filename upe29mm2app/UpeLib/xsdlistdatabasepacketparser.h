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
	* Конструктор
	* @param parent родительский объект
	*/
	XsdListDatabasePacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdListDatabasePacketParser();

	std::tr1::shared_ptr<ListDatabasePacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<ListDatabasePacketData>& packetData);

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

private:
	void setUsername(const QString& username);
	void setPassword(const QString& password);
	void setResult(ListDatabasePacketData::ListDatabaseResult listDatabaseResult);

	/**
	* Преобразовать документ xml в последовательность байт
	* @return последовательность байт
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<ListDatabasePacketData> m_packetData;
};

#endif // XSDLISTDATABASEPACKETPARSER_H
