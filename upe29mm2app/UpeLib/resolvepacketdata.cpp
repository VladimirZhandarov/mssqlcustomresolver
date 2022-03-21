#include "resolvepacketdata.h"

#include "labelsettingsreader.h"

PrimaryNextValue::PrimaryNextValue(const QString& fieldName, qint64 nextValue) : m_fieldName(fieldName), m_nextValue(nextValue)
{

}

PrimaryNextValue::~PrimaryNextValue()
{

}

const QString& PrimaryNextValue::getFieldName() const
{
	return m_fieldName;
}

qint64 PrimaryNextValue::getNextValue() const
{
	return m_nextValue;
}

BinaryDataDescription::BinaryDataDescription(const QString& fieldName, qint64 dataSize, bool equalData, bool nullData) : m_fieldName(fieldName),
	m_dataSize(dataSize), m_equalData(equalData), m_nullData(nullData)
{

}

BinaryDataDescription::~BinaryDataDescription()
{

}

const QString& BinaryDataDescription::getFieldName() const
{
	return m_fieldName;
}

qint64 BinaryDataDescription::getDataSize() const
{
	return m_dataSize;
}

bool BinaryDataDescription::isEqualData() const
{
	return m_equalData;
}

bool BinaryDataDescription::isNullData() const
{
	return m_nullData;
}

FieldMetaData::FieldMetaData(const QString& name, XsdType type, int minOccurs, QObject* parent) : QObject(parent), m_name(name), m_type(type),
	m_minOccurs(minOccurs)
{

}

FieldMetaData::~FieldMetaData()
{

}

const QString& FieldMetaData::getName() const
{
	return m_name;
}

FieldMetaData::XsdType FieldMetaData::getType() const
{
	return m_type;
}

int FieldMetaData::getMinOccurs() const
{
	return m_minOccurs;
}

ForeignKeyDataSet::ForeignKeyDataSet(const QString& fieldName, const QString& foreignTable,
	const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector, const QVector<QVariant>& dataVector,
	const QVector<BinaryDataDescription>& binaryDataDescriptions) : m_fieldName(fieldName),
	m_foreignTable(foreignTable), m_fieldMetaDataVector(fieldMetaDataVector), m_dataVector(dataVector)
{
	QVector<BinaryDataDescription>::const_iterator iterEnd = binaryDataDescriptions.constEnd();
	for(QVector<BinaryDataDescription>::const_iterator iter = binaryDataDescriptions.constBegin(); iter != iterEnd; iter++)
	{
		m_binaryDataDescriptionMap.insert((*iter).getFieldName(), *iter);
	}
}

ForeignKeyDataSet::~ForeignKeyDataSet()
{

}

const QString& ForeignKeyDataSet::getFieldName() const
{
	return m_fieldName;
}

const QString& ForeignKeyDataSet::getForeignTable() const
{
	return m_foreignTable;
}

const QVector<QVariant>& ForeignKeyDataSet::getDataVector() const
{
	return m_dataVector;
}

void ForeignKeyDataSet::setTableViewName(const QString& tableViewName)
{
	m_tableViewName = tableViewName;
}

void ForeignKeyDataSet::setFieldLabelVector(const QVector<std::tr1::shared_ptr<FieldData> >& fieldLabelVector)
{
	m_fieldLabelVector = fieldLabelVector;
}

QString ForeignKeyDataSet::getStringValue() const
{
	QString result = "";
	if(!m_tableViewName.isEmpty())
	{
		result = "Table: " + m_tableViewName + "\r\n";
	}
	else if(!m_foreignTable.isEmpty())
	{
		result = "Table: " + m_foreignTable + "\r\n";
	}
	QMap<QString, std::tr1::shared_ptr<FieldMetaData> > fieldMetaDataMap;
	QMap<QString, QVariant> sourceDataMap;
	for(int i = 0; i < m_fieldMetaDataVector.size(); i++)
	{
		Q_ASSERT(m_fieldMetaDataVector[i].get());
		QString name = m_fieldMetaDataVector[i]->getName();
		fieldMetaDataMap.insert(name, m_fieldMetaDataVector[i]);
		if(i < m_dataVector.size())
		{
			sourceDataMap.insert(name, m_dataVector[i]);
		}
	}

	QMap<QString, std::tr1::shared_ptr<FieldData> > fieldLabelMap;
	for(int i = 0; i < m_fieldLabelVector.size(); i++)
	{
		Q_ASSERT(m_fieldLabelVector[i].get());
		QString fieldName = m_fieldLabelVector[i]->getFieldName();
		fieldLabelMap.insert(fieldName, m_fieldLabelVector[i]);
	}

	QStringList sortedFieldNameList;
	for(int i = 0; i < m_fieldLabelVector.size(); i++)
	{
		Q_ASSERT(m_fieldLabelVector[i].get());
		sortedFieldNameList.append(m_fieldLabelVector[i]->getFieldName());
	}
	for(QMap<QString, std::tr1::shared_ptr<FieldMetaData> >::const_iterator iter = fieldMetaDataMap.constBegin(); iter != fieldMetaDataMap.constEnd(); iter++)
	{
		if(!fieldLabelMap.contains(iter.key()))
		{
			sortedFieldNameList.append(iter.key());
		}
	}

	for(int i = 0; i < sortedFieldNameList.size(); i++)
	{
		QString fieldName = sortedFieldNameList[i];
		BinaryDataDescription binaryDataDescription = m_binaryDataDescriptionMap.value(fieldName);
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
		QString value = sourceDataMap.value(fieldNameKey).toString();
		if(!binaryDataDescription.getFieldName().isEmpty())
		{
			value = "BINARY DATA";
			if(binaryDataDescription.isNullData())
			{
				value += " NULL";
			}
			else
			{
				value += " SIZE = " + QString::number(binaryDataDescription.getDataSize());
			}
		}
		result += fieldName + " = " + value + "\r\n";
	}
	return result;
}

QString ForeignKeyDataSet::getRowGuid() const
{
	return "";
}

ResolvePacketData::ResolvePacketData(ResolveType resolveType, SourceIdentifier sourceIdentifier, const QString&  publisherName, const QString&  subscriberName,
	const QString& publisherDBName, const QString& subscriberDBName, const QString& tableName, int errorCode, const QString& errorMessage,
	const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector, const QVector<QVariant>& sourceDataVector,
	const QVector<QVariant>& source2DataVector, ResolveResult result, QObject* parent) : QObject(parent),
	m_resolveType(resolveType), m_sourceIdentifier(sourceIdentifier), m_publisherName(publisherName), m_subscriberName(subscriberName),
	m_publisherDBName(publisherDBName), m_subscriberDBName(subscriberDBName), m_tableName(tableName), m_errorCode(errorCode), m_errorMessage(errorMessage),
	m_fieldMetaDataVector(fieldMetaDataVector), m_sourceDataVector(sourceDataVector), m_source2DataVector(source2DataVector), m_result(result)
{

}

ResolvePacketData::~ResolvePacketData()
{

}

ResolvePacketData::ResolveType ResolvePacketData::getResolveType() const
{
	return m_resolveType;
}

ResolvePacketData::SourceIdentifier ResolvePacketData::getSourceIdentifier() const
{
	return m_sourceIdentifier;
}

const QString& ResolvePacketData::getPublisherName() const
{
	return m_publisherName;
}

const QString& ResolvePacketData::getSubscriberName() const
{
	return m_subscriberName;
}

const QString& ResolvePacketData::getPublisherDBName() const
{
	return m_publisherDBName;
}

const QString& ResolvePacketData::getSubscriberDBName() const
{
	return m_subscriberDBName;
}

const QString& ResolvePacketData::getTableName() const
{
	return m_tableName;
}

int ResolvePacketData::getErrorCode() const
{
	return m_errorCode;
}

const QString& ResolvePacketData::getErrorMessage() const
{
	return m_errorMessage;
}

const QVector<std::tr1::shared_ptr<FieldMetaData> >& ResolvePacketData::getFieldMetaDataVector() const
{
	return m_fieldMetaDataVector;
}

const QVector<QVariant>& ResolvePacketData::getSourceDataVector() const
{
	return m_sourceDataVector;
}

const QVector<QVariant>& ResolvePacketData::getSource2DataVector() const
{
	return m_source2DataVector;
}

void ResolvePacketData::setSourcePrimaryNextValues(const QVector<PrimaryNextValue>& sourcePrimaryNextValues)
{
	m_sourcePrimaryNextValues = sourcePrimaryNextValues;
}

const QVector<PrimaryNextValue>& ResolvePacketData::getSourcePrimaryNextValues() const
{
	return m_sourcePrimaryNextValues;
}

void ResolvePacketData::setSource2PrimaryNextValues(const QVector<PrimaryNextValue>& source2PrimaryNextValues)
{
	m_source2PrimaryNextValues = source2PrimaryNextValues;
}

const QVector<PrimaryNextValue>& ResolvePacketData::getSource2PrimaryNextValues() const
{
	return m_source2PrimaryNextValues;
}

void ResolvePacketData::setSourceDataSetForeignKeys(const QVector<ForeignKeyDataSet>& sourceDataSetForeignKeys)
{
	m_sourceDataSetForeignKeys = sourceDataSetForeignKeys;
}

const QVector<ForeignKeyDataSet>& ResolvePacketData::getSourceDataSetForeignKeys() const
{
	return m_sourceDataSetForeignKeys;
}

void ResolvePacketData::setSource2DataSetForeignKeys(const QVector<ForeignKeyDataSet>& source2DataSetForeignKeys)
{
	m_source2DataSetForeignKeys = source2DataSetForeignKeys;
}

const QVector<ForeignKeyDataSet>& ResolvePacketData::getSource2DataSetForeignKeys() const
{
	return m_source2DataSetForeignKeys;
}

void ResolvePacketData::setSourceBinaryDataDescriptions(const QVector<BinaryDataDescription>& sourceBinaryDataDescriptions)
{
	QVector<BinaryDataDescription>::const_iterator iterEnd = sourceBinaryDataDescriptions.constEnd();
	for(QVector<BinaryDataDescription>::const_iterator iter = sourceBinaryDataDescriptions.constBegin(); iter != iterEnd; iter++)
	{
		m_sourceBinaryDataDescriptionMap.insert((*iter).getFieldName(), *iter);
	}
}

BinaryDataDescription ResolvePacketData::getSourceBinaryDataDescription(const QString& name) const
{
	return m_sourceBinaryDataDescriptionMap.value(name);
}

void ResolvePacketData::setSource2BinaryDataDescriptions(const QVector<BinaryDataDescription>& source2BinaryDataDescriptions)
{
	QVector<BinaryDataDescription>::const_iterator iterEnd = source2BinaryDataDescriptions.constEnd();
	for(QVector<BinaryDataDescription>::const_iterator iter = source2BinaryDataDescriptions.constBegin(); iter != iterEnd; iter++)
	{
		m_source2BinaryDataDescriptionMap.insert((*iter).getFieldName(), *iter);
	}
}

BinaryDataDescription ResolvePacketData::getSource2BinaryDataDescription(const QString& name) const
{
	return m_source2BinaryDataDescriptionMap.value(name);
}

qint64 ResolvePacketData::getMaxSourcesPrimaryNextValue(const QString& fieldName)
{
	qint64 result = -1;
	for(int i = 0; i < m_sourcePrimaryNextValues.size(); i++)
	{
		if(m_sourcePrimaryNextValues[i].getFieldName() == fieldName)
		{
			result = m_sourcePrimaryNextValues[i].getNextValue();
		}
	}
	qint64 result2 = -1;
	for(int i = 0; i < m_source2PrimaryNextValues.size(); i++)
	{
		if(m_source2PrimaryNextValues[i].getFieldName() == fieldName)
		{
			result2 = m_source2PrimaryNextValues[i].getNextValue();
		}
	}
	if(result > result2)
	{
		return result;
	}
	return result2;
}

ResolvePacketData::ResolveResult ResolvePacketData::getResult() const
{
	return m_result;
}

QString ResolvePacketData::getSourceToolTipString(const QString& fieldName) const
{
	for(int i = 0; i < m_sourceDataSetForeignKeys.size(); i++)
	{
		if(m_sourceDataSetForeignKeys[i].getFieldName() == fieldName)
		{
			return m_sourceDataSetForeignKeys[i].getStringValue();
		}
	}
	return "";
}

QString ResolvePacketData::getSource2ToolTipString(const QString& fieldName) const
{
	for(int i = 0; i < m_source2DataSetForeignKeys.size(); i++)
	{
		if(m_source2DataSetForeignKeys[i].getFieldName() == fieldName)
		{
			return m_source2DataSetForeignKeys[i].getStringValue();
		}
	}
	return "";
}
