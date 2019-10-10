#include "rtc_reactor.h"
#include <QtWidgets/QApplication>

int xxmain(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RTCReactor w;
	w.show();
	return a.exec();
}
