#ifndef DATACOMPAREWIDGET_H
#define DATACOMPAREWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "listwidget.h"
#include "globalvar.h"
#include "readsource.h"


namespace Ui {
class DataCompareWidget;
}

class DataCompareWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataCompareWidget(QWidget *parent = 0);
    ~DataCompareWidget();

    //*****************20170315*****************//
    void getExcelData();
    //*****************20170315*****************//


private slots:

    void on_chooseFileBtn_clicked();

    void on_compareBtn_clicked();

    void on_clearBtn_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::DataCompareWidget *ui;

    //*****************20170315*****************//
    QMap<QString,QVector<QString>> qMemory_time_Map;
    QMap<QString,QVector<QString>> qCpu_time_Map;
    QMap<QString,QVector<QString>> qCputemp_time_Map;
    QMap<QString,QVector<QString>> qBatterytemp_time_Map;
    QMap<QString,QVector<QString>> qWifi_time_Map;
    QMap<QString,QVector<QString>> qMobile_time_Map;


    QMap<QString,QVector<QString>> qMemory_warning_Map;
    QMap<QString,QVector<QString>> qCpu_warning_Map;
    QMap<QString,QVector<QString>> qCputemp_warning_Map;
    QMap<QString,QVector<QString>> qBatterytemp_warning_Map;
    QMap<QString,QVector<QString>> qWifi_warning_Map;
    QMap<QString,QVector<QString>> qMobile_warning_Map;

    QMap<QString,QVector<QString>> qMemory_data_Map;
    QMap<QString,QVector<QString>> qCpu_data_Map;
    QMap<QString,QVector<QString>> qCputemp_data_Map;
    QMap<QString,QVector<QString>> qBatterytemp_data_Map;
    QMap<QString,QVector<QString>> qWifi_data_Map;
    QMap<QString,QVector<QString>> qMobile_data_Map;
    //*****************20170315*****************//

    QVector<QVector<QString>> qMemory_data_Vector;
    QVector<QVector<QString>> qCpu_data_Vector;
    QVector<QVector<QString>> qBatterytemp_data_Vector;
    QVector<QVector<QString>> qCputemp_data_Vector;
    QVector<QVector<QString>> qWifi_data_Vector;
    QVector<QVector<QString>> qMobile_data_Vector;

    QVector<QVector<QString>> qMemory_time_Vector;
    QVector<QVector<QString>> qCpu_time_Vector;
    QVector<QVector<QString>> qCputemp_time_Vector;
    QVector<QVector<QString>> qBatterytemp_time_Vector;
    QVector<QVector<QString>> qWifi_time_Vector;
    QVector<QVector<QString>> qMobile_time_Vector;

signals:
   /* void SendCompareData(QMap<QString,QVector<QString>> qMemory_data_Map,QMap<QString,QVector<QString>> qMemory_time_Map,
                         QMap<QString,QVector<QString>> qCpu_data_Map,QMap<QString,QVector<QString>> qCpu_time_Map,
                         QMap<QString,QVector<QString>> qCputemp_data_Map,QMap<QString,QVector<QString>> qCputemp_time_Map,
                         QMap<QString,QVector<QString>> qBatterytemp_data_Map,QMap<QString,QVector<QString>> qBatterytemp_time_Map);*/
    void SendCompareData(QVector<QVector<QString>> qMemory_data_Vector,QVector<QVector<QString>> qMemory_time_Vector,
                             QVector<QVector<QString>> qCpu_data_Vector,QVector<QVector<QString>> qCpu_time_Vector,
                             QVector<QVector<QString>> qCputemp_data_Vector,QVector<QVector<QString>> qCputemp_time_Vector,
                             QVector<QVector<QString>> qBatterytemp_data_Vector,QVector<QVector<QString>> qBatterytemp_time_Vector,
                             QVector<QVector<QString>> qWifi_data_Vector,QVector<QVector<QString>> qWifi_time_Vector,
                             QVector<QVector<QString>> qMobile_data_Vector,QVector<QVector<QString>> qMobile_time_Vector);
    void WindowDeactiveSignal();

};

#endif // DATACOMPAREWIDGET_H
