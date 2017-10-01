#include "redshiftqttray.h"

RedShiftQtTray::RedShiftQtTray(QMainWindow* parent) : QSystemTrayIcon(parent)
{
    menu = new QMenu();

    status = new QAction(tr("&Enabled"), this);
    connect(status, SIGNAL(triggered()), parent, SLOT(toggleRedshift()));
    status->setCheckable(true);
    menu->addAction(status);

    suspend = new QMenu(tr("&Suspend for"));
    connect(suspend, SIGNAL(triggered(QAction*)), parent, SLOT(suspendRedshift(QAction*)));

    suspend30m = new QAction(tr("&30 minutes"));
    suspend30m->setData(30);
    suspend->addAction(suspend30m);

    suspend1h = new QAction(tr("&1 hour"));
    suspend1h->setData(60);
    suspend->addAction(suspend1h);

    suspend2h = new QAction(tr("&2 hours"));
    suspend2h->setData(120);
    suspend->addAction(suspend2h);

    menu->addMenu(suspend);

    log = new QAction(tr("&View Info"), this);
    connect(log, SIGNAL(triggered()), parent, SLOT(showLog()));
    menu->addAction(log);

    menu->addSeparator();

    prefs = new QAction(tr("&Preferences"), this);
    connect(prefs, SIGNAL(triggered()), parent, SLOT(showPrefs()));
    menu->addAction(prefs);

    menu->addSeparator();

    quit = new QAction(tr("&Quit"), this);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    menu->addAction(quit);

    setContextMenu(menu);
    setIcon(parent->windowIcon());
    setToolTip(parent->windowTitle());

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            parent, SLOT(toggleRedshift(QSystemTrayIcon::ActivationReason)));
}

RedShiftQtTray::~RedShiftQtTray()
{

}

