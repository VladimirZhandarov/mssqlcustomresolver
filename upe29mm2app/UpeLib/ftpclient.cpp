#include "ftpclient.h"

#include <QtFtp/qftp.h>

#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

FtpClient::FtpClient(const QString& dirPath, QObject* parent) : QObject(parent), m_ftp(0), m_dirPath(dirPath), m_file(0)
{

}

FtpClient::~FtpClient()
{
	clear();
}

bool FtpClient::connectToFtp(const QString& url)
{
	if(m_ftp && m_ftp->hasPendingCommands())
	{
		return true;
	}
	if(m_file)
	{
		return  true;
	}
	QUrl fileUrl(url);
	if(m_ftp && fileUrl.authority() != m_currentUrl.authority())
	{
		clear();
	}
	if(!m_ftp)
	{
		m_ftp = new QFtp(this);
		//m_ftp->setTextCodec(QTextCodec::codecForName("UTF-8"));
		connect(m_ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftp_commandFinished(int, bool)));
		connect(m_ftp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SIGNAL(dataTransferProgress(qint64, qint64)));
		m_currentUrl.setUrl(url);

		if(!fileUrl.isValid() || fileUrl.scheme().toLower() != QLatin1String("ftp"))
		{
			m_ftp->connectToHost(url, 21);
			m_ftp->login();
		}
		else
		{
			m_ftp->connectToHost(fileUrl.host(), fileUrl.port(21));
			if(!fileUrl.userName().isEmpty())
			{
				m_ftp->login(QUrl::fromPercentEncoding(fileUrl.userName().toLatin1()), fileUrl.password());
			}
			else
			{
				m_ftp->login();
			}
		}
	}
	return true;
}

bool FtpClient::loadFile(const QString& fileName)
{
	if(fileName.isEmpty())
	{
		return false;
	}
	QString filePath = m_dirPath + "/" + fileName;
	QFileInfo fileInfo(filePath);
	QDir dir(m_dirPath);
	if(!dir.mkpath(fileInfo.absolutePath()))
	{
		return false;
	}
	m_file = new QFile(filePath);

	if(!m_file->open(QIODevice::WriteOnly))
	{
		delete m_file;
		m_file = 0;
		return false;
	}
	m_ftp->get(fileName, m_file);
	return true;
}

bool FtpClient::uploadFile(const QString& fileName)
{
	if(fileName.isEmpty())
	{
		return false;
	}
	QString filePath = m_dirPath + "/" + fileName;
	m_file = new QFile(filePath);

	if(!m_file->open(QIODevice::ReadOnly))
	{
		delete m_file;
		m_file = 0;
		return false;
	}
	m_ftp->put(m_file, fileName);
	return true;
}

void FtpClient::ftp_commandFinished(int, bool error)
{
	QString errorMsg = "";
	if(m_ftp->currentCommand() == QFtp::ConnectToHost)
	{
		if(error)
		{
			errorMsg = m_ftp->errorString();
		}
	}
	else if(m_ftp->currentCommand() == QFtp::Get)
	{
		m_file->close();
		if(error)
		{
			errorMsg = m_ftp->errorString();
		}
		else
		{

		}
		m_file = 0;
		emit commandFinished(errorMsg);
	}
	else if(m_ftp->currentCommand() == QFtp::Put)
	{
		m_file->close();
		if(error)
		{
			errorMsg = m_ftp->errorString();
		}
		else
		{
		}
		m_file = 0;
		emit commandFinished(errorMsg);
	}
}

void FtpClient::clear()
{
	if(m_ftp)
	{
		disconnect(m_ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftp_commandFinished(int, bool)));
		disconnect(m_ftp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SIGNAL(dataTransferProgress(qint64, qint64)));

		m_ftp->abort();
		m_ftp->deleteLater();
		m_ftp = 0;
	}
}
