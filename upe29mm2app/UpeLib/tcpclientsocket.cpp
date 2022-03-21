#include "tcpclientsocket.h"

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>

TcpClientSocket::TcpClientSocket(QObject* parent) : BaseClientSocket(parent)
{
	m_isServerSide = false;
	createQtConnection();
	m_connectedSocket = false;
}

TcpClientSocket::TcpClientSocket(qintptr socketDescriptor, QObject* parent) : BaseClientSocket(parent)
{
	m_isServerSide = false;
	m_connectedSocket = m_tcpSocket.setSocketDescriptor(socketDescriptor);
	createQtConnection();
	if(isInitedWSA())
	{

	}
}

void TcpClientSocket::createQtConnection()
{
	connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadFromSocket()));
	connect(&m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(error(QAbstractSocket::SocketError)));
	connect(&m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
	connect(&m_tcpSocket, SIGNAL(connected()), this, SIGNAL(connected()));
	connect(&m_tcpSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(onTcpConnected()));
	connect(&m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onTcpDisconnected()));
}

TcpClientSocket::~TcpClientSocket()
{

}

void TcpClientSocket::tcpConnect(const QString& ipAddress, int port)
{
	if(!m_connectedSocket)
	{
		m_tcpSocket.connectToHost(ipAddress, port);
		if(!m_tcpSocket.waitForConnected(3000))
		{
			return;
		}
	}
	m_connectedSocket = true;
}

void TcpClientSocket::tcpDisconnect()
{
	m_tcpSocket.disconnectFromHost();
	m_connectedSocket = false;
}

bool TcpClientSocket::isConnected() const
{
	return m_connectedSocket;
}

void TcpClientSocket::send(const QByteArray& packet)
{
	if(m_connectedSocket)
	{
		int size = packet.size();
		int result = m_tcpSocket.write(packet);
		if(size > result)
		{
			QString message = "no write " + QString::number(size) + " > " + QString::number(result);
		}
	}
}

void TcpClientSocket::onSocketError(QAbstractSocket::SocketError socketError)
{
	m_connectedSocket = false;
	BaseClientSocket::onSocketError(socketError);
}

void TcpClientSocket::onTcpConnected()
{
	m_connectedSocket = true;
}

void TcpClientSocket::onTcpDisconnected()
{
	m_connectedSocket = false;
}
