#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QtWidgets/QLabel>

#include <QtCore/QProcess>

/**
* ������-������ � ��������� � ��������� � ������������ �����
*/
class ButtonLabel : public QLabel
{
	Q_OBJECT
public:
	/**
	* �����������
	* @param parent ������������ ������
	* @param f �����
	*/
	ButtonLabel(QWidget * parent = 0, Qt::WindowFlags f = 0);

	/**
	* ����������
	*/
	~ButtonLabel();

	/**
	* ���������� �������� ������.
	* @param picture ��������.
	*/
	void setOriginalPicture(const QString& picture);

	/**
	* ���������� �������� ������ (�������).
	* @param picture ��������.
	*/
	void setPressedPicture(const QString& picture);

	/**
	* ���������� ���� � ������������ �����.
	* @param path ���� � ������������ �����.
	*/
	void setApplicationPath(const QString& path);

protected:
	/**
	* ������ ���� ������.
	* @param event.
	*/
	virtual void mousePressEvent(QMouseEvent* event);

	/**
	* ������ ���� ��������.
	* @param event.
	*/
	virtual void mouseReleaseEvent(QMouseEvent* event);

protected slots:
	/**
	* ������ ������.
	*/
	void onClicked();

signals:
	/**
	* ������ ������.
	*/
	void clicked();

private:
	QPixmap m_originalPixmap;	///< �������� ������.
	QPixmap m_pressedPixmap;	///< �������� ������ (�������).
	bool m_workOnPress;			
	QString m_applicationPath;	///< ���� � ������������ �����.
	QProcess m_appProcess;		///< �������.
};

#endif // BUTTONLABEL_H
