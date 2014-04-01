#include "qsparkle.h"

Qsparkle::Qsparkle(QString appcastUrl, QObject *parent): QObject(parent), _currentVersion("")
{
	semver::version semver(QCoreApplication::applicationVersion().toStdString());

	this->_url = appcastUrl;

	this->_timer = new QTimer(this);
	this->_networkManager = new QNetworkAccessManager(this);
	this->_currentVersion = semver;
	this->_largestItem = NULL;
	this->_releaseWindow = NULL;

	connect(this->_timer, SIGNAL(timeout()), this, SLOT(_onTimer()));
	connect(this->_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(_onDownloadComplete(QNetworkReply*)));

	connect(this, SIGNAL(newVersion(semver::version)), this, SLOT(_onNewVersion(semver::version)));
	connect(this, SIGNAL(noNewVersion(semver::version)), this, SLOT(_onNoNewVersion(semver::version)));
}

void Qsparkle::check()
{
	qDebug() << "Checking for new version...";

	QUrl url(this->_url);
	QNetworkRequest request;

	request.setUrl(url);
	this->_networkManager->get(request);
}

void Qsparkle::checkPeriodic(int seconds)
{
	this->_timer->start(seconds);
}

void Qsparkle::stopPeriodic()
{
	this->_timer->stop();
}

void Qsparkle::_clearItems()
{
	for (int i = 0; i < this->_items.count(); i++) {
		delete this->_items.at(i);
	}

	this->_items.clear();
}

QString Qsparkle::_readXmlElement(QXmlStreamReader *xml)
{
	if (xml->tokenType() != QXmlStreamReader::StartElement) {
		qDebug() << "No start element" << xml->name() << xml->tokenString();
		return "";
	}

	xml->readNext();

	if (xml->tokenType() != QXmlStreamReader::Characters) {
		qDebug() << "No characters";
		return "";
	}

	return xml->text().toString();
}

void Qsparkle::_onDownloadComplete(QNetworkReply *reply)
{
	this->_clearItems();

	if (reply->error() == QNetworkReply::NoError) {
		qDebug() << "Got response from server.";

		QByteArray response = reply->readAll();
		QXmlStreamReader xml;

		xml.addData(response);

		// Parse appcast
		while (!xml.atEnd()) {
			QXmlStreamReader::TokenType token = xml.readNext();

			if (token == QXmlStreamReader::StartDocument) {
				continue;
			}

			if (token == QXmlStreamReader::StartElement) {

				// Parse items
				if (xml.name() == "item") {
					QString title;
					QString pubDate;
					QString link;
					QList<QsparkleItemVersion*> versions;

					// Parse item elements
					while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "item")) {
						if (xml.tokenType() != QXmlStreamReader::StartElement) {
							xml.readNext();
							continue;
						}

						QString name = xml.name().toString();

						if (name == "title") {
							title = this->_readXmlElement(&xml);
						}

						if (name == "pubDate") {
							pubDate = this->_readXmlElement(&xml);
						}

						if (name == "guid") {
							link = this->_readXmlElement(&xml);
						}

						// Parse item versions
						if (name == "enclosure") {
							QString url;
							QString version;
							QString platform;
							qint64 length;
							QString type;

							url = xml.attributes().value("url").toString();
							version = xml.attributes().value("version").toString();
							platform = xml.attributes().value("platform").toString();
							length = xml.attributes().value("length").toLongLong();
							type = xml.attributes().value("type").toString();

							QsparkleItemVersion *versionItem = new QsparkleItemVersion(url, version, platform, length, type, this);
							versions.append(versionItem);
						}

						xml.readNext();
					}

					QsparkleItem *item = new QsparkleItem(title, pubDate, link, versions, this);
					this->_items.append(item);

					// Set new largest version
					if (this->_largestItem == NULL) {
						qDebug() << "Set first item to largest version";

						this->_largestItem = item;
					} else {
						qDebug() << "Check for larger versions";

						if (item->versionForOs() != NULL && this->_largestItem->versionForOs() != NULL && item->versionForOs()->isGreaterThan(this->_largestItem->versionForOs()->version())) {
							qDebug() << "Set version" << QString::fromUtf8(item->versionForOs()->version().getVersion().c_str()) << "to new largest version";

							this->_largestItem = item;
						}
					}
				}
			}
		}

		qDebug() << "Finished parsing.";

		if (this->_largestItem != NULL) {
			if (this->_largestItem->versionForOs() != NULL) {
				qDebug() << "Largest version" << this->_largestItem->versionForOs()->version().getVersion().c_str();
				qDebug() << "Current version" << this->_currentVersion.getVersion().c_str();

				if (this->_largestItem->versionForOs()->isGreaterThan(this->_currentVersion)) {
					emit newVersion(this->_largestItem->versionForOs()->version());
				} else {
					emit noNewVersion(this->_largestItem->versionForOs()->version());
				}
			} else {
				qDebug() << "No new version for this os";
			}
		} else {
			qDebug() << "No items in appcast";
		}
	} else {
		switch (reply->error()) {
			case QNetworkReply::ConnectionRefusedError:
				qWarning("Error: Connection refused '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::RemoteHostClosedError:
				qWarning("Error: Host closed connection before the reply was received '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::HostNotFoundError:
				qWarning("Error: Host not found '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::TimeoutError:
				qWarning("Error: Timeout '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::OperationCanceledError:
				qWarning("Error: Operation canceled '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::SslHandshakeFailedError:
				qWarning("Error: SSL handshake failed '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::TemporaryNetworkFailureError:
				qWarning("Error: Temporary network failure '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::NetworkSessionFailedError:
				qWarning("Error: Network session failed '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::BackgroundRequestNotAllowedError:
				qWarning("Error: Background request is not allowed '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProxyConnectionRefusedError:
				qWarning("Error: Proxy connection refused '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProxyConnectionClosedError:
				qWarning("Error: Proxy connection closed '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProxyNotFoundError:
				qWarning("Error: Proxy not found '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProxyTimeoutError:
				qWarning("Error: Proxy timeout '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProxyAuthenticationRequiredError:
				qWarning("Error: Proxy requires authentication '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ContentAccessDenied:
				qWarning("Error: Access denied '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ContentOperationNotPermittedError:
				qWarning("Error: Content operation is not permitted '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ContentNotFoundError:
				qWarning("Error: Content not found '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::AuthenticationRequiredError:
				qWarning("Error: Authentication required '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ContentReSendError:
				qWarning("Error: Content needed to be send again but failed '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProtocolUnknownError:
				qWarning("Error: Unknown protocol '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProtocolInvalidOperationError:
				qWarning("Error: Invalid protocol '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::UnknownNetworkError:
				qWarning("Error: Unknown network error '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::UnknownProxyError:
				qWarning("Error: Unknown proxy error '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::UnknownContentError:
				qWarning("Error: Unknown content error '%s'", reply->errorString().toUtf8().constData());
				break;
			case QNetworkReply::ProtocolFailure:
				qWarning("Error: Protocol failure '%s'", reply->errorString().toUtf8().constData());
				break;
			default:
				qWarning("Error: Unknown error '%s'", reply->errorString().toUtf8().constData());
				break;
		}
	}

	reply->deleteLater();
}

void Qsparkle::_onTimer()
{
	this->check();
}

void Qsparkle::_onNewVersion(semver::version newVersion)
{
	qDebug() << "New version" << newVersion.getVersion().c_str();

	if (this->_releaseWindow == NULL) {
		this->_releaseWindow = new QsparkleReleaseWindow(this->_largestItem->link(), QString(newVersion.getVersion().c_str()));
		//this->_releaseWindow->setWindowModality(Qt::WindowModal);

		connect(this->_releaseWindow, SIGNAL(install(QString)), this, SLOT(_onInstall(QString)));
		connect(this->_releaseWindow, SIGNAL(skip(QString)), this, SLOT(_onSkip(QString)));
		connect(this->_releaseWindow, SIGNAL(remindLater(QString)), this, SLOT(_onRemindLater(QString)));

		this->_releaseWindow->show();
	} else {
		qDebug() << "Release window already open";
	}
}

void Qsparkle::_onNoNewVersion(semver::version latestVersion)
{
	qDebug() << "No version" << latestVersion.getVersion().c_str();

	this->_releaseWindow->close();
	delete this->_releaseWindow;
}

void Qsparkle::_onInstall(QString version)
{
	qDebug() << "Install new version" << version;

	this->_releaseWindow->close();
	delete this->_releaseWindow;

	this->_downloadDialog = new QsparkleDownloadDialog(this->_largestItem->versionForOs()->url());
	//this->_downloadDialog->setWindowModality(Qt::WindowModal);
	this->_downloadDialog->show();
}

void Qsparkle::_onSkip(QString version)
{
	qDebug() << "Skip version" << version;

	this->_releaseWindow->close();
	delete this->_releaseWindow;
}

void Qsparkle::_onRemindLater(QString version)
{
	qDebug() << "Remind later for version" << version;
}
