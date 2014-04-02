#include "qsparkledownloaddialog.h"
#include "ui_qsparkledownloaddialog.h"

QsparkleDownloadDialog::QsparkleDownloadDialog(QUrl url, QWidget *parent): QWidget(parent), _ui(new Ui::QsparkleDownloadDialog)
{
	this->_ui->setupUi(this);
	connect(this->_ui->installButton, SIGNAL(clicked()), this, SLOT(_onInstall()));

	this->setWindowTitle(tr("Downloading %1...").arg(qApp->applicationName()));

	this->_downloadManager = new QNetworkAccessManager(this);
	connect(this->_downloadManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(_onDownloadFinished(QNetworkReply*)));
	connect(this->_downloadManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(_onSslErrors(QNetworkReply*,QList<QSslError>)));

	QUrl downloadUrl(url);
	QNetworkRequest request;

	request.setUrl(downloadUrl);
	this->_reply = this->_downloadManager->get(request);

	connect(this->_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(_onDownloadProgress(qint64,qint64)));

	qDebug() << "Starting to download" << url;
}

void QsparkleDownloadDialog::_onDownloadProgress(qint64 received, qint64 total)
{
	double progress = static_cast<double>(received) / total * 100;

	qDebug() << "Downloading file ..." << qRound(progress) << "%" << progress;

	this->_ui->progressBar->setValue(qRound(progress));
}

void QsparkleDownloadDialog::_onDownloadFinished(QNetworkReply *reply)
{
	qDebug() << "Download finished";

	if (reply->error() == QNetworkReply::NoError) {
		QString filename = QDir::toNativeSeparators(QDir::tempPath() + "/" + qApp->applicationName() + "_" + QString::number(QDateTime::currentDateTime().toTime_t()));
		this->_file = new QFile(filename);

		qDebug() << "Save file to" << filename;

		this->_ui->progressBar->setValue(100);

		if (this->_file->open(QFile::Append)) {
			this->_file->write(reply->readAll());
			this->_file->flush();
			this->_file->close();
		}

		this->_ui->installButton->setEnabled(true);
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

void QsparkleDownloadDialog::_onInstall()
{
	qDebug() << "Installing file" << this->_file->fileName();

	emit install(this->_file->fileName());
}

QsparkleDownloadDialog::~QsparkleDownloadDialog()
{
	delete this->_ui;
}
