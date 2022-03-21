#ifndef SINVENTCUSTOMRESOLVERCLIENTAPP_H
#define SINVENTCUSTOMRESOLVERCLIENTAPP_H

#include <QtWidgets/QMainWindow>
#include "ui_sinventcustomresolverclientapp.h"

#include <QtWidgets/QSystemTrayIcon>

#include "memory"

#include <QtGui/QStandardItemModel>

#include <rawpacketclientsocket.h>

class RawPacketClientSocket;
class XsdSettingsReader;
class LabelSettingsReader;
class ResolvePacketData;

class XsdResolvePacketParser;
class XsdInformPacketParser;
class XsdDBConnectErrorPacketParser;
class XsdManagePacketParser;

class QFile;

/**
* @class SinventCustomResolverClientApp
* Главное окно приложения
*/
class SinventCustomResolverClientApp : public QMainWindow
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	SinventCustomResolverClientApp(QWidget* parent = 0);

	/**
	* Деструктор
	*/
	~SinventCustomResolverClientApp();

protected:
	/**
	* Обработчик открытия окна
	* @param event событие открытия окна
	*/
	virtual void showEvent(QShowEvent* event);

	/**
	* Обработчик закрытия окна
	* @param event событие закрытия окна
	*/
	virtual void closeEvent(QCloseEvent* event);

protected slots:
	/**
	* Получен покет от сервиса (в качестве мобильного рабочего места)
	* @param packetType тип пакета синхронизации
	* @param recvArray данные пакета синхронизации
	*/
	void onDataReceived(int packetType, QByteArray recvArray);

	/**
	* Получен покет от сервиса (в качестве главного рабочего места)
	* @param packetType тип пакета синхронизации
	* @param recvArray данные пакета синхронизации
	*/
	void onMainDataReceived(int packetType, QByteArray recvArray);

	/**
	* Нажата кнопка "Принять" для решения конфликта
	*/
	void acceptPB_clicked();

	/**
	* Нажата кнопка "Отмена" для решения конфликта
	*/
	void rejectPB_clicked();
	
	/**
	* Нажата кнопка "Пользователькая 1" для решения конфликта
	*/
	void custom1PB_clicked();

	/**
	* Нажата кнопка "Пользователькая 2" для решения конфликта
	*/
	void custom2PB_clicked();

	/**
	* Нажата кнопка "Начала/Окончания" синхронизации
	* @param checked =TRUE - начало синхронизации
	*                =FALSE - окончание синхронизации
	*/
	void startPB_toggled(bool checked);

	/**
	* Изменение флага "Разрешать конфликты самому"
	* @param checked флаг "Разрешать конфликты самому"
	*/
	void resolveConflictAction_toggled(bool checked);

private:
	void openLog();
	void closeLog();

	/**
	* Разобрать и отработать пакет разрешения конфликта синхронизации
	* @param recvArray пакет данных
	*/
	void processResolvePacket(QByteArray recvArray);

	/**
	* Разобрать и отработать пакет с информацией о процессе синхронизации
	* @param recvArray пакет данных
	*/
	void processInformPacket(QByteArray recvArray);

	/**
	* Разобрать и отработать пакет с ошибкой соединения с базой данных
	* @param recvArray пакет данных
	*/
	void processDBConnectErrorPacket(QByteArray recvArray);

	/**
	* Разобрать и отработать пакет управления процессом синхронизации
	* @param recvArray пакет данных
	*/
	void processManagePacket(QByteArray recvArray);

	/**
	* Добавить сообщение в окно журнала
	* @param message сообщение
	*/
	void addLogViewListMessage(const QString& message);

	/**
	* Начать следующий шаг синхронизации
	*/
	void startNextStep();

	/**
	* Отправить ответ для разрешения конфликта синхронизации
	* @param data пакет данных
	* @param mainSocket признак главного клиента (РМО)
	*/
	void sendResolvePacket(const QByteArray& data, bool mainSocket = false);

	/**
	* Отправить пакет управления процессом сихронизации
	* @param data пакет данных
	* @param mainSocket признак главного клиента (РМО)
	*/
	void sendManagePacket(const QByteArray& data, bool mainSocket = false);

	/**
	* Отправить пакет сихронизации
	* @param packetType тип пакета сихронизации
	* @param data пакет данных
	* @param mainSocket признак главного клиента (РМО)
	*/
	void sendPacket(RawPacketClientSocket::PacketType packetType, const QByteArray& data, bool mainSocket = false);

	/**
	* Очистить окно разрешения конфликта
	*/
	void clear();

	Ui::SinventCustomResolverClientApp ui;

	int m_currentDBNameIndex;	///< Индекс текущей базы данных, для которой проводится процесс синхронизации
	QString m_currentDBName;	///< Имя текущей базы данных, для которой проводится процесс синхронизации
	bool m_currentDBRestartNeeded;	///< Необходима повторная синхронизация для текущей базы данных
	bool m_mobileNoResolve;			///< Признак разрешения конфликта только главным РМО

	RawPacketClientSocket* m_rawPacketClientSocket;		///< Клиент для присоединения к сервису как моблиьного РМО
	RawPacketClientSocket* m_mainRawPacketClientSocket;	///< Клиент для присоединения к сервису как главного РМО
	XsdSettingsReader* m_xsdSettingsReader;				///< Reader настроек приложения
	LabelSettingsReader* m_labelSettingsReader;			///< Reader трансляциииме полей таблиц и имен таблиц

	std::tr1::shared_ptr<XsdResolvePacketParser> m_resolvePacketParser;					///< Parser пакета разрешения конфликта синхронизации
	std::tr1::shared_ptr<XsdInformPacketParser> m_informPacketParser;					///< Parser пакета с информацией о процессе синхронизации
	std::tr1::shared_ptr<XsdDBConnectErrorPacketParser> m_dbConnectErrorPacketParser;	///< Parser пакета с ошибкой соединения с базой данных
	std::tr1::shared_ptr<XsdManagePacketParser> m_managePacketParser;					///< Parser пакета управления

	std::tr1::shared_ptr<ResolvePacketData> m_currentPacketData;	///< Текущий запрос на разрашение кофликта синхронизации
	bool m_inWork;						///< Признак - синхронизация в процессе
	bool m_synchronizationErrorExist;	///< Признак - синхронизация содержит ошибки
	bool m_synchronizationFinished;		///< Признак - синхронизация окончена

	QStandardItemModel* m_logModel;	///< Модель для списка сообщений (журнал)
	QTextStream* m_logStream;
	QFile* m_logFile;
};

#endif // SINVENTCUSTOMRESOLVERCLIENTAPP_H
