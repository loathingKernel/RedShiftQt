#include "redshiftqtprefs.h"
#include "ui_redshiftqtprefs.h"

RedShiftQtPrefs::RedShiftQtPrefs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RedShiftQtPrefs)
{
    ui->setupUi(this);
    setWindowTitle(parent->windowTitle() + " Preferences");

    conf = new QSettings(
                QSettings::IniFormat,
                QSettings::UserScope,
                qApp->organizationName(),
                qApp->applicationName(),
                this);

    connect(ui->toolButtonPath, SIGNAL(clicked()), this, SLOT(onToolButtonPathClicked()));
    connect(ui->toolButtonConf, SIGNAL(clicked()), this, SLOT(onToolButtonConfClicked()));
    connect(ui->checkBoxLocalSettings, SIGNAL(stateChanged(int)),
            this, SLOT(onCheckBoxLocalSettingsChanged(int)));

    connect(ui->comboBoxLocProvider, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onComboBoxLocProviderTextChanged(QString)));
    connect(ui->pushButtonLocDetect, SIGNAL(clicked()), this, SLOT(requestLocation()));
    connect(ui->checkBoxBr, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxBrStateChanged(int)));
    connect(ui->checkBoxGam, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxGamStateChanged(int)));

    connect(ui->pushButtonImport, SIGNAL(clicked()), this, SLOT(onImportClicked()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(onCancelClicked()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SLOT(onApplyClicked()));

    doubleSpinBoxListGamDay.append(ui->doubleSpinBoxGamDayR);
    doubleSpinBoxListGamDay.append(ui->doubleSpinBoxGamDayG);
    doubleSpinBoxListGamDay.append(ui->doubleSpinBoxGamDayB);

    doubleSpinBoxListGamNight.append(ui->doubleSpinBoxGamNightR);
    doubleSpinBoxListGamNight.append(ui->doubleSpinBoxGamNightG);
    doubleSpinBoxListGamNight.append(ui->doubleSpinBoxGamNightB);

#ifdef Q_OS_LINUX
    ui->comboBoxTempMethod->addItem("drm");
    ui->comboBoxTempMethod->addItem("randr");
    ui->comboBoxTempMethod->addItem("vidmode");
    ui->comboBoxLocProvider->addItem("geoclue2");
#endif
#ifdef Q_OS_DARWIN
    ui->comboBoxTempMethod->addItem("quartz");
    ui->comboBoxLocProvider->addItem("corelocation");
#endif
#ifdef Q_OS_WIN
    ui->comboBoxTempMethod->addItem("wingdi");
#endif
    ui->comboBoxLocProvider->addItem("manual");

    onCheckBoxLocalSettingsChanged(Qt::CheckState::Unchecked);
    onComboBoxLocProviderTextChanged("default");
    onCheckBoxBrStateChanged(Qt::CheckState::Unchecked);
    onCheckBoxGamStateChanged(Qt::CheckState::Unchecked);

    resize(minimumSizeHint());
    setFixedSize(size());

    loadConf();
}

RedShiftQtPrefs::~RedShiftQtPrefs()
{
    delete ui;
}

void RedShiftQtPrefs::onToolButtonPathClicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Find redshift executable"),
                "",
                "redshift executable (redshift.exe)");

    ui->lineEditPath->setText(path);
}

void RedShiftQtPrefs::onToolButtonConfClicked()
{
    QString conf = QFileDialog::getOpenFileName(
                this,
                tr("Find redshift configuration file"),
                QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation).first(),
                "redshift configuration (redshift.conf)");

    ui->lineEditConf->setText(conf);
}


void RedShiftQtPrefs::onCheckBoxLocalSettingsChanged(int state)
{
    ui->groupBoxBr->setEnabled(state);
    ui->groupBoxGam->setEnabled(state);
    ui->groupBoxLoc->setEnabled(state);
    ui->groupBoxTemp->setEnabled(state);
}

void RedShiftQtPrefs::onComboBoxLocProviderTextChanged(QString text)
{
    if(text == "manual") {
        ui->lineEditLocLat->setEnabled(true);
        ui->lineEditLocLon->setEnabled(true);
        ui->pushButtonLocDetect->setEnabled(true);
    } else {
        ui->lineEditLocLat->setEnabled(false);
        ui->lineEditLocLon->setEnabled(false);
        ui->pushButtonLocDetect->setEnabled(false);
    }
}

void RedShiftQtPrefs::requestLocation()
{
    ui->pushButtonLocDetect->setDisabled(true);

    if(manager.isNull()){
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

        manager->get(QNetworkRequest(QUrl("http://ip-api.com/json")));
    }
}

void RedShiftQtPrefs::replyFinished(QNetworkReply* reply)
{
    QString document(reply->readAll());
    manager->deleteLater();
    reply->deleteLater();

    QJsonDocument parser;
    QJsonObject json = parser.fromJson(document.toUtf8()).object();

    QMessageBox msg;
    QString info = QString("<h4>Detected Location</h4>"
                           "<b>City:</b> %1, %2, %3<br/>"
                           "<b>Timezone:</b> %4<br/>"
                           "<b>IP:</b> %5<br/>").arg(
            json["city"].toString(),
            json["regionName"].toString(),
            json["countryCode"].toString(),
            json["timezone"].toString(),
            json["query"].toString());
    msg.setText(info);
    msg.exec();

    ui->pushButtonLocDetect->setEnabled(true);

    ui->lineEditLocLat->setText(QString("%1").arg(json["lat"].toDouble()));
    ui->lineEditLocLon->setText(QString("%1").arg(json["lon"].toDouble()));
}

void RedShiftQtPrefs::onCheckBoxBrStateChanged(int state) {
    ui->doubleSpinBoxBrDay->setEnabled(state > 0);
    ui->doubleSpinBoxBrNight->setEnabled(state > 1);
}

void RedShiftQtPrefs::onCheckBoxGamStateChanged(int state) {
    doubleSpinBoxListGamDay.setEnabled(state > 0);
    doubleSpinBoxListGamNight.setEnabled(state > 1);
}

void RedShiftQtPrefs::onImportClicked()
{
    if(ui->lineEditConf->text().isEmpty()){
        ui->lineEditConf->setStyleSheet("background-color: rgba(255, 0, 0, 50);");
        ui->lineEditConf->setFocus();
        QMessageBox msg;
        msg.setText("<h4>Error:</h4> Missing redshift cofiguration file location (redshift.conf)");
        msg.exec();
        return;
    } else {
        ui->lineEditConf->setStyleSheet(styleSheet());
    }

    importConf();
}

void RedShiftQtPrefs::onCancelClicked()
{
    loadConf();
}

void RedShiftQtPrefs::onApplyClicked()
{
    QStringList errors;
    QMessageBox msg;
    msg.setText("<h4>Error(s):</h4>");
    if(ui->lineEditPath->text().isEmpty()) {
        errors.append("Missing redshift executable location.");
    } else {
        if(ui->checkBoxLocalSettings->checkState()) {
            if(ui->comboBoxLocProvider->currentText() == "manual") {
                if(ui->lineEditLocLat->text().isEmpty())
                    errors.append("Missing location latitude.");
                if(ui->lineEditLocLon->text().isEmpty())
                    errors.append("Missing location longitude.");
            }
        } else {
            if(ui->lineEditConf->text().isEmpty()) {
                errors.append("Missing redshift cofiguration file location.");
            }
        }
    }

    if(errors.isEmpty()) {
        saveConf();
        emit confChanged();
    }
    else {
        msg.setText(msg.text() + errors.join("<br/>"));
        msg.exec();
    }
}

void RedShiftQtPrefs::importConf()
{
    QSettings import(ui->lineEditConf->text(), QSettings::IniFormat, this);
    import.beginGroup("redshift");
    ui->spinBoxTempDay->setValue(import.value("temp-day").toInt());
    ui->spinBoxTempNight->setValue(import.value("temp-night").toInt());
    ui->checkBoxTempTrans->setChecked(import.value("transition").toBool());

    double brightness_day = import.value("brightness-day").toDouble();
    double brightness_night = import.value("brightness-night").toDouble();
    if(brightness_day != 0.0 && brightness_night != 0.0) {
        ui->checkBoxBr->setCheckState(Qt::CheckState::Checked);
        ui->doubleSpinBoxBrDay->setValue(brightness_day);
        ui->doubleSpinBoxBrNight->setValue(brightness_night);
    } else {
        double brightness = import.value("brightness").toDouble();
        if(brightness != 0.0) {
            ui->checkBoxBr->setCheckState(Qt::CheckState::PartiallyChecked);
            ui->doubleSpinBoxBrDay->setValue(brightness);
        } else {
            ui->checkBoxBr->setCheckState(Qt::CheckState::Unchecked);
        }
    }

    QStringList gamma_day = import.value("gamma-day").toString().split(':', QString::SkipEmptyParts);
    QStringList gamma_night = import.value("gamma-night").toString().split(':', QString::SkipEmptyParts);
    if(!gamma_day.isEmpty() && !gamma_night.isEmpty()) {
        ui->checkBoxGam->setCheckState(Qt::CheckState::Checked);
        doubleSpinBoxListGamDay.setValue(gamma_day);
        doubleSpinBoxListGamNight.setValue(gamma_night);
    } else {
        QStringList gamma = import.value("gamma").toString().split(':', QString::SkipEmptyParts);
        if(!gamma.isEmpty()) {
            ui->checkBoxGam->setCheckState(Qt::CheckState::PartiallyChecked);
            doubleSpinBoxListGamDay.setValue(gamma);
        } else {
            ui->checkBoxGam->setCheckState(Qt::CheckState::Unchecked);
        }
    }

    QString method = import.value("adjustment-method").toString();
    QString provider = import.value("location-provider").toString();
    import.endGroup();

    if(!method.isEmpty()) {
        ui->comboBoxTempMethod->setCurrentText(method);
        import.beginGroup(method);
        import.endGroup();
    }

    if(!provider.isEmpty()) {
        ui->comboBoxLocProvider->setCurrentText(provider);
        import.beginGroup(provider);
        ui->lineEditLocLat->setText(import.value("lat").toString());
        ui->lineEditLocLon->setText(import.value("lon").toString());
        import.endGroup();
    }
}

void RedShiftQtPrefs::loadConf()
{
    ui->lineEditPath->setText(conf->value("redshift_path", "").toString());
    ui->lineEditConf->setText(conf->value("config_path", "").toString());
    ui->checkBoxLocalSettings->setChecked(conf->value("use_own_conf").toBool());
    ui->checkBoxRun->setChecked(conf->value("start_enabled").toBool());
    ui->checkBoxShow->setChecked(conf->value("show_prefs").toBool());

    conf->beginGroup("color-temp");
    QString method = conf->value("method").toString();
    ui->comboBoxTempMethod->setCurrentText(method);
    ui->spinBoxTempDay->setValue(conf->value("day").toInt());
    ui->spinBoxTempNight->setValue(conf->value("night").toInt());
    ui->checkBoxTempTrans->setChecked(conf->value("transition").toBool());
    conf->endGroup();

    conf->beginGroup("location");
    QString provider = conf->value("provider").toString();
    ui->comboBoxLocProvider->setCurrentText(provider);
    ui->lineEditLocLat->setText(conf->value("latitude").toString());
    ui->lineEditLocLon->setText(conf->value("longitude").toString());
    conf->endGroup();

    conf->beginGroup("brightness");
    ui->checkBoxBr->setCheckState(static_cast<Qt::CheckState>(conf->value("adj-type").toInt()));
    ui->doubleSpinBoxBrDay->setValue(conf->value("day").toDouble());
    ui->doubleSpinBoxBrNight->setValue(conf->value("night").toDouble());
    conf->endGroup();

    conf->beginGroup("gamma");
    ui->checkBoxGam->setCheckState(static_cast<Qt::CheckState>(conf->value("adj-type").toInt()));
    doubleSpinBoxListGamDay.setValue(conf->value("day").toString().split(':', QString::SkipEmptyParts));
    doubleSpinBoxListGamNight.setValue(conf->value("night").toString().split(':', QString::SkipEmptyParts));
    conf->endGroup();
}

void RedShiftQtPrefs::saveConf()
{
    conf->setValue("redshift_path", ui->lineEditPath->text());
    conf->setValue("config_path", ui->lineEditConf->text());
    conf->setValue("use_own_conf", ui->checkBoxLocalSettings->checkState());
    conf->setValue("start_enabled", ui->checkBoxRun->checkState());
    conf->setValue("show_prefs", ui->checkBoxShow->checkState());

    conf->beginGroup("color-temp");
    conf->setValue("method", ui->comboBoxTempMethod->currentText());
    conf->setValue("day", ui->spinBoxTempDay->value());
    conf->setValue("night", ui->spinBoxTempNight->value());
    conf->setValue("transition", ui->checkBoxTempTrans->checkState());
    conf->endGroup();

    conf->beginGroup("location");
    conf->setValue("provider", ui->comboBoxLocProvider->currentText());
    conf->setValue("latitude", ui->lineEditLocLat->text().toDouble());
    conf->setValue("longitude", ui->lineEditLocLon->text().toDouble());
    conf->endGroup();

    conf->beginGroup("brightness");
    conf->setValue("adj-type", ui->checkBoxBr->checkState());
    conf->setValue("day", ui->doubleSpinBoxBrDay->value());
    conf->setValue("night", ui->doubleSpinBoxBrNight->value());
    conf->endGroup();

    conf->beginGroup("gamma");
    conf->setValue("adj-type", ui->checkBoxGam->checkState());
    conf->setValue("day", doubleSpinBoxListGamDay.value().join(":"));
    conf->setValue("night", doubleSpinBoxListGamNight.value().join(":"));
    conf->endGroup();
}

