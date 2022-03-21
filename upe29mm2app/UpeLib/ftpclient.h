#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include "upelib_global.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QUrl>

class QFtp;
class QFile;

class UPELIB_EXPORT FtpClient : public QObject
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param dirPath
	* @param parent родительский объект
	*/
	FtpClient(const QString& dirPath, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~FtpClient();

	bool connectToFtp(const QString& url);

	bool loadFile(const QString& fileName);
	bool uploadFile(const QString& fileName);

signals:
	void dataTransferProgress(qint64 done, qint64 total);
	void commandFinished(QString errorMsg);

protected slots:
	void ftp_commandFinished(int, bool error);

protected:
	void clear();

	QFtp* m_ftp;
	QFile* m_file;
	QUrl m_currentUrl;
	QString m_dirPath;
};

#endif // FTPCLIENT_H
