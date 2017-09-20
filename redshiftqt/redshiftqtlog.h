#ifndef REDSHIFTQTLOG_H
#define REDSHIFTQTLOG_H

#include <QMainWindow>

namespace Ui {
class RedShiftQtLog;
}

class RedShiftQtLog : public QMainWindow
{
    Q_OBJECT

public:
    explicit RedShiftQtLog(QWidget *parent = 0);
    ~RedShiftQtLog();

private:
    Ui::RedShiftQtLog *ui;
};

#endif // REDSHIFTQTLOG_H
