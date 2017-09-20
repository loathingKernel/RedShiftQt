#ifndef REDSHIFTQTTRAY_H
#define REDSHIFTQTTRAY_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QAction>
#include <QMenu>

class RedShiftQtTray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit RedShiftQtTray(QMainWindow *parent = 0);
    ~RedShiftQtTray();

private slots:
    void showPrefs();
    void showLog();

    void activated(QSystemTrayIcon::ActivationReason);

private:
    QAction* actionPrefs;
    QAction* actionLog;
    QAction* actionQuit;
    QMenu* trayMenu;
};

#endif // REDSHIFTQTTRAY_H
