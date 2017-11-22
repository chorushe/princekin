#include "generatexlsx.h"

GenerateXlsx::GenerateXlsx(QObject* parent)
{
}

//*******************通用*******************//
void GenerateXlsx::setXlsxSaveDir(const QString &arg_text)
{
    qXlsxSaveDir=arg_text;
}

void GenerateXlsx::setStartTime(const QString &arg_text)
{
    qStartTime=arg_text;
}

void GenerateXlsx::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}


void GenerateXlsx::setMemWarningValue(const QString &arg_text)
{
    qMemWarningValue=arg_text;
}

void GenerateXlsx::setCpuWarningValue(const QString &arg_text)
{
    qCpuWarningValue=arg_text;
}

void GenerateXlsx::setCpuTempWarningValue(const QString &arg_text)
{
    qCpuTempWarningValue=arg_text;
}

void GenerateXlsx::setBatteryTempWarningValue(const QString &arg_text)
{
    qBatteryTempWarningValue=arg_text;
}

void GenerateXlsx::setWifiWarningValue(const QString &arg_text)
{
    qWifiWarningValue=arg_text;
}

void GenerateXlsx::setMobileWarningValue(const QString &arg_text)
{
    qMobileWarningValue=arg_text;
}

void GenerateXlsx::setFrames(const QString &arg_text)
{
    qFrames=arg_text;
}

void GenerateXlsx::setLunchTime(const QString &arg_text)
{
    qLaunchTime=arg_text;
}

void GenerateXlsx::setStatiList(QStringList arg_list)
{
    qStatisticsDataList=arg_list;
}

//*******************通用*******************//

//*******************遍历*******************//
void GenerateXlsx::setTravelStartTime(const QString &arg_text)
{
    qTravelStartTime=arg_text;
}

void GenerateXlsx::setTravelEndTime(const QString &arg_text)
{
    qTravelEndTime=arg_text;
}

void GenerateXlsx::setTravelTime(const QString &arg_text)
{
    qTravelTime=arg_text;
}

void GenerateXlsx::setCheckNumber(const QString &arg_text)
{
    qCheckNumber=arg_text;
}

void GenerateXlsx::setClickNumber(const QString &arg_text)
{
    qClickNumber=arg_text;
}

void GenerateXlsx::setErrorNumber(const QString &arg_text)
{
    qErrorNumber=arg_text;
}

void GenerateXlsx::setTravelActivityNumber(const QString &arg_text)
{
    qTravelActivityNumber=arg_text;
}

void GenerateXlsx::setMeasureVector(QVector<MeasureReport> arg_vector)
{
    qMeasureVector=arg_vector;
}
//*******************遍历*******************//


//*******************统计*******************//
void GenerateXlsx::setUrlTotalNum(int arg_num)
{
    qUrlTotalNum=arg_num;
}

void GenerateXlsx::setUrlErrorNum(int arg_num)
{
    qUrlErrorNum=arg_num;
}

void GenerateXlsx::setUrlSuccessNum(int arg_num)
{
    qUrlSuccessNum=arg_num;
}
//*******************统计*******************//

//*******************monkey*******************//
void GenerateXlsx::setMonkeyTime(const QString &arg_text)
{
    qMonkeyTime=arg_text;
}
//*******************monkey*******************//

void GenerateXlsx::setPackageName(const QString &arg_text)
{
    qPackageName=arg_text;
}



void GenerateXlsx::createExcel(const QString &arg_mark)
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

        //*****************20170606**********//
        performanceHtml();
        //*****************20170606**********//
        chart2();
    }

}


void GenerateXlsx::setAppInfo_universal()
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


    //*****************20170606**********//
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
    //*****************20170606**********//

    if(!qStatisticsDataList.isEmpty())
    {
        WriteInfo::setStatiList(qStatisticsDataList);
    }

}



void GenerateXlsx::setAppInfo_travel()
{
    WriteInfo::setTravelStartTime(qTravelStartTime);
    WriteInfo::setTravelEndTime(qTravelEndTime);
    WriteInfo::setTravelTime(qTravelTime);
    WriteInfo::setCheckNumber(qCheckNumber);
    WriteInfo::setClickNumber(qClickNumber);
    WriteInfo::setErrorNumber(qErrorNumber);
    WriteInfo::setTravelActivityNumber(qTravelActivityNumber);
}

void GenerateXlsx::setAppInfo_behaviour()
{
    WriteInfo::setUrlTotalNum(qUrlTotalNum);
    WriteInfo::setUrlErrorNum(qUrlErrorNum);
    WriteInfo::setUrlSuccessNum(qUrlSuccessNum);
}

void GenerateXlsx::setAppInfo_monkey()
{
    WriteInfo::setMonkeyTime(qMonkeyTime);
}


void GenerateXlsx::writeInfo(const QString &arg_mark)
{
    //WriteInfo::writeUrl(qTargetXlsx);
    WriteInfo::writeUniversal(qTargetXlsx,arg_mark,qDeviceId);
}



void GenerateXlsx::getValues(const QString &arg_mark)
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



void GenerateXlsx::chart()
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

void GenerateXlsx::chart2()
{
    QThreadPool *pool=new QThreadPool;

    using DataController::Controller;
    Controller *controllerInstance=new Controller;

    controllerInstance->setAutoDelete(true);

    controllerInstance->setXlsx(qTargetXlsx);

    controllerInstance->setMemTime_Vector(memTime);
    controllerInstance->setCpuTime_Vector(cpuTime);
    controllerInstance->setCpuTempTime_Vector(cpuTempTime);
    controllerInstance->setBatteryTempTime_Vector(batteryTime);
    controllerInstance->setWifiTime_Vector(wifiTime);
    controllerInstance->setMobileTime_Vector(mobileTime);

    controllerInstance->setMemWarningValue(qMemWarningValue);
    controllerInstance->setCpuWarningValue(qCpuWarningValue);
    controllerInstance->setCpuTempWarningValue(qCpuTempWarningValue);
    controllerInstance->setBatteryTempWarningValue(qBatteryTempWarningValue);
    controllerInstance->setWifiWarningValue(qWifiWarningValue);
    controllerInstance->setMobileWarningValue(qMobileWarningValue);

    controllerInstance->setMemory_Vector(memNum);
    controllerInstance->setCpu_Vector(cpuNum);
    controllerInstance->setCpuTemp_Vector(cpuTempNum);
    controllerInstance->setBatteryTemp_Vector(batteryNum);
    controllerInstance->setWifi_Vector(wifiNum);
    controllerInstance->setMobile_Vector(mobileNum);

    connect(controllerInstance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveDataWorkerResult(const QString &))   );

    pool->start(controllerInstance);
}


void GenerateXlsx::receiveDataWorkerResult(const QString &arg_result)
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


    QThreadPool *pool=new QThreadPool;

    using ChartController::Controller;
    Controller *controllerInstance=new Controller;

    controllerInstance->setAutoDelete(true);
    controllerInstance->setCmdList(cmdList);


    connect(controllerInstance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveChartWorkerResult(const QString &))   );

    pool->start(controllerInstance);
}

void GenerateXlsx::receiveChartWorkerResult(const QString &arg_result)
{
    //QMessageBox::information(NULL,"提示","excel生成完毕");
    //sendMail();
}

void GenerateXlsx::getAppInfo1(const QString &arg_mark)
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


void GenerateXlsx::getAppInfo2(const QString &arg_mark)
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
void GenerateXlsx::getXXX()
{
    getAppSize();
}

void GenerateXlsx::getAppSize()
{
    AppSize appsize;
    qAppSize2=appsize.getSize(qDeviceId);
}

void GenerateXlsx::sendMail()
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
        qDebug()<<cmdLine;
        ExeCmd::runCmd(cmdLine);
    }
}

//*****************20170505*************************//

//*****************20170606**********//
void GenerateXlsx::performanceHtml()
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
void GenerateXlsx::createBaseExcel(const QString &arg_mark)
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
