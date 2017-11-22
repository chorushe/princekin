#ifndef MEASUREREPORT_H
#define MEASUREREPORT_H

#include <QWidget>
#include <QObject>
#include <QDateTime>
#include <QVector>

class MeasureReport
{

public:
    MeasureReport();

    QString deviceModel;
    QString deviceName;
    QString measureContent;
    double memInfo[4];//数组中的四个数依次代表最大值、最小值、均值、测量次数
    double cpuInfo[4];
    double batteryInfo[4];
    double cpuTempInfo[4];

    double wifiTrafficstats;
    double mobileTrafficstats;

    QDateTime startTime;
    QDateTime endTime;
    QString internalTime;

    int memOverThresTime;
    int cpuOverThresTime;
    int batteryOverThresTime;
    int cpuTempOverThresTime;
    int wifiOverThresTime;
    int mobileOverThresTime;

    QVector<double> memData;
    QVector<double> cpuData;
    QVector<double> batteryData;
    QVector<double> cpuTempData;
    QVector<double> wifiData;
    QVector<double> mobileData;

    QVector<QString> memTime;
    QVector<QString> cpuTime;
    QVector<QString> batteryTime;
    QVector<QString> cpuTempTime;
    QVector<QString> wifiTime;
    QVector<QString> mobileTime;

signals:

public slots:
};

#endif // MEASUREREPORT_H
