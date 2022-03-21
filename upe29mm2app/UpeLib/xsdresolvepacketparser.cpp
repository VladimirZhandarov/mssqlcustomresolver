#include "xsdresolvepacketparser.h"

QString XsdResolvePacketParser::resolvePacketTagName = "ResolvePacket";

QString XsdResolvePacketParser::resolveTypeTagName = "resolveType";
QString XsdResolvePacketParser::sourceIdentifierTagName = "sourceIdentifier";
QString XsdResolvePacketParser::publisherNameTagName = "publisherName";
QString XsdResolvePacketParser::subscriberNameTagName = "subscriberName";
QString XsdResolvePacketParser::publisherDBNameTagName = "publisherDBName";
QString XsdResolvePacketParser::subscriberDBNameTagName = "subscriberDBName";

QString XsdResolvePacketParser::errorCodeTagName = "errorCode";
QString XsdResolvePacketParser::errorMessageTagName = "errorMessage";
QString XsdResolvePacketParser::sourceDataSetTagName = "sourceDataSet";
QString XsdResolvePacketParser::source2DataSetTagName = "source2DataSet";
QString XsdResolvePacketParser::sourcePrimaryNextValuesTagName = "sourcePrimaryNextValues";
QString XsdResolvePacketParser::source2PrimaryNextValuesTagName = "source2PrimaryNextValues";
QString XsdResolvePacketParser::sourceDataSetForeignKeysTagName = "sourceDataSetForeignKeys";
QString XsdResolvePacketParser::source2DataSetForeignKeysTagName = "source2DataSetForeignKeys";
QString XsdResolvePacketParser::binaryDataDescriptionsTagName = "binaryDataDescriptions";
QString XsdResolvePacketParser::sourceBinaryDataDescriptionsTagName = "sourceBinaryDataDescriptions";
QString XsdResolvePacketParser::source2BinaryDataDescriptionsTagName = "source2BinaryDataDescriptions";

QString XsdResolvePacketParser::resultTagName = "result";

QString XsdResolvePacketParser::primaryNextValueTagName = "PrimaryNextValue";
QString XsdResolvePacketParser::foreignKeyDataSetTagName = "ForeignKeyDataSet";
QString XsdResolvePacketParser::binaryDataDescriptionTagName = "BinaryDataDescription";
QString XsdResolvePacketParser::fieldNameTagName = "fieldName";
QString XsdResolvePacketParser::nextValueTagName = "nextValue";
QString XsdResolvePacketParser::dataSetTagName = "dataSet";
QString XsdResolvePacketParser::dataSizeTagName = "dataSize";
QString XsdResolvePacketParser::equalDataTagName = "equalData";
QString XsdResolvePacketParser::nullDataTagName = "nullData";

QString XsdResolvePacketParser::xsSchemaTagName = "xs:schema";
QString XsdResolvePacketParser::xsElementTagName = "xs:element";
QString XsdResolvePacketParser::xsComplexTypeTagName = "xs:complexType";
QString XsdResolvePacketParser::xsChoiceTagName = "xs:choice";
QString XsdResolvePacketParser::xsSequenceTagName = "xs:sequence";

QString XsdResolvePacketParser::idAttrName = "id";
QString XsdResolvePacketParser::nameAttrName = "name";
QString XsdResolvePacketParser::typeAttrName = "type";
QString XsdResolvePacketParser::minOccursAttrName = "type";

QString XsdResolvePacketParser::diffgrDiffgramTagName = "diffgr:diffgram";
QString XsdResolvePacketParser::diffgrIdAttrName = "diffgr:id";
QString XsdResolvePacketParser::rowguidAttrName = "rowguid";

static const char* ResolveTypeEnumStrings[] =
{
	"InsertResolveType",
	"InsertErrorResolveType",
	"UpdateResolveType",
	"UpdateErrorResolveType",
	"DeleteResolveType",
	"DeleteErrorResolveType",
	"UpdateConflictsResolveType",
	"UpdateDeleteConflictResolveType",
	0
};

static const char* SourceIdentifierEnumStrings[] =
{
	"SourceIsPublisher",
	"SourceIsSubscriber",
	0
};

static const char* ResolveResultEnumStrings[] =
{
	"UndefineResolveResult",
	"AcceptResolveResult",
	"RejectResolveResult",
	"CustomResolveResult",
	"Custom2ResolveResult",
	0
};

static const char* XsdTypeEnumStrings[] =
{
	"xs:string",
	"xs:int",
	0
};

XsdResolvePacketParser::XsdResolvePacketParser(QObject* parent) : QObject(parent)
{

}

XsdResolvePacketParser::~XsdResolvePacketParser()
{

}

std::tr1::shared_ptr<ResolvePacketData> XsdResolvePacketParser::getPacket() const
{
	return m_packetData;
}

bool XsdResolvePacketParser::parse(const QString& content)
{
	m_packetData = std::tr1::shared_ptr<ResolvePacketData>();

	m_doc.setContent(content);
	QDomElement resolvePacketElement = m_doc.firstChildElement(resolvePacketTagName);
	if(resolvePacketElement.isNull())
	{
		return false;
	}

	QDomElement resolveTypeElement = resolvePacketElement.firstChildElement(resolveTypeTagName);
	if(resolveTypeElement.isNull())
	{
		return false;
	}
	QString resolveTypeString = resolveTypeElement.firstChild().toCharacterData().data();
	ResolvePacketData::ResolveType resolveType = ResolvePacketData::ResolveType::UndefineResolveType;
	int resolveTypeIndex = resolveType + 1;
	while(ResolveTypeEnumStrings[resolveTypeIndex] != 0)
	{
		if(ResolveTypeEnumStrings[resolveTypeIndex] == resolveTypeString)
		{
			resolveType = (ResolvePacketData::ResolveType)resolveTypeIndex;
			break;
		}
		resolveTypeIndex++;
	}

	QDomElement sourceIdentifierElement = resolvePacketElement.firstChildElement(sourceIdentifierTagName);
	if(sourceIdentifierElement.isNull())
	{
		return false;
	}
	QString sourceIdentifierString = sourceIdentifierElement.firstChild().toCharacterData().data();
	ResolvePacketData::SourceIdentifier sourceIdentifierType = ResolvePacketData::SourceIdentifier::SourceIsUndefined;
	int sourceIdentifierTypeIndex = sourceIdentifierType + 1;
	while(SourceIdentifierEnumStrings[sourceIdentifierTypeIndex] != 0)
	{
		if(SourceIdentifierEnumStrings[sourceIdentifierTypeIndex] == sourceIdentifierString)
		{
			sourceIdentifierType = (ResolvePacketData::SourceIdentifier)sourceIdentifierTypeIndex;
			break;
		}
		sourceIdentifierTypeIndex++;
	}

	QDomElement publisherNameElement = resolvePacketElement.firstChildElement(publisherNameTagName);
	if(publisherNameElement.isNull())
	{
		return false;
	}
	QString publisherName = publisherNameElement.firstChild().toCharacterData().data();

	QDomElement subscriberNameElement = resolvePacketElement.firstChildElement(subscriberNameTagName);
	if(subscriberNameElement.isNull())
	{
		return false;
	}
	QString subscriberName = subscriberNameElement.firstChild().toCharacterData().data();

	QDomElement publisherDBNameElement = resolvePacketElement.firstChildElement(publisherDBNameTagName);
	QString publisherDBName = "";
	if(!publisherDBNameElement.isNull())
	{
		publisherDBName = publisherDBNameElement.firstChild().toCharacterData().data();
	}

	QDomElement subscriberDBNameElement = resolvePacketElement.firstChildElement(subscriberDBNameTagName);
	QString subscriberDBName = "";
	if(!subscriberDBNameElement.isNull())
	{
		subscriberDBName = subscriberDBNameElement.firstChild().toCharacterData().data();
	}

	int errorCode = 0;
	QDomElement errorCodeElement = resolvePacketElement.firstChildElement(errorCodeTagName);
	if(!errorCodeElement.isNull())
	{
		errorCode = errorCodeElement.firstChild().toCharacterData().data().toInt();
	}

	QString errorMessage = "";
	QDomElement errorMessageElement = resolvePacketElement.firstChildElement(errorMessageTagName);
	if(!errorMessageElement.isNull())
	{
		errorMessage = errorMessageElement.firstChild().toCharacterData().data();
	}

	QDomElement sourceDataSetElement = resolvePacketElement.firstChildElement(sourceDataSetTagName);
	if(sourceDataSetElement.isNull())
	{
		return false;
	}
	QVector<std::tr1::shared_ptr<FieldMetaData> > fieldMetaDataVector = createFieldMetaDataVector(sourceDataSetElement);
	QString tableTagName = getTableTagName(sourceDataSetElement);
	QVector<QVariant> sourceDataVector = createDataVector(sourceDataSetElement, tableTagName, fieldMetaDataVector);

	QDomElement source2DataSetElement = resolvePacketElement.firstChildElement(source2DataSetTagName);
	QVector<QVariant> source2DataVector;
	if(!source2DataSetElement.isNull())
	{
		QVector<std::tr1::shared_ptr<FieldMetaData> > fieldMetaDataVector2 = createFieldMetaDataVector(sourceDataSetElement);
		QString tableTagName2 = getTableTagName(sourceDataSetElement);
		source2DataVector = createDataVector(source2DataSetElement, tableTagName2, fieldMetaDataVector2);
	}

	QVector<PrimaryNextValue> sourcePrimaryNextValues;
	QDomElement sourcePrimaryNextValuesElement = resolvePacketElement.firstChildElement(sourcePrimaryNextValuesTagName);
	if(!sourcePrimaryNextValuesElement.isNull())
	{
		for(QDomElement primaryKeyDataElement = sourcePrimaryNextValuesElement.firstChildElement(primaryNextValueTagName); !primaryKeyDataElement.isNull();
			primaryKeyDataElement = primaryKeyDataElement.nextSiblingElement(primaryNextValueTagName))
		{
			QDomElement fieldNameElement = primaryKeyDataElement.firstChildElement(fieldNameTagName);
			QString fieldName = "";
			if(!fieldNameElement.isNull())
			{
				fieldName = fieldNameElement.firstChild().toCharacterData().data();
			}
			QDomElement nextValueElement = primaryKeyDataElement.firstChildElement(nextValueTagName);
			qint64 nextValue = 0;
			if(!nextValueElement.isNull())
			{
				nextValue = nextValueElement.firstChild().toCharacterData().data().toLongLong();
			}
			sourcePrimaryNextValues.append(PrimaryNextValue(fieldName, nextValue));
		}
	}

	QVector<PrimaryNextValue> source2PrimaryNextValues;
	QDomElement source2PrimaryNextValuesElement = resolvePacketElement.firstChildElement(source2PrimaryNextValuesTagName);
	if(!source2PrimaryNextValuesElement.isNull())
	{
		for(QDomElement primaryKeyDataElement = source2PrimaryNextValuesElement.firstChildElement(primaryNextValueTagName); !primaryKeyDataElement.isNull();
			primaryKeyDataElement = primaryKeyDataElement.nextSiblingElement(primaryNextValueTagName))
		{
			QDomElement fieldNameElement = primaryKeyDataElement.firstChildElement(fieldNameTagName);
			QString fieldName = "";
			if(!fieldNameElement.isNull())
			{
				fieldName = fieldNameElement.firstChild().toCharacterData().data();
			}
			QDomElement nextValueElement = primaryKeyDataElement.firstChildElement(nextValueTagName);
			int nextValue = 0;
			if(!nextValueElement.isNull())
			{
				nextValue = nextValueElement.firstChild().toCharacterData().data().toInt();
			}
			source2PrimaryNextValues.append(PrimaryNextValue(fieldName, nextValue));
		}
	}

	QVector<ForeignKeyDataSet> sourceDataSetForeignKeys;
	QDomElement sourceDataSetForeignKeysElement = resolvePacketElement.firstChildElement(sourceDataSetForeignKeysTagName);
	if(!sourceDataSetForeignKeysElement.isNull())
	{
		for(QDomElement foreignKeyDataElement = sourceDataSetForeignKeysElement.firstChildElement(foreignKeyDataSetTagName); !foreignKeyDataElement.isNull();
			foreignKeyDataElement = foreignKeyDataElement.nextSiblingElement(foreignKeyDataSetTagName))
		{
			QDomElement fieldNameElement = foreignKeyDataElement.firstChildElement(fieldNameTagName);
			QString fieldName = "";
			if(!fieldNameElement.isNull())
			{
				fieldName = fieldNameElement.firstChild().toCharacterData().data();
			}
			QDomElement dataSetElement = foreignKeyDataElement.firstChildElement(dataSetTagName);
			QVector<QVariant> dataVector;
			QVector<std::tr1::shared_ptr<FieldMetaData> > fieldMetaDataVector;
			QString foreignTable = getTableTagName(dataSetElement);
			if(!dataSetElement.isNull())
			{
				fieldMetaDataVector = createFieldMetaDataVector(dataSetElement);
				dataVector = createDataVector(dataSetElement, foreignTable, fieldMetaDataVector);
			}
			QVector<BinaryDataDescription> binaryDataDescriptions;
			QDomElement binaryDataDescriptionsElement = foreignKeyDataElement.firstChildElement(binaryDataDescriptionsTagName);
			if(!binaryDataDescriptionsElement.isNull())
			{
				for(QDomElement binaryDataDescriptionElement = binaryDataDescriptionsElement.firstChildElement(binaryDataDescriptionTagName); !binaryDataDescriptionElement.isNull();
					binaryDataDescriptionElement = binaryDataDescriptionElement.nextSiblingElement(binaryDataDescriptionTagName))
				{
					QDomElement fieldNameElement = binaryDataDescriptionElement.firstChildElement(fieldNameTagName);
					QString fieldName = "";
					if(!fieldNameElement.isNull())
					{
						fieldName = fieldNameElement.firstChild().toCharacterData().data();
					}
					QDomElement dataSizeElement = binaryDataDescriptionElement.firstChildElement(dataSizeTagName);
					qint64 dataSize = 0;
					if(!dataSizeElement.isNull())
					{
						dataSize = dataSizeElement.firstChild().toCharacterData().data().toLongLong();
					}
					QDomElement equalDataElement = binaryDataDescriptionElement.firstChildElement(equalDataTagName);
					bool equalData = false;
					if(!equalDataElement.isNull() && equalDataElement.firstChild().toCharacterData().data().toLower() == "true")
					{
						equalData = true;
					}
					QDomElement nullDataElement = binaryDataDescriptionElement.firstChildElement(nullDataTagName);
					bool nullData = false;
					if(!nullDataElement.isNull() && nullDataElement.firstChild().toCharacterData().data().toLower() == "true")
					{
						nullData = true;
					}
					binaryDataDescriptions.append(BinaryDataDescription(fieldName, dataSize, equalData, nullData));
				}
			}
			sourceDataSetForeignKeys.append(ForeignKeyDataSet(fieldName, foreignTable, fieldMetaDataVector, dataVector, binaryDataDescriptions));
		}
	}

	QVector<ForeignKeyDataSet> source2DataSetForeignKeys;
	QDomElement source2DataSetForeignKeysElement = resolvePacketElement.firstChildElement(source2DataSetForeignKeysTagName);
	if(!source2DataSetForeignKeysElement.isNull())
	{
		for(QDomElement foreignKeyDataElement = source2DataSetForeignKeysElement.firstChildElement(foreignKeyDataSetTagName); !foreignKeyDataElement.isNull();
			foreignKeyDataElement = foreignKeyDataElement.nextSiblingElement(foreignKeyDataSetTagName))
		{
			QDomElement fieldNameElement = foreignKeyDataElement.firstChildElement(fieldNameTagName);
			QString fieldName = "";
			if(!fieldNameElement.isNull())
			{
				fieldName = fieldNameElement.firstChild().toCharacterData().data();
			}
			QDomElement dataSetElement = foreignKeyDataElement.firstChildElement(dataSetTagName);
			QVector<QVariant> dataVector;
			QVector<std::tr1::shared_ptr<FieldMetaData> > fieldMetaDataVector;
			QString foreignTable = getTableTagName(dataSetElement);
			if(!dataSetElement.isNull())
			{
				fieldMetaDataVector = createFieldMetaDataVector(dataSetElement);
				dataVector = createDataVector(dataSetElement, foreignTable, fieldMetaDataVector);
			}
			QVector<BinaryDataDescription> binaryDataDescriptions;
			QDomElement binaryDataDescriptionsElement = foreignKeyDataElement.firstChildElement(binaryDataDescriptionsTagName);
			if(!binaryDataDescriptionsElement.isNull())
			{
				for(QDomElement binaryDataDescriptionElement = binaryDataDescriptionsElement.firstChildElement(binaryDataDescriptionTagName); !binaryDataDescriptionElement.isNull();
					binaryDataDescriptionElement = binaryDataDescriptionElement.nextSiblingElement(binaryDataDescriptionTagName))
				{
					QDomElement fieldNameElement = binaryDataDescriptionElement.firstChildElement(fieldNameTagName);
					QString fieldName = "";
					if(!fieldNameElement.isNull())
					{
						fieldName = fieldNameElement.firstChild().toCharacterData().data();
					}
					QDomElement dataSizeElement = binaryDataDescriptionElement.firstChildElement(dataSizeTagName);
					qint64 dataSize = 0;
					if(!dataSizeElement.isNull())
					{
						dataSize = dataSizeElement.firstChild().toCharacterData().data().toLongLong();
					}
					QDomElement equalDataElement = binaryDataDescriptionElement.firstChildElement(equalDataTagName);
					bool equalData = false;
					if(!equalDataElement.isNull() && equalDataElement.firstChild().toCharacterData().data().toLower() == "true")
					{
						equalData = true;
					}
					QDomElement nullDataElement = binaryDataDescriptionElement.firstChildElement(nullDataTagName);
					bool nullData = false;
					if(!nullDataElement.isNull() && nullDataElement.firstChild().toCharacterData().data().toLower() == "true")
					{
						nullData = true;
					}
					binaryDataDescriptions.append(BinaryDataDescription(fieldName, dataSize, equalData, nullData));
				}
			}
			source2DataSetForeignKeys.append(ForeignKeyDataSet(fieldName, foreignTable, fieldMetaDataVector, dataVector, binaryDataDescriptions));
		}
	}

	QVector<BinaryDataDescription> sourceBinaryDataDescriptions;
	QDomElement sourceBinaryDataDescriptionsElement = resolvePacketElement.firstChildElement(sourceBinaryDataDescriptionsTagName);
	if(!sourceBinaryDataDescriptionsElement.isNull())
	{
		for(QDomElement binaryDataDescriptionElement = sourceBinaryDataDescriptionsElement.firstChildElement(binaryDataDescriptionTagName); !binaryDataDescriptionElement.isNull();
			binaryDataDescriptionElement = binaryDataDescriptionElement.nextSiblingElement(binaryDataDescriptionTagName))
		{
			QDomElement fieldNameElement = binaryDataDescriptionElement.firstChildElement(fieldNameTagName);
			QString fieldName = "";
			if(!fieldNameElement.isNull())
			{
				fieldName = fieldNameElement.firstChild().toCharacterData().data();
			}
			QDomElement dataSizeElement = binaryDataDescriptionElement.firstChildElement(dataSizeTagName);
			qint64 dataSize = 0;
			if(!dataSizeElement.isNull())
			{
				dataSize = dataSizeElement.firstChild().toCharacterData().data().toLongLong();
			}
			QDomElement equalDataElement = binaryDataDescriptionElement.firstChildElement(equalDataTagName);
			bool equalData = false;
			if(!equalDataElement.isNull() && equalDataElement.firstChild().toCharacterData().data().toLower() == "true")
			{
				equalData = true;
			}
			QDomElement nullDataElement = binaryDataDescriptionElement.firstChildElement(nullDataTagName);
			bool nullData = false;
			if(!nullDataElement.isNull() && nullDataElement.firstChild().toCharacterData().data().toLower() == "true")
			{
				nullData = true;
			}
			sourceBinaryDataDescriptions.append(BinaryDataDescription(fieldName, dataSize, equalData, nullData));
		}
	}

	QVector<BinaryDataDescription> source2BinaryDataDescriptions;
	QDomElement source2BinaryDataDescriptionsElement = resolvePacketElement.firstChildElement(source2BinaryDataDescriptionsTagName);
	if(!source2BinaryDataDescriptionsElement.isNull())
	{
		for(QDomElement binaryDataDescriptionElement = source2BinaryDataDescriptionsElement.firstChildElement(binaryDataDescriptionTagName); !binaryDataDescriptionElement.isNull();
			binaryDataDescriptionElement = binaryDataDescriptionElement.nextSiblingElement(binaryDataDescriptionTagName))
		{
			QDomElement fieldNameElement = binaryDataDescriptionElement.firstChildElement(fieldNameTagName);
			QString fieldName = "";
			if(!fieldNameElement.isNull())
			{
				fieldName = fieldNameElement.firstChild().toCharacterData().data();
			}
			QDomElement dataSizeElement = binaryDataDescriptionElement.firstChildElement(dataSizeTagName);
			qint64 dataSize = 0;
			if(!dataSizeElement.isNull())
			{
				dataSize = dataSizeElement.firstChild().toCharacterData().data().toLongLong();
			}
			QDomElement equalDataElement = binaryDataDescriptionElement.firstChildElement(equalDataTagName);
			bool equalData = false;
			if(!equalDataElement.isNull() && equalDataElement.firstChild().toCharacterData().data().toLower() == "true")
			{
				equalData = true;
			}
			QDomElement nullDataElement = binaryDataDescriptionElement.firstChildElement(nullDataTagName);
			bool nullData = false;
			if(!nullDataElement.isNull() && nullDataElement.firstChild().toCharacterData().data().toLower() == "true")
			{
				nullData = true;
			}
			source2BinaryDataDescriptions.append(BinaryDataDescription(fieldName, dataSize, equalData, nullData));
		}
	}

	QDomElement resultElement = resolvePacketElement.firstChildElement(resultTagName);
	if(resultElement.isNull())
	{
		return false;
	}
	QString resultString = resultElement.firstChild().toCharacterData().data();
	ResolvePacketData::ResolveResult resultType = ResolvePacketData::ResolveResult::UndefineResolveResult;
	int resultTypeIndex = resultType;
	while(ResolveResultEnumStrings[resultTypeIndex] != 0)
	{
		if(ResolveResultEnumStrings[resultTypeIndex] == resultString)
		{
			resultType = (ResolvePacketData::ResolveResult)resultTypeIndex;
			break;
		}
		resultTypeIndex++;
	}
	m_packetData = std::tr1::shared_ptr<ResolvePacketData>(new ResolvePacketData(resolveType, sourceIdentifierType, publisherName, subscriberName,
		publisherDBName, subscriberDBName, tableTagName, errorCode, errorMessage, fieldMetaDataVector, sourceDataVector, source2DataVector, resultType));
	m_packetData->setSourcePrimaryNextValues(sourcePrimaryNextValues);
	m_packetData->setSource2PrimaryNextValues(source2PrimaryNextValues);
	m_packetData->setSourceDataSetForeignKeys(sourceDataSetForeignKeys);
	m_packetData->setSource2DataSetForeignKeys(source2DataSetForeignKeys);
	m_packetData->setSourceBinaryDataDescriptions(sourceBinaryDataDescriptions);
	m_packetData->setSource2BinaryDataDescriptions(source2BinaryDataDescriptions);

	return true;
}

QByteArray XsdResolvePacketParser::setAcceptResult()
{
	setResult(ResolvePacketData::ResolveResult::AcceptResolveResult);
	return docToByteArray();
}

QByteArray XsdResolvePacketParser::setRejectResult()
{
	setResult(ResolvePacketData::ResolveResult::RejectResolveResult);
	return docToByteArray();
}

QByteArray XsdResolvePacketParser::setCustomResult()
{
	setResult(ResolvePacketData::ResolveResult::CustomResolveResult);
	return docToByteArray();
}

QByteArray XsdResolvePacketParser::setCustom2Result()
{
	setResult(ResolvePacketData::ResolveResult::Custom2ResolveResult);
	return docToByteArray();
}

QVector<std::tr1::shared_ptr<FieldMetaData> > XsdResolvePacketParser::createFieldMetaDataVector(QDomElement& dataSetElement) const
{
	QVector<std::tr1::shared_ptr<FieldMetaData> > result;
	if(dataSetElement.isNull())
	{
		return result;
	}
	QDomElement sourceDataSetXsSchema0Element = dataSetElement.firstChildElement(xsSchemaTagName);
	QString dataSetTagName = sourceDataSetXsSchema0Element.attribute(idAttrName);
	QDomElement sourceDataSetXsSchema1Element = sourceDataSetXsSchema0Element.firstChildElement(xsElementTagName);
	QDomElement sourceDataSetXsSchema2Element = sourceDataSetXsSchema1Element.firstChildElement(xsComplexTypeTagName);
	QDomElement sourceDataSetXsSchema3Element = sourceDataSetXsSchema2Element.firstChildElement(xsChoiceTagName);
	QDomElement sourceDataSetXsSchema4Element = sourceDataSetXsSchema3Element.firstChildElement(xsElementTagName);
	QString tableTagName = sourceDataSetXsSchema4Element.attribute(nameAttrName);
	QDomElement sourceDataSetXsSchema5Element = sourceDataSetXsSchema4Element.firstChildElement(xsComplexTypeTagName);
	QDomElement sourceDataSetXsSchema6Element = sourceDataSetXsSchema5Element.firstChildElement(xsSequenceTagName);
	for(QDomElement sourceDataElement = sourceDataSetXsSchema6Element.firstChildElement(xsElementTagName); !sourceDataElement.isNull();
		sourceDataElement = sourceDataElement.nextSiblingElement(xsElementTagName))
	{
		QString name = sourceDataElement.attribute(nameAttrName);
		QString typeString = sourceDataElement.attribute(typeAttrName);
		FieldMetaData::XsdType type = FieldMetaData::XsdType::StringXsdType;
		int typeIndex = type;
		while(XsdTypeEnumStrings[typeIndex] != 0)
		{
			if(XsdTypeEnumStrings[typeIndex] == typeString)
			{
				type = (FieldMetaData::XsdType)typeIndex;
				break;
			}
			typeIndex++;
		}
		int minOccurs = sourceDataElement.attribute(minOccursAttrName).toInt();
		result.push_back(std::tr1::shared_ptr<FieldMetaData>(new FieldMetaData(name, type, minOccurs)));
	}
	return result;
}

QString XsdResolvePacketParser::getTableTagName(QDomElement& dataSetElement) const
{
	if(dataSetElement.isNull())
	{
		return "";
	}
	QDomElement sourceDataSetXsSchema0Element = dataSetElement.firstChildElement(xsSchemaTagName);
	QString dataSetTagName = sourceDataSetXsSchema0Element.attribute(idAttrName);
	QDomElement sourceDataSetXsSchema1Element = sourceDataSetXsSchema0Element.firstChildElement(xsElementTagName);
	QDomElement sourceDataSetXsSchema2Element = sourceDataSetXsSchema1Element.firstChildElement(xsComplexTypeTagName);
	QDomElement sourceDataSetXsSchema3Element = sourceDataSetXsSchema2Element.firstChildElement(xsChoiceTagName);
	QDomElement sourceDataSetXsSchema4Element = sourceDataSetXsSchema3Element.firstChildElement(xsElementTagName);
	return sourceDataSetXsSchema4Element.attribute(nameAttrName);
}

QVector<QVariant> XsdResolvePacketParser::createDataVector(QDomElement& sourceDataSetElement, const QString& tableTagName,
	const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector) const
{
	QVector<QVariant> result;
	if(sourceDataSetElement.isNull())
	{
		return result;
	}
	QDomElement sourceDataSetXsDataElement = sourceDataSetElement.firstChildElement(diffgrDiffgramTagName);
	QDomElement dataSetElement = sourceDataSetXsDataElement.firstChildElement();
	QDomElement tableElement = dataSetElement.firstChildElement(tableTagName);
	QString tableName = tableElement.attribute(diffgrIdAttrName);
	for(int i = 0; i < fieldMetaDataVector.size(); i++)
	{
		QDomElement dataElement = tableElement.firstChildElement(fieldMetaDataVector[i]->getName());
		if(!dataElement.isNull())
		{
			switch(fieldMetaDataVector[i]->getType())
			{
				case FieldMetaData::XsdType::StringXsdType:
				{
					result.push_back(dataElement.firstChild().toCharacterData().data());
					break;
				}
				case FieldMetaData::XsdType::IntXsdType:
				{
					result.push_back(dataElement.firstChild().toCharacterData().data().toInt());
					break;
				}
				default:
				{
					result.push_back(dataElement.firstChild().toCharacterData().data());
				}
			}
		}
		else
		{
			result.push_back(QVariant());
		}
	}
	return result;
}


void XsdResolvePacketParser::setResult(ResolvePacketData::ResolveResult resolveResult)
{
	QDomElement resolvePacketElement = m_doc.firstChildElement(resolvePacketTagName);
	if(resolvePacketElement.isNull())
	{
		return;
	}
	QDomElement resultElement = resolvePacketElement.firstChildElement(resultTagName);
	QDomCharacterData data = resultElement.firstChild().toCharacterData();
	data.setData(ResolveResultEnumStrings[resolveResult]);
}

QByteArray XsdResolvePacketParser::docToByteArray() const
{
	QByteArray data = m_doc.toByteArray();
	data.replace('\0', "&#x0;");
	return data;
}
