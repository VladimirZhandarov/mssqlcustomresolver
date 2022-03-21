#include <QtWidgets/QApplication>

#include "backupapp.h"

#include <QtCore/QLibraryInfo> 
#include <QtCore/QTranslator> 

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	/*QString translationsPath(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	QLocale locale = QLocale::system();
	QTranslator qtTranslator;
	if(qtTranslator.load(locale, "qt", "_", translationsPath))
	{
		a.installTranslator(&qtTranslator);
	}
	QTranslator qtBaseTranslator;
	if(qtBaseTranslator.load(locale, "qtbase", "_", translationsPath))
	{
		a.installTranslator(&qtBaseTranslator);
	}*/
	BackupApp backupApp;
	return a.exec();
}
