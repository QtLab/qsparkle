#include "qsparkle.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setApplicationName("QSparkle");
	QCoreApplication::setApplicationVersion("1.9.0");
	QCoreApplication::setOrganizationDomain("https://github.com/thelfensdrfer/qsparkle");
	QCoreApplication::setOrganizationName("thelfensdrfer");

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	QTranslator myappTranslator;
	myappTranslator.load("myapp_" + QLocale::system().name());
	app.installTranslator(&myappTranslator);

	app.setQuitOnLastWindowClosed(false);

	Qsparkle *sparkle = new Qsparkle("http://127.0.0.1/appcast.xml");
	sparkle->check();

	return app.exec();
}

