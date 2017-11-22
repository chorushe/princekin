#ifndef PERFORMANCEWIDGET_H
#define PERFORMANCEWIDGET_H

#include <QMainWindow>
#include <QSplitter>
#include <QDebug>
#include <QListView>
#include <QStringListModel>
#include <QTimer>
#include <QHBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QVector>
#include <QEvent>
#include <QtXlsx>
#include <QCloseEvent>
#include <QToolBar>
#include <QDesktopWidget>
#include <QApplication>
#include <QThread>
#include <QPointer>

#include "qcustomplot.h"
#include "ui/setting/performancesetup.h"
#include "monkeysetup.h"
#include "assistmeasure.h"
#include "globalvar.h"
#include "trafficcontroller2.h"
#include "trafficworker2.h"
#include "execmd.h"
#include "netstatecontroller.h"
#include "netstateworker.h"
#include "trafficstatdetail.h"
#include "performancedetail.h"
#include "tempdetail.h"
#include "chartdetail.h"
#include "helper.h"
#include "datacomparewidget.h"
#include "assistbehaviorworker.h"
#include "generatexlsx.h"
#include "framescontroller.h"
#include "excelcontroller.h"
#include "adbshellcontroller.h"

using namespace ExcelController;
using namespace AdbShellController;
using namespace QXlsx;


namespace Ui {
class PerformanceWidget;
}

class PerformanceWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit PerformanceWidget(QWidget *parent = 0);
    ~PerformanceWidget();
    void showInit();

protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void changeEvent(QEvent *event);
private:
    void widgetInit();
    void createToolBar();
    void dataInit();

    void closeEvent(QCloseEvent *e);
    void exportReport();

    void getTraffic(const QString &uid);
    QString getUid();
    void getNetState();
    void startService();
    void stopTraffic();
    void stopService();

    void compareBcak();

    void createBaseData();
    void createExcel();

    void getXXX();
    void getFrames();
    bool isActive();
    void getLunchTime();

    void getMemoryCpuIndex();

signals:
    void SendMemPlotSignal(QString numStr,double min,double max);
    void SendMemStopSignal();
    void SendCpuPlotSignal(QString numStr,double min,double max);
    void SendCpuStopSignal();

    void SendMemWriteEdit(QString numStr,QString time);
    void SendCpuWriteEdit(QString numStr,QString time);
    void SendBatteryWriteEdit(QString numStr,QString time);
    void SendCpuTempWriteEdit(QString numStr,QString time);

    void SendBatteryPlotSignal(QString numStr,double min,double max);
    void SendBatteryStopSignal();
    void SendCPUTempPlotSignal(QString numStr,double min,double max);
    void SendCPUTempStopSignal();

    void SendWifiPlotSignal(QString numStr,double min,double max);
    void SendMobilePlotSignal(QString numStr,double min,double max);

    void SendTrafficStopSignal();

    void sendStateChange(bool state);
    void sendSettingSig(QString memThres,QString cpuThres, QString batteryThres,QString cpuTempThres,QString wifiThres,QString mobileThres);

    void SendThresMax(QString memThres,QString cpuThres,QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres);


private slots:

    void ShowSetupWindow();
    void RecieveData(QVariant var);

    void getMemoryCpu();
    void getBatteryTemp();
    void getCPUTemp();

    void MemPlotChart(QString numStr,double min,double max);
    void RecieveMemStopSignal();
    void CpuPlotChart(QString numStr,double min,double max);

    void BatteryPlotChart(QString numStr, double min,double max);
    void CPUTempPlotChart(QString numStr, double min,double max);
    void RecieveBatteryStopSignal();
    void mouseMoveEvent(QMouseEvent *e);

    void RecieveDevicesSigal(QStringList devicesList);

    void on_equipBtn_clicked();

    void on_packageCombo_currentIndexChanged(const QString &arg1);

    void on_equipListView_clicked(const QModelIndex &index);

    void on_packageListView_clicked(const QModelIndex &index);

    void on_startBtn_clicked();

   // void on_exportBtn_clicked();

    void on_clearBtn_clicked();

    void on_reportDetailBtn_clicked();

    void WifiDataHandle(const QString &,const QString & );
    void MobileDataHandle(const QString &,const QString & );
    void WifiPlotChart(QString numStr, double min,double max);
    void MobilePlotChart(QString numStr, double min,double max);

    void MemPlotDetail();
    void CpuPlotDetail();
    void BatteryPlotDetail();
    void CpuTempPlotDetail();
    void WifiPlotDetail();
    void MobilePlotDetail();

    void RecieveTrafficStopSignal();


    void RecieveCompareData(QVector<QVector<QString>> qMemory_data_Vector, QVector<QVector<QString>> qMemory_time_Vector,
                            QVector<QVector<QString>> qCpu_data_Vector, QVector<QVector<QString>> qCpu_time_Vector,
                            QVector<QVector<QString>> qCputemp_data_Vector, QVector<QVector<QString>> qCputemp_time_Vector,
                            QVector<QVector<QString>> qBatterytemp_data_Vector, QVector<QVector<QString>> qBatterytemp_time_Vector,
                            QVector<QVector<QString> > qWifi_data_Vector, QVector<QVector<QString> > qWifi_time_Vector,
                            QVector<QVector<QString> > qMobile_data_Vector, QVector<QVector<QString> > qMobile_time_Vector);

    void on_dataCompareBtn_clicked();

    void on_dataCpmpareBtn_2_clicked();

    void RecieveWindowDeactive();

    void ReadStatisticsData(QStringList statistics);


    void receiveFramesResult(const QString&,const QString&);
    void receiveWorkerFinished();
    void receiveWorkerResult1(const QString&,const QString&);
    void receiveWorkerResult2(const QString&,const QString&);


private:
    Ui::PerformanceWidget *ui;

    QToolBar *toolBar;
    QToolButton *settingButton;
    QStringList equipList;
    QStringListModel *equipModel=NULL;
    QStringList packageList;
    QStringListModel *packageModel=NULL;

    QString deviceName;
    QString packageName;

    //从设置里传回来的数据
    int time;
    int flag;
    int flagGetMem;//获取内存时，可能会暂缓失去数据，判断三次都没数据才算设备丢失
    int flagGetCPU;//获取CPU时，可能会暂缓失去数据，判断三次都没数据才算设备丢失
    int flagError=false;//如果是因为设备连接出现错误，该标志位为true；在接受停止信号后会有弹框

    int flagGetCPUTotal;
    int flagGetMemTotal;
    int flagErrorNumBattery;
    int flagErrorNumCpu;
    int focusFlag;
    int numForPackageIndexChange=0;
    int numForDeviceSignal=0;
    int dSec;

    QTimer *timer=NULL;
    QThread *timerThread=NULL;

    QString strMemThres;
    QString strCpuThres;
    QString strBatteryThres;
    QString strCpuTempThres;
    QString strWifiThres;
    QString strMobileThres;


    PerformanceSetup *setupDialog;

    QString fileNameForReport;

    //开始/停止状态
    bool isStartFlag=false;

    bool isBatteryOverThres;
    bool isCpuTempOverThres;
    bool isWifiOverThres;
    bool isMobileOverThres;

    int memIndex=6;
    int cpuIndex=2;

    TrafficStatDetail *trafficDetail=NULL;
    performanceDetail *perforDetail=NULL;
    tempDetail *tempdetail=NULL;

    DataCompareWidget *dataWidget=NULL;

    bool compareModal=false;
    ChartDetail *chartDetail=NULL;

    QStringList xmlPaths;
    QString IP;
    QString port;
    bool isDebug;

    QDateTime startTime;
    QDateTime endTime;

    double memAve=0;
    double cpuAve=0;
    double batteryAve=0;
    double cpuTempAve=0;
    double wifiTrafficstats=0;
    double mobileTrafficstats=0;


    QString qStartTime;
    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qCpuTempWarningValue;
    QString qBatteryTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;
    QString qLunchTime;
    QString qFrames;
    QString qOldPackageName;
    QStringList qStatisticsDataList;

};

#endif // PERFORMANCEWIDGET_H
