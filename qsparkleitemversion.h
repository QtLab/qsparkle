#ifndef QSPARKLEITEMVERSION_H
#define QSPARKLEITEMVERSION_H

#include "lib/semver/include/semver/version.hpp"

#include <QObject>
#include <QDebug>
#include <QUrl>

class QsparkleItemVersion : public QObject
{
		Q_OBJECT
	public:
		QsparkleItemVersion(QUrl url, QString version, QString platform, qint64 length, QString type, QObject *parent = 0);

		bool isGreaterThan(semver::version version);
		QString platform();
		QUrl url();
		semver::version version();

	private:
		QUrl _url;
		semver::version _version;
		QString _platform;
		qint64 _length;
		QString _type;
};

#endif // QSPARKLEITEMVERSION_H
