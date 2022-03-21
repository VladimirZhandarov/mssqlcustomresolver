#ifndef XSDBACKUPDATABASEPACKETPARSER_H
#define XSDBACKUPDATABASEPACKETPARSER_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "memory"

#include "backupdatabasepacketdata.h"

class UPELIB_EXPORT XsdBackupDatabasePacketParser : public QObject
{
	Q_OBJECT
	static QString backupDatabasePacketTagName;

	static QString usernameTagName;
	static QString passwordTagName;
	static QString dbNameTagName;
	static QString messageTagName;
	static QString resultTagName;

public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	XsdBackupDatabasePacketParser(QObject* parent = 0);

	/**
	* Деструктор
	*/
	~XsdBackupDatabasePacketParser();

	std::tr1::shared_ptr<BackupDatabasePacketData> getPacket() const;

	QByteArray createPacket(const std::tr1::shared_ptr<BackupDatabasePacketData>& packetData);

	/**
	* Разобрать строку
	* @param content строка
	* @return строка успешно разбрана
	*/
	bool parse(const QString& content);

private:
	void setUsername(const QString& username);
	void setPassword(const QString& password);

	/**
	* Установить имя базы данных
	* @param dbName имя базы данных
	*/
	void setDBName(const QString& dbName);
	
	void setResult(BackupDatabasePacketData::BackupDatabaseResult backupDatabaseResult);

	/**
	* Преобразовать документ xml в последовательность байт
	* @return последовательность байт
	*/
	QByteArray docToByteArray() const;

	QDomDocument m_doc;	///< Документ xml

	std::tr1::shared_ptr<BackupDatabasePacketData> m_packetData;
};

#endif // XSDBACKUPDATABASEPACKETPARSER_H
