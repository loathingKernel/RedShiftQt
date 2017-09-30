#ifndef REDSHIFTQTPREFS_H
#define REDSHIFTQTPREFS_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPointer>

#include "qdoublespinboxlist.h"

namespace Ui {
class RedShiftQtPrefs;
}

class RedShiftQtPrefs : public QDialog
{
    Q_OBJECT

public:
    explicit RedShiftQtPrefs(QWidget *parent = 0);
    ~RedShiftQtPrefs();

signals:
    void confChanged();

private slots:
    void onToolButtonPathClicked(void);
    void onToolButtonConfClicked(void);
    void onCheckBoxLocalSettingsChanged(int);

    void onComboBoxLocProviderTextChanged(QString);
    void requestLocation(void);
    void replyFinished(QNetworkReply*);

    void onCheckBoxBrStateChanged(int);
    void onCheckBoxGamStateChanged(int);

    void onImportClicked(void);
    void onCancelClicked(void);
    void onApplyClicked(void);

private:
    void importConf(void);
    void loadConf(void);
    void saveConf(void);

    Ui::RedShiftQtPrefs *ui;

    QSettings* conf;
    QPointer<QNetworkAccessManager> manager;

    QDoubleSpinBoxList doubleSpinBoxListGamDay;
    QDoubleSpinBoxList doubleSpinBoxListGamNight;
};

#endif // REDSHIFTQTPREFS_H
