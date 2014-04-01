#ifndef QSPARKLE_H
#define QSPARKLE_H

#include "qsparkleitem.h"
#include "qsparkleitemversion.h"
#include "qsparklereleasewindow.h"
#include "qsparkledownloaddialog.h"

#include <QCoreApplication>
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
		QList<QsparkleItem*> _items;
		QsparkleItem *_largestItem;
		QNetworkAccessManager *_networkManager;
		QTimer *_timer;

		QsparkleReleaseWindow *_releaseWindow;
		QsparkleDownloadDialog *_downloadDialog;

		QString _url;
		semver::version _currentVersion;

		void _clearItems();
		QString _readXmlElement(QXmlStreamReader *xml);

	private slots:
		void _onTimer();
		void _onDownloadComplete(QNetworkReply *reply);
		void _onNewVersion(semver::version newVersion);
		void _onNoNewVersion(semver::version latestVersion);

		void _onInstall(QString version);
		void _onSkip(QString version);
		void _onRemindLater(QString version);

	signals:
		void newVersion(semver::version newVersion);
		void noNewVersion(semver::version latestVersion);
};

#endif // QSPARKLE_H
