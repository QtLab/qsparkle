#include "qsparkleitem.h"

QsparkleItem::QsparkleItem(QString title, QString published, QString link, QList<QsparkleItemVersion*> versions, QObject *parent): QObject(parent)
{
	qDebug() << "Create new item" << title;

	this->_title = title;
	this->_published = QDateTime::fromString(published, Qt::RFC2822Date);
	this->_link = link;

	this->_versions = versions;
}

QsparkleItemVersion *QsparkleItem::versionForOs()
{
	for (int i = 0; i < this->_versions.count(); i++) {
		#ifdef Q_OS_WIN
			if (this->_versions.at(i)->platform() == "Q_OS_WIN")
				return this->_versions.at(i);
		#endif

		#ifdef Q_OS_MAC
			if (this->_versions.at(i)->platform() == "Q_OS_MAC") {
				return this->_versions.at(i);
			}
		#endif

		#ifdef Q_OS_LINUX
			if (this->_versions.at(i)->platform() == "Q_OS_LINUX")
				return this->_versions.at(i);
		#endif
	}

	return NULL;
}

QString QsparkleItem::link()
{
	return this->_link;
}
