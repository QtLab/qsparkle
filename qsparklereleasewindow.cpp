#include "qsparklereleasewindow.h"
#include "ui_qsparkledialog.h"

QsparkleReleaseWindow::QsparkleReleaseWindow(QString url, QString newVersion, QWidget *parent): QWidget(parent), _ui(new Ui::QsparkleDialog)
{
	this->_ui->setupUi(this);
	this->_version = newVersion;

	this->_ui->titleLabel->setText(tr("A new version of %1 is available!").arg(qApp->applicationName()));
	this->_ui->descriptionLabel->setText(tr("%1 %2 is now available - you have %3. Would you like to download it now?")
										 .arg(QCoreApplication::applicationName())
										 .arg(newVersion)
										 .arg(QCoreApplication::applicationVersion())
	);

	this->_ui->releaseWebView->load(QUrl(url));

	connect(this->_ui->installButton, SIGNAL(clicked()), this, SLOT(_onInstallClick()));
	connect(this->_ui->remindLaterButton, SIGNAL(clicked()), this, SLOT(_onRemindLaterClick()));
	connect(this->_ui->skipButton, SIGNAL(clicked()), this, SLOT(_onSkipClick()));

	qDebug() << "Created new release notes dialog with url" << url;
}

void QsparkleReleaseWindow::_onRemindLaterClick()
{
	emit remindLater(this->_version);
}

void QsparkleReleaseWindow::_onSkipClick()
{
	emit skip(this->_version);
}

void QsparkleReleaseWindow::_onInstallClick()
{
	emit install(this->_version);
}

QsparkleReleaseWindow::~QsparkleReleaseWindow()
{
	delete this->_ui;
}
