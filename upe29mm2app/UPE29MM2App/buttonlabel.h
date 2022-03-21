#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QtWidgets/QLabel>

#include <QtCore/QProcess>

/**
* Объект-кнопка с картинкой и привязкой к запускаемому файлу
*/
class ButtonLabel : public QLabel
{
	Q_OBJECT
public:
	/**
	* Конструктор
	* @param parent родительский объект
	* @param f флаги
	*/
	ButtonLabel(QWidget * parent = 0, Qt::WindowFlags f = 0);

	/**
	* Деструктор
	*/
	~ButtonLabel();

	/**
	* Установить картинку кнопки.
	* @param picture картинка.
	*/
	void setOriginalPicture(const QString& picture);

	/**
	* Установить картинку кнопки (нажатую).
	* @param picture картинка.
	*/
	void setPressedPicture(const QString& picture);

	/**
	* Установить путь к запускаемому файлу.
	* @param path путь к запускаемому файлу.
	*/
	void setApplicationPath(const QString& path);

protected:
	/**
	* Кнопка мыши нажата.
	* @param event.
	*/
	virtual void mousePressEvent(QMouseEvent* event);

	/**
	* Кнопка мыши отпущена.
	* @param event.
	*/
	virtual void mouseReleaseEvent(QMouseEvent* event);

protected slots:
	/**
	* Кнопка нажата.
	*/
	void onClicked();

signals:
	/**
	* Кнопка нажата.
	*/
	void clicked();

private:
	QPixmap m_originalPixmap;	///< Картинка кнопки.
	QPixmap m_pressedPixmap;	///< Картинка кнопки (нажатая).
	bool m_workOnPress;			
	QString m_applicationPath;	///< Путь к запускаемому файлу.
	QProcess m_appProcess;		///< Процесс.
};

#endif // BUTTONLABEL_H
