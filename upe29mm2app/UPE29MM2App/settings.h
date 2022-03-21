#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QSettings>

#include <QtCore/QFileInfo>

/**
* ���������
*/
class Settings : public QSettings
{
	static QString synchButtonGroup;
	static QString backupButtonGroup;
	static QString restoreButtonGroup;
	static QString startButtonGroup;
	static QString exitButtonGroup;

	static QString showKey;
	static QString applicationPathKey;
	static QString originalPictureFilePathKey;
	static QString pressedPictureFilePathKey;

	Q_OBJECT
public:

	enum ButtonType
	{
		SYNCH_BUTTON = 0,	///< �������������.
		BACKUP_BUTTON,		///< ������� ��������� �����.
		RESTORE_BUTTON,		///< ������������ �� ��������� �����..
		START_BUTTON,		///< ��������� ��������� ������ � ��.
		EXIT_BUTTON			///< �����.
	};	///< ��� ������

	/**
	* �����������
	* @param fileInfo ���� � ����� ��������
	* @param parent ������������ ������
	*/
	Settings(const QFileInfo& fileInfo, QObject* parent = 0);

	/**
	* ����������
	*/
	~Settings();

	/**
	* �������� ������� ������ ������
	* @param buttonType ��� ������
	* @return =TRUE - �������� ������
	*/
	bool isShow(ButtonType buttonType);

	/**
	* �������� ���� � ����� ���������
	* @param buttonType ��� ������
	* @return ���� � ����� ���������
	*/
	QString getApplicationPath(ButtonType buttonType);

	/**
	* �������� �������� ������
	* @param buttonType ��� ������
	* @return �������� ������
	*/
	QString getOriginalPictureFilePath(ButtonType buttonType);

	/**
	* �������� �������� ������ (�������)
	* @param buttonType ��� ������
	* @return �������� ������ (�������)
	*/
	QString getPressedPictureFilePath(ButtonType buttonType);

private:
	/**
	* ������� ��������� �� ���������
	*/
	void setDefaultValues();

	/**
	* ������� �������� ��������
	* @param buttonType ��� ������
	* @param show ������� ������ ������
	* @param applicationPath ���� � ����� ���������
	* @param originalPictureFilePath �������� ������
	* @param pressedPictureFilePath �������� ������ (�������)
	*/
	void setValues(ButtonType buttonType, bool show, const QString& applicationPath, const QString& originalPictureFilePath,
		const QString& pressedPictureFilePath);

	QMap<ButtonType, QString> m_typeMap;	///< Map ���� �� ���� ������
};

#endif // SETTINGS_H
