#ifndef REDSHIFTQTTRAY_H
#define REDSHIFTQTTRAY_H

#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

class RedShiftQtTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit RedShiftQtTray(QMainWindow *parent = 0);
    ~RedShiftQtTray();

    QAction* status;
    QMenu* suspend;

private:

    QAction* suspend30m;
    QAction* suspend1h;
    QAction* suspend2h;

    QAction autostart;

    QAction* prefs;
    QAction* log;
    QAction* quit;
    QMenu* menu;
};

#endif // REDSHIFTQTTRAY_H
