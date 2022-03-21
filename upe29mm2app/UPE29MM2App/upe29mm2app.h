#ifndef UPE29MM2APP_H
#define UPE29MM2APP_H

#include <QtWidgets/QWidget>
#include "ui_upe29mm2app.h"

#include"settings.h"

/**
* Приложение.
*/
class UPE29MM2App : public QWidget
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param parent родительский объект
	*/
	UPE29MM2App(QWidget* parent = 0);

	/**
	* Деструктор
	*/
	~UPE29MM2App();

protected:
	/**
	* Показать окно
	* @param event
	*/
	virtual void showEvent(QShowEvent* event);

private:
	/**
	* Настроить область под кнопку
	* @param label область
	* @param buttonType тип кнопки
	*/
	void setupButtonLabel(ButtonLabel* label, Settings::ButtonType buttonType);

	Ui::UPE29MM2AppClass ui;

	Settings* m_settings;	///< Настроика
	int m_lowHeight;
};

#endif // UPE29MM2APP_H
