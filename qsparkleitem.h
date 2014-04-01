#ifndef QSPARKLEITEM_H
#define QSPARKLEITEM_H

#include <QObject>

class QsparkleItem : public QObject
{
		Q_OBJECT
	public:
		QsparkleItem(QString title, QDateTime published, QString link, QObject *parent = 0);

	signals:

	public slots:

};

#endif // QSPARKLEITEM_H
