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

    ui->plainTextEditLog->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

RedShiftQtLog::~RedShiftQtLog()
{
    delete ui;
}

void RedShiftQtLog::setStatus(QString status)
{
    ui->labelStatusInfo->setText(status);
}

void RedShiftQtLog::setInfo(QStringList info)
{

}

void RedShiftQtLog::appendToLog(QString input)
{
    ui->plainTextEditLog->appendPlainText(QTime::currentTime().toString() + ": " + input);
}

void RedShiftQtLog::closeEvent(QCloseEvent* event)
{
    //hide();
    //event->ignore();
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
