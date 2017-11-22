#include "performance.h"

Performance::Performance(QObject* parent)
{
}
Performance::~Performance()
{
}


//*****************20170303*****************//
void Performance::setFirstLevelDirName(const QString &arg_firstLevelDirName)
{
    qFirstLevelDirName=arg_firstLevelDirName;
}

void Performance::setSecondLevelDirNameList(QStringList arg_secondLevelDirNameList)
{
    qSecondLevelDirNameList=arg_secondLevelDirNameList;
}

void Performance::setMemWarningValue(const QString &arg_warningValue)
{
    qMemWarningValue=arg_warningValue;
}

void Performance::setCpuWarningValue(const QString &arg_warningValue)
{
    qCpuWarningValue=arg_warningValue;
}

void Performance::setBatteryTempWarningValue(const QString &arg_warningValue)
{
    qBatteryTempWarningValue=arg_warningValue;
}

void Performance::setCpuTempWarningValue(const QString &arg_warningValue)
{
    qCpuTempWarningValue=arg_warningValue;
}

void Performance::setWifiWarningValue(const QString &arg_warningValue)
{
    qWifiWarningValue=arg_warningValue;
}

void Performance::setMobileWarningValue(const QString &arg_warningValue)
{
    qMobileWarningValue=arg_warningValue;
}
//*****************20170303*****************//


void Performance::setDeviceIdList(QStringList arg_deviceIdList)
{
    qDeviceIdList=arg_deviceIdList;
}

void Performance::setWriteResult(WriteResult *arg_writeResultInstance)
{
    qWriteResultInstance=arg_writeResultInstance;
    connect(this,SIGNAL( sendMinMax_memory(const QString &,int,int) ),qWriteResultInstance,SLOT( receiveMinMax_memory(const QString &,int,int) ) );
    connect(this,SIGNAL( sendMinMax_cpu(const QString &,int,int) ),qWriteResultInstance,SLOT( receiveMinMax_cpu(const QString &,int,int) ) );
    connect(this,SIGNAL( sendMinMax_battery(const QString &,int,int) ),qWriteResultInstance,SLOT( receiveMinMax_battery(const QString &,int,int) ) );
    connect(this,SIGNAL( sendMinMax_cpuTemp(const QString &,int,int) ),qWriteResultInstance,SLOT( receiveMinMax_cpuTemp(const QString &,int,int) ) );
    connect(this,SIGNAL( sendTraffic(const QString &,QHash<QString,QString>) ),qWriteResultInstance,SLOT( receiveTraffic(const QString &,QHash<QString,QString> ) ) );
}

void Performance::startWatcher()
{
    qNetState_Controller_Hash.clear();
    qMem_Controller_Hash.clear();
    qCpu_Controller_Hash.clear();
    qBatteryTemp_Controller_Hash.clear();
    qCpuTemp_Controller_Hash.clear();
    qTraffic_Controller_Hash.clear();


    gMin_Mem_Hash.clear();
    gMax_Mem_Hash.clear();

    gMin_Cpu_Hash.clear();
    gMax_Cpu_Hash.clear();

    gMin_Batterytemp_Hash.clear();
    gMax_Batterytemp_Hash.clear();


    gMin_Cputemp_Hash.clear();
    gMax_Cputemp_Hash.clear();

    gData_Traffic_Hash.clear();

    //*****************20170303*****************//
    qMemTime_vector.clear();
    qCpuTime_vector.clear();
    qCpuTempTime_vector.clear();
    qBatteryTempTime_vector.clear();
    qWifiTime_vector.clear();
    qMobileTime_vector.clear();

    gMemValue_list.clear();
    gCpuValue_list.clear();
    gCpuTempValue_list.clear();
    gBatteryTempValue_list.clear();
    qWifiValue_list.clear();
    qMobileValue_list.clear();

    qMemValue_vector.clear();
    qCpuValue_vector.clear();
    qCpuTempValue_vector.clear();
    qBatteryTempValue_vector.clear();
    qWifiValue_vector.clear();
    qMobileValue_vector.clear();


    qMem_EndTime=QDateTime::currentDateTime();
    qCpu_EndTime=QDateTime::currentDateTime();
    qCpuTemp_EndTime=QDateTime::currentDateTime();
    qBatteryTemp_EndTime=QDateTime::currentDateTime();
    qWifi_EndTime=QDateTime::currentDateTime();
    qMobile_EndTime=QDateTime::currentDateTime();

    qInterval=getInterval();
    //*****************20170303*****************//

    getPerformance();
}



void Performance::getPerformance()
{
    getNetState();
    getMemory();
    getCpu();
    getBatteryTemperature();
    getCpuTemperature();
    getTraffic();
}

void Performance::getNetState()
{
    QString cmdLine;
    foreach(QString deviceId,qDeviceIdList)
    {
        cmdLine="adb -s " + deviceId + " logcat -s TrafficService";
        NetStateController::Controller *controllerInstance=new NetStateController::Controller;
        qNetState_Controller_Hash.insert(deviceId,controllerInstance);

        connect(controllerInstance,SIGNAL(sendProcFinished(const QString &)),this,SLOT(receiveNetStateProcFinished(const QString &) ) );
        controllerInstance->setDeviceId(deviceId);
        controllerInstance->setCmdLine(cmdLine);
        controllerInstance->startController();
    }
}

void Performance::receiveNetStateProcFinished(const QString &arg_text)
{
    qProcFinishedDeviceId=arg_text;
}

void Performance::getMemory()
{
    QString cmdLine;
    QThreadPool *pool=new QThreadPool();
    foreach(QString deviceId,qDeviceIdList)
    {
        cmdLine="adb -s " + deviceId + " shell dumpsys meminfo " + gPackageName;

        MemController::Controller *controllerInstance=new MemController::Controller;

        qMem_Controller_Hash.insert(deviceId,controllerInstance);

        controllerInstance->setAutoDelete(true);
        controllerInstance->setDeviceId(deviceId);
        controllerInstance->setCmdLine(cmdLine);

        connect(controllerInstance,SIGNAL( sendResult(const QString &,const QString &,int,int) ),this,SLOT(receiveMemoryResult(const QString &,const QString &,int,int) )  ,Qt::DirectConnection  );

        pool->start(controllerInstance);
    }
}


void Performance::getCpu()
{
    QString cmdLine;
    QThreadPool *pool=new QThreadPool();    
    foreach(QString deviceId,qDeviceIdList)
    {
        cmdLine="cmd /c adb -s " + deviceId + " shell top -n 1 | findstr /E " + gPackageName;

        CpuController::Controller *controllerInstance=new CpuController::Controller;

        qCpu_Controller_Hash.insert(deviceId,controllerInstance);

        controllerInstance->setAutoDelete(false);
        controllerInstance->setDeviceId(deviceId);
        controllerInstance->setCmdLine(cmdLine);

        connect(controllerInstance, SIGNAL(sendResult(const QString &,const QString &,int,int)), this, SLOT(receiveCpuResult(const QString &,const QString &,int,int)) ,Qt::DirectConnection);

        pool->start(controllerInstance);
    }
}


void Performance::getBatteryTemperature()
{
    QString cmdLine;
    QThreadPool *pool=new QThreadPool();
    foreach(QString deviceId,qDeviceIdList)
    {
        cmdLine="cmd /c adb -s " + deviceId + " shell dumpsys battery | findstr temperature:";

        BatteryTempController::Controller *controllerInstance=new BatteryTempController::Controller;

        qBatteryTemp_Controller_Hash.insert(deviceId,controllerInstance);

        controllerInstance->setAutoDelete(true);
        controllerInstance->setDeviceId(deviceId);
        controllerInstance->setCmdLine(cmdLine);

        connect(controllerInstance, SIGNAL(sendResult(const QString &,const QString &,int,int)), this, SLOT(receiveBatteryTemperatureResult(const QString &,const QString &,int,int)),Qt::DirectConnection);

        pool->start(controllerInstance);
    }
}


void Performance::getCpuTemperature()
{
    QString cmdLine;
    QThreadPool *pool=new QThreadPool();
    foreach(QString deviceId,qDeviceIdList)
    {
        cmdLine="adb -s " + deviceId + " shell cat /sys/class/thermal/thermal_zone*/temp";

        CpuTempController::Controller *controllerInstance=new CpuTempController::Controller;

        qCpuTemp_Controller_Hash.insert(deviceId,controllerInstance);

        controllerInstance->setAutoDelete(true);
        controllerInstance->setDeviceId(deviceId);
        controllerInstance->setCmdLine(cmdLine);

        connect(controllerInstance, SIGNAL(sendResult(const QString &,const QString &,int,int)), this, SLOT(receiveCpuTemperatureResult(const QString &,const QString &,int,int)),Qt::DirectConnection);

        pool->start(controllerInstance);
    }

}


void Performance::getTraffic()
{
    QString uid;
    QString cmdLine;
    QThreadPool *pool=new QThreadPool();
    foreach(QString deviceId,qDeviceIdList)
    {
        uid=gUidHash.value(deviceId);
        if(uid.isEmpty())
        {
        }
        else
        {
            cmdLine="cmd /c adb -s " + deviceId + " shell cat /proc/net/xt_qtaguid/stats | findstr "+uid;
            TrafficController::Controller *controllerInstance=new TrafficController::Controller;

            qTraffic_Controller_Hash.insert(deviceId,controllerInstance);

            controllerInstance->setAutoDelete(true);
            controllerInstance->setDeviceId(deviceId);
            controllerInstance->setCmdLine(cmdLine);

            connect(controllerInstance, SIGNAL(sendResult(const QString &,const QString &)), this, SLOT(receiveTrafficResult(const QString &,const QString & )),Qt::DirectConnection);

            pool->start(controllerInstance);
        }
    }
}



void Performance::receiveMemoryResult(const QString &arg_deviceId,const QString &arg_currentValue,int arg_min,int arg_max)
{
    gMin_Mem_Hash.insert(arg_deviceId,arg_min);
    gMax_Mem_Hash.insert(arg_deviceId,arg_max);
    //*****************20170303*****************//
    getChartMem(arg_deviceId,arg_currentValue);
    //*****************20170303*****************//
}

void Performance::receiveCpuResult(const QString &arg_deviceId,const QString &arg_currentValue,int arg_min,int arg_max)
{
    gMin_Cpu_Hash.insert(arg_deviceId,arg_min);
    gMax_Cpu_Hash.insert(arg_deviceId,arg_max);
    //*****************20170303*****************//
    getChartCpu(arg_deviceId,arg_currentValue);
    //*****************20170303*****************//
}

void Performance::receiveBatteryTemperatureResult(const QString &arg_deviceId,const QString &arg_currentValue,int arg_min,int arg_max)
{
    gMin_Batterytemp_Hash.insert(arg_deviceId,arg_min);
    gMax_Batterytemp_Hash.insert(arg_deviceId,arg_max);
    //*****************20170303*****************//
    getChartBatteryTemp(arg_deviceId,arg_currentValue);
    //*****************20170303*****************//
}

void Performance::receiveCpuTemperatureResult(const QString &arg_deviceId,const QString &arg_currentValue,int arg_min,int arg_max)
{
    gMin_Cputemp_Hash.insert(arg_deviceId,arg_min);
    gMax_Cputemp_Hash.insert(arg_deviceId,arg_max);
    //*****************20170303*****************//
    getChartCpuTemp(arg_deviceId,arg_currentValue);
    //*****************20170303*****************//
}

void Performance::receiveTrafficResult(const QString &arg_deviceIdnetState,const QString &arg_text)
{
    gData_Traffic_Hash.insert(arg_deviceIdnetState,arg_text);
    //*****************20170303*****************//
    getWifiMobile(arg_deviceIdnetState,arg_text);
    //*****************20170303*****************//
}

void Performance::receiveStopPerformance(const QString &arg_deviceId)
{
    stopMemory(arg_deviceId);
    stopCpu(arg_deviceId);
    stopBattery(arg_deviceId);
    stopCputemp(arg_deviceId);
    stopTraffic(arg_deviceId);

    //*****************20170303*****************//
    createExcel(arg_deviceId);
    //*****************20170303*****************//
}

void Performance::stopMemory(const QString &arg_deviceId)
{
    QList<QString> keyList=qMem_Controller_Hash.keys();
    if(keyList.indexOf(arg_deviceId)==-1)
    {
    }
    else
    {
        qMem_Controller_Hash.value(arg_deviceId)->stopController();
        qMem_Controller_Hash.remove(arg_deviceId);
    }

    int minValue;
    int maxValue;

    minValue=gMin_Mem_Hash.value(arg_deviceId);
    maxValue=gMax_Mem_Hash.value(arg_deviceId);

    emit sendMinMax_memory(arg_deviceId,minValue,maxValue);
}

void Performance::stopCpu(const QString &arg_deviceId)
{
    QList<QString> keyList=qCpu_Controller_Hash.keys();
    if(keyList.indexOf(arg_deviceId)==-1)
    {
    }
    else
    {

        qCpu_Controller_Hash.value(arg_deviceId)->stopController();
        qCpu_Controller_Hash.remove(arg_deviceId);
    }

    int minValue;
    int maxValue;

    minValue=gMin_Cpu_Hash.value(arg_deviceId);
    maxValue=gMax_Cpu_Hash.value(arg_deviceId);

    emit sendMinMax_cpu(arg_deviceId,minValue,maxValue);
}

void Performance::stopBattery(const QString &arg_deviceId)
{
    QList<QString> keyList=qBatteryTemp_Controller_Hash.keys();
    if(keyList.indexOf(arg_deviceId)==-1)
    {
    }
    else
    {
        qBatteryTemp_Controller_Hash.value(arg_deviceId)->stopController();
        qBatteryTemp_Controller_Hash.remove(arg_deviceId);
    }

    int minValue;
    int maxValue;

    minValue=gMin_Batterytemp_Hash.value(arg_deviceId);
    maxValue=gMax_Batterytemp_Hash.value(arg_deviceId);

    emit sendMinMax_battery(arg_deviceId,minValue,maxValue);
}

void Performance::stopCputemp(const QString &arg_deviceId)
{
    QList<QString> keyList=qCpuTemp_Controller_Hash.keys();
    if(keyList.indexOf(arg_deviceId)==-1)
    {
    }
    else
    {
        qCpuTemp_Controller_Hash.value(arg_deviceId)->stopController();
        qCpuTemp_Controller_Hash.remove(arg_deviceId);
    }

    int minValue;
    int maxValue;

    minValue=gMin_Cputemp_Hash.value(arg_deviceId);
    maxValue=gMax_Cputemp_Hash.value(arg_deviceId);

    emit sendMinMax_cpuTemp(arg_deviceId,minValue,maxValue);

}

void Performance::stopTraffic(const QString &arg_deviceId)
{
    QList<QString> keyList=qTraffic_Controller_Hash.keys();
    if(keyList.indexOf(arg_deviceId)==-1)
    {
    }
    else
    {
        qTraffic_Controller_Hash.value(arg_deviceId)->stopController();
        qTraffic_Controller_Hash.remove(arg_deviceId);
    }
    emit sendTraffic(arg_deviceId,gData_Traffic_Hash);
}

void Performance::stopNetState(const QString &arg_deviceId)
{
    QList<QString> keyList=qNetState_Controller_Hash.keys();
    if(keyList.indexOf(arg_deviceId)==-1)
    {
    }
    else
    {
        if(arg_deviceId==qProcFinishedDeviceId)
        {
            qNetState_Controller_Hash.remove(arg_deviceId);
        }
        else
        {
            qNetState_Controller_Hash.value(arg_deviceId)->stopController();
            qNetState_Controller_Hash.remove(arg_deviceId);
        }
        qProcFinishedDeviceId="";
    }

}




void Performance::receiveClearPerformance()
{
}



void Performance::testSlot()
{

}

//*****************20170303*****************//
void Performance::getChartMem(const QString&arg_deviceId,const QString&arg_currentValue)
{
    qMem_CurrentTime=qMem_EndTime.addSecs(qInterval);
    qMem_EndTime=qMem_CurrentTime;
    qTimeStr=qMem_CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
    qMemTime_vector.append(qTimeStr);

    gMemValue_list.append(arg_deviceId + "->" + arg_currentValue);
}

void Performance::getChartCpu(const QString&arg_deviceId,const QString&arg_currentValue)
{
    qCpu_CurrentTime=qCpu_EndTime.addSecs(qInterval);
    qCpu_EndTime=qCpu_CurrentTime;
    qTimeStr=qCpu_CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
    qCpuTime_vector.append(qTimeStr);

    gCpuValue_list.append(arg_deviceId + "->" + arg_currentValue);
}

void Performance::getChartCpuTemp(const QString&arg_deviceId,const QString&arg_currentValue)
{
    qCpuTemp_CurrentTime=qCpuTemp_EndTime.addSecs(qInterval);
    qCpuTemp_EndTime=qCpuTemp_CurrentTime;
    qTimeStr=qCpuTemp_CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
    qCpuTempTime_vector.append(qTimeStr);

    gCpuTempValue_list.append(arg_deviceId + "->" + arg_currentValue);
}

void Performance::getChartBatteryTemp(const QString&arg_deviceId,const QString&arg_currentValue)
{
    qBatteryTemp_CurrentTime=qBatteryTemp_EndTime.addSecs(qInterval);
    qBatteryTemp_EndTime=qBatteryTemp_CurrentTime;
    qTimeStr=qBatteryTemp_CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
    qBatteryTempTime_vector.append(qTimeStr);

    gBatteryTempValue_list.append(arg_deviceId + "->" + arg_currentValue);
}

void Performance::getWifiMobile(const QString &arg_deviceId,const QString&arg_currentValue)
{
    QString deviceId;
    QString wifiValue;
    QString mobileValue;
    QStringList splitResult;

    splitResult=arg_deviceId.split("=");
    deviceId=splitResult.at(0);

    splitResult=arg_currentValue.split("=");
    wifiValue=splitResult.at(0);
    mobileValue=splitResult.at(2);

    if(wifiValue!="0.00")
    {
        qWifi_CurrentTime=qWifi_EndTime.addSecs(qInterval);
        qWifi_EndTime=qWifi_CurrentTime;
        qTimeStr=qWifi_CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
        qWifiTime_vector.append(qTimeStr);
        qWifiValue_list.append(deviceId + "->" + wifiValue);
    }
    if(mobileValue!="0.00")
    {
        qMobile_CurrentTime=qMobile_EndTime.addSecs(qInterval);
        qMobile_EndTime=qMobile_CurrentTime;
        qTimeStr=qMobile_CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
        qMobileTime_vector.append(qTimeStr);
        qMobileValue_list.append(deviceId + "->" + mobileValue);
    }
}

int Performance::getInterval()
{
    bool isOK;
    int d=gInterval.toInt(&isOK,10);
    if(isOK)
    {
    }
    else
    {
        d=6;
    }
    return d;
}


void Performance::createExcel(const QString &arg_deviceId)
{
    getValues(arg_deviceId);
    mkExcelDir(arg_deviceId);
    chart();
    performanceHtml();
}

void Performance::getValues(const QString &arg_deviceId)
{
    double doubleValue;
    QString tempStr;
    QStringList tempList;
    QStringList splitResult;

    tempList=gMemValue_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        doubleValue=tempStr.toDouble();
        qMemValue_vector.append(doubleValue);
    }

    tempList=gCpuValue_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        doubleValue=tempStr.toDouble();
        qCpuValue_vector.append(doubleValue);
    }

    tempList=gCpuTempValue_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        doubleValue=tempStr.toDouble();
        qCpuTempValue_vector.append(doubleValue);
    }

    tempList=gBatteryTempValue_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        doubleValue=tempStr.toDouble();
        qBatteryTempValue_vector.append(doubleValue);
    }

    tempList=qWifiValue_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        doubleValue=tempStr.toDouble();
        qWifiValue_vector.append(doubleValue);
    }

    tempList=qMobileValue_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        doubleValue=tempStr.toDouble();
        qMobileValue_vector.append(doubleValue);
    }

}


void Performance::mkExcelDir(const QString &arg_deviceId)
{
    QString secondDirName;
    QStringList secondDirNameList;

    secondDirNameList=qSecondLevelDirNameList.filter(arg_deviceId);
    secondDirName=secondDirNameList.at(0);
//D:\princekinWorkspace\report\20170927\com.sohu.qianfan\sohuvideoreport_20170927_155913
    qSaveDir=gWorkSpace +  QDir::separator() + "report" + QDir::separator() + qFirstLevelDirName + QDir::separator() + secondDirName + QDir::separator() + "performance";
    qSaveDir=gDetailedPath + QDir::separator() + secondDirName + QDir::separator() + "performance";
}

void Performance::chart()
{
    QThreadPool *pool=new QThreadPool;

    DataController::Controller *controllerInstance=new DataController::Controller;

    controllerInstance->setAutoDelete(true);
    controllerInstance->setXlsx(qSaveDir + QDir::separator() + "report.xlsx");

    controllerInstance->setMemTime_Vector(qMemTime_vector);
    controllerInstance->setCpuTime_Vector(qCpuTime_vector);
    controllerInstance->setCpuTempTime_Vector(qCpuTempTime_vector);
    controllerInstance->setBatteryTempTime_Vector(qBatteryTempTime_vector);
    controllerInstance->setWifiTime_Vector(qWifiTime_vector);
    controllerInstance->setMobileTime_Vector(qMobileTime_vector);


    controllerInstance->setMemWarningValue(qMemWarningValue);
    controllerInstance->setCpuWarningValue(qCpuWarningValue);
    controllerInstance->setCpuTempWarningValue(qCpuTempWarningValue);
    controllerInstance->setBatteryTempWarningValue(qBatteryTempWarningValue);
    controllerInstance->setWifiWarningValue(qWifiWarningValue);
    controllerInstance->setWifiWarningValue(qMobileWarningValue);

    controllerInstance->setMemory_Vector(qMemValue_vector);
    controllerInstance->setCpu_Vector(qCpuValue_vector);
    controllerInstance->setCpuTemp_Vector(qCpuTempValue_vector);
    controllerInstance->setBatteryTemp_Vector(qBatteryTempValue_vector);
    controllerInstance->setWifi_Vector(qWifiValue_vector);
    controllerInstance->setMobile_Vector(qMobileValue_vector);

    connect(controllerInstance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveDataWorkerResult(const QString &)) ,Qt::DirectConnection  );

    pool->start(controllerInstance);
}

void Performance::receiveDataWorkerResult(const QString &arg_result)
{
    QStringList cmdList;
    QString cmdLine;
    QString jar;
    QString reportxlsx;
    QString sheet;
    QString mark;

    jar=gConfigDir + QDir::separator() + "chart.jar";
    reportxlsx=qSaveDir + QDir::separator() + "report.xlsx";

    sheet="memory";
    mark="memory";
    cmdLine="java -jar " + jar + " " + reportxlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);


    sheet="cpu";
    mark="cpu";
    cmdLine="java -jar " + jar + " " + reportxlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);

    sheet="cputemp";
    mark="cputemp";
    cmdLine="java -jar " + jar + " " + reportxlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);


    sheet="batterytemp";
    mark="batterytemp";
    cmdLine="java -jar " + jar + " " + reportxlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);

    sheet="wifi";
    mark="wifi";
    cmdLine="java -jar " + jar + " " + reportxlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);


    sheet="mobile";
    mark="mobile";
    cmdLine="java -jar " + jar + " " + reportxlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);

    QThreadPool *pool=new QThreadPool;
    ChartController::Controller *controllerInstance=new ChartController::Controller;
    controllerInstance->setAutoDelete(true);
    controllerInstance->setCmdList(cmdList);
    connect(controllerInstance,SIGNAL(sendChartWorkerResult(const QString &)),this,SLOT(receiveChartWorkerResult(const QString &)) ,Qt::DirectConnection );
    pool->start(controllerInstance);
}

void Performance::receiveChartWorkerResult(const QString &arg_result)
{
}


void Performance::performanceHtml()
{
    QString htmlDir=qSaveDir + QDir::separator() + "html";
    Helper::createPath(htmlDir);

    QThreadPool *pool=new QThreadPool;
    ReplayHtmlController::Controller *controllerInstance=new ReplayHtmlController::Controller;
    controllerInstance->setAutoDelete(true);
    controllerInstance->setSaveDir(qSaveDir,htmlDir);
    controllerInstance->setWarningValue(qMemWarningValue,qCpuWarningValue,qCpuTempWarningValue,qBatteryTempWarningValue,qWifiWarningValue,qMobileWarningValue);
    controllerInstance->setTime_vector(qMemTime_vector,qCpuTime_vector,qCpuTempTime_vector,qBatteryTempTime_vector,qWifiTime_vector,qMobileTime_vector);
    controllerInstance->setValue_vector(qMemValue_vector,qCpuValue_vector,qCpuTempValue_vector,qBatteryTempValue_vector,qWifiValue_vector,qMobileValue_vector);

    connect(controllerInstance,SIGNAL(sendControllerfinished(const QString &)),this,SLOT(receiveControllerfinished(const QString &))   ,Qt::DirectConnection);
    pool->start(controllerInstance);
}

void Performance::receiveControllerfinished(const QString &arg_text)
{
    emit sendHtmlFinished(arg_text);
}

//*****************20170606*****************//
