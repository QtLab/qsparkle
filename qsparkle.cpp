#include "qsparkle.h"

Qsparkle::Qsparkle(QString appcastUrl, QObject *parent): QObject(parent)
{
	this->_url = appcastUrl;

	this->_timer = new QTimer(this);
	this->_networkManager = new QNetworkAccessManager(this);

	connect(this->_timer, SIGNAL(timeout()), this, SLOT(_onTimer()));
	connect(this->_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(_onDownloadComplete(QNetworkReply*)));
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

void Qsparkle::_onDownloadComplete(QNetworkReply *reply)
{
	if (reply->error() == QNetworkReply::NoError) {
		qDebug() << "Got response from server.";

		QByteArray response = reply->readAll();
		QXmlStreamReader xml;
		xml.addData(response);

		while (!xml.atEnd()) {
			QXmlStreamReader::TokenType token = xml.readNext();

			if (token == QXmlStreamReader::StartDocument) {
				continue;
			}

			if (token == QXmlStreamReader::StartElement) {
				if (xml.name() == "item") {
					qDebug() << "item";
				}
		   }
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
