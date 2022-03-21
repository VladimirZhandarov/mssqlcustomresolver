#include "sinventcustomresolverclientapp.h"

#include "xsdresolvepacketparser.h"
#include "xsdinformpacketparser.h"
#include "xsddbconnecterrorpacketparser.h"
#include "xsdmanagepacketparser.h"

#include <rawpacketclientsocket.h>

#include "xsdsettingsreader.h"
#include "labelsettingsreader.h"
#include "exception.h"

#include <QtWidgets/QMessageBox>

#include <QtGui/QCloseEvent>

#include <QtCore/QDir>
#include <QtCore/QDateTime>

enum TableCol
{
	NAME_TABLE_COLNUM = 0,
	VALUE1_TABLE_COLNUM,
	VALUE2_TABLE_COLNUM,
	SIZE_TABLE_COLNUM
};

SinventCustomResolverClientApp::SinventCustomResolverClientApp(QWidget* parent) : QMainWindow(parent), m_inWork(false), m_currentDBNameIndex(0),
	m_currentDBRestartNeeded(false), m_mobileNoResolve(false)
{
	ui.setupUi(this);

	ui.custom1PB->hide();
	ui.custom2PB->hide();

#ifdef QT_DEBUG
	ui.logTE->show();
#else
	ui.logTE->hide();
#endif

	m_logModel = new  QStandardItemModel();
	m_logModel->setColumnCount(1);
	ui.logLV->setModel(m_logModel);

	QDir::current().mkdir("log");
	m_logStream = 0;
	m_logFile = 0;

	m_resolvePacketParser = std::tr1::shared_ptr<XsdResolvePacketParser>(new XsdResolvePacketParser());
	m_informPacketParser = std::tr1::shared_ptr<XsdInformPacketParser>(new XsdInformPacketParser());
	m_dbConnectErrorPacketParser = std::tr1::shared_ptr<XsdDBConnectErrorPacketParser>(new XsdDBConnectErrorPacketParser());
	m_managePacketParser = std::tr1::shared_ptr<XsdManagePacketParser>(new XsdManagePacketParser());

	m_xsdSettingsReader = new XsdSettingsReader("settings.xml", this);
	m_labelSettingsReader = new LabelSettingsReader("label.xml", this);
	QTcpSocket* tcpSocket = new QTcpSocket(this);
	m_rawPacketClientSocket = new RawPacketClientSocket(tcpSocket->socketDescriptor(), this);
	connect(m_rawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), this, SLOT(onDataReceived(int, QByteArray)));

	if(m_xsdSettingsReader->isMainClient())
	{
		QTcpSocket* mainTcpSocket = new QTcpSocket(this);
		m_mainRawPacketClientSocket = new RawPacketClientSocket(tcpSocket->socketDescriptor(), this);
		connect(m_mainRawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), this, SLOT(onMainDataReceived(int, QByteArray)));
	}
	else
	{
		m_mainRawPacketClientSocket = 0;
	}

	qApp->setFont(m_xsdSettingsReader->getApplicationFont());
	ui.dataTW->setFont(m_xsdSettingsReader->getDataTableFont());
	ui.logLV->setFont(m_xsdSettingsReader->getLogFont());

	for(int i = 0; i < SIZE_TABLE_COLNUM; i++)
	{
		ui.dataTW->setColumnHidden(i, true);
	}
	Q_ASSERT(m_xsdSettingsReader);
	if(!m_xsdSettingsReader->isMainClient())
	{
		ui.resolveConflictAction->setChecked(m_xsdSettingsReader->isOwnResolve());
	}
	else
	{
		ui.resolveConflictAction->setChecked(true);
		ui.resolveConflictAction->setEnabled(false);
		Q_ASSERT(m_mainRawPacketClientSocket);
		m_mainRawPacketClientSocket->tcpConnect(m_xsdSettingsReader->getHostName(), m_xsdSettingsReader->getPort());
		if(!m_mainRawPacketClientSocket->isConnected())
		{
			QMessageBox::warning(this, windowTitle(), tr("Could not open connection with resolver service as main client\r\n") +
				tr("with address %1:%2").arg(m_xsdSettingsReader->getHostName()).arg(m_xsdSettingsReader->getPort()));
		}
		Q_ASSERT(m_managePacketParser.get());
		std::tr1::shared_ptr<ManagePacketData> packetData = std::tr1::shared_ptr<ManagePacketData>(new ManagePacketData(ManagePacketData::StartManageType,
			m_xsdSettingsReader->getDBServerInstanceName(), ManagePacketData::UndefineManageResult));
		packetData->setMainClient(true);
		packetData->setOwnResolve(m_xsdSettingsReader->isOwnResolve());
		QByteArray data = m_managePacketParser->createPacket(packetData);
		sendManagePacket(data, true);
	}

	/*setWindowTitle(tr("Клиент разрешения конфликтов БД"));
	ui.modeMenu->setTitle(tr("Режим работы"));
	ui.resolveConflictAction->setText(tr("Разрешать конфликты"));

	ui.resolveTypeL->setText(tr("Тип изменения данных:"));
	ui.errorGB->setTitle(tr("Ошибка"));
	ui.insertSourceL->setText(tr("Источник данных:"));
	ui.tableNameL->setText(tr("Название таблицы:"));

	ui.logGroupBox->setTitle(tr("Журнал работы"));

	ui.startPB->setText(tr("Начать синхронизацию"));*/
}

SinventCustomResolverClientApp::~SinventCustomResolverClientApp()
{
	closeLog();
}

void SinventCustomResolverClientApp::showEvent(QShowEvent* event)
{
	QMainWindow::showEvent(event);
	int colWidth = ui.dataTW->width() / SIZE_TABLE_COLNUM;
	for(int i = 0; i < SIZE_TABLE_COLNUM; i++)
	{
		ui.dataTW->setColumnWidth(i, colWidth);
	}
}

void SinventCustomResolverClientApp::closeEvent(QCloseEvent* event)
{
	if(!m_inWork)
	{
		event->accept();
	}
	else
	{
		QMessageBox::StandardButton buttonResult = QMessageBox::information(this, windowTitle(), tr("Abort synchronization and close window.\nAre you sure?"),
			QMessageBox::Yes, QMessageBox::No);
		if(buttonResult == QMessageBox::Yes)
		{
			event->accept();
		}
		else
		{
			event->ignore();
		}
	}
}

void SinventCustomResolverClientApp::onDataReceived(int packetType, QByteArray recvArray)
{
	Q_ASSERT(m_xsdSettingsReader);
	if(packetType == RawPacketClientSocket::RESOLVE_PACKET_TYPE && m_xsdSettingsReader->isOwnResolve())
	{
		processResolvePacket(recvArray);
	}
	if(packetType == RawPacketClientSocket::INFORM_PACKET_TYPE)
	{
		processInformPacket(recvArray);
	}
	if(packetType == RawPacketClientSocket::DB_CONNECT_ERROR_PACKET_TYPE)
	{
		processDBConnectErrorPacket(recvArray);
	}
	else if(packetType == RawPacketClientSocket::MANAGE_PACKET_TYPE)
	{
		processManagePacket(recvArray);
	}
}

void SinventCustomResolverClientApp::onMainDataReceived(int packetType, QByteArray recvArray)
{
	if(packetType == RawPacketClientSocket::RESOLVE_PACKET_TYPE)
	{
		Q_ASSERT(m_xsdSettingsReader);
		if(!m_mobileNoResolve)
		{
			return;
		}
	}
	onDataReceived(packetType, recvArray);
}

void SinventCustomResolverClientApp::acceptPB_clicked()
{
	Q_ASSERT(m_resolvePacketParser.get());
	if(m_currentPacketData.get())
	{
		ResolvePacketData::ResolveType resolveType = m_currentPacketData->getResolveType();
		if(resolveType == ResolvePacketData::ResolveType::InsertErrorResolveType ||
			resolveType == ResolvePacketData::ResolveType::UpdateErrorResolveType ||
			resolveType == ResolvePacketData::ResolveType::DeleteErrorResolveType)
		{
			m_currentDBRestartNeeded = true;
		}
	}
	QByteArray data = m_resolvePacketParser->setAcceptResult();
	sendResolvePacket(data, m_mobileNoResolve);
	clear();
	ui.logTE->setText(data);
}

void SinventCustomResolverClientApp::rejectPB_clicked()
{
	Q_ASSERT(m_resolvePacketParser.get());
	QByteArray data = m_resolvePacketParser->setRejectResult();
	sendResolvePacket(data, m_mobileNoResolve);
	clear();
	ui.logTE->setText(data);
}

void SinventCustomResolverClientApp::custom1PB_clicked()
{
	Q_ASSERT(m_resolvePacketParser.get());
	QByteArray data = m_resolvePacketParser->setCustomResult();
	sendResolvePacket(data, m_mobileNoResolve);
	clear();
	ui.logTE->setText(data);
}

void SinventCustomResolverClientApp::custom2PB_clicked()
{
	Q_ASSERT(m_resolvePacketParser.get());
	QByteArray data = m_resolvePacketParser->setCustom2Result();
	sendResolvePacket(data, m_mobileNoResolve);
	clear();
	ui.logTE->setText(data);
}

void SinventCustomResolverClientApp::startPB_toggled(bool checked)
{
	if(checked)
	{
		openLog();
		m_inWork = false;
		Q_ASSERT(m_logModel);
		m_logModel->clear();
		m_synchronizationErrorExist = false;
		m_synchronizationFinished = false;
		Q_ASSERT(m_xsdSettingsReader && m_rawPacketClientSocket);

		m_rawPacketClientSocket->tcpConnect(m_xsdSettingsReader->getHostName(), m_xsdSettingsReader->getPort());
		if(!m_rawPacketClientSocket->isConnected())
		{
			m_synchronizationFinished = true;
			QMessageBox::warning(this, windowTitle(), tr("Could not open connection with resolver service\r\n") +
				tr("with address %1:%2").arg(m_xsdSettingsReader->getHostName()).arg(m_xsdSettingsReader->getPort()));
			ui.startPB->setChecked(false);
			return;
		}
		if(m_xsdSettingsReader->isMainClient() && m_mainRawPacketClientSocket)
		{
			disconnect(m_mainRawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), this, SLOT(onMainDataReceived(int, QByteArray)));
		}
		Q_ASSERT(m_managePacketParser.get());
		std::tr1::shared_ptr<ManagePacketData> packetData = std::tr1::shared_ptr<ManagePacketData>( new ManagePacketData(ManagePacketData::StartManageType,
			m_xsdSettingsReader->getDBServerInstanceName(), ManagePacketData::UndefineManageResult));
		packetData->setOwnResolve(m_xsdSettingsReader->isOwnResolve());
		QByteArray data = m_managePacketParser->createPacket(packetData);
		sendManagePacket(data);
		ui.startPB->setText(tr("Stop synchronization"));
		m_inWork = true;
		if(!m_xsdSettingsReader->isMainClient())
		{
			ui.resolveConflictAction->setEnabled(false);
		}
	}
	else
	{
		QMessageBox::StandardButton buttonResult = QMessageBox::Yes;
		if(!m_synchronizationFinished)
		{
			buttonResult = QMessageBox::information(this, windowTitle(), tr("Abort synchronization.\nAre you sure?"), QMessageBox::Yes, QMessageBox::No);
		}
		if(buttonResult == QMessageBox::Yes)
		{
			Q_ASSERT(m_managePacketParser.get());
			std::tr1::shared_ptr<ManagePacketData> packetData = std::tr1::shared_ptr<ManagePacketData>(new ManagePacketData(ManagePacketData::StopManageType,
				m_xsdSettingsReader->getDBServerInstanceName(), ManagePacketData::UndefineManageResult));
			QByteArray data = m_managePacketParser->createPacket(packetData);
			sendManagePacket(data);
			ui.startPB->setText(tr("Start synchronization"));
			m_inWork = false;
			clear();
			if(m_xsdSettingsReader->isMainClient() && m_mainRawPacketClientSocket)
			{
				connect(m_mainRawPacketClientSocket, SIGNAL(dataReceived(int, QByteArray)), this, SLOT(onMainDataReceived(int, QByteArray)));
			}
			if(!m_xsdSettingsReader->isMainClient())
			{
				ui.resolveConflictAction->setEnabled(true);
			}
		}
		closeLog();
	}
}

void SinventCustomResolverClientApp::resolveConflictAction_toggled(bool checked)
{
	Q_ASSERT(m_xsdSettingsReader);
	m_xsdSettingsReader->setOwnResolve(checked);
}

void SinventCustomResolverClientApp::openLog()
{
	QString currentDateTimeString = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
	m_logFile = new QFile(QDir::currentPath() + "/log/upe_m2_" + currentDateTimeString + ".txt");
	m_logStream = 0;
	if(m_logFile->open(QFile::WriteOnly | QIODevice::Append | QIODevice::Unbuffered))
	{
		m_logStream = new QTextStream(m_logFile);
		m_logStream->setCodec("utf-8");
	}
}

void SinventCustomResolverClientApp::closeLog()
{
	if(m_logStream)
	{
		m_logStream->flush();
		delete m_logStream;
	}
	m_logStream = 0;
	if(m_logFile)
	{
		m_logFile->close();
		delete m_logFile;
	}
	m_logFile = 0;
}

void SinventCustomResolverClientApp::processResolvePacket(QByteArray recvArray)
{
	Q_ASSERT(m_resolvePacketParser.get());
	clear();
	ui.dataTW->horizontalHeader()->setStretchLastSection(false);
	if(m_resolvePacketParser->parse(QString(recvArray)))
	{
		m_currentPacketData = m_resolvePacketParser->getPacket();
		if(m_currentPacketData.get())
		{
			hide();
			show();
			bool errorShow = false;
			int colCount = 2;
			QStringList headerLabels;
			if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsPublisher)
			{
				headerLabels.append(tr("Field name"));
				headerLabels.append(tr("Server value"));
				headerLabels.append(tr("Mobile value"));
			}
			else if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsSubscriber)
			{
				headerLabels.append(tr("Field name"));
				headerLabels.append(tr("Mobile value"));
				headerLabels.append(tr("Server value"));
			}
			ui.dataTW->setHorizontalHeaderLabels(headerLabels);

			switch(m_currentPacketData->getResolveType())
			{
				case ResolvePacketData::ResolveType::InsertResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Insert data"));
					break;
				}
				case ResolvePacketData::ResolveType::InsertErrorResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Insert error data"));
					errorShow = true;
					colCount = 3;
					ui.acceptPB->setText(tr("Move value 1"));
					ui.rejectPB->setText(tr("Move value 2"));
					ui.custom1PB->show();
					ui.custom2PB->show();

					if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsPublisher)
					{
						ui.acceptPB->setText(tr("Move server value"));
						ui.rejectPB->setText(tr("Move mobile value"));
						ui.custom1PB->setText(tr("Only server value"));
						ui.custom2PB->setText(tr("Only mobile value"));
					}
					else if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsSubscriber)
					{
						ui.acceptPB->setText(tr("Move mobile value"));
						ui.rejectPB->setText(tr("Move server value"));
						ui.custom1PB->setText(tr("Only mobile value"));
						ui.custom2PB->setText(tr("Only server value"));
					}

					QStringList toolTip;
					const QVector<PrimaryNextValue>& sourcePrimaryNextValues = m_currentPacketData->getSourcePrimaryNextValues();
					const QVector<PrimaryNextValue>& source2PrimaryNextValues = m_currentPacketData->getSource2PrimaryNextValues();
					Q_ASSERT(sourcePrimaryNextValues.size() == source2PrimaryNextValues.size());
					for(int i = 0; i < sourcePrimaryNextValues.size() && i < source2PrimaryNextValues.size(); i++)
					{
						qint64 maxValue = sourcePrimaryNextValues[i].getNextValue();
						if(maxValue < source2PrimaryNextValues[i].getNextValue())
						{
							maxValue = source2PrimaryNextValues[i].getNextValue();
						}
						if(maxValue != 0)
						{
							toolTip.append(QString("%1 = %2").arg(source2PrimaryNextValues[i].getFieldName()).arg(maxValue));
						}
					}
					ui.acceptPB->setToolTip(tr("Set value: ") + toolTip.join("; "));
					ui.rejectPB->setToolTip(tr("Set value: ") + toolTip.join("; "));;
					break;
				}
				case ResolvePacketData::ResolveType::UpdateResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Update data"));
					break;
				}
				case ResolvePacketData::ResolveType::UpdateErrorResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Update error data"));
					errorShow = true;
					colCount = 3;
					ui.acceptPB->setText(tr("Move value 1"));
					ui.rejectPB->setText(tr("Move value 2"));
					ui.custom1PB->show();
					ui.custom2PB->show();

					if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsPublisher)
					{
						ui.acceptPB->setText(tr("Move server value"));
						ui.rejectPB->setText(tr("Move mobile value"));
						ui.custom1PB->setText(tr("Only server value"));
						ui.custom2PB->setText(tr("Only mobile value"));
					}
					else if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsSubscriber)
					{
						ui.acceptPB->setText(tr("Move mobile value"));
						ui.rejectPB->setText(tr("Move server value"));
						ui.custom1PB->setText(tr("Only mobile value"));
						ui.custom2PB->setText(tr("Only server value"));
					}

					QStringList toolTip;
					const QVector<PrimaryNextValue>& sourcePrimaryNextValues = m_currentPacketData->getSourcePrimaryNextValues();
					const QVector<PrimaryNextValue>& source2PrimaryNextValues = m_currentPacketData->getSource2PrimaryNextValues();
					Q_ASSERT(sourcePrimaryNextValues.size() == source2PrimaryNextValues.size());
					for(int i = 0; i < sourcePrimaryNextValues.size() && i < source2PrimaryNextValues.size(); i++)
					{
						int maxValue = sourcePrimaryNextValues[i].getNextValue();
						if(maxValue < source2PrimaryNextValues[i].getNextValue())
						{
							maxValue = source2PrimaryNextValues[i].getNextValue();
						}
						if(maxValue != 0)
						{
							toolTip.append(QString("%1 = %2").arg(source2PrimaryNextValues[i].getFieldName()).arg(maxValue));
						}
					}
					ui.acceptPB->setToolTip(tr("Set value: ") + toolTip.join("; "));
					ui.rejectPB->setToolTip(tr("Set value: ") + toolTip.join("; "));;
					break;
				}
				case ResolvePacketData::ResolveType::DeleteResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Delete data"));
					break;
				}
				case ResolvePacketData::ResolveType::DeleteErrorResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Delete error data"));
					break;
				}
				case ResolvePacketData::ResolveType::UpdateConflictsResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Update conflicts data"));
					colCount = 3;
					if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsPublisher)
					{
						ui.acceptPB->setText(tr("Apply server value"));
						ui.rejectPB->setText(tr("Apply mobile value"));
					}
					else if(m_currentPacketData->getSourceIdentifier() == ResolvePacketData::SourceIsSubscriber)
					{
						ui.acceptPB->setText(tr("Apply mobile value"));
						ui.rejectPB->setText(tr("Apply server value"));
					}
					break;
				}
				case ResolvePacketData::ResolveType::UpdateDeleteConflictResolveType:
				{
					ui.resolveTypeValueL->setText(tr("Update delete conflict data"));
					colCount = 2;
					ui.acceptPB->setText(tr("Apply update"));
					ui.rejectPB->setText(tr("Apply delete"));
					break;
				}
				case ResolvePacketData::ResolveType::UndefineResolveType:
				default:
				{
					ui.resolveTypeValueL->setText(tr("N/A"));
					colCount = 0;
					break;
				}
			}
			ui.errorGB->setVisible(errorShow);
			ui.errorTE->setText(tr("Error code: ") + QString::number(m_currentPacketData->getErrorCode()) + "\n" + m_currentPacketData->getErrorMessage());

			QString sourceTypeName = "";
			QString sourceName = "";
			QString destinationName = "";
			QString sourceDBName = "";
			QString destinationDBName = "";
			switch(m_currentPacketData->getSourceIdentifier())
			{
				case ResolvePacketData::SourceIdentifier::SourceIsPublisher:
				{
					sourceTypeName = tr("Publisher");
					sourceName = m_currentPacketData->getPublisherName();
					sourceDBName = m_currentPacketData->getPublisherDBName();
					destinationName = m_currentPacketData->getSubscriberName();
					destinationDBName = m_currentPacketData->getSubscriberDBName();
					break;
				}
				case ResolvePacketData::SourceIdentifier::SourceIsSubscriber:
				{
					sourceTypeName = tr("Subscriber");
					sourceName = m_currentPacketData->getSubscriberName();
					sourceDBName = m_currentPacketData->getSubscriberDBName();
					destinationName = m_currentPacketData->getPublisherName();
					destinationDBName = m_currentPacketData->getPublisherDBName();
					break;
				}
				case ResolvePacketData::SourceIdentifier::SourceIsUndefined:
				default:
				{
					sourceTypeName = tr("N/A");
					break;
				}
			}
			Q_ASSERT(m_xsdSettingsReader);
			QStringList dbNameList = m_xsdSettingsReader->getDBNameList();
			ui.insertSourceValueL->setText(QString("%1 : %2(%3) -> %4(%5)").arg(sourceTypeName).arg(sourceName).arg(sourceDBName).arg(destinationName).
				arg(destinationDBName));

			QVector<std::tr1::shared_ptr<FieldData> > fieldLabelVector;
			QString tableName = m_currentPacketData->getTableName();
			Q_ASSERT(m_labelSettingsReader);
			const QMap<QString, std::tr1::shared_ptr<DbData> >& dbMap = m_labelSettingsReader->getDbMap();
			std::tr1::shared_ptr<DbData> dbData = dbMap.value(m_currentDBName);
			if(dbData.get())
			{
				const QMap<QString, std::tr1::shared_ptr<TableData> >& tableMap = dbData->getTableMap();
				std::tr1::shared_ptr<TableData> tableData = tableMap.value(tableName);
				if(tableData.get())
				{
					QString tableViewName = tableData->getViewName();
					fieldLabelVector = tableData->getFieldVector();
					if(!tableViewName.isEmpty())
					{
						tableName = tableViewName;
					}
				}

				QVector<ForeignKeyDataSet> sourceDataSetForeignKeys = m_currentPacketData->getSourceDataSetForeignKeys();
				for(int i = 0; i < sourceDataSetForeignKeys.size(); i++)
				{
					QString foreignTableName = sourceDataSetForeignKeys[i].getForeignTable();
					if(!foreignTableName.isEmpty())
					{
						std::tr1::shared_ptr<TableData> foreignTableData = tableMap.value(foreignTableName);
						if(foreignTableData.get())
						{
							sourceDataSetForeignKeys[i].setTableViewName(foreignTableData->getViewName());
							sourceDataSetForeignKeys[i].setFieldLabelVector(foreignTableData->getFieldVector());
						}
					}
				}
				m_currentPacketData->setSourceDataSetForeignKeys(sourceDataSetForeignKeys);

				QVector<ForeignKeyDataSet> source2DataSetForeignKeys = m_currentPacketData->getSource2DataSetForeignKeys();
				for(int i = 0; i < source2DataSetForeignKeys.size(); i++)
				{
					QString foreignTableName = source2DataSetForeignKeys[i].getForeignTable();
					if(!foreignTableName.isEmpty())
					{
						std::tr1::shared_ptr<TableData> foreignTableData = tableMap.value(foreignTableName);
						if(foreignTableData.get())
						{
							source2DataSetForeignKeys[i].setTableViewName(foreignTableData->getViewName());
							source2DataSetForeignKeys[i].setFieldLabelVector(foreignTableData->getFieldVector());
						}
					}
				}
				m_currentPacketData->setSource2DataSetForeignKeys(source2DataSetForeignKeys);
			}
			ui.tableNameValueL->setText(tableName);

			QMap<QString, std::tr1::shared_ptr<FieldData> > fieldLabelMap;
			for(int i = 0; i < fieldLabelVector.size(); i++)
			{
				Q_ASSERT(fieldLabelVector[i].get());
				QString fieldName = fieldLabelVector[i]->getFieldName();
				fieldLabelMap.insert(fieldName, fieldLabelVector[i]);
			}
			
			const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector = m_currentPacketData->getFieldMetaDataVector();
			for(int i = 0; i < colCount; i++)
			{
				ui.dataTW->setColumnHidden(i, false);
			}
			for(int i = colCount; i < ui.dataTW->columnCount(); i++)
			{
				ui.dataTW->setColumnHidden(i, true);
			}
			const QVector<QVariant>& sourceDataVector = m_currentPacketData->getSourceDataVector();
			const QVector<QVariant>& source2DataVector = m_currentPacketData->getSource2DataVector();
			QMap<QString, std::tr1::shared_ptr<FieldMetaData> > fieldMetaDataMap;
			QMap<QString, QVariant> sourceDataMap;
			QMap<QString, QVariant> source2DataMap;
			for(int i = 0; i < fieldMetaDataVector.size(); i++)
			{
				Q_ASSERT(fieldMetaDataVector[i].get());
				QString name = fieldMetaDataVector[i]->getName();
				fieldMetaDataMap.insert(name, fieldMetaDataVector[i]);
				if(i < sourceDataVector.size())
				{
					sourceDataMap.insert(name, sourceDataVector[i]);
				}
				if(i < source2DataVector.size())
				{
					source2DataMap.insert(name, source2DataVector[i]);
				}
			}
			QStringList sortedFieldNameList;
			for(int i = 0; i < fieldLabelVector.size(); i++)
			{
				Q_ASSERT(fieldLabelVector[i].get());
				sortedFieldNameList.append(fieldLabelVector[i]->getFieldName());
			}
			for(QMap<QString, std::tr1::shared_ptr<FieldMetaData> >::const_iterator iter = fieldMetaDataMap.constBegin(); iter != fieldMetaDataMap.constEnd();
				iter++)
			{
				if(!fieldLabelMap.contains(iter.key()))
				{
					sortedFieldNameList.append(iter.key());
				}
			}
			ui.dataTW->setRowCount(sortedFieldNameList.size());
			ui.dataTW->blockSignals(true);

			for(int i = 0; i < sortedFieldNameList.size(); i++)
			{
				QString fieldName = sortedFieldNameList[i];
				BinaryDataDescription binaryDataDescription = m_currentPacketData->getSourceBinaryDataDescription(fieldName);
				BinaryDataDescription binaryDataDescription2 = m_currentPacketData->getSource2BinaryDataDescription(fieldName);
				QString fieldNameKey = fieldName;
				std::tr1::shared_ptr<FieldMetaData> fieldMetaData = fieldMetaDataMap.value(fieldName);
				if(!fieldMetaData.get())
				{
					continue;
				}
				std::tr1::shared_ptr<FieldData> fieldData = fieldLabelMap.value(fieldName);
				bool bold = true;
				if(fieldData.get())
				{
					bold = fieldData->isBold();
					QString fieldViewName = fieldData->getViewName();
					if(!fieldViewName.isEmpty())
					{
						fieldName = fieldViewName;
					}
				}
				QTableWidgetItem* nameItem = new QTableWidgetItem(fieldName);
				QFont itemFont = ui.dataTW->font();
				itemFont.setBold(bold);
				nameItem->setFont(itemFont);
				QColor color = Qt::black;
				if(sourceDataMap.value(fieldNameKey) != source2DataMap.value(fieldNameKey))
				{
					color = Qt::darkRed;
				}
				if(!binaryDataDescription.getFieldName().isEmpty() && !binaryDataDescription2.getFieldName().isEmpty())
				{
					if(!binaryDataDescription.isEqualData())
					{
						color = Qt::darkRed;
					}
				}
				QString toolTipString1 = m_currentPacketData->getSourceToolTipString(fieldNameKey);
				QString toolTipString2 = m_currentPacketData->getSource2ToolTipString(fieldNameKey);
				if(!toolTipString1.isEmpty() && !toolTipString2.isEmpty())
				{
					if(toolTipString1 != toolTipString2)
					{
						color = Qt::darkRed;
					}
				}

				nameItem->setTextColor(color);
				nameItem->setFlags(nameItem->flags() &  ~Qt::ItemIsEditable);
				ui.dataTW->setItem(i, NAME_TABLE_COLNUM, nameItem);
				QString value1 = sourceDataMap.value(fieldNameKey).toString();
				if(!binaryDataDescription.getFieldName().isEmpty())
				{
					value1 = "BINARY DATA";
					if(binaryDataDescription.isNullData())
					{
						value1 += " NULL";
					}
					else
					{
						value1 += " SIZE = " + QString::number(binaryDataDescription.getDataSize());
					}
				}
				QTableWidgetItem* itemValue1 = new QTableWidgetItem(value1);
				if(!toolTipString1.isEmpty())
				{
					itemValue1->setToolTip(toolTipString1);
				}
				itemValue1->setFont(itemFont);
				itemValue1->setTextColor(color);
				itemValue1->setFlags(itemValue1->flags() &  ~Qt::ItemIsEditable);
				ui.dataTW->setItem(i, VALUE1_TABLE_COLNUM, itemValue1);

				QString value2 = source2DataMap.value(fieldNameKey).toString();
				if(!binaryDataDescription2.getFieldName().isEmpty())
				{
					value2 = "BINARY DATA";
					if(binaryDataDescription2.isNullData())
					{
						value2 += " NULL";
					}
					else
					{
						value2 += " SIZE = " + QString::number(binaryDataDescription2.getDataSize());
					}
				}
				QTableWidgetItem* itemValue2 = new QTableWidgetItem(value2);
				if(!toolTipString2.isEmpty())
				{
					itemValue2->setToolTip(toolTipString2);
				}
				itemValue2->setFont(itemFont);
				itemValue2->setTextColor(color);
				itemValue2->setFlags(itemValue2->flags() &  ~Qt::ItemIsEditable);
				ui.dataTW->setItem(i, VALUE2_TABLE_COLNUM, itemValue2);
			}
			ui.dataTW->blockSignals(false);
		}
	}
	ui.dataTW->horizontalHeader()->setStretchLastSection(true);
	if(m_currentPacketData.get())
	{
		ui.acceptPB->setEnabled(true);
		ui.rejectPB->setEnabled(true);
	}
	else
	{
		ui.acceptPB->setEnabled(false);
		ui.rejectPB->setEnabled(false);
	}
	ui.logTE->setPlainText(recvArray);
}

void SinventCustomResolverClientApp::processInformPacket(QByteArray recvArray)
{
	Q_ASSERT(m_informPacketParser.get());
	if(m_informPacketParser->parse(QString(recvArray)))
	{
		std::tr1::shared_ptr<InformPacketData> informPacketData = m_informPacketParser->getPacket();
		if(informPacketData.get())
		{
			addLogViewListMessage(informPacketData->getMessage());
		}
	}
}

void SinventCustomResolverClientApp::processDBConnectErrorPacket(QByteArray recvArray)
{
	Q_ASSERT(m_dbConnectErrorPacketParser.get());
	if(m_dbConnectErrorPacketParser->parse(QString(recvArray)))
	{
		std::tr1::shared_ptr<DBConnectErrorPacketData> dbConnectErrorPacketData = m_dbConnectErrorPacketParser->getPacket();
		if(dbConnectErrorPacketData.get())
		{
			m_synchronizationFinished = true;
			QString publisherString = "publisher";
			if(!dbConnectErrorPacketData->isPulisher())
			{
				publisherString = "subscriber";
			}
			QString userName = dbConnectErrorPacketData->getUserName();
			if(userName.isEmpty())
			{
				userName = "Windows Authentification";
			}
			else
			{
				userName = "username: " + userName;
			}
			ui.startPB->setChecked(false);
			QMessageBox::warning(this, windowTitle(), tr("Resolver could not open connection with ") + publisherString + tr(" server ") +
				dbConnectErrorPacketData->getServerName() + "\r\n" + tr("with database: %1 and table name: %2 %3").
				arg(dbConnectErrorPacketData->getDBName()).arg(dbConnectErrorPacketData->getTableName()).arg(userName));
		}
	}
}

void SinventCustomResolverClientApp::processManagePacket(QByteArray recvArray)
{
	Q_ASSERT(m_managePacketParser.get());
	if(m_managePacketParser->parse(QString(recvArray)))
	{
		std::tr1::shared_ptr<ManagePacketData> managePacketData = m_managePacketParser->getPacket();
		if(managePacketData.get())
		{
			Q_ASSERT(m_xsdSettingsReader);
			if(m_xsdSettingsReader->isMainClient())
			{
				if(managePacketData->getManageType() == ManagePacketData::StartMainClientType && !managePacketData->getDBServerInstanceName().isEmpty())
				{
					m_mobileNoResolve = !managePacketData->isOwnResolve();
					addLogViewListMessage(tr("Resolve process on ") + managePacketData->getDBServerInstanceName() + tr(" started"));
				}
				else if(managePacketData->getManageType() == ManagePacketData::StopMainClientType && !managePacketData->getDBServerInstanceName().isEmpty())
				{
					m_mobileNoResolve = false;
					addLogViewListMessage(tr("Resolve process on ") + managePacketData->getDBServerInstanceName() + tr(" stoped"));
				}
			}
			if(managePacketData->getErrorCode() != 0 && managePacketData->getManageType() != ManagePacketData::NextStepManageType)
			{
				m_synchronizationFinished = true;
				ui.startPB->setChecked(false);
				QMessageBox::warning(this, windowTitle(), managePacketData->getErrorMessage());
			}
			else
			{
				if(managePacketData->getManageType() == ManagePacketData::StartManageType)
				{
					if(managePacketData->getResult() == ManagePacketData::AcceptManageResult)
					{
						addLogViewListMessage(tr("Resolver service accept connection"));
						if(!m_mobileNoResolve)
						{
							m_currentDBNameIndex = 0;
							m_currentDBName = "";
							startNextStep();
						}
					}
					else if(managePacketData->getResult() == ManagePacketData::RejectManageResult)
					{
						QMessageBox::warning(this, windowTitle(), tr("Resolver service reject connection"));
					}
				}
				else if(managePacketData->getManageType() == ManagePacketData::StopManageType)
				{
					if(managePacketData->getResult() == ManagePacketData::AcceptManageResult)
					{
						addLogViewListMessage(tr("Resolver service accept disconnection"));
					}
				}
				else if(managePacketData->getManageType() == ManagePacketData::NextStepManageType)
				{
					if(managePacketData->getResult() == ManagePacketData::AcceptManageResult)
					{
						addLogViewListMessage(tr("Database \"") + managePacketData->getDBName() + tr("\" synchronization started"));
					}
					else if(managePacketData->getResult() == ManagePacketData::RejectManageResult)
					{
						addLogViewListMessage(tr("Database \"") + managePacketData->getDBName() + tr("\" synchronization error:") + managePacketData->getErrorMessage());
						if(!m_mobileNoResolve)
						{
							if(!m_currentDBRestartNeeded)
							{
								m_currentDBNameIndex++;
							}
							else
							{
								m_currentDBRestartNeeded = false;
							}
							m_synchronizationErrorExist = true;
							startNextStep();
						}
					}
				}
				else if(managePacketData->getManageType() == ManagePacketData::NextStepEndedManageType)
				{
					if(managePacketData->getResult() == ManagePacketData::AcceptManageResult)
					{
						addLogViewListMessage(tr("Database \"") + managePacketData->getDBName() + tr("\" synchronization ended"));
					}
					else if(managePacketData->getResult() == ManagePacketData::RejectManageResult)
					{
						addLogViewListMessage(tr("Database \"") + managePacketData->getDBName() + tr("\" synchronization ended with error"));
						m_synchronizationErrorExist = true;
					}
					else if(managePacketData->getResult() == ManagePacketData::SnapShotNeededManageResult)
					{
						addLogViewListMessage(tr("Database \"") + managePacketData->getDBName() + tr("\" snapshot needed"));
						m_currentDBRestartNeeded = true;
						return;
					}
					else if(managePacketData->getResult() == ManagePacketData::SnapShotEndedManageResult)
					{
						addLogViewListMessage(tr("Database \"") + managePacketData->getDBName() + tr("\" snapshot maked"));
					}

					if(!m_mobileNoResolve)
					{
						if(!m_currentDBRestartNeeded)
						{
							m_currentDBNameIndex++;
						}
						else
						{
							m_currentDBRestartNeeded = false;
						}
						startNextStep();
					}
				}
			}
		}
	}
}

void SinventCustomResolverClientApp::addLogViewListMessage(const QString& message)
{
	Q_ASSERT(m_logModel);
	QStandardItem* textItem = new QStandardItem(message);
	textItem->setEditable(false);
	m_logModel->appendRow(textItem);
	ui.logLV->scrollTo(textItem->index());
	if(m_logStream && m_logStream->device())
	{
		*m_logStream << message << endl;
	}
}

void SinventCustomResolverClientApp::startNextStep()
{
	Q_ASSERT(m_xsdSettingsReader);
	QStringList dbNameList = m_xsdSettingsReader->getDBNameList();
	if(m_currentDBNameIndex < dbNameList.size())
	{
		Q_ASSERT(m_managePacketParser.get());
		std::tr1::shared_ptr<ManagePacketData> packetData = std::tr1::shared_ptr<ManagePacketData>(new ManagePacketData(ManagePacketData::NextStepManageType,
			m_xsdSettingsReader->getDBServerInstanceName(), ManagePacketData::UndefineManageResult));
		m_currentDBName = dbNameList[m_currentDBNameIndex];
		packetData->setDBName(m_currentDBName);
		sendManagePacket(m_managePacketParser->createPacket(packetData));
	}
	else
	{
		m_synchronizationFinished = true;
		QString suffixString = "";
		if(m_synchronizationErrorExist)
		{
			suffixString = tr(" with error");
		}
		QMessageBox::information(this, windowTitle(), tr("Databases synchronization finished") + suffixString);
		ui.startPB->setChecked(false);
	}
}

void SinventCustomResolverClientApp::sendResolvePacket(const QByteArray& data, bool mainSocket)
{
	sendPacket(RawPacketClientSocket::RESOLVE_PACKET_TYPE, data, mainSocket);
}

void SinventCustomResolverClientApp::sendManagePacket(const QByteArray& data, bool mainSocket)
{
	sendPacket(RawPacketClientSocket::MANAGE_PACKET_TYPE, data, mainSocket);
}

void SinventCustomResolverClientApp::sendPacket(RawPacketClientSocket::PacketType packetType, const QByteArray& data, bool mainSocket)
{
	QByteArray array;
	qint64 dataLen = data.size() + sizeof(int);
	array.append((char*)&dataLen, sizeof(qint64));
	array.append((char*)&packetType, sizeof(int));
	array.append(data);
	if(!mainSocket)
	{
		Q_ASSERT(m_rawPacketClientSocket);
		m_rawPacketClientSocket->send(array);
	}
	else
	{
		Q_ASSERT(m_mainRawPacketClientSocket);
		m_mainRawPacketClientSocket->send(array);
	}
}

void SinventCustomResolverClientApp::clear()
{
	ui.resolveTypeValueL->clear();
	ui.insertSourceValueL->clear();
	ui.tableNameValueL->clear();

	ui.dataTW->clearContents();
	ui.dataTW->setRowCount(0);
	ui.acceptPB->setEnabled(false);
	ui.rejectPB->setEnabled(false);

	//ui.logTE->clear();

	ui.errorTE->clear();

	ui.acceptPB->setText(tr("Apply"));
	ui.rejectPB->setText(tr("Cancel"));

	ui.acceptPB->setToolTip("");
	ui.rejectPB->setToolTip("");

	ui.custom1PB->hide();
	ui.custom2PB->hide();

	m_synchronizationErrorExist = false;
	m_synchronizationFinished = false;
}
