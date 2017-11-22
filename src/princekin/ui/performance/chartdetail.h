#ifndef CHARTDETAIL_H
#define CHARTDETAIL_H

#include <QWidget>
#include "globalvar.h"
#include <QTimer>
#include <QFileInfo>

namespace Ui {
class ChartDetail;
}

class ChartDetail : public QWidget
{
    Q_OBJECT

public:
    explicit ChartDetail(QWidget *parent = 0);
    ~ChartDetail();

    QTimer *timer;

    void startTimer();
    void setFlag(QString flag);
    void plotChart();

    QString chartFlag="mem";

private:
    Ui::ChartDetail *ui;


    QString strMemThres;
    QString strCpuThres;
    QString strBatteryThres;
    QString strCpuTempThres;
    QString strWifiThres;
    QString strMobileThres;

    QVector<QVector<QString>> qMemory_data_Vector;
    QVector<QVector<QString>> qCpu_data_Vector;
    QVector<QVector<QString>> qBatterytemp_data_Vector;
    QVector<QVector<QString>> qCputemp_data_Vector;
    QVector<QVector<QString>> qMobile_data_Vector;
    QVector<QVector<QString>> qWifi_data_Vector;

    QVector<QVector<QString>> qMemory_time_Vector;
    QVector<QVector<QString>> qCpu_time_Vector;
    QVector<QVector<QString>> qCputemp_time_Vector;
    QVector<QVector<QString>> qBatterytemp_time_Vector;
    QVector<QVector<QString>> qMobile_time_Vector;
    QVector<QVector<QString>> qWifi_time_Vector;

    void closeEvent();

public slots:
    void StartDrawChart();
    void RecieveThresMax(QString memThres,QString cpuThres,QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres);

    void RecieveCompareData(QVector<QVector<QString>> qMemory_data_Vector, QVector<QVector<QString>> qMemory_time_Vector,
                            QVector<QVector<QString>> qCpu_data_Vector, QVector<QVector<QString>> qCpu_time_Vector,
                            QVector<QVector<QString>> qCputemp_data_Vector, QVector<QVector<QString>> qCputemp_time_Vector,
                            QVector<QVector<QString>> qBatterytemp_data_Vector, QVector<QVector<QString>> qBatterytemp_time_Vector,
                            QVector<QVector<QString> > qWifi_data_Vector, QVector<QVector<QString> > qWifi_time_Vector,
                            QVector<QVector<QString> > qMobile_data_Vector, QVector<QVector<QString> > qMobile_time_Vector);
};

#endif // CHARTDETAIL_H
