#ifndef BACKUPSETTINGWIZARDPAGE_H
#define BACKUPSETTINGWIZARDPAGE_H

#include <QtWidgets/QWizardPage>

#include "ui_backupsettingwizardpage.h"

class OsqlSettingsReader;

class BackupSettingWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	BackupSettingWizardPage(OsqlSettingsReader* osqlSettingsReader, QWidget* parent = 0);
	~BackupSettingWizardPage();

	virtual bool isComplete() const;

	QString getDirectory() const;
	QString getUsername() const;
	QString getPassword() const;
	bool isLocalDataBase() const;

protected slots:
	void directoryTB_clicked();
	void usernameAndPasswordGroupBox_clicked();
	void usernameLE_textChanged(QString /*text*/);
	void passwordLE_textChanged(QString /*text*/);

private:
	Ui::BackupSettingWizardPage ui;
};

#endif // BACKUPSETTINGWIZARDPAGE_H