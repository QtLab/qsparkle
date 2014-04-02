#ifndef QSPARKLEDOWNLOADDIALOG_H
#define QSPARKLEDOWNLOADDIALOG_H

#include <QWidget>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <QDir>
#include <QFile>
#include <QDateTime>

namespace Ui {
	class QsparkleDownloadDialog;
}

class QsparkleDownloadDialog : public QWidget
{
	Q_OBJECT

	public:
		explicit QsparkleDownloadDialog(QUrl url, QWidget *parent = 0);
		~QsparkleDownloadDialog();

	private:
		Ui::QsparkleDownloadDialog *_ui;

		QNetworkAccessManager *_downloadManager;
		QNetworkReply *_reply;
		QFile *_file;

	private slots:
		void _onDownloadProgress(qint64 received, qint64 total);
		void _onDownloadFinished(QNetworkReply *reply);
		void _onSslErrors(QNetworkReply *reply, QList<QSslError> sslErrors);
		void _onInstall();

	signals:
		void install(QString filename);
};

#endif // QSPARKLEDOWNLOADDIALOG_H
