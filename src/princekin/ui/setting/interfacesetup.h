#ifndef INTERFACESETUP_H
#define INTERFACESETUP_H

#include <QWidget>
#include <QVariant>
#include "globalvar.h"
#include <QFileDialog>
#include "parsexml.h"

namespace Ui {
class InterfaceSetup;
}

typedef struct __signalInterfaceData_s
{
    bool isMemCheck;
    bool isCpuCheck;
    bool isBatteryCheck;
    bool isCpuTempCheck;
    bool isWifiCheck;
    bool isMobileCheck;
    QString memThres;
    QString cpuThres;
    QString batteryThres;
    QString cpuTempThres;
    QString wifiThres;
    QString mobileThres;

    bool isAllTest;
    QString method;
    QString filterStr;
    QString filePath;
}signalInterfaceData_s;

Q_DECLARE_METATYPE(signalInterfaceData_s)

class InterfaceSetup : public QWidget
{
    Q_OBJECT

public:
    explicit InterfaceSetup(QWidget *parent = 0);
    ~InterfaceSetup();

private:
    Ui::InterfaceSetup *ui;

    bool isMemCheck;
    bool isCpuCheck;
    bool isCpuTempCheck;
    bool isBatteryCheck;
    bool isWifiCheck;
    bool isMobileCheck;

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void SendData(QVariant var);

public slots:
    void stateChanged(bool state);
    void recieveSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
                                           QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres);
private slots:
    void on_allTestCBtn_clicked();
};

#endif // INTERFACESETUP_H
