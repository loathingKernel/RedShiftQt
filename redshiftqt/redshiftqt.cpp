#include "redshiftqt.h"

RedShiftQt::RedShiftQt(QWidget *parent) :
    QMainWindow(parent)
{

    setWindowIcon(QIcon(":/app/icon"));
    setWindowTitle(qApp->applicationName());
    setGeometry(QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    QSize(0,0),
                    qApp->desktop()->availableGeometry()));

    conf = new QSettings(
                QSettings::IniFormat,
                QSettings::UserScope,
                qApp->organizationName(),
                qApp->applicationName(),
                this);

    tray = new RedShiftQtTray(this);
    tray->show();

    log = new RedShiftQtLog(this);

    info_timer = new QTimer(this);
    info_timer->setTimerType(Qt::VeryCoarseTimer);

    susp_timer = new QTimer(this);
    connect(susp_timer, SIGNAL(timeout()), this, SLOT(toggle()));
    susp_timer->setTimerType(Qt::VeryCoarseTimer);

    redshift = new QProcess(this);
    connect(redshift, SIGNAL(stateChanged(QProcess::ProcessState)),
            this, SLOT(onRedshiftStateChanged(QProcess::ProcessState)));
    //connect(redshift, SIGNAL(readyRead()), this, SLOT(onProcReadyRead()));
    //connect(redshift, SINGAL(readyReadStandardError(), this, SLOT(onProcReadyRead)));
    onRedshiftStateChanged(QProcess::NotRunning);

    redshift->setProgram(conf->value("redshift_path").toString());
    redshift->setArguments(getArguments());

    if(conf->value("start_enabled").toBool())
        redshift->start(QProcess::ReadOnly);

    helper = new QProcess(this);
    helper->setProgram(conf->value("redshift_path").toString());

    prefs = new RedShiftQtPrefs(this);
    connect(prefs, SIGNAL(confChanged()), this, SLOT(onConfChanged()));

    if(conf->value("show_prefs").toBool())
        prefs->show();
}

RedShiftQt::~RedShiftQt()
{
    redshift->kill();
    redshift->waitForFinished();
    helper->kill();
    helper->waitForFinished();
}

void RedShiftQt::onRedshiftStateChanged(QProcess::ProcessState state)
{
    switch(state) {
    case QProcess::Running :
        tray->setIcon(QIcon(":/tray/on"));
        tray->setToolTip(windowTitle() +": Running");
        tray->status->setChecked(true);
        tray->suspend->setEnabled(true);
        log->setStatus(QString("Enabled"));
        log->appendToLog(redshift->program().toUtf8() + " " + redshift->arguments().join(" ").toUtf8());
        break;

    case QProcess::NotRunning :
        tray->setIcon(QIcon(":/tray/off"));
        tray->setToolTip(windowTitle() +": Suspended");
        tray->status->setChecked(false);
        tray->suspend->setEnabled(false);
        log->setInfo(QStringList("Disabled"));
        log->appendToLog("Redshift stopped");
        break;

    default:
        break;
    }
    qDebug("Process state: %d", redshift->state());
}

void RedShiftQt::onReadyRead()
{

}

void RedShiftQt::onConfChanged()
{
    int state = redshift->state();
    if(state) {
        redshift->kill();
        redshift->waitForFinished();
    }
    redshift->setProgram(conf->value("redshift_path").toString());
    redshift->setArguments(getArguments());
    helper->setProgram(conf->value("redshift_path").toString());
    if(state) {
        redshift->start(QProcess::ReadOnly);
    }
}

void RedShiftQt::toggle(void)
{
    if(redshift->state() == QProcess::Running) {
        redshift->kill();
        redshift->waitForFinished();
        helper->setArguments(QStringList("-x"));
        helper->start(QProcess::ReadOnly);
        helper->waitForFinished();
    } else {
        susp_timer->stop();
        redshift->start(QProcess::ReadOnly);
        redshift->waitForStarted();
    }
}

void RedShiftQt::suspend(QAction* action)
{
    toggle();

    susp_timer->setInterval(action->data().toInt() * 60 * 1000);
    susp_timer->start();

    tray->setToolTip(tray->toolTip() + QString(" for %1 minutes").arg(action->data().toInt()));
    log->appendToLog(QString("Suspending reshift for %1 minutes").arg(susp_timer->interval()/60/1000));
}

void RedShiftQt::activated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        toggle();
}

void RedShiftQt::showPrefs()
{
    prefs->setVisible(true);
    prefs->raise();
}

void RedShiftQt::showLog()
{
    log->setVisible(true);
    log->raise();
}

QStringList RedShiftQt::getArguments()
{
    QStringList args;
    if(conf->value("use_own_conf").toBool()) {

        conf->beginGroup("color-temp");
        QString method(conf->value("method").toString());
        if(method != "default") {
            args.append("-m");
            args.append(method);
        }
        args.append("-t");
        args.append(QString("%1:%2").arg(
                        conf->value("day").toString(),
                        conf->value("night").toString()));
        if(!conf->value("transition").toBool()) args.append("-r");
        conf->endGroup();

        conf->beginGroup("location");
        QString provider(conf->value("provider").toString());
        if(provider != "default") {
            args.append("-l");
            if(provider == "manual") {
                args.append(QString("%1:%2").arg(
                                conf->value("latitude").toString(),
                                conf->value("longitude").toString()));
            } else {
                args.append(provider);
            }
        }
        conf->endGroup();

        conf->beginGroup("brightness");
        if(conf->value("adj-type").toInt() > 0) {
            args.append("-b");
            if(conf->value("adj-type").toInt() > 1) {
                args.append(QString("%1:%2").arg(
                                conf->value("day").toString(),
                                conf->value("night").toString()));
            } else {
                args.append(conf->value("day").toString());
            }
        }
        conf->endGroup();

        conf->beginGroup("gamma");
        if(conf->value("adj-type").toInt() > 0) {
            args.append("-g");
            if(conf->value("adj-type").toInt() > 1) {
                args.append(conf->value("day").toString());
            } else {
                args.append(conf->value("day").toString());
            }
        }
        conf->endGroup();

    } else {
        args.append("-c");
        args.append(conf->value("config_path").toString());
    }
    return args;
}
