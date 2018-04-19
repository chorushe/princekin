#ifndef BEHAVIOURSETUP_H
#define BEHAVIOURSETUP_H

#include <QWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QDebug>
#include <QCloseEvent>
#include <QPointer>
#include <QRadioButton>
#include <QLineEdit>
#include "xmleditdialog.h"
#include "globalvar.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QToolTip>
#include <QCursor>

#include "parsexml.h"
#include "globalvar.h"

namespace Ui {
class BehaviourSetup;
}

typedef struct __signalBehaviorData_s
{
    QString xmlRootPath;
    bool isMemCheck;
    bool isCpuCheck;
    bool isBatteryCheck;
    bool isCpuTempCheck;
    bool isWifiCheck;
    bool isMobileCheck;
    bool isDebugVersion;
    QString memThres;
    QString cpuThres;
    QString batteryThres;
    QString cpuTempThres;
    QString wifiThres;
    QString mobileThres;
    QString mobileSystem;

}signalBehaviorData_s;

Q_DECLARE_METATYPE(signalBehaviorData_s)

class BehaviourSetup : public QWidget
{
    Q_OBJECT

public:
    explicit BehaviourSetup(QWidget *parent = 0);
    ~BehaviourSetup();

    void setMobilePaternAndroid(bool flag);

signals:
    void SendData(QVariant var);

private:
    Ui::BehaviourSetup *ui;

    bool isMemCheck;
    bool isCpuCheck;
    bool isCpuTempCheck;
    bool isBatteryCheck;
    bool isWifiCheck;
    bool isMobileCheck;

    QString filePath;
    QString deviceName;
    bool isDebugVersion;

    QString mobileSystem="android";

    void parseIP_PortToBat();


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_memCBtn_clicked();
    void on_cpuCBtn_clicked();
    void on_batteryCBtn_clicked();
    void on_cpuTempCBtn_clicked();
    void on_wifiCBtn_clicked();
    void on_mobileCBtn_clicked();

    void on_debugRBtn_clicked();
    void on_releaseRBtn_clicked();

    void on_memLineEdit_textChanged(const QString &arg1);
    void on_cpuLineEdit_textChanged(const QString &arg1);
    void on_batteryLineEdit_textChanged(const QString &arg1);
    void on_cpuTempLineEdit_textChanged(const QString &arg1);
    void on_wifiLineEdit_textChanged(const QString &arg1);
    void on_mobileLineEdit_textChanged(const QString &arg1);

    void stateChanged(bool state);
    void recieveSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
                           QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres);
    void on_browseBtn_clicked();
    void on_RBtnAndroid_clicked();
    void on_RBtnIos_clicked();
};

#endif // BEHAVIOURSETUP_H
