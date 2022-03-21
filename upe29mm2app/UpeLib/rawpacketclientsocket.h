#ifndef RAWPACKETCLIENTSOCKET_H
#define RAWPACKETCLIENTSOCKET_H

#include "upelib_global.h"

#include "tcpclientsocket.h"

#include <QtCore/QString>
#include <QtCore/QStringList>

/**
* @class RawPacketClientSocket
* Сокет для получения пакетов данных
*/
class UPELIB_EXPORT RawPacketClientSocket : public TcpClientSocket
{
	Q_OBJECT
public:
	enum PacketType
	{
		RESOLVE_PACKET_TYPE = 1,		///< Пакет разрешения конфликта синхронизации
		INFORM_PACKET_TYPE,				///< Пакет с информацией о процессе синхронизации
		DB_CONNECT_ERROR_PACKET_TYPE,	///< Пакет с ошибкой соединения с базой данных
		MANAGE_PACKET_TYPE,				///< Пакет управления процессом синхронизации
		LIST_DATABASE_PACKET_TYPE,
		BACKUP_DATABASE_PACKET_TYPE,
		RESTORE_DATABASE_PACKET_TYPE,
		REPLICATION_OBJECT_PACKET_TYPE,
		CREATE_PUBLICATION_PACKET_TYPE,
		LIST_SERVER_PACKET_TYPE,
		CREATE_SUBSCRIBER_PACKET_TYPE
	};	///< Тип пакета синхронизации

	/**
	* Конструктор
	* @param parent родительский объект
	*/
	RawPacketClientSocket(QObject* parent = 0);

	/**
	* Конструктор
	* @param socketDescriptor дескриптор сокета
	* @param parent родительский объект
	*/
	RawPacketClientSocket(qintptr socketDescriptor, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~RawPacketClientSocket();

	void sendManagePacket(const QByteArray& data);
	void sendListDatabasePacket(const QByteArray& data);
	void sendBackupPacket(const QByteArray& data);
	void sendRestorePacket(const QByteArray& data);
	void sendReplicationObjectPacket(const QByteArray& data);
	void sendCreatePublictionPacket(const QByteArray& data);
	void sendListServerPacket(const QByteArray& data);
	void sendCreateSubscriberPacket(const QByteArray& data);

protected slots:
	/**
	* @copydoc BaseClientSocket::onReadFromSocket()
	*/
	virtual void onReadFromSocket();

private:
	/**
	* Инициализация начальными значениями полей класса
	*/
	void init();

	void sendPacket(PacketType packetType, const QByteArray& data);

protected:
	qint64 m_packetSizeToGet;	///< Количество байт, которое необходимо получить для полного получения нового пакета

	int m_state;	///< Стадия получения пакета телеметрии
					///< =0 - получаем заголовок пакета телеметрии
					///< =1 - получем данные пакета телеметрии

	bool m_serverMode;		///< Режим сокета TRUE - для сервера
};

#endif // RAWPACKETCLIENTSOCKET_H
