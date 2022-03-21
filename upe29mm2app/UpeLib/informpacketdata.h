#ifndef INFORMPACKETDATA_H
#define INFORMPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

/**
* @class InformPacketData
* Пакет с информацией о процессе синхронизации
*/
class UPELIB_EXPORT InformPacketData : public QObject
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param errorCode код ошибки
	* @param message сообщение
	* @param parent родительский объект
	*/
	InformPacketData(int errorCode, const QString& message, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~InformPacketData();

	/**
	* Получить код ошибки
	* @return код ошибки
	*/
	int getErrorCode() const;

	/**
	* Получить сообщение
	* @return сообщение
	*/
	const QString& getMessage() const;

protected:
	int m_errorCode;	///< Код ошибки
	QString m_message;	///< Сообщение
};

#endif // INFORMPACKETDATA_H
