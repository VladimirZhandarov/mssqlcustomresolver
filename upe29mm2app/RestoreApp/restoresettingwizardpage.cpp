#include "restoresettingwizardpage.h"

#include <databasetreemodel.h>

#include <QtWidgets/QFileDialog>

#include <QtCore/QDir>

#include <osqlsettingsreader.h>

RestoreSettingWizardPage::RestoreSettingWizardPage(OsqlSettingsReader* osqlSettingsReader, QWidget* parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	Q_ASSERT(osqlSettingsReader);

	DataBaseTreeModel* dataBaseTreeModel = new DataBaseTreeModel();
	ui.databasesTV->setModel(dataBaseTreeModel);
	ui.databasesTV->setHeaderHidden(true);
	connect(dataBaseTreeModel, SIGNAL(checkStateChanged()), this, SLOT(onDatabasesCheckStateChanged()));

	QString lastBackupDir = osqlSettingsReader->getLastBackupDir();
	if(QDir(lastBackupDir).exists())
	{
		ui.directoryLE->setText(lastBackupDir);

		QDir directory(lastBackupDir);
		QStringList nameFilters;
		nameFilters << "*.bak";
		QFileInfoList fileList = directory.entryInfoList(nameFilters, QDir::Files, QDir::Name);
		QStringList databases;
		for(int i = 0; i < fileList.size(); i++)
		{
			databases.append(fileList[i].baseName());
		}
		DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
		if(dataBaseTreeModel)
		{
			dataBaseTreeModel->setDatabases(databases, m_dbNameList);
			onDatabasesCheckStateChanged();
		}
	}
	ui.usernameAndPasswordGroupBox->setChecked(osqlSettingsReader->getLastBackupUseUsername());

	ui.sourceTypeCB->setCurrentIndex(1);

	/*ui.directoryL->setText(tr("Каталог:"));
	ui.sourceTypeL->setText(tr("Тип источника данных:"));
	ui.sourceTypeCB->clear();
	ui.sourceTypeCB->addItem(tr("Локальные базы данных"));
	ui.sourceTypeCB->addItem(tr("Удаленные базы данных"));
	ui.usernameAndPasswordGroupBox->setTitle(tr("Использовать пользователя базы данных"));
	ui.usernameL->setText(tr("Имя пользователя:"));
	ui.passwordL->setText(tr("Пароль:"));
	ui.databasesL->setText(tr("Базы данных:"));*/
}

RestoreSettingWizardPage::~RestoreSettingWizardPage()
{

}

bool RestoreSettingWizardPage::isComplete() const
{
	if(ui.directoryLE->text().isEmpty())
	{
		return false;
	}
	DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
	if(dataBaseTreeModel && dataBaseTreeModel->getCheckedDatabases().isEmpty())
	{
		return false;
	}
	if(!ui.usernameAndPasswordGroupBox->isChecked())
	{
		return true;
	}
	return !ui.usernameLE->text().trimmed().isEmpty() && !ui.passwordLE->text().trimmed().isEmpty();
}

QString RestoreSettingWizardPage::getDirectory() const
{
	return ui.directoryLE->text();
}

QString RestoreSettingWizardPage::getUsername() const
{
	return ui.usernameLE->text();
}

QString RestoreSettingWizardPage::getPassword() const
{
	return ui.passwordLE->text();
}

bool RestoreSettingWizardPage::isLocalDataBase() const
{
	return ui.sourceTypeCB->currentIndex() == 0;
}

QStringList RestoreSettingWizardPage::getDBNameList() const
{
	QStringList result;
	DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
	if(dataBaseTreeModel)
	{
		result = dataBaseTreeModel->getCheckedDatabases();
	}
	return result;
}

void RestoreSettingWizardPage::directoryTB_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose directory for restore"), ui.directoryLE->text());
	if(!dir.isEmpty())
	{
		ui.directoryLE->setText(dir);
		emit completeChanged();
		QDir directory(dir);
		QStringList nameFilters;
		nameFilters << "*.bak";
		QFileInfoList fileList = directory.entryInfoList(nameFilters, QDir::Files, QDir::Name);
		QStringList databases;
		for(int i = 0; i < fileList.size(); i++)
		{
			databases.append(fileList[i].baseName());
		}
		DataBaseTreeModel* dataBaseTreeModel = dynamic_cast<DataBaseTreeModel*>(ui.databasesTV->model());
		if(dataBaseTreeModel)
		{
			dataBaseTreeModel->setDatabases(databases, m_dbNameList);
			onDatabasesCheckStateChanged();
		}
	}
}

void RestoreSettingWizardPage::usernameAndPasswordGroupBox_clicked()
{
	emit completeChanged();
}

void RestoreSettingWizardPage::usernameLE_textChanged(QString /*text*/)
{
	emit completeChanged();
}

void RestoreSettingWizardPage::passwordLE_textChanged(QString /*text*/)
{
	emit completeChanged();
}

void RestoreSettingWizardPage::onDatabasesCheckStateChanged()
{
	emit completeChanged();
}
