#ifndef QSPARKLEDIALOG_H
#define QSPARKLEDIALOG_H

#include <QWidget>
#include <QCoreApplication>
#include <QDebug>

namespace Ui {
	class QsparkleReleaseWindow;
}

class QsparkleReleaseWindow : public QWidget
{
	Q_OBJECT

	public:
		explicit QsparkleReleaseWindow(QString url, QString newVersion, QPixmap icon, QWidget *parent = 0);
		~QsparkleReleaseWindow();

	private:
		Ui::QsparkleReleaseWindow *_ui;

		QString _version;

	private slots:
		void _onRemindLaterClick();
		void _onSkipClick();
		void _onInstallClick();

	signals:
		void remindLater(QString version);
		void skip(QString version);
		void install(QString version);
};

#endif // QSPARKLEDIALOG_H
