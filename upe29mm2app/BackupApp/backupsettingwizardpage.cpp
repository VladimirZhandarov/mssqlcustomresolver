#include "backupsettingwizardpage.h"

#include <QtWidgets/QFileDialog>

#include <QtCore/QDir>

#include <osqlsettingsreader.h>

BackupSettingWizardPage::BackupSettingWizardPage(OsqlSettingsReader* osqlSettingsReader, QWidget* parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	Q_ASSERT(osqlSettingsReader);
	QString lastBackupDir = osqlSettingsReader->getLastBackupDir();
	if(QDir(lastBackupDir).exists())
	{
		ui.directoryLE->setText(lastBackupDir);
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
	ui.passwordL->setText(tr("Пароль:"));*/
}

BackupSettingWizardPage::~BackupSettingWizardPage()
{

}

bool BackupSettingWizardPage::isComplete() const
{
	if(ui.directoryLE->text().isEmpty())
	{
		return false;
	}
	if(!ui.usernameAndPasswordGroupBox->isChecked())
	{
		return true;
	}
	return !ui.usernameLE->text().trimmed().isEmpty() && !ui.passwordLE->text().trimmed().isEmpty();
}

QString BackupSettingWizardPage::getDirectory() const
{
	return ui.directoryLE->text();
}

QString BackupSettingWizardPage::getUsername() const
{
	return ui.usernameLE->text();
}

QString BackupSettingWizardPage::getPassword() const
{
	return ui.passwordLE->text();
}

bool BackupSettingWizardPage::isLocalDataBase() const
{
	return ui.sourceTypeCB->currentIndex() == 0;
}

void BackupSettingWizardPage::directoryTB_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose directory for backup"), ui.directoryLE->text());
	if(!dir.isEmpty())
	{
		ui.directoryLE->setText(dir);
		emit completeChanged();
	}
}

void BackupSettingWizardPage::usernameAndPasswordGroupBox_clicked()
{
	emit completeChanged();
}

void BackupSettingWizardPage::usernameLE_textChanged(QString /*text*/)
{
	emit completeChanged();
}

void BackupSettingWizardPage::passwordLE_textChanged(QString /*text*/)
{
	emit completeChanged();
}
