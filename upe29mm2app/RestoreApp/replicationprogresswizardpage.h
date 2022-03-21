#ifndef REPLICATIONPROGRESSWIZARDPAGE_H
#define REPLICATIONPROGRESSWIZARDPAGE_H

#include <QtWidgets/QWizardPage>

#include "ui_replicationprogresswizardpage.h"

#include <QtCore/QProcess>

#include "memory"

class RawPacketClientSocket;
class XsdBackupDatabasePacketParser;

class FtpClient;

class OsqlSettingsReader;

class ReplicationProgressWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	ReplicationProgressWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent = 0);
	~ReplicationProgressWizardPage();

	void init();

	virtual bool isComplete() const;

signals:
	/**
	* Сигнал об окончании процесса создания резервной копии
	*/
	void canceledSignal();

	protected slots:
	void showLogPB_toggled(bool toggled);

	/**
	* Нажата кнопка закрытия диалога
	*/
	//void closePB_clicked();

	/**
	* Вывод сообщения об ошибке
	* @param errorMessage сообщения об ошибке
	*/
	void onErrorMessage(QString errorMessage);

	/**
	* Процесс osql.exe окончен.
	*/
	void process_finished(int exitCode, QProcess::ExitStatus exitStatus);

	void onDataReceived(int packetType, QByteArray recvArray);

private:
	/**
	* Начать процесс osql.exe для базы данных.
	* @param step индекс базы данных.
	*/
	void startProgress(int step);

	void sendBackupPacket(const QByteArray& data);

	void processListDatabasePacket(QByteArray recvArray);
	void processBackupDatabasePacket(QByteArray recvArray);

	Ui::ReplicationProgressWizardPage ui;

	RawPacketClientSocket* m_rawPacketClientSocket;
	std::tr1::shared_ptr<XsdBackupDatabasePacketParser> m_backupDatabasePacketParser;

	QProcess m_process;	///< Процесс
	FtpClient* m_ftpClient;

	QString m_dir;					///< Каталог для сохранения резервных копий баз данных
	QString m_host;
	quint16 m_port;
	QString m_osqlPath;				///< Путь к osql.exe
	bool m_local;
	QString m_dbServerInstanceName;	///< Имя СУБД
	QString m_ftpUrl;
	QStringList m_dbNameList;		///< Список баз данных
	QString m_currentDbName;
	QString m_currentFilename;
	int m_step;						///< Индекс текущей базы данных
};

#endif // REPLICATIONPROGRESSWIZARDPAGE_H
