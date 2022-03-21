#include "rawpacketclientsocket.h"

#include <Windows.h>
#include <assert.h>

#include <QtCore/QStringList>

RawPacketClientSocket::RawPacketClientSocket(QObject* parent) : TcpClientSocket(parent), m_serverMode(false)
{
	init();
}

RawPacketClientSocket::RawPacketClientSocket(qintptr socketDescriptor, QObject* parent) : TcpClientSocket(socketDescriptor, parent)
{
	init();
}

RawPacketClientSocket::~RawPacketClientSocket()
{

}

void RawPacketClientSocket::sendManagePacket(const QByteArray& data)
{
	sendPacket(MANAGE_PACKET_TYPE, data);
}

void RawPacketClientSocket::sendListDatabasePacket(const QByteArray& data)
{
	sendPacket(LIST_DATABASE_PACKET_TYPE, data);
}

void RawPacketClientSocket::sendBackupPacket(const QByteArray& data)
{
	sendPacket(BACKUP_DATABASE_PACKET_TYPE, data);
}

void RawPacketClientSocket::sendRestorePacket(const QByteArray& data)
{
	sendPacket(RESTORE_DATABASE_PACKET_TYPE, data);
}

void RawPacketClientSocket::sendReplicationObjectPacket(const QByteArray& data)
{
	sendPacket(REPLICATION_OBJECT_PACKET_TYPE, data);
}

void RawPacketClientSocket::sendCreatePublictionPacket(const QByteArray& data)
{
	sendPacket(CREATE_PUBLICATION_PACKET_TYPE, data);
}

void RawPacketClientSocket::sendListServerPacket(const QByteArray& data)
{
	sendPacket(LIST_SERVER_PACKET_TYPE, data);
}

void RawPacketClientSocket::sendCreateSubscriberPacket(const QByteArray& data)
{
	sendPacket(CREATE_SUBSCRIBER_PACKET_TYPE, data);
}

void RawPacketClientSocket::onReadFromSocket()
{
	unsigned int packetCount = 0;
	int bytes = m_tcpSocket.bytesAvailable();
	while(m_tcpSocket.bytesAvailable() >= m_packetSizeToGet)
	{
		if(m_state == 0)
		{
			packetCount = 0;
			m_recvArray.clear();
			QByteArray recvArrayDataLen = m_tcpSocket.read(sizeof(qint64));
			memcpy(&m_packetSizeToGet, recvArrayDataLen.data(), sizeof(qint64));
			QByteArray packetTypeByteArray = m_tcpSocket.read(sizeof(int));
			memcpy(&m_packetType, packetTypeByteArray.data(), sizeof(int));
			m_packetSizeToGet = m_packetSizeToGet - sizeof(int);
			if(m_packetSizeToGet >= 0)
			{
				m_state = 1;
				continue;
			}
		}
		if(m_state == 1)
		{
			m_recvArray.append(m_tcpSocket.read(m_packetSizeToGet));
			if(!m_recvArray.isEmpty())
			{
				emit dataReceived(m_packetType, m_recvArray);
			}
		}
		m_state = 0;
		m_packetSizeToGet = sizeof(qint64) + sizeof(int);
	}
}

void RawPacketClientSocket::init()
{
	m_packetSizeToGet = sizeof(qint64) + sizeof(int);
	m_state = 0;
	m_packetType = 0;
	m_recvArray.clear();
}

void RawPacketClientSocket::sendPacket(PacketType packetType, const QByteArray& data)
{
	QByteArray array;
	qint64 dataLen = data.size() + sizeof(int);
	array.append((char*)&dataLen, sizeof(qint64));
	array.append((char*)&packetType, sizeof(int));
	array.append(data);
	send(array);
}
