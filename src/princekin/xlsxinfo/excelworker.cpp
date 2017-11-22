#include "excelworker.h"

namespace ExcelWorker
{

Worker::Worker()
{
}
Worker::~Worker()
{
}

//*******************通用********************//
void Worker::setMark(const QString &arg_text)
{
    qMark=arg_text;
}

void Worker::setXlsxSaveDir(const QString &arg_text)
{
    qXlsxSaveDir=arg_text;
}

void Worker::setStartTime(const QString &arg_text)
{
    qStartTime=arg_text;
}

void Worker::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void Worker::setLunchTime(const QString &arg_text)
{
    qLaunchTime=arg_text;
}

void Worker::setFrames(const QString &arg_text)
{
    qFrames=arg_text;
}

void Worker::setMemWarningValue(const QString &arg_text)
{
    qMemWarningValue=arg_text;
}

void Worker::setCpuWarningValue(const QString &arg_text)
{
    qCpuWarningValue=arg_text;
}

void Worker::setCpuTempWarningValue(const QString &arg_text)
{
    qCpuTempWarningValue=arg_text;
}

void Worker::setBatteryTempWarningValue(const QString &arg_text)
{
    qBatteryTempWarningValue=arg_text;
}

void Worker::setWifiWarningValue(const QString &arg_text)
{
    qWifiWarningValue=arg_text;
}

void Worker::setMobileWarningValue(const QString &arg_text)
{
    qMobileWarningValue=arg_text;
}

//*******************通用*******************//

//*******************遍历*******************//
void Worker::setTravelStartTime(const QString &arg_text)
{
    qTravelStartTime=arg_text;
}

void Worker::setTravelEndTime(const QString &arg_text)
{
    qTravelEndTime=arg_text;
}

void Worker::setTravelTime(const QString &arg_text)
{
    qTravelTime=arg_text;
}

void Worker::setCheckNumber(const QString &arg_text)
{
    qCheckNumber=arg_text;
}

void Worker::setClickNumber(const QString &arg_text)
{
    qClickNumber=arg_text;
}

void Worker::setErrorNumber(const QString &arg_text)
{
    qErrorNumber=arg_text;
}

void Worker::setTravelActivityNumber(const QString &arg_text)
{
    qTravelActivityNumber=arg_text;
}
//*******************遍历*******************//


//*******************统计*******************//
void Worker::setUrlTotalNum(int arg_num)
{
    qUrlTotalNum=arg_num;
}

void Worker::setUrlErrorNum(int arg_num)
{
    qUrlErrorNum=arg_num;
}

void Worker::setUrlSuccessNum(int arg_num)
{
    qUrlSuccessNum=arg_num;
}
//*******************统计*******************//

//*******************monkey*******************//
void Worker::setMonkeyTime(const QString &arg_text)
{
    qMonkeyTime=arg_text;
}
//*******************monkey*******************//


void Worker::setPackageName(const QString &arg_text)
{
    qPackageName=arg_text;
}

void Worker::setStatiList(QStringList arg_list)
{
    qStatisticsDataList=arg_list;
}

void Worker::setMeasureVector(QVector<MeasureReport> arg_vector)
{
    qMeasureVector=arg_vector;
}


void Worker::doWork()
{
    qDebug()<<"nnnnnnnnnnnnnnnnnnnnnnnnnn";
    qDebug()<<qMark;
    if(qMark.startsWith("base-"))
    {
        QStringList splitResult=qMark.split("-");
        createBaseExcel(splitResult.at(1));
    }
    else
    {
        createExcel(qMark);
    }
}


void Worker::createExcel(const QString &arg_mark)
{
    WriteInfo::setXlsxSaveDir(qXlsxSaveDir);
    if(arg_mark=="travel")
    {
        getAppInfo1(arg_mark);
        setAppInfo_universal();
        setAppInfo_travel();
        writeInfo(arg_mark);

        getValues(arg_mark);
        chart();
    }
    if(arg_mark=="behaviour")
    {
        getAppInfo1(arg_mark);
        setAppInfo_universal();
        setAppInfo_behaviour();
        writeInfo(arg_mark);

        getValues(arg_mark);
        chart();
    }

    if(arg_mark=="monkey")
    {
        getAppInfo2(arg_mark);
        setAppInfo_universal();
        setAppInfo_monkey();
        writeInfo(arg_mark);

        getValues(arg_mark);
        chart();
    }

    else if(arg_mark=="interface")
    {
        getAppInfo1(arg_mark);
        setAppInfo_universal();
        writeInfo(arg_mark);

        getValues(arg_mark);
        chart();
    }

    else if(arg_mark=="performance")
    {
        getAppInfo2(arg_mark);
        setAppInfo_universal();
        writeInfo(arg_mark);
        performanceHtml();
        chart2();
    }

}


void Worker::setAppInfo_universal()
{
    WriteInfo::setIconPath(qIconPath);
    WriteInfo::setAppChineseName(qAppChineseName);
    WriteInfo::setAppVersion(qAppVersion);
    WriteInfo::setAppSize(qAppSize);
    WriteInfo::setLaunchTime(qLaunchTime);
    WriteInfo::setPlatform(qPlatform);
    WriteInfo::setTestDate(qTestDate);
    WriteInfo::setMobileBrand(qMobileBrand);
    WriteInfo::setMobileModel(qMobileModel);
    WriteInfo::setMobileVersion(qMobileVersion);
    WriteInfo::setMobileVmsize(qMobileVmsize);

    WriteInfo::setAppSize2(qAppSize2);
    WriteInfo::setFrames(qFrames);

    int len;
    len=qMeasureVector.size();

    if(len==0)
    {
        for(int i=0;i<4;i++)
        {
            qMemInfoList.append("");
        }

        for(int i=0;i<4;i++)
        {
            qCpuInfoList.append("");
        }

        for(int i=0;i<4;i++)
        {
            qBatteryInfoList.append("");
        }

        for(int i=0;i<4;i++)
        {
            qCpuTempInfoList.append("");
        }

        qAllWifi=0;
        qAllMobile=0;
    }
    else
    {
        for(int i=0;i<4;i++)
        {
            qMemInfoList.append(QString::number(qMeasureVector.at(len-1).memInfo[i]));
        }

        for(int i=0;i<4;i++)
        {
            qCpuInfoList.append(QString::number(qMeasureVector.at(len-1).cpuInfo[i]));
        }

        for(int i=0;i<4;i++)
        {
            qBatteryInfoList.append(QString::number(qMeasureVector.at(len-1).batteryInfo[i]));
        }

        for(int i=0;i<4;i++)
        {
            qCpuTempInfoList.append(QString::number(qMeasureVector.at(len-1).cpuTempInfo[i]));
        }

        qAllWifi=qMeasureVector.at(len-1).wifiTrafficstats;
        qAllMobile=qMeasureVector.at(len-1).mobileTrafficstats;

    }

    WriteInfo::setMem(qMemInfoList);
    WriteInfo::setCpu(qCpuInfoList);
    WriteInfo::setBattery(qBatteryInfoList);
    WriteInfo::setCpuTemp(qCpuTempInfoList);
    WriteInfo::setAllWifi(qAllWifi);
    WriteInfo::setAllMobile(qAllMobile);

    if(!qStatisticsDataList.isEmpty())
    {
        WriteInfo::setStatiList(qStatisticsDataList);
    }

}



void Worker::setAppInfo_travel()
{
    WriteInfo::setTravelStartTime(qTravelStartTime);
    WriteInfo::setTravelEndTime(qTravelEndTime);
    WriteInfo::setTravelTime(qTravelTime);
    WriteInfo::setCheckNumber(qCheckNumber);
    WriteInfo::setClickNumber(qClickNumber);
    WriteInfo::setErrorNumber(qErrorNumber);
    WriteInfo::setTravelActivityNumber(qTravelActivityNumber);
}

void Worker::setAppInfo_behaviour()
{
    WriteInfo::setUrlTotalNum(qUrlTotalNum);
    WriteInfo::setUrlErrorNum(qUrlErrorNum);
    WriteInfo::setUrlSuccessNum(qUrlSuccessNum);
}

void Worker::setAppInfo_monkey()
{
    WriteInfo::setMonkeyTime(qMonkeyTime);
}


void Worker::writeInfo(const QString &arg_mark)
{
    //WriteInfo::writeUrl(qTargetXlsx);
    WriteInfo::writeUniversal(qTargetXlsx,arg_mark,qDeviceId);
}



void Worker::getValues(const QString &arg_mark)
{
    int len;
    len=qMeasureVector.size();

    qMemTime_vector=qMeasureVector.at(len-1).memTime;
    qCpuTime_vector=qMeasureVector.at(len-1).cpuTime;
    qCpuTempTime_vector=qMeasureVector.at(len-1).cpuTempTime;
    qBatteryTempTime_vector=qMeasureVector.at(len-1).batteryTime;
    qWifiTime_vector=qMeasureVector.at(len-1).wifiTime;
    qMobileTime_vector=qMeasureVector.at(len-1).mobileTime;

    qMemValue_vector=qMeasureVector.at(len-1).memData;
    qCpuValue_vector=qMeasureVector.at(len-1).cpuData;
    qCpuTempValue_vector=qMeasureVector.at(len-1).cpuTempData;
    qBatteryTempValue_vector=qMeasureVector.at(len-1).batteryData;
    qWifiValue_vector=qMeasureVector.at(len-1).wifiData;
    qMobileValue_vector=qMeasureVector.at(len-1).mobileData;



    //*****************20170606**********//
    HtmlInfo html;
    QString htmlDir=qXlsxSaveDir + QDir::separator() + "html";
    Helper::createPath(htmlDir);
    html.setSaveDir(qXlsxSaveDir,htmlDir);

    html.setWarningValue(qMemWarningValue,qCpuWarningValue,qCpuTempWarningValue,qBatteryTempWarningValue,qWifiWarningValue,qMobileWarningValue);
    html.setTime_vector(qMemTime_vector,qCpuTime_vector,qCpuTempTime_vector,qBatteryTempTime_vector,qWifiTime_vector,qMobileTime_vector);
    html.setValue_vector(qMemValue_vector,qCpuValue_vector,qCpuTempValue_vector,qBatteryTempValue_vector,qWifiValue_vector,qMobileValue_vector);

    html.createHtml();
    //*****************20170606**********//
}



void Worker::chart()
{
    QThreadPool *pool=new QThreadPool;
    using DataController::Controller;
    Controller *controllerInstance=new Controller;

    controllerInstance->setAutoDelete(true);

    controllerInstance->setXlsx(qTargetXlsx);
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
    controllerInstance->setMobileWarningValue(qMobileWarningValue);

    controllerInstance->setMemory_Vector(qMemValue_vector);
    controllerInstance->setCpu_Vector(qCpuValue_vector);
    controllerInstance->setCpuTemp_Vector(qCpuTempValue_vector);
    controllerInstance->setBatteryTemp_Vector(qBatteryTempValue_vector);
    controllerInstance->setWifi_Vector(qWifiValue_vector);
    controllerInstance->setMobile_Vector(qMobileValue_vector);

    connect(controllerInstance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveDataWorkerResult(const QString &))   );

    pool->start(controllerInstance);
}

void Worker::chart2()
{
    QThreadPool *pool=new QThreadPool;
    using DataController::Controller;
    Controller *instance=new Controller;

    instance->setAutoDelete(true);
    instance->setXlsx(qTargetXlsx);
    instance->setMemWarningValue(qMemWarningValue);
    instance->setCpuWarningValue(qCpuWarningValue);
    instance->setCpuTempWarningValue(qCpuTempWarningValue);
    instance->setBatteryTempWarningValue(qBatteryTempWarningValue);
    instance->setWifiWarningValue(qWifiWarningValue);
    instance->setMobileWarningValue(qMobileWarningValue);

    instance->setMemTime_Vector(memTime);
    instance->setCpuTime_Vector(cpuTime);
    instance->setCpuTempTime_Vector(cpuTempTime);
    instance->setBatteryTempTime_Vector(batteryTime);
    instance->setWifiTime_Vector(wifiTime);
    instance->setMobileTime_Vector(mobileTime);

    instance->setMemory_Vector(memNum);
    instance->setCpu_Vector(cpuNum);
    instance->setCpuTemp_Vector(cpuTempNum);
    instance->setBatteryTemp_Vector(batteryNum);
    instance->setWifi_Vector(wifiNum);
    instance->setMobile_Vector(mobileNum);

    connect(instance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveDataWorkerResult(const QString &)));

    pool->start(instance);
}

void Worker::receiveDataWorkerResult(const QString &arg_result)
{
    QStringList cmdList;
    QString cmdLine;
    QString jar;
    QString sheet;
    QString mark;

    jar=gConfigDir + QDir::separator() + "chart.jar";

    sheet="memory";
    mark="memory";
    cmdLine="java -jar " + jar + " " + qTargetXlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);


    sheet="cpu";
    mark="cpu";
    cmdLine="java -jar " + jar + " " + qTargetXlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);

    sheet="cputemp";
    mark="cputemp";
    cmdLine="java -jar " + jar + " " + qTargetXlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);


    sheet="batterytemp";
    mark="batterytemp";
    cmdLine="java -jar " + jar + " " + qTargetXlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);

    sheet="wifi";
    mark="wifi";
    cmdLine="java -jar " + jar + " " + qTargetXlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);


    sheet="mobile";
    mark="mobile";
    cmdLine="java -jar " + jar + " " + qTargetXlsx + " " + sheet + " " + mark;
    cmdList.append(cmdLine);


    using ChartWorker::Worker;
    Worker *workerInstance=new Worker();
    connect(workerInstance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveChartWorkerResult(const QString &))  ,Qt::DirectConnection  );
    workerInstance->setCmdList(cmdList);
    workerInstance->startWorker();
}

void Worker::receiveChartWorkerResult(const QString &arg_result)
{
    emit sendWorkerFinished();
    sendMail();
}

void Worker::getAppInfo1(const QString &arg_mark)
{
    QString apkName;
    QString filePath;
    QString sourceXlsx;

    sourceXlsx=gConfigDir + QDir::separator() + arg_mark + "_report.xlsx";
    qTargetXlsx=qXlsxSaveDir + QDir::separator() + "report_" + qStartTime + ".xlsx";

    XlsxInfo::copyFile(sourceXlsx,qTargetXlsx);

    if(qPackageName==gOldPackageName)
    {
        apkName=XlsxInfo::getApkName();

        filePath=gApkDir + QDir::separator() + apkName;
        QFile file(filePath);

        if(file.exists())
        {
            qIconResPath=XlsxInfo::getIconResPath(apkName);
            qIconPath=XlsxInfo::getIconPath(apkName,qIconResPath);
            qAppChineseName=XlsxInfo::getAppChineseName(apkName);
            qAppVersion=XlsxInfo::getAppVersion(apkName);
            qAppSize=XlsxInfo::getAppSize(apkName);
        }
        else
        {
            qIconPath="";
            qAppChineseName="";
            qAppVersion="";
            qAppSize="";
        }
        getXXX();
    }
    else
    {
        qIconPath="";
        qAppChineseName=qPackageName;
        qAppVersion="";
        qAppSize="";
    }

    //qLaunchTime="0";
    qTestDate=XlsxInfo::getDate();
    qPlatform=XlsxInfo::getPlatform();

    qMobileBrand=XlsxInfo::getMobileBrand(qDeviceId);
    qMobileModel=XlsxInfo::getMobileModel(qDeviceId);
    qMobileVersion=XlsxInfo::getMobileVersion(qDeviceId);
    qMobileVmsize=XlsxInfo::getMobileVmsize(qDeviceId);


}


void Worker::getAppInfo2(const QString &arg_mark)
{
    QString sourceXlsx;

    sourceXlsx=gConfigDir + QDir::separator() + arg_mark + "_report.xlsx";
    qTargetXlsx=qXlsxSaveDir + QDir::separator() + "report_" + qStartTime + ".xlsx";
    XlsxInfo::copyFile(sourceXlsx,qTargetXlsx);

    if(qPackageName=="")
    {
        //没有选择应用
        qIconPath="";//手机
        qAppChineseName="";
        qAppVersion="";
        qAppSize="";
        qAppSize2="";
    }
    else
    {
        //如果选择应用
        if(qPackageName!=gOldPackageName)
        {
            qIconPath="";
            qAppChineseName=qPackageName;
            qAppVersion="";
            qAppSize="";
            qAppSize2="";
        }
        else
        {
            QString apkName;
            QString filePath;

            apkName=XlsxInfo::getApkName();
            filePath=gApkDir + QDir::separator() + apkName;
            QFile file(filePath);

            if(file.exists())
            {
                qIconResPath=XlsxInfo::getIconResPath(apkName);
                qIconPath=XlsxInfo::getIconPath(apkName,qIconResPath);
                qAppChineseName=XlsxInfo::getAppChineseName(apkName);
                qAppVersion=XlsxInfo::getAppVersion(apkName);
                qAppSize=XlsxInfo::getAppSize(apkName);
            }
            else
            {
                qIconPath="";
                qAppChineseName="";
                qAppVersion="";
                qAppSize="";
            }
            getXXX();
        }
    }


    //qLaunchTime="0";
    qTestDate=XlsxInfo::getDate();
    qPlatform=XlsxInfo::getPlatform();

    qMobileBrand=XlsxInfo::getMobileBrand(qDeviceId);
    qMobileModel=XlsxInfo::getMobileModel(qDeviceId);
    qMobileVersion=XlsxInfo::getMobileVersion(qDeviceId);
    qMobileVmsize=XlsxInfo::getMobileVmsize(qDeviceId);
}



//*****************20170505*************************//
void Worker::getXXX()
{
    getAppSize();
}

void Worker::getAppSize()
{
    AppSize appsize;
    qAppSize2=appsize.getSize(qDeviceId);
}

void Worker::sendMail()
{
    QString filename=gConfigDir + QDir::separator() + "emailsender.txt";

    QStringList list=Helper::getList(filename);

    if(list.size()>=2)
    {
        QString senderName=list.at(0);
        QString senderKey=list.at(1);
        QString cmdLine;

        QString receiver=gConfigDir + QDir::separator() + "email.txt";
        cmdLine="cmd /c java -jar " + gConfigDir + QDir::separator() + "sendmail.jar" + " " + senderName + " " + senderKey + " " + receiver +  " 测试报告" + " " + qTargetXlsx;
        ExeCmd::runCmd(cmdLine);
    }
}

//*****************20170505*************************//

//*****************20170606**********//
void Worker::performanceHtml()
{
    HtmlInfo html;
    QString htmlDir=qXlsxSaveDir + QDir::separator() + "html";
    Helper::createPath(htmlDir);
    html.setSaveDir(qXlsxSaveDir,htmlDir);

    html.setWarningValue(qMemWarningValue,qCpuWarningValue,qCpuTempWarningValue,qBatteryTempWarningValue,qWifiWarningValue,qMobileWarningValue);
    html.setTime_vector(memTime,cpuTime,cpuTempTime,batteryTime,wifiTime,mobileTime);
    html.setValue_vector(memNum,cpuNum,cpuTempNum,batteryNum,wifiNum,mobileNum);

    html.createHtml();
}
//*****************20170606**********//

//*****************20170630**********//
void Worker::createBaseExcel(const QString &arg_mark)
{
    WriteInfo::setXlsxSaveDir(qXlsxSaveDir);
    if(arg_mark=="travel")
    {
        getAppInfo1(arg_mark);
        setAppInfo_universal();
        setAppInfo_travel();
        writeInfo(arg_mark);
    }
    if(arg_mark=="behaviour")
    {
        getAppInfo1(arg_mark);
        setAppInfo_universal();
        setAppInfo_behaviour();
        writeInfo(arg_mark);
    }

    if(arg_mark=="monkey")
    {
        getAppInfo2(arg_mark);
        setAppInfo_universal();
        setAppInfo_monkey();
        writeInfo(arg_mark);
    }

    else if(arg_mark=="interface")
    {
        getAppInfo1(arg_mark);
        setAppInfo_universal();
        writeInfo(arg_mark);
    }

    else if(arg_mark=="performance")
    {
        getAppInfo2(arg_mark);
        setAppInfo_universal();
        writeInfo(arg_mark);

        //*****************20170606**********//
        performanceHtml();
        //*****************20170606**********//
    }

}
//*****************20170630**********//


}
