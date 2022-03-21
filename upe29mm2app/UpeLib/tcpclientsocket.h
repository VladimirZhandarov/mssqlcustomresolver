#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include "upelib_global.h"

#include "baseclientsocket.h"

#include <QtCore/QString>
#include <QtCore/QByteArray>

#include <QtNetwork/QTcpSocket>

/**
* @class TcpClientSocket
* TCP-сокет клиента для взаимодействия с сервером. Используется сокет Qt, но с
* более "тонкой" настройкой сокета с помощью библиотеки winsock2
*/
class UPELIB_EXPORT TcpClientSocket : public BaseClientSocket
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	TcpClientSocket(QObject* parent = 0);

	/**
	* Конструктор
	* @param socketDescriptor дескриптор сокета
	* @param parent родительский объект
	*/
	TcpClientSocket(qintptr socketDescriptor, QObject* parent);

	/**
	* Деструктор
	*/
	virtual ~TcpClientSocket();

	/**
	* Проверка соединения сокета
	* @return =true - соединение для сокета установлено
	*/
	bool isConnected() const;

	/**
	* Отправка данных в сокет
	* @param packet массив данных
	*/
	virtual void send(const QByteArray& packet);

	/**
	* Соединиться с сервером по TCP
	* @param ipAddress IP-адрес сервера
	* @param port порт сервера
	*/
	virtual void tcpConnect(const QString& ipAddress, int port);
	
	/**
	* Разорвать соединение с сервером
	*/
	virtual void tcpDisconnect();

signals:
	/*
	* Сигнал о возникновении ошибки при работе сокета
	* @param socketError ошибка сокета
	*/
	void error(QAbstractSocket::SocketError socketError);

	/*
	* Сигнал о получении соединения с сервером
	*/
	void connected();

	/*
	* Сигнал о разрыве соединения с сервером
	*/
	void disconnected();

	/*
	* Сигнал о получении данных
	* @param packetType тип пакета
	* @param recvArray пакет
	*/
	void dataReceived(int packetType, QByteArray recvArray);

protected slots:
	/**
	* См. описание в базовом классе
	*/
	virtual void onSocketError(QAbstractSocket::SocketError socketError);

	/**
	* Слот для обработки нового соединения
	*/
	virtual void onTcpConnected();

	/**
	* Слот для обработки разрыва соединения с сервером
	*/
	virtual void onTcpDisconnected();

private:
	/**
	* Установить все необходимые connect`ы Qt
	*/
	void createQtConnection();

protected:
	QTcpSocket m_tcpSocket;		///< Сокет TCP библиотеки Qt

	int m_packetType;
	QByteArray m_recvArray;		///< Массив данных полученных из сокета

	bool m_connectedSocket;		///< Признак установленого соединения
	bool m_isServerSide;		///< Признак сокета со строны сервера
};

#endif // TCPCLIENTSOCKET_H
