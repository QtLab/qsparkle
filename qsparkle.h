#ifndef QSPARKLE_H
#define QSPARKLE_H

#include <QObject>
#include <QDebug>

#include <QTimer>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QXmlStreamReader>

class Qsparkle : public QObject
{
		Q_OBJECT
	public:
		Qsparkle(QString appcastUrl, QObject *parent = 0);

		void check();
		void checkPeriodic(int seconds);
		void stopPeriodic();

	private:
		QString _url;
		QNetworkAccessManager *_networkManager;
		QTimer *_timer;

	private slots:
		void _onTimer();
		void _onDownloadComplete(QNetworkReply *reply);

};

#endif // QSPARKLE_H
