#ifndef RESTOREPROGRESSWIZARDPAGE_H
#define RESTOREPROGRESSWIZARDPAGE_H

#include <QtWidgets/QWizardPage>

#include "ui_restoreprogresswizardpage.h"

#include "memory"

class RawPacketClientSocket;
class XsdRestoreDatabasePacketParser;

class FtpClient;

class OsqlSettingsReader;

class RestoreProgressWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	RestoreProgressWizardPage(OsqlSettingsReader* osqlSettingsReader, RawPacketClientSocket* rawPacketClientSocket, QWidget* parent = 0);
	~RestoreProgressWizardPage();

	void init(const QString& dir, const QString& username, const QString& password, bool localDataBase, const QStringList& dbNameList);

	virtual bool isComplete() const;

signals:
	/**
	* Сигнал об окончании процесса создания резервной копии
	*/
	void canceledSignal();

	void finishedSignal();

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
	//void process_finished(int exitCode, QProcess::ExitStatus exitStatus);

	void onDataReceived(int packetType, QByteArray recvArray);

	void onFtpDataTransferProgress(qint64 done, qint64 total);
	void onFtpCommandFinished(QString errorMsg);

private:
	void startServerProgress(int step);

	/**
	* Начать процесс osql.exe для базы данных.
	* @param step индекс базы данных.
	*/
	void startProgress(int step);

	Ui::RestoreProgressWizardPage ui;

	RawPacketClientSocket* m_rawPacketClientSocket;
	std::tr1::shared_ptr<XsdRestoreDatabasePacketParser> m_restoreDatabasePacketParser;

	FtpClient* m_ftpClient;

	QString m_dir;					///< Каталог для сохранения резервных копий баз данных
	QString m_username;
	QString m_password;
	QString m_host;
	quint16 m_port;
	bool m_local;
	QString m_dbServerInstanceName;	///< Имя СУБД
	QString m_ftpUrl;
	QStringList m_dbNameList;		///< Список баз данных
	QString m_currentDbName;
	QString m_currentServerDbName;
	QString m_currentFilename;
	QString m_currentFilePath;
	int m_step;						///< Индекс текущей базы данных
	int m_copyIndex;
};

#endif // RESTOREPROGRESSWIZARDPAGE_H
