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
* ������� ���� ����������
*/
class SinventCustomResolverClientApp : public QMainWindow
{
	Q_OBJECT
public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	SinventCustomResolverClientApp(QWidget* parent = 0);

	/**
	* ����������
	*/
	~SinventCustomResolverClientApp();

protected:
	/**
	* ���������� �������� ����
	* @param event ������� �������� ����
	*/
	virtual void showEvent(QShowEvent* event);

	/**
	* ���������� �������� ����
	* @param event ������� �������� ����
	*/
	virtual void closeEvent(QCloseEvent* event);

protected slots:
	/**
	* ������� ����� �� ������� (� �������� ���������� �������� �����)
	* @param packetType ��� ������ �������������
	* @param recvArray ������ ������ �������������
	*/
	void onDataReceived(int packetType, QByteArray recvArray);

	/**
	* ������� ����� �� ������� (� �������� �������� �������� �����)
	* @param packetType ��� ������ �������������
	* @param recvArray ������ ������ �������������
	*/
	void onMainDataReceived(int packetType, QByteArray recvArray);

	/**
	* ������ ������ "�������" ��� ������� ���������
	*/
	void acceptPB_clicked();

	/**
	* ������ ������ "������" ��� ������� ���������
	*/
	void rejectPB_clicked();
	
	/**
	* ������ ������ "��������������� 1" ��� ������� ���������
	*/
	void custom1PB_clicked();

	/**
	* ������ ������ "��������������� 2" ��� ������� ���������
	*/
	void custom2PB_clicked();

	/**
	* ������ ������ "������/���������" �������������
	* @param checked =TRUE - ������ �������������
	*                =FALSE - ��������� �������������
	*/
	void startPB_toggled(bool checked);

	/**
	* ��������� ����� "��������� ��������� ������"
	* @param checked ���� "��������� ��������� ������"
	*/
	void resolveConflictAction_toggled(bool checked);

private:
	void openLog();
	void closeLog();

	/**
	* ��������� � ���������� ����� ���������� ��������� �������������
	* @param recvArray ����� ������
	*/
	void processResolvePacket(QByteArray recvArray);

	/**
	* ��������� � ���������� ����� � ����������� � �������� �������������
	* @param recvArray ����� ������
	*/
	void processInformPacket(QByteArray recvArray);

	/**
	* ��������� � ���������� ����� � ������� ���������� � ����� ������
	* @param recvArray ����� ������
	*/
	void processDBConnectErrorPacket(QByteArray recvArray);

	/**
	* ��������� � ���������� ����� ���������� ��������� �������������
	* @param recvArray ����� ������
	*/
	void processManagePacket(QByteArray recvArray);

	/**
	* �������� ��������� � ���� �������
	* @param message ���������
	*/
	void addLogViewListMessage(const QString& message);

	/**
	* ������ ��������� ��� �������������
	*/
	void startNextStep();

	/**
	* ��������� ����� ��� ���������� ��������� �������������
	* @param data ����� ������
	* @param mainSocket ������� �������� ������� (���)
	*/
	void sendResolvePacket(const QByteArray& data, bool mainSocket = false);

	/**
	* ��������� ����� ���������� ��������� ������������
	* @param data ����� ������
	* @param mainSocket ������� �������� ������� (���)
	*/
	void sendManagePacket(const QByteArray& data, bool mainSocket = false);

	/**
	* ��������� ����� ������������
	* @param packetType ��� ������ ������������
	* @param data ����� ������
	* @param mainSocket ������� �������� ������� (���)
	*/
	void sendPacket(RawPacketClientSocket::PacketType packetType, const QByteArray& data, bool mainSocket = false);

	/**
	* �������� ���� ���������� ���������
	*/
	void clear();

	Ui::SinventCustomResolverClientApp ui;

	int m_currentDBNameIndex;	///< ������ ������� ���� ������, ��� ������� ���������� ������� �������������
	QString m_currentDBName;	///< ��� ������� ���� ������, ��� ������� ���������� ������� �������������
	bool m_currentDBRestartNeeded;	///< ���������� ��������� ������������� ��� ������� ���� ������
	bool m_mobileNoResolve;			///< ������� ���������� ��������� ������ ������� ���

	RawPacketClientSocket* m_rawPacketClientSocket;		///< ������ ��� ������������� � ������� ��� ���������� ���
	RawPacketClientSocket* m_mainRawPacketClientSocket;	///< ������ ��� ������������� � ������� ��� �������� ���
	XsdSettingsReader* m_xsdSettingsReader;				///< Reader �������� ����������
	LabelSettingsReader* m_labelSettingsReader;			///< Reader ������������� ����� ������ � ���� ������

	std::tr1::shared_ptr<XsdResolvePacketParser> m_resolvePacketParser;					///< Parser ������ ���������� ��������� �������������
	std::tr1::shared_ptr<XsdInformPacketParser> m_informPacketParser;					///< Parser ������ � ����������� � �������� �������������
	std::tr1::shared_ptr<XsdDBConnectErrorPacketParser> m_dbConnectErrorPacketParser;	///< Parser ������ � ������� ���������� � ����� ������
	std::tr1::shared_ptr<XsdManagePacketParser> m_managePacketParser;					///< Parser ������ ����������

	std::tr1::shared_ptr<ResolvePacketData> m_currentPacketData;	///< ������� ������ �� ���������� �������� �������������
	bool m_inWork;						///< ������� - ������������� � ��������
	bool m_synchronizationErrorExist;	///< ������� - ������������� �������� ������
	bool m_synchronizationFinished;		///< ������� - ������������� ��������

	QStandardItemModel* m_logModel;	///< ������ ��� ������ ��������� (������)
	QTextStream* m_logStream;
	QFile* m_logFile;
};

#endif // SINVENTCUSTOMRESOLVERCLIENTAPP_H
