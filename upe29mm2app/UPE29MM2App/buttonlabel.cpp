#include "buttonlabel.h"

ButtonLabel::ButtonLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent, f), m_workOnPress(true)
{
	connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

ButtonLabel::~ButtonLabel()
{

}

void ButtonLabel::setOriginalPicture(const QString& picture)
{
	m_originalPixmap = QPixmap(picture);
	setPixmap(m_originalPixmap);
}

void ButtonLabel::setPressedPicture(const QString& picture)
{
	m_pressedPixmap = QPixmap(picture);
}

void ButtonLabel::setApplicationPath(const QString& path)
{
	m_applicationPath = path;
}

void ButtonLabel::mousePressEvent(QMouseEvent* event)
{
	setPixmap(m_pressedPixmap);
	if(m_workOnPress)
	{
		emit clicked();
	}
}

void ButtonLabel::mouseReleaseEvent(QMouseEvent* event)
{
	setPixmap(m_originalPixmap);
	if(!m_workOnPress)
	{
		emit clicked();
	}
}

void ButtonLabel::onClicked()
{
	if(!m_applicationPath.isEmpty() && m_appProcess.state() == QProcess::NotRunning)
	{
		m_appProcess.start(m_applicationPath);
	}
}
