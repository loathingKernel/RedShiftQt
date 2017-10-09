#include "redshiftqtlog.h"
#include "ui_redshiftqtlog.h"

RedShiftQtLog::RedShiftQtLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RedShiftQtLog)
{
    ui->setupUi(this);
    setWindowTitle(parent->windowTitle() + " Log");

    connect(ui->pushButtonClear, SIGNAL(clicked()), this, SLOT(clearLog()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(saveLog()));
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked()),
            this, SLOT(close()));

    ui->plainTextEditLog->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

RedShiftQtLog::~RedShiftQtLog()
{
    delete ui;
}

void RedShiftQtLog::setStatus(QString status)
{
    ui->labelStatusValue->setText(status);
}

void RedShiftQtLog::setInfo(QString info)
{
    QStringList info_list = info.split('\n');
    QStringList::iterator i;
    for(i = info_list.begin(); i != info_list.end(); ++i) {
        if(i->startsWith("Location")) {
            ui->labelLocValue->setText(i->split(": ").at(1));
            ++i;
        }
        if(i->startsWith("Period")) {
            ui->labelPeriodValue->setText(i->split(": ").at(1));
            ++i;
        }
        if(i->startsWith("Color temp")) {
            ui->labelTempValue->setText(i->split(": ").at(1));
        }
    }
}

void RedShiftQtLog::appendToLog(QString input)
{
    ui->plainTextEditLog->appendPlainText(QTime::currentTime().toString() + ": " + input);
}

void RedShiftQtLog::clearLog()
{
    ui->plainTextEditLog->clear();
}

void RedShiftQtLog::saveLog()
{
    QString filename = QFileDialog::getSaveFileName(
                this, "Save Log",
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/redshiftqt_log.txt",
                "Text files (*.txt);;All Files (*)");

    if (filename.isEmpty()) {
        return;
    } else {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
        QDataStream out(&file);
        out.writeBytes(ui->plainTextEditLog->toPlainText().toUtf8(),
                       static_cast<uint>(ui->plainTextEditLog->toPlainText().length()));
    }
}
