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

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(startRedshift()));
    timer->setTimerType(Qt::VeryCoarseTimer);

    helper = new QProcess(this);
    helper->setProgram(conf->value("redshift_path", "").toString());

    redshift = new QProcess(this);
    connect(redshift, SIGNAL(started()), this, SLOT(onRedshiftStarted()));
    connect(redshift, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onRedshiftFinished()));
    redshift->setProgram(conf->value("redshift_path", "").toString());
    redshift->setArguments(getArguments());
    onRedshiftFinished();
    if(conf->value("start_enabled", false).toBool())
        redshift->start(QProcess::ReadOnly);

    prefs = new RedShiftQtPrefs(this);
    connect(prefs, SIGNAL(confChanged()), this, SLOT(onConfChanged()));

    if(conf->value("show_prefs", true).toBool())
        showPrefs();
}

RedShiftQt::~RedShiftQt()
{
    stopRedshift();
}

QString RedShiftQt::getRedshiftInfo()
{
    helper->setArguments(QStringList("-pv"));
    helper->start(QProcess::ReadOnly);
    helper->waitForFinished();
    return(helper->readAllStandardOutput());
}

void RedShiftQt::startRedshift()
{
    timer->stop();
    redshift->start(QProcess::ReadOnly);
    redshift->waitForStarted();
}

void RedShiftQt::stopRedshift()
{
    redshift->kill();
    redshift->waitForFinished();
    helper->setArguments(QStringList("-x"));
    helper->start(QProcess::ReadOnly);
    helper->waitForFinished();
}

void RedShiftQt::onRedshiftStarted()
{
    tray->setIcon(QIcon(":/tray/on"));
    tray->setToolTip(windowTitle() + ": Running");
    tray->status->setChecked(true);
    tray->suspend->setEnabled(true);
    log->setStatus(QString("Enabled"));
    log->setInfo(getRedshiftInfo());
    log->appendToLog("Redshift was started with: " + redshift->arguments().join(" ").toUtf8());
}

void RedShiftQt::onRedshiftFinished()
{
    tray->setIcon(QIcon(":/tray/off"));
    tray->setToolTip(windowTitle() + ": Suspended");
    tray->status->setChecked(false);
    tray->suspend->setEnabled(false);
    log->setStatus(QString("Disabled"));
    log->setInfo(getRedshiftInfo());
    log->appendToLog("Redshift was suspended");
}

void RedShiftQt::onConfChanged()
{
    int state = redshift->state();
    if(state) stopRedshift();
    redshift->setProgram(conf->value("redshift_path").toString());
    redshift->setArguments(getArguments());
    helper->setProgram(conf->value("redshift_path").toString());
    if(state) startRedshift();
}

void RedShiftQt::toggleRedshift()
{
    if(redshift->state()) stopRedshift();
    else startRedshift();
}

void RedShiftQt::toggleRedshift(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger) toggleRedshift();
}

void RedShiftQt::suspendRedshift(QAction* action)
{
    stopRedshift();

    timer->setInterval(action->data().toInt() * 60 * 1000);
    timer->start();

    tray->setToolTip(tray->toolTip() + QString(" for %1 minutes").arg(action->data().toInt()));
    log->appendToLog(QString("Suspending reshift for %1 minutes").arg(timer->interval()/60/1000));
}

void RedShiftQt::showPrefs()
{
    prefs->setVisible(true);
    prefs->raise();
}

void RedShiftQt::showLog()
{
    log->setInfo(getRedshiftInfo());
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
