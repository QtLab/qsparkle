#include "qsparkledownloaddialog.h"
#include "ui_qsparkledownloaddialog.h"

QsparkleDownloadDialog::QsparkleDownloadDialog(QUrl url, QWidget *parent): QWidget(parent), _ui(new Ui::QsparkleDownloadDialog)
{
	this->_ui->setupUi(this);
	this->_downloadManager = new QNetworkAccessManager(this);

	connect(this->_downloadManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(_onDownloadFinished(QNetworkReply*)));
	connect(this->_downloadManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(_onSslErrors(QNetworkReply*,QList<QSslError>)));

	QUrl downloadUrl(url);
	QNetworkRequest request;

	request.setUrl(downloadUrl);
	this->_reply = this->_downloadManager->get(request);

	connect(this->_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(_onDownloadProgress()));

	qDebug() << "Starting to download" << url;
}

void QsparkleDownloadDialog::_onDownloadProgress(qint64 received, qint64 total)
{
	double progress = total/received * 100;

	qDebug() << "Downloading file ..." << qRound(progress) << "%";

	this->_ui->progressBar->setValue(qRound(progress));
}

void QsparkleDownloadDialog::_onDownloadFinished(QNetworkReply *reply)
{
	qDebug() << "Download finished";

	if (reply->error() != QNetworkReply::NoError) {
		QFile *file = new QFile("update.zip");

		if (file->open(QFile::Append)) {
			file->write(reply->readAll());
			file->flush();
			file->close();
		}

		delete file;
	} else {
		qWarning("Error downloading file (%d): %s", reply->error(), reply->errorString().toUtf8().constData());
	}

	reply->deleteLater();
}

void QsparkleDownloadDialog::_onSslErrors(QNetworkReply *reply, QList<QSslError> sslErrors)
{
	for (int i = 0; i < sslErrors.count(); i++) {
		qWarning("SSL error: %s", sslErrors.at(i).errorString().toUtf8().constData());
	}

	reply->ignoreSslErrors();
}

QsparkleDownloadDialog::~QsparkleDownloadDialog()
{
	delete this->_ui;
}
