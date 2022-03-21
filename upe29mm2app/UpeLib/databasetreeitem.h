#ifndef DATABASETREEITEM_H
#define DATABASETREEITEM_H

#include "upelib_global.h"

#include <QtCore/QString>
#include <QtCore/QList>

class UPELIB_EXPORT DataBaseTreeItem
{
public:
	enum NodeType
	{
		RootNodeType = 0,
		DataBaseNodeType,
		ObjectGroupNodeType,
		ObjectNodeType
	};

	DataBaseTreeItem(NodeType nodeType, const QString& name, bool checked, DataBaseTreeItem* parent = 0);
	~DataBaseTreeItem();

	NodeType getNodeType() const;
	const QString& getName() const;

	void setChecked(bool checked);
	Qt::CheckState getCheckState() const;

	void addChild(DataBaseTreeItem* child);

	DataBaseTreeItem* getParent() const;
	const QList<DataBaseTreeItem*>& getChildList() const;
	DataBaseTreeItem* getChild(int index) const;
	void clearChildList();

	void updateCheckState();

private:
	bool updateCheckStateInner();
	void setCheckedInner(bool checked);

	NodeType m_nodeType;
	QString m_name;
	Qt::CheckState m_checkState;

	DataBaseTreeItem* m_parent;
	QList<DataBaseTreeItem*> m_childList;
};

#endif // DATABASETREEITEM_H
