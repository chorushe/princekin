#ifndef ASSISTMEASURE_H
#define ASSISTMEASURE_H

#include <QWidget>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "measurereport.h"
#include <QVector>
#include "execmd.h"


class AssistMeasure : public QWidget
{
    Q_OBJECT
public:
    explicit AssistMeasure(QWidget *parent = 0);

    QVector<MeasureReport> reportInfo;

    int timerTime;
    QTimer *assistTimer=NULL;
    QThread *assistTimerThread=NULL;

    QString deviceName;
    QString deviceModel;
    QString packageName;
    bool stopFlag;

    bool isCpuCheck;
    bool isMemCheck;
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

    bool isMemOverThres;
    bool isCpuOverThres;
    bool isBatteryOverThres;
    bool isCpuTempOverThres;
    bool isMobileOverThres;
    bool isWifiOverThres;

    QTime memTime;
    QTime cpuTime;
    QTime batteryTime;
    QTime cpuTempTime;
    QTime wifiTime;
    QTime mobileTime;

    int memIndex=6;
    int cpuIndex=2;

    void StartMeasure();
    void StopMeasure();
    void ExportReport(QString filePath);
    QString calculateTimeFromMS(int time);
    void getMemoryCpuIndex();

    int c=1;//测流量时用来判断是否是第一次的标志位

signals:
    void sendCpuTempOverThresSignal(bool flag,QString deviceName);
    void sendBatteryOverThresSignal(bool flag,QString deviceName);
    void sendCpuOverThresSignal(bool flag,QString deviceName);
    void sendMemOverThresSignal(bool flag,QString deviceName);
    void sendWifiOverThresSignal(bool flag,QString deviceName);
    void sendMobileOverThresSignal(bool flag,QString deviceName);

private slots:
    void getMemory();
    void getCPU();
    void getBattery();
    void getCpuTemp();
    void getTrafficstats();

private:
    double currentLrxWifi;
    double currentLrxMobile;

    double currentLtxWifi;
    double currentLtxMobile;

    double one_rxWifi;
    double one_rxWifi_M;
    double one_rxWifi2;

    double one_txWifi;
    double one_txWifi_M;
    double one_txWifi2;

    double one_rxMobile;
    double one_rxMobile_M;

    double one_rxMobile2;

    double one_txMobile;
    double one_txMobile_M;

    double one_txMobile2;
};

#endif // ASSISTMEASURE_H
