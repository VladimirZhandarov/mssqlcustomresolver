#include "sinventcustomresolverclientapp.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SinventCustomResolverClientApp win;
	win.show();
	return a.exec();
}
