#include "exception.h"

#include <QtCore/QString>

CanNotListenConnectionException::CanNotListenConnectionException(const std::string& ipAddress, quint16 port) : m_ipAddress(ipAddress), m_port(port)
{

}

CanNotListenConnectionException::~CanNotListenConnectionException()
{

}

const std::string& CanNotListenConnectionException::getIpAddress() const
{
	return m_ipAddress;
}

quint16 CanNotListenConnectionException::getPort() const
{
	return m_port;
}
