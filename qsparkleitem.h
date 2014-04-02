#ifndef QSPARKLEITEM_H
#define QSPARKLEITEM_H

#include "qsparkleitemversion.h"

#include <QObject>
#include <QDebug>
#include <QDateTime>

class QsparkleItem : public QObject
{
		Q_OBJECT
	public:
		QsparkleItem(QString title, QString published, QString link, QList<QsparkleItemVersion*> versions, QObject *parent = 0);
		~QsparkleItem();

		QsparkleItemVersion *versionForOs();
		QString link();

	private:
		QString _title;
		QDateTime _published;
		QString _link;
		QList<QsparkleItemVersion*> _versions;

	signals:

	public slots:

};

#endif // QSPARKLEITEM_H
