#ifndef BASECLIENTSOCKET_H
#define BASECLIENTSOCKET_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtNetwork/QAbstractSocket>

/**
* @class BaseClientSocket
* Базовый сокет клиента для взаимодействия с сервером. Используется сокет Qt, но с более "тонкой" настройкой сокета с помощью библиотеки winsock2
*/
class UPELIB_EXPORT BaseClientSocket : public QObject
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	BaseClientSocket(QObject* parent = 0);

	/**
	* Деструктор
	*/
	virtual ~BaseClientSocket();

protected:
	/**
	* Проверка инициализации библиотеки winsock2
	* @return библиотека winsock2 проинициализирована
	*/
	bool isInitedWSA() const;

protected slots:
	/**
	* Слот для обработки ошибок в работе сокета Qt
	* @param socketError библиотека winsock2 проинициализирована
	*/
	virtual void onSocketError(QAbstractSocket::SocketError socketError);

	/**
	* Слот для обработки новых полученных данных из сокета
	*/
	virtual void onReadFromSocket() = 0;

private:
	/**
	* Инициализация библиотеки winsock2
	* @return библиотека winsock2 проинициализирована
	*/
	bool initWSA();

	static bool initedWSA;  ///< Признак успешности инициализации библиотеки winsock2
};

#endif // BASECLIENTSOCKET_H
