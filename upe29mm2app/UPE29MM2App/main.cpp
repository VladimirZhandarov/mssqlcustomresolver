#include "upe29mm2app.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UPE29MM2App w;
	w.show();
	return a.exec();
}
