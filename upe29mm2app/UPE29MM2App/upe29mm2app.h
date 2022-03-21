#ifndef UPE29MM2APP_H
#define UPE29MM2APP_H

#include <QtWidgets/QWidget>
#include "ui_upe29mm2app.h"

#include"settings.h"

/**
* ����������.
*/
class UPE29MM2App : public QWidget
{
	Q_OBJECT
public:
	/**
	* �����������
	* @param parent ������������ ������
	*/
	UPE29MM2App(QWidget* parent = 0);

	/**
	* ����������
	*/
	~UPE29MM2App();

protected:
	/**
	* �������� ����
	* @param event
	*/
	virtual void showEvent(QShowEvent* event);

private:
	/**
	* ��������� ������� ��� ������
	* @param label �������
	* @param buttonType ��� ������
	*/
	void setupButtonLabel(ButtonLabel* label, Settings::ButtonType buttonType);

	Ui::UPE29MM2AppClass ui;

	Settings* m_settings;	///< ���������
	int m_lowHeight;
};

#endif // UPE29MM2APP_H
