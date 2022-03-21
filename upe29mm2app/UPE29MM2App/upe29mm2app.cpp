#include "upe29mm2app.h"

#include"settings.h"

UPE29MM2App::UPE29MM2App(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);

	m_settings = new Settings(QFileInfo("upeAppSettings.ini"), this);

	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	m_lowHeight = geometry().height();
	showFullScreen();

#ifdef RUSSIAN_VERSION
	ui.synchL->setOriginalPicture(":/Resources/Button_Synchronization_original.png");
	ui.synchL->setPressedPicture(":/Resources/Button_Synchronization_pressed.png");
	ui.backupL->setOriginalPicture(":/Resources/Button_Backup_copy_original.png");
	ui.backupL->setPressedPicture(":/Resources/Button_Backup_copy_pressed.png");
	ui.restoreL->setOriginalPicture(":/Resources/Button_Restore_original.png");
	ui.restoreL->setPressedPicture(":/Resources/Button_Restore_pressed.png");
	ui.startL->setOriginalPicture(":/Resources/Button_Start_original.png");
	ui.startL->setPressedPicture(":/Resources/Button_Start_pressed.png");
	ui.exitL->setOriginalPicture(":/Resources/Button_Exit_original.png");
	ui.exitL->setPressedPicture(":/Resources/Button_Exit_pressed.png");
#else
	ui.synchL->setOriginalPicture(":/Resources/Button_Synchronization_original_ENG.png");
	ui.synchL->setPressedPicture(":/Resources/Button_Synchronization_pressed_ENG.png");
	ui.backupL->setOriginalPicture(":/Resources/Button_Backup_copy_original_ENG.png");
	ui.backupL->setPressedPicture(":/Resources/Button_Backup_copy_pressed_ENG.png");
	ui.restoreL->setOriginalPicture(":/Resources/Button_Restore_original_ENG.png");
	ui.restoreL->setPressedPicture(":/Resources/Button_Restore_pressed_ENG.png");
	ui.startL->setOriginalPicture(":/Resources/Button_Start_original_ENG.png");
	ui.startL->setPressedPicture(":/Resources/Button_Start_pressed_ENG.png");
	ui.exitL->setOriginalPicture(":/Resources/Button_Exit_original_ENG.png");
	ui.exitL->setPressedPicture(":/Resources/Button_Exit_pressed_ENG.png");

	ui.nameL->setPixmap(QPixmap(":/Resources/Name_UPE-29MM2_ENG.png"));
	ui.migIconL->setPixmap(QPixmap(":/Resources/MiG_ENG.png"));
	ui.sinventL->setPixmap(QPixmap(":/Resources/Sinvent_ENG.png"));
#endif

	connect(ui.exitL, SIGNAL(clicked()), this, SLOT(close()));

	setupButtonLabel(ui.synchL, Settings::SYNCH_BUTTON);
	setupButtonLabel(ui.backupL, Settings::BACKUP_BUTTON);
	setupButtonLabel(ui.restoreL, Settings::RESTORE_BUTTON);
	setupButtonLabel(ui.startL, Settings::START_BUTTON);
	setupButtonLabel(ui.exitL, Settings::EXIT_BUTTON);
}

UPE29MM2App::~UPE29MM2App()
{

}

void UPE29MM2App::showEvent(QShowEvent* event)
{
	QRect mainRect = geometry();
	int mainWidth = mainRect.width();
	int mainHeight = mainRect.height();

	QRect nameRect = ui.nameL->geometry();
	int nameWidth = nameRect.width();
	nameRect.setLeft((mainWidth - nameWidth) / 2);
	nameRect.setWidth(nameWidth);
	ui.nameL->setGeometry(nameRect);

	QRect migIconRect = ui.migIconL->geometry();
	int migIconHeight = migIconRect.height();
	migIconRect.setTop(mainHeight - migIconHeight - 30);
	migIconRect.setHeight(migIconHeight);
	ui.migIconL->setGeometry(migIconRect);

	QRect sinventRect = ui.sinventL->geometry();
	int sinventHeight = sinventRect.height();
	int sinventWidth = sinventRect.width();
	sinventRect.setTop(mainHeight - sinventHeight - 30);
	sinventRect.setLeft(mainWidth - sinventWidth - 30);
	sinventRect.setHeight(sinventHeight);
	sinventRect.setWidth(sinventWidth);
	ui.sinventL->setGeometry(sinventRect);

	QRect migRect = ui.migL->geometry();
	migRect.setBottom(mainHeight - 79);
	migRect.setRight(mainWidth - 53);
	ui.migL->setGeometry(migRect);

	QRect startRect = ui.startL->geometry();
	int startHeight = startRect.height();
	int startWidth = startRect.width();
	int startCenter = startRect.top() + startHeight / 2;
	startCenter = mainHeight * startCenter / m_lowHeight;
	startRect.setTop(startCenter - startHeight / 2);
	startRect.setLeft(mainWidth - startWidth - 93);
	startRect.setHeight(startHeight);
	startRect.setWidth(startWidth);
	ui.startL->setGeometry(startRect);

	QRect exitRect = ui.exitL->geometry();
	int exitHeight = exitRect.height();
	int exitWidth = exitRect.width();
	int exitCenter = exitRect.top() + exitHeight / 2;
	exitCenter = mainHeight * exitCenter / m_lowHeight;
	exitRect.setTop(exitCenter - exitHeight / 2);
	exitRect.setLeft(mainWidth - exitWidth - 93);
	exitRect.setHeight(exitHeight);
	exitRect.setWidth(exitWidth);
	ui.exitL->setGeometry(exitRect);

	QRect synchRect = ui.synchL->geometry();
	int synchHeight = synchRect.height();
	int synchWidth = synchRect.width();
	int synchCenter = synchRect.top() + synchHeight / 2;
	synchCenter = mainHeight * synchCenter / m_lowHeight;
	synchRect.setTop(synchCenter - synchHeight / 2);
	synchRect.setHeight(synchHeight);
	synchRect.setWidth(synchWidth);
	ui.synchL->setGeometry(synchRect);

	QRect backupRect = ui.backupL->geometry();
	int backupHeight = backupRect.height();
	int backupWidth = backupRect.width();
	int backupCenter = backupRect.top() + backupHeight / 2;
	backupCenter = mainHeight * backupCenter / m_lowHeight;
	backupRect.setTop(backupCenter - backupHeight / 2);
	backupRect.setHeight(backupHeight);
	backupRect.setWidth(backupWidth);
	ui.backupL->setGeometry(backupRect);

	QRect restoreRect = ui.restoreL->geometry();
	int restoreHeight = restoreRect.height();
	int restoreWidth = restoreRect.width();
	int restoreCenter = restoreRect.top() + restoreHeight / 2;
	restoreCenter = mainHeight * restoreCenter / m_lowHeight;
	restoreRect.setTop(restoreCenter - restoreHeight / 2);
	restoreRect.setHeight(restoreHeight);
	restoreRect.setWidth(restoreWidth);
	ui.restoreL->setGeometry(restoreRect);

	QWidget::showEvent(event);
}

void UPE29MM2App::setupButtonLabel(ButtonLabel* label, Settings::ButtonType buttonType)
{
	Q_ASSERT(label);
	Q_ASSERT(m_settings);
	if(!m_settings->isShow(buttonType))
	{
		label->hide();
	}
	else
	{
		QString originalPicture = m_settings->getOriginalPictureFilePath(buttonType);
		if(!originalPicture.isEmpty())
		{
			label->setOriginalPicture(originalPicture);
		}
		QString pressedPicture = m_settings->getPressedPictureFilePath(buttonType);
		if(!pressedPicture.isEmpty())
		{
			label->setPressedPicture(pressedPicture);
		}
		label->setApplicationPath(m_settings->getApplicationPath(buttonType));
	}
}
