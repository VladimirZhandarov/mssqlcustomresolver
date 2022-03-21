#ifndef DATABASETREEMODEL_H
#define DATABASETREEMODEL_H

#include "upelib_global.h"

#include <QtCore/QAbstractItemModel>

class DataBaseTreeItem;

class UPELIB_EXPORT DataBaseTreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	DataBaseTreeModel(QObject* parent = 0);
	~DataBaseTreeModel();

	void setDatabases(const QStringList& databases, const QStringList& checkedDatabases);

	void setTables(const QString& database, const QStringList& tables);
	void setStoredProcedures(const QString& database, const QStringList& storedProcedures);
	void setUserFunctions(const QString& database, const QStringList& userFunctions);

	QStringList getCheckedDatabases() const;

	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	virtual QVariant data(const QModelIndex& index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex& index) const;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

signals:
	void checkStateChanged();

private:
	DataBaseTreeItem* getDataBase(const QString& database) const;
	DataBaseTreeItem* castDataBaseTreeItem(const QModelIndex& index) const;

	DataBaseTreeItem* m_root;
};

#endif // DATABASETREEMODEL_H
