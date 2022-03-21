#include "databasetreeitem.h"

DataBaseTreeItem::DataBaseTreeItem(NodeType nodeType, const QString& name, bool checked, DataBaseTreeItem* parent) : m_nodeType(nodeType), m_name(name),
	m_parent(parent)
{
	if(checked)
	{
		m_checkState = Qt::Checked;
	}
	else
	{
		m_checkState = Qt::Unchecked;
	}
}

DataBaseTreeItem::~DataBaseTreeItem()
{
	for(int i = 0; i < m_childList.size(); i++)
	{
		delete m_childList[i];
		m_childList[i] = 0;
	}
}

DataBaseTreeItem::NodeType DataBaseTreeItem::getNodeType() const
{
	return m_nodeType;
}

const QString& DataBaseTreeItem::getName() const
{
	return m_name;
}

void DataBaseTreeItem::setChecked(bool checked)
{
	setCheckedInner(checked);
	if(m_parent)
	{
		m_parent->updateCheckState();
	}
}

Qt::CheckState DataBaseTreeItem::getCheckState() const
{
	return m_checkState;
}

void DataBaseTreeItem::addChild(DataBaseTreeItem* child)
{
	m_childList.append(child);
}

DataBaseTreeItem* DataBaseTreeItem::getParent() const
{
	return m_parent;
}

const QList<DataBaseTreeItem*>& DataBaseTreeItem::getChildList() const
{
	return m_childList;
}

DataBaseTreeItem* DataBaseTreeItem::getChild(int index) const
{
	if(index >= 0 && index < m_childList.size())
	{
		return m_childList[index];
	}
	return 0;
}

void DataBaseTreeItem::clearChildList()
{
	for(int i = 0; i < m_childList.size(); i++)
	{
		delete m_childList[i];
		m_childList[i] = 0;
	}
	m_childList.clear();
}

void DataBaseTreeItem::updateCheckState()
{
	if(!updateCheckStateInner())
	{
		if(m_parent)
		{
			m_parent->updateCheckState();
		}
	}
}

bool DataBaseTreeItem::updateCheckStateInner()
{
	bool result = false;
	Qt::CheckState oldCheckState = m_checkState;
	int checkedChildCount = 0;
	int unCheckedChildCount = 0;
	for(int i = 0; i < m_childList.size(); i++)
	{
		Qt::CheckState childCheckState = m_childList[i]->getCheckState();
		if(childCheckState == Qt::PartiallyChecked)
		{
			m_checkState = Qt::PartiallyChecked;
			break;
		}
		else if(childCheckState == Qt::Checked)
		{
			checkedChildCount++;
			if(unCheckedChildCount > 0)
			{
				m_checkState = Qt::PartiallyChecked;
				break;
			}
		}
		else
		{
			unCheckedChildCount++;
			if(checkedChildCount > 0)
			{
				m_checkState = Qt::PartiallyChecked;
				break;
			}
		}
	}
	if(checkedChildCount == m_childList.size())
	{
		m_checkState = Qt::Checked;
	}
	if(unCheckedChildCount == m_childList.size())
	{
		m_checkState = Qt::Unchecked;
	}
	if(oldCheckState != m_checkState)
	{
		if(m_parent)
		{
			m_parent->updateCheckState();
			result = true;
		}
	}
	return result;
}

void DataBaseTreeItem::setCheckedInner(bool checked)
{
	if(checked)
	{
		m_checkState = Qt::Checked;
	}
	else
	{
		m_checkState = Qt::Unchecked;
	}
	for(int i = 0; i < m_childList.size(); i++)
	{
		m_childList[i]->setCheckedInner(checked);
	}
}
