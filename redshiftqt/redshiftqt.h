#ifndef REDSHIFTQT_H
#define REDSHIFTQT_H

#include <QApplication>
#include <QMainWindow>
#include <QStyle>
#include <QDesktopWidget>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

#include "redshiftqttray.h"
#include "redshiftqtprefs.h"
#include "redshiftqtlog.h"

namespace Ui {
class RedShiftQt;
}

class RedShiftQt : public QMainWindow
{
    Q_OBJECT
public:
    explicit RedShiftQt(QWidget *parent = 0);
    ~RedShiftQt();

private slots:
    void onRedshiftStateChanged(QProcess::ProcessState);
    void onReadyRead();

    void onConfChanged();

    void toggle();
    void suspend(QAction*);
    void activated(QSystemTrayIcon::ActivationReason);
    void showPrefs();
    void showLog();

private:
    RedShiftQtTray *tray;
    RedShiftQtPrefs *prefs;
    RedShiftQtLog *log;

    QProcess *redshift;
    QProcess *helper;

    QTimer *susp_timer;
    QTimer *info_timer;

    QSettings *conf;

    QStringList getArguments(void);
};

#endif // REDSHIFTQT_H
