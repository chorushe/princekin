#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "memcontroller.h"
#include "cpucontroller.h"
#include "batterytempcontroller.h"
#include "cputempcontroller.h"
#include "netstatecontroller.h"
#include "trafficcontroller.h"
#include "execmd.h"
#include "writeresult.h"
#include "htmlinfo.h"
#include "replayhtml.h"
#include "helper.h"
#include "chartcontroller.h"
#include "datacontroller.h"
#include "globalvar.h"
#include "replayhtmlcontroller.h"


#include <QFileInfo>
#include <QStringList>
#include <QTime>
#include <QVector>
#include <QMessageBox>

using namespace MemController;
using namespace CpuController;
using namespace BatteryTempController;
using namespace CpuTempController;
using namespace TrafficController;
using namespace NetStateController;
using namespace ReplayHtmlController;

class Performance : public QObject
{
    Q_OBJECT
public:
    Performance(QObject* parent=0);
    ~Performance();

public:
    void setMemWarningValue(const QString&);
    void setCpuWarningValue(const QString&);
    void setBatteryTempWarningValue(const QString&);
    void setCpuTempWarningValue(const QString&);
    void setWifiWarningValue(const QString&);
    void setMobileWarningValue(const QString&);

    void setDeviceIdList(QStringList);
    void setWriteResult(WriteResult*);
    void startWatcher();
    void getPerformance();

    //*****************20170303*****************//
    void setFirstLevelDirName(const QString&);
    void setSecondLevelDirNameList(QStringList);
    //*****************20170303*****************//

private:
    void getMemory();
    void getCpu();
    void getBatteryTemperature();
    void getCpuTemperature();
    void getTraffic();
    void initTrafficstatsEnv();
    void getNetState();


    void stopMemory(const QString&);
    void stopCpu(const QString&);
    void stopBattery(const QString&);
    void stopCputemp(const QString&);
    void stopTraffic(const QString&);
    void stopNetState(const QString&);



    void startCpuWorker(const QString&,const QString&);
    void startBatteryTempWorker(const QString&,const QString&);
    void startCpuTempWorker(const QString&,const QString&);
    void startTrafficWorker(const QString&,const QString&);

    //*****************20170303*****************//
    void getChartMem(const QString&,const QString&);
    void getChartCpu(const QString&,const QString&);
    void getChartBatteryTemp(const QString&,const QString&);
    void getChartCpuTemp(const QString&,const QString&);
    void getWifiMobile(const QString&,const QString&);

    void createExcel(const QString&);
    void getValues(const QString&);
    void mkExcelDir(const QString&);
    void chart();
    int getInterval();
    //*****************20170303*****************//

    //*****************20170606*****************//
    void performanceHtml();
    //*****************20170606*****************//

signals:
    void sendMinMax_memory(const QString &,int,int);
    void sendMinMax_cpu(const QString &,int,int);
    void sendMinMax_battery(const QString &,int,int);
    void sendMinMax_cpuTemp(const QString &,int,int);
    void sendTraffic(const QString &,QHash<QString,QString>);
    void sendHtmlFinished(const QString &);


private slots:
    void receiveMemoryResult(const QString &,const QString &,int,int);
    void receiveCpuResult(const QString &,const QString &,int,int);
    void receiveBatteryTemperatureResult(const QString &,const QString &,int,int);
    void receiveCpuTemperatureResult(const QString &,const QString &,int,int);    
    void receiveTrafficResult(const QString &,const QString &);

    void receiveStopPerformance(const QString &);
    void receiveClearPerformance();
    void testSlot();
    void receiveNetStateProcFinished(const QString &);

    void receiveDataWorkerResult(const QString &);
    void receiveChartWorkerResult(const QString &);
    void receiveControllerfinished(const QString &);

private:
    bool firstMemWarning;


    QString qProcFinishedDeviceId;

    QStringList qDeviceIdList;

    WriteResult *qWriteResultInstance;


    QHash<QString,NetStateController::Controller*> qNetState_Controller_Hash;
    QHash<QString,MemController::Controller*> qMem_Controller_Hash;
    QHash<QString,CpuController::Controller*> qCpu_Controller_Hash;
    QHash<QString,BatteryTempController::Controller*> qBatteryTemp_Controller_Hash;
    QHash<QString,CpuTempController::Controller*> qCpuTemp_Controller_Hash;
    QHash<QString,TrafficController::Controller*> qTraffic_Controller_Hash;

    QHash<QString,int> qMin_Mem_Hash;
    QHash<QString,int> qMax_Mem_Hash;

    QHash<QString,int> qMin_Cpu_Hash;
    QHash<QString,int> qMax_Cpu_Hash;

    QHash<QString,int> qMin_Batterytemp_Hash;
    QHash<QString,int> qMax_Batterytemp_Hash;

    QHash<QString,int> qMin_Cputemp_Hash;
    QHash<QString,int> qMax_Cputemp_Hash;

    QHash<QString,QString> qData_Traffic_Hash;

    QHash<QString,QThreadPool*> qp;





    //***************************************
    QString qSaveDir;
    QString qTimeStr;
    QString qFirstLevelDirName;
    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qBatteryTempWarningValue;
    QString qCpuTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;

    QStringList qSecondLevelDirNameList;

    QVector<QString> qMemTime_vector;
    QVector<QString> qCpuTime_vector;
    QVector<QString> qCpuTempTime_vector;
    QVector<QString> qBatteryTempTime_vector;
    QVector<QString> qWifiTime_vector;
    QVector<QString> qMobileTime_vector;

    QStringList qMemValue_list;
    QStringList qCpuValue_list;
    QStringList qCpuTempValue_list;
    QStringList qBatteryTempValue_list;
    QStringList qWifiValue_list;
    QStringList qMobileValue_list;

    QVector<double> qMemValue_vector;
    QVector<double> qCpuValue_vector;
    QVector<double> qCpuTempValue_vector;
    QVector<double> qBatteryTempValue_vector;
    QVector<double> qWifiValue_vector;
    QVector<double> qMobileValue_vector;


    QDateTime qEndTime;
    QDateTime qCurrentTime;
    QDateTime qMem_EndTime;
    QDateTime qMem_CurrentTime;
    QDateTime qCpu_EndTime;
    QDateTime qCpu_CurrentTime;
    QDateTime qCpuTemp_EndTime;
    QDateTime qCpuTemp_CurrentTime;
    QDateTime qBatteryTemp_EndTime;
    QDateTime qBatteryTemp_CurrentTime;
    QDateTime qWifi_EndTime;
    QDateTime qWifi_CurrentTime;
    QDateTime qMobile_EndTime;
    QDateTime qMobile_CurrentTime;

    int qInterval;
};

#endif // PERFORMANCE_H
