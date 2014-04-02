#include "qsparkleitemversion.h"

QsparkleItemVersion::QsparkleItemVersion(QUrl url, QString version, QString platform, qint64 length, QString type, QObject *parent): QObject(parent), _version("")
{
	semver::version semver(version.toStdString());

	qDebug() << "Create new version" << url << semver.getVersion().c_str();

	this->_url = url;
	this->_version = semver;
	this->_platform = platform;
	this->_length = length;
	this->_type = type;

	if (!this->_version.isValid()) {
		qWarning("Version %s is invalid!", version.toUtf8().constData());
	}
}

bool QsparkleItemVersion::isGreaterThan(semver::version version)
{
	return (this->_version > version);
}

QString QsparkleItemVersion::platform()
{
	return this->_platform;
}

QUrl QsparkleItemVersion::url()
{
	return this->_url;
}

semver::version QsparkleItemVersion::version()
{
	return this->_version;
}

QsparkleItemVersion::~QsparkleItemVersion()
{

}
