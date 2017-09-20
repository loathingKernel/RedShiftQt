#include "redshiftqttray.h"

RedShiftQtTray::RedShiftQtTray(QMainWindow* parent) : QSystemTrayIcon(parent)
{
    actionPrefs = new QAction(tr("&Preferences"), this);
    connect(actionPrefs, SIGNAL(triggered()), this, SLOT(showPrefs()));

    actionLog = new QAction(tr("&View Log"), this);
    connect(actionLog, SIGNAL(triggered()), this, SLOT(showLog()));

    actionQuit = new QAction(tr("&Quit"), this);
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayMenu = new QMenu();
    trayMenu->addAction(actionPrefs);
    trayMenu->addAction(actionLog);
    trayMenu->addSeparator();
    trayMenu->addAction(actionQuit);

    setContextMenu(trayMenu);
    setIcon(parent->windowIcon());
    setToolTip(parent->windowTitle());

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(activated(QSystemTrayIcon::ActivationReason)));
}

RedShiftQtTray::~RedShiftQtTray()
{

}

void RedShiftQtTray::showPrefs()
{

}

void RedShiftQtTray::showLog()
{

}

void RedShiftQtTray::activated(QSystemTrayIcon::ActivationReason reason)
{

}
