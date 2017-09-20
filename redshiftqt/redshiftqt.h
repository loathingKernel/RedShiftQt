#ifndef REDSHIFTQT_H
#define REDSHIFTQT_H

#include <QMainWindow>
#include <QIcon>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QSettings>
#include <QFileDialog>

#include "redshiftqttray.h"

namespace Ui {
class RedShiftQt;
}

class RedShiftQt : public QMainWindow
{
    Q_OBJECT

public:
    explicit RedShiftQt(QWidget *parent = 0);
    ~RedShiftQt();

protected slots:
    void onToolButtonPathClicked(void);
    void onCheckBoxLocalSettingsChanged(void);

private:
    QSettings* prefs;
    QProcess* redshift;

    RedShiftQtTray* tray;
    Ui::RedShiftQt* ui;
};

#endif // REDSHIFTQT_H
