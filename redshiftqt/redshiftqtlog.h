#ifndef REDSHIFTQTLOG_H
#define REDSHIFTQTLOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTime>

namespace Ui {
class RedShiftQtLog;
}

class RedShiftQtLog : public QDialog
{
    Q_OBJECT

public:
    explicit RedShiftQtLog(QWidget *parent = 0);
    ~RedShiftQtLog();

    void setStatus(QString);
    void setInfo(QString);
    void appendToLog(QString);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void clearLog(void);
    void saveLog(void);

private:
    Ui::RedShiftQtLog *ui;

    QTimer *timer;
};

#endif // REDSHIFTQTLOG_H
