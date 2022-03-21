#include "baseclientsocket.h"

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>

bool BaseClientSocket::initedWSA = false;

BaseClientSocket::BaseClientSocket(QObject* parent) : QObject(parent)
{
	initWSA();
}

BaseClientSocket::~BaseClientSocket()
{

}

bool BaseClientSocket::isInitedWSA() const
{
	return initedWSA;
}

bool BaseClientSocket::initWSA()
{
	if(!initedWSA)
	{
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if(iResult != NO_ERROR)
		{
			return false;
		}
		initedWSA = true;
	}
	return initedWSA;
}

void BaseClientSocket::onSocketError(QAbstractSocket::SocketError socketError)
{
	switch(socketError)
	{
		case QAbstractSocket::RemoteHostClosedError:
		{
			break;
		}
		case QAbstractSocket::HostNotFoundError:
		{
			break;
		}
		case QAbstractSocket::ConnectionRefusedError:
		{
			break;
		}
		default:
		{

		}
	}
}
