#include "labelsettingsreader.h"

#include <QtCore/QFile>

QString LabelSettingsReader::tableLabelTagName = "tableLabel";
QString LabelSettingsReader::dbNameTagName = "dbName";
QString LabelSettingsReader::tableTagName = "table";
QString LabelSettingsReader::fieldTagName = "field";
QString LabelSettingsReader::nameAttrName = "name";
QString LabelSettingsReader::viewNameAttrName = "viewName";
QString LabelSettingsReader::boldAttrName = "bold";

FieldData::FieldData(const QString& fieldName, const QString& viewName, bool bold) : m_fieldName(fieldName), m_viewName(viewName), m_bold(bold)
{

}

FieldData::~FieldData()
{

}

const QString& FieldData::getFieldName() const
{
	return m_fieldName;
}

const QString& FieldData::getViewName() const
{
	return m_viewName;
}

bool FieldData::isBold() const
{
	return m_bold;
}

TableData::TableData(const QString& viewName, const QVector<std::tr1::shared_ptr<FieldData> >& fieldVector) : m_viewName(viewName), m_fieldVector(fieldVector)
{

}

TableData::~TableData()
{

}

const QString& TableData::getViewName() const
{
	return m_viewName;
}

const QVector<std::tr1::shared_ptr<FieldData> >& TableData::getFieldVector() const
{
	return m_fieldVector;
}

DbData::DbData(const QMap<QString, std::tr1::shared_ptr<TableData> >& tableMap) : m_tableMap(tableMap)
{

}

DbData::~DbData()
{

}

const QMap<QString, std::tr1::shared_ptr<TableData> >& DbData::getTableMap() const
{
	return m_tableMap;
}

LabelSettingsReader::LabelSettingsReader(const QString& filename, QObject* parent) : QObject(parent)
{
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly))
	{
		m_doc.setContent(&file);
	}

	QDomElement rootDomElement = m_doc.firstChildElement(tableLabelTagName);
	if(rootDomElement.isNull())
	{
		return;
	}

	for(QDomElement dbNameDomElement = rootDomElement.firstChildElement(dbNameTagName); !dbNameDomElement.isNull();
		dbNameDomElement = dbNameDomElement.nextSiblingElement(dbNameTagName))
	{
		QString dbName = dbNameDomElement.attribute(nameAttrName);
		if(!dbName.isEmpty())
		{
			QMap<QString, std::tr1::shared_ptr<TableData> > tableMap;
			for(QDomElement tableDomElement = dbNameDomElement.firstChildElement(tableTagName); !tableDomElement.isNull();
				tableDomElement = tableDomElement.nextSiblingElement(tableTagName))
			{
				QString tableName = tableDomElement.attribute(nameAttrName);
				QString tableViewName = tableDomElement.attribute(viewNameAttrName);
				if(!tableName.isEmpty())
				{
					QVector<std::tr1::shared_ptr<FieldData> > fieldVector;
					for(QDomElement fieldDomElement = tableDomElement.firstChildElement(fieldTagName); !fieldDomElement.isNull();
						fieldDomElement = fieldDomElement.nextSiblingElement(fieldTagName))
					{
						QString fieldName = fieldDomElement.attribute(nameAttrName);
						QString fieldViewName = fieldDomElement.attribute(viewNameAttrName);
						bool fieldBold = (fieldDomElement.attribute(boldAttrName).toLower() != "false");
						if(!fieldName.isEmpty())
						{
							fieldVector.append(std::tr1::shared_ptr<FieldData>(new FieldData(fieldName, fieldViewName, fieldBold)));
						}
					}
					tableMap.insert(tableName, std::tr1::shared_ptr<TableData>(new TableData(tableViewName, fieldVector)));
				}
			}
			m_dbMap.insert(dbName, std::tr1::shared_ptr<DbData>(new DbData(tableMap)));
		}
	}
}

LabelSettingsReader::~LabelSettingsReader()
{
	m_dbMap.clear();
}

const QMap<QString, std::tr1::shared_ptr<DbData> >& LabelSettingsReader::getDbMap() const
{
	return m_dbMap;
}
