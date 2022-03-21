#include "databasetreemodel.h"

#include "databasetreeitem.h"

DataBaseTreeModel::DataBaseTreeModel(QObject* parent) : QAbstractItemModel(parent)
{
	m_root = new DataBaseTreeItem(DataBaseTreeItem::RootNodeType, tr("All databases"), true);
	//m_root = new DataBaseTreeItem(DataBaseTreeItem::RootNodeType, tr("Все базы данных"), true);
}

DataBaseTreeModel::~DataBaseTreeModel()
{

}

void DataBaseTreeModel::setDatabases(const QStringList& databases, const QStringList& checkedDatabases)
{
	Q_ASSERT(m_root);
	if(m_root->getChildList().size() > 0)
	{
		beginRemoveRows(index(0, 0), 0, m_root->getChildList().size() - 1);
		m_root->clearChildList();
		endRemoveRows();
	}
	if(databases.size() > 0)
	{
		beginInsertRows(index(0, 0), 0, databases.size() - 1);
		for(int i = 0; i < databases.size(); i++)
		{
			DataBaseTreeItem* item = new DataBaseTreeItem(DataBaseTreeItem::DataBaseNodeType, databases[i], checkedDatabases.contains(databases[i]), m_root);
			m_root->addChild(item);
		}
		m_root->updateCheckState();
		endInsertRows();
	}
}

void DataBaseTreeModel::setTables(const QString& database, const QStringList& tables)
{
	DataBaseTreeItem* dataBaseTreeItem = getDataBase(database);
	if(dataBaseTreeItem)
	{
		DataBaseTreeItem* tableGroupTreeItem = new DataBaseTreeItem(DataBaseTreeItem::ObjectGroupNodeType, tr("Tables"), true, dataBaseTreeItem);
		//DataBaseTreeItem* tableGroupTreeItem = new DataBaseTreeItem(DataBaseTreeItem::ObjectGroupNodeType, tr("Таблицы"), true, dataBaseTreeItem);
		dataBaseTreeItem->addChild(tableGroupTreeItem);
		for(int i = 0; i < tables.size(); i++)
		{
			DataBaseTreeItem* item = new DataBaseTreeItem(DataBaseTreeItem::ObjectNodeType, tables[i], true, tableGroupTreeItem);
			tableGroupTreeItem->addChild(item);
		}
		tableGroupTreeItem->updateCheckState();
	}
}

void DataBaseTreeModel::setStoredProcedures(const QString& database, const QStringList& storedProcedures)
{
	DataBaseTreeItem* dataBaseTreeItem = getDataBase(database);
	if(dataBaseTreeItem)
	{
		DataBaseTreeItem* storedProcedureGroupTreeItem = new DataBaseTreeItem(DataBaseTreeItem::ObjectGroupNodeType, tr("Stored procedures"), true, dataBaseTreeItem);
		//DataBaseTreeItem* storedProcedureGroupTreeItem = new DataBaseTreeItem(DataBaseTreeItem::ObjectGroupNodeType, tr("Хранимые процедуры"), true, dataBaseTreeItem);
		dataBaseTreeItem->addChild(storedProcedureGroupTreeItem);
		for(int i = 0; i < storedProcedures.size(); i++)
		{
			DataBaseTreeItem* item = new DataBaseTreeItem(DataBaseTreeItem::ObjectNodeType, storedProcedures[i], true, storedProcedureGroupTreeItem);
			storedProcedureGroupTreeItem->addChild(item);
		}
		storedProcedureGroupTreeItem->updateCheckState();
	}
}

void DataBaseTreeModel::setUserFunctions(const QString& database, const QStringList& userFunctions)
{
	DataBaseTreeItem* dataBaseTreeItem = getDataBase(database);
	if(dataBaseTreeItem)
	{
		DataBaseTreeItem* userFunctionGroupTreeItem = new DataBaseTreeItem(DataBaseTreeItem::ObjectGroupNodeType, tr("User functions"), true, dataBaseTreeItem);
		//DataBaseTreeItem* userFunctionGroupTreeItem = new DataBaseTreeItem(DataBaseTreeItem::ObjectGroupNodeType, tr("Пользовательские функции"), true, dataBaseTreeItem);
		dataBaseTreeItem->addChild(userFunctionGroupTreeItem);
		for(int i = 0; i < userFunctions.size(); i++)
		{
			DataBaseTreeItem* item = new DataBaseTreeItem(DataBaseTreeItem::ObjectNodeType, userFunctions[i], true, userFunctionGroupTreeItem);
			userFunctionGroupTreeItem->addChild(item);
		}
		userFunctionGroupTreeItem->updateCheckState();
	}
}

QStringList DataBaseTreeModel::getCheckedDatabases() const
{
	const QList<DataBaseTreeItem*>& child = m_root->getChildList();
	QStringList result;
	for(int i = 0; i < child.size(); i++)
	{
		if(child[i]->getCheckState() != Qt::Unchecked)
		{
			result.append(child[i]->getName());
		}
	}
	return result;
}

bool DataBaseTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if(index.isValid())
	{
		DataBaseTreeItem* dataBaseTreeItem = castDataBaseTreeItem(index);
		if(dataBaseTreeItem && index.column() == 0)
		{
			if(role == Qt::CheckStateRole)
			{
				dataBaseTreeItem->setChecked(value.toBool());
				emit dataChanged(QModelIndex(), QModelIndex());
				emit checkStateChanged();
				return true;
			}
		}
	}
	return QAbstractItemModel::setData(index, value, role);
}

QVariant DataBaseTreeModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}
	DataBaseTreeItem* dataBaseTreeItem = castDataBaseTreeItem(index);
	if(!dataBaseTreeItem)
	{
		return QVariant();
	}
	if(index.column() == 0)
	{
		if(role == Qt::CheckStateRole)
		{
			return dataBaseTreeItem->getCheckState();
		}
		else if(role == Qt::DisplayRole)
		{
			return dataBaseTreeItem->getName();
		}
	}
	return QVariant();
}

Qt::ItemFlags DataBaseTreeModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags result = Qt::NoItemFlags;
	if(index.isValid())
	{
		result = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
	}
	return result;
}

QVariant DataBaseTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex DataBaseTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if(!parent.isValid())
	{
		return createIndex(0, 0, m_root);
	}
	else
	{
		DataBaseTreeItem* parentItem = castDataBaseTreeItem(parent);
		if(parentItem)
		{
			DataBaseTreeItem* childItem = parentItem->getChild(row);
			if(childItem)
			{
				return createIndex(row, column, childItem);
			}
		}
	}
	return QModelIndex();
}

QModelIndex DataBaseTreeModel::parent(const QModelIndex& index) const
{
	if(!index.isValid())
	{
		return QModelIndex();
	}
	DataBaseTreeItem* dataBaseItem = castDataBaseTreeItem(index);
	if(!dataBaseItem)
	{
		return QModelIndex();
	}
	DataBaseTreeItem* parentObjectItem = castDataBaseTreeItem(index)->getParent();
	if(!parentObjectItem)
	{
		return QModelIndex();
	}
	if(parentObjectItem == m_root)
	{
		return createIndex(0, 0, m_root);
	}
	int row = parentObjectItem->getChildList().indexOf(dataBaseItem);
	return createIndex(row, 0, parentObjectItem);
}

int DataBaseTreeModel::rowCount(const QModelIndex& parent) const
{
	if(!parent.isValid())
	{
		return 1;
	}
	DataBaseTreeItem* dataBaseItem = castDataBaseTreeItem(parent);
	if(dataBaseItem)
	{
		return dataBaseItem->getChildList().size();
	}
	return 0;
}

int DataBaseTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

DataBaseTreeItem* DataBaseTreeModel::getDataBase(const QString& database) const
{
	const QList<DataBaseTreeItem*>& child = m_root->getChildList();
	DataBaseTreeItem* result = 0;
	for(int i = 0; i < child.size(); i++)
	{
		if(child[i]->getName() == database)
		{
			result = child[i];
			break;
		}
	}
	return result;
}

DataBaseTreeItem* DataBaseTreeModel::castDataBaseTreeItem(const QModelIndex& index) const
{
	if(index.isValid())
	{
		return reinterpret_cast<DataBaseTreeItem*>(index.internalPointer());
	}
	return 0;
}
