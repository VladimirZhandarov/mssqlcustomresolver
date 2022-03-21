#ifndef RESTORESETTINGWIZARDPAGE_H
#define RESTORESETTINGWIZARDPAGE_H

#include <QtWidgets/QWizardPage>

#include "ui_restoresettingwizardpage.h"

class OsqlSettingsReader;

class RestoreSettingWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	RestoreSettingWizardPage(OsqlSettingsReader* osqlSettingsReader, QWidget* parent = 0);
	~RestoreSettingWizardPage();

	virtual bool isComplete() const;

	QString getDirectory() const;
	QString getUsername() const;
	QString getPassword() const;
	bool isLocalDataBase() const;
	QStringList getDBNameList() const;

protected slots:
	void directoryTB_clicked();
	void usernameAndPasswordGroupBox_clicked();
	void usernameLE_textChanged(QString /*text*/);
	void passwordLE_textChanged(QString /*text*/);
	void onDatabasesCheckStateChanged();

private:
	Ui::RestoreSettingWizardPage ui;

	QStringList m_dbNameList;
};

#endif // RESTORESETTINGWIZARDPAGE_H