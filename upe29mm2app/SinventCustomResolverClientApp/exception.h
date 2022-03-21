#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <windows.h>
#include <stdio.h>
#include <exception>
#include <string>

#include <QtCore/qglobal.h>

/**
* @class CanNotListenConnectionException
* Класс исключения "Не удалось начать слущать сокет на получение запросов на соединение"
*/
class CanNotListenConnectionException : public std::exception
{
public:
	/**
	* Конструктор
	* @param ipAddress IP-адрес сокета, на котором поизводится прослушивание
	* @param port номер порта сокета, на котором поизводится прослушивание
	* @param kpmName имя КПМ, который работает на сокете
	*/
	CanNotListenConnectionException(const std::string& ipAddress, quint16 port);

	/**
	* Деструктор
	*/
	virtual ~CanNotListenConnectionException();

	/**
	* Получить IP-адрес сокета, на котором поизводится прослушивание
	* @return IP-адрес сокета
	*/
	const std::string& getIpAddress() const;

	/**
	* Получить номер порта сокета, на котором поизводится прослушивание
	* @return номер порта сокета
	*/
	quint16 getPort() const;

private:
	std::string m_ipAddress;	///< IP-адрес сокета, на котором поизводится прослушивание
	quint16 m_port;				///< Номер порта сокета, на котором поизводится прослушивание
};

#endif // EXCEPTION_H
