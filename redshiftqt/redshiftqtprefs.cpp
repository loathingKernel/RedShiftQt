#include "redshiftqt.h"
#include "ui_redshiftqt.h"


RedShiftQt::RedShiftQt(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RedShiftQt)
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    connect(ui->toolButtonPath, SIGNAL(clicked()), this, SLOT(onToolButtonPathClicked()));

    connect(ui->checkBoxLocalSettings, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxLocalSettingsChanged()));

    tray = new RedShiftQtTray(this);
    tray->show();

    this->show();
}

RedShiftQt::~RedShiftQt()
{
    delete ui;
}

void RedShiftQt::onToolButtonPathClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Find redshift executable"), "/", tr("redshift (redshift.exe)"));

    ui->lineEditPath->setText(path);
}

void RedShiftQt::onCheckBoxLocalSettingsChanged()
{
    bool state = !ui->checkBoxLocalSettings->isChecked();

    ui->groupBoxBrightness->setDisabled(state);
    ui->groupBoxGamma->setDisabled(state);
    ui->groupBoxLocation->setDisabled(state);
    ui->groupBoxTemp->setDisabled(state);

}
