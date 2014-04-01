#include "qsparkle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Qsparkle *sparkle = new Qsparkle("http://127.0.0.1/appcast.xml");
	sparkle->check();

	return a.exec();
}

