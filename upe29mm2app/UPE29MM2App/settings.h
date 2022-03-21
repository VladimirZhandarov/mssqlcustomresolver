#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QSettings>

#include <QtCore/QFileInfo>

/**
* Настроика
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
		SYNCH_BUTTON = 0,	///< Синхронизация.
		BACKUP_BUTTON,		///< Создать резервную копию.
		RESTORE_BUTTON,		///< Восстановить из резервной копии..
		START_BUTTON,		///< Запустить программу работы с БД.
		EXIT_BUTTON			///< Выход.
	};	///< Тип кнопки

	/**
	* Конструктор
	* @param fileInfo путь к файлу настроек
	* @param parent родительский объект
	*/
	Settings(const QFileInfo& fileInfo, QObject* parent = 0);

	/**
	* Деструктор
	*/
	~Settings();

	/**
	* Получить признак показа кнопки
	* @param buttonType тип кнопки
	* @return =TRUE - показать кнопку
	*/
	bool isShow(ButtonType buttonType);

	/**
	* Получить путь к файлу программы
	* @param buttonType тип кнопки
	* @return путь к файлу программы
	*/
	QString getApplicationPath(ButtonType buttonType);

	/**
	* Получить картинку кнопки
	* @param buttonType тип кнопки
	* @return картинка кнопки
	*/
	QString getOriginalPictureFilePath(ButtonType buttonType);

	/**
	* Получить картинку кнопки (нажатая)
	* @param buttonType тип кнопки
	* @return картинка кнопки (нажатая)
	*/
	QString getPressedPictureFilePath(ButtonType buttonType);

private:
	/**
	* Создать настройки по умолчанию
	*/
	void setDefaultValues();

	/**
	* Указать значения настроек
	* @param buttonType тип кнопки
	* @param show признак показа кнопки
	* @param applicationPath путь к файлу программы
	* @param originalPictureFilePath картинка кнопки
	* @param pressedPictureFilePath картинка кнопки (нажатая)
	*/
	void setValues(ButtonType buttonType, bool show, const QString& applicationPath, const QString& originalPictureFilePath,
		const QString& pressedPictureFilePath);

	QMap<ButtonType, QString> m_typeMap;	///< Map тэга по типу конпки
};

#endif // SETTINGS_H
