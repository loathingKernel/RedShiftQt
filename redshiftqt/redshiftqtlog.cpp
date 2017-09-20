#include "redshiftqtlog.h"
#include "ui_redshiftqtlog.h"

RedShiftQtLog::RedShiftQtLog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RedShiftQtLog)
{
    ui->setupUi(this);
}

RedShiftQtLog::~RedShiftQtLog()
{
    delete ui;
}
