#include "createcrashreport.h"

CreateCrashReport::CreateCrashReport()
{
}


void CreateCrashReport::receiveCrashResult(const QString &arg_deviceId,const QString &arg_firstLevelDirName,const QString &arg_secondLevelDirName, const QString &arg_moduleBaseName, const QString &arg_scriptBaseName,const QString &arg_appInfo,QStringList arg_mobileInfoList)
{
    QString crashSourceXlsx;
    QString crashTargetXlsx;
    QString deviceSavePath;
    QString logcatPath;
    QStringList splitResult;
    QString logfile;
    //deviceSavePath=gWorkSpace +  QDir::separator() + "report" +  QDir::separator() + arg_firstLevelDirName + QDir::separator() + arg_secondLevelDirName;;
    deviceSavePath=gDetailedPath +  QDir::separator() + arg_secondLevelDirName;;
    deviceSavePath=QDir::toNativeSeparators(deviceSavePath);
    logcatPath=deviceSavePath + QDir::separator() + "logcat";
    logcatPath=QDir::toNativeSeparators(logcatPath);

    QDir dir(logcatPath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList logList=dir.entryList();
    QString sss=logList.join(" ");
    splitResult=sss.split(" ");
    sss="";

    if(splitResult.size()==1&&splitResult.at(0).isEmpty())
    {
        sss="";
    }
    else
    {
        for(QString var:splitResult)
        {
            logfile=logcatPath+QDir::separator()+var;
            logfile=QDir::toNativeSeparators(logfile);
            sss=sss+logfile+" ";
        }
        sss=sss.trimmed();
    }

    crashSourceXlsx=gConfigDir + QDir::separator() + "crash.xlsx";
    crashTargetXlsx=deviceSavePath + QDir::separator() + arg_secondLevelDirName + "_crash.xlsx";
    QFile::copy(crashSourceXlsx,crashTargetXlsx);

    QString appName;
    QString appVersion;
    QString fileSize;
    QString platform;
    QString testDate;

    Format format=getNormalFormat();


    splitResult=arg_appInfo.split("=");
    if(splitResult.size()==5)
    {
        appName=splitResult.at(0);
        appVersion=splitResult.at(1);
        fileSize=splitResult.at(2);
        platform=splitResult.at(3);
        testDate=splitResult.at(4);
    }

    QString strCell;

    Document xlsxDoc(crashTargetXlsx);
    xlsxDoc.selectSheet("crash");

    QImage fitimgpic=getImage();
    xlsxDoc.insertImage(0,0,fitimgpic);

    QString index1="1";
    QString index2="2";
    strCell="A" + index1 + ":" + "A" + index2;
    xlsxDoc.mergeCells(strCell, format);


    strCell="C1";
    writeStyle(xlsxDoc,strCell,appName,format);

    strCell="C2";
    writeStyle(xlsxDoc,strCell,fileSize,format);

    strCell="E1";
    writeStyle(xlsxDoc,strCell,appVersion,format);

    strCell="E2";
    writeStyle(xlsxDoc,strCell,platform,format);

    QString brand;
    QString model;
    QString version;
    QString wmsize;
    QString strLine=arg_deviceId + "=";
    QStringList filterList=arg_mobileInfoList.filter(strLine);

    if(filterList.size()!=0)
    {
        splitResult=filterList.at(0).split("=");
        if(splitResult.size()==5)
        {
            brand=splitResult.at(1);
            strCell="A5";
            writeStyle(xlsxDoc,strCell,brand,format);

            model=splitResult.at(2);
            strCell="B5";
            writeStyle(xlsxDoc,strCell,model,format);

            version=splitResult.at(3);
            strCell="C5";
            writeStyle(xlsxDoc,strCell,version,format);

            wmsize=splitResult.at(4);
            strCell="D5";
            writeStyle(xlsxDoc,strCell,wmsize,format);
        }
    }

    QString crashTime=Helper::getTime2("yyyyMMdd_hhmmss");

    strCell="A8";
    writeStyle(xlsxDoc,strCell,arg_moduleBaseName,format);

    strCell="B8";
    writeStyle(xlsxDoc,strCell,arg_scriptBaseName+".txt",format);

    strCell="C8";
    writeStyle(xlsxDoc,strCell,crashTime,format);

    strCell="D8";
    writeStyle(xlsxDoc,strCell,testDate,format);

    Format hyperlinkFormat;
    hyperlinkFormat=getHyperlinkFormat();
    QUrl url=QUrl::fromLocalFile(logcatPath);

    xlsxDoc.currentWorksheet()->writeHyperlink(8,5,url,hyperlinkFormat,"具体详情","");


    /*
    strCell="E9";
    writeStyle(xlsxDoc,strCell,wmsize,format);

    strCell="9";
    writeStyle(xlsxDoc,strCell,wmsize,format);
    */



    //QString secStr=QString::number(dsec,'f',2);
    //QString secStr=QString::number(dsec);

    QString perValue;
    QString aveValue;

    perValue=getMinMaxMemory(arg_deviceId,gMin_Mem_Hash);
    strCell="C11";
    writeStyle(xlsxDoc,strCell,perValue,format);

    perValue=getMinMaxMemory(arg_deviceId,gMax_Mem_Hash);
    strCell="D11";
    writeStyle(xlsxDoc,strCell,perValue,format);

    aveValue=getMemoryAveValue(arg_deviceId,gMemValue_list);
    strCell="E11";
    writeStyle(xlsxDoc,strCell,aveValue,format);


    perValue=getMinMaxCpu(arg_deviceId,gMin_Cpu_Hash);
    strCell="C12";
    writeStyle(xlsxDoc,strCell,perValue,format);

    perValue=getMinMaxCpu(arg_deviceId,gMax_Cpu_Hash);
    strCell="D12";
    writeStyle(xlsxDoc,strCell,perValue,format);

    aveValue=getCpuAveValue(arg_deviceId,gCpuValue_list);
    strCell="E12";
    writeStyle(xlsxDoc,strCell,aveValue,format);


    perValue=getMinMaxBatterytemp(arg_deviceId,gMin_Batterytemp_Hash);
    strCell="C13";
    writeStyle(xlsxDoc,strCell,perValue,format);

    perValue=getMinMaxBatterytemp(arg_deviceId,gMax_Batterytemp_Hash);
    strCell="D13";
    writeStyle(xlsxDoc,strCell,perValue,format);

    aveValue=getBatterytempAveValue(arg_deviceId,gBatteryTempValue_list);
    strCell="E13";
    writeStyle(xlsxDoc,strCell,aveValue,format);


    perValue=getMinMaxCputemp(arg_deviceId,gMin_Cputemp_Hash);
    strCell="C14";
    writeStyle(xlsxDoc,strCell,perValue,format);

    perValue=getMinMaxCputemp(arg_deviceId,gMax_Cputemp_Hash);
    strCell="D14";
    writeStyle(xlsxDoc,strCell,perValue,format);

    aveValue=getCputempAveValue(arg_deviceId,gCpuTempValue_list);
    strCell="E14";
    writeStyle(xlsxDoc,strCell,aveValue,format);


    QString wifimobileValue=getWifiMobile(arg_deviceId);
    splitResult=wifimobileValue.split("=");
    QString wifiValue=splitResult.at(0);
    QString mobileValue=splitResult.at(1);

    strCell="C15";
    writeStyle(xlsxDoc,strCell,wifiValue,format);
    strCell="E15";
    writeStyle(xlsxDoc,strCell,mobileValue,format);


    xlsxDoc.currentWorksheet()->setSelected(false);
    xlsxDoc.save();


    QFile file(gConfigDir+QDir::separator()+"mailContent_crash.txt");
    QTextStream outStream(&file);
    if(!file.open(QIODevice::WriteOnly))
    {
        file.close();
    }

    QString tempStr;

    tempStr="To All:";
    outStream<<tempStr<<endl;

    tempStr="APP名称: "+appName;
    outStream<<tempStr<<endl;

    tempStr="APP版本: "+appVersion;
    outStream<<tempStr<<endl;

    tempStr="测试平台: android";
    outStream<<tempStr<<endl;

    tempStr="手机型号: "+model;
    outStream<<tempStr<<endl;

    tempStr="手机系统版本: "+version;
    outStream<<tempStr<<endl;


    tempStr="Crash脚本模块: "+arg_moduleBaseName;
    outStream<<tempStr<<endl;

    tempStr="Crash用例名称: "+arg_scriptBaseName+".txt";
    outStream<<tempStr<<endl;

    tempStr="Crash发生时间: "+crashTime;
    outStream<<tempStr<<endl;

    tempStr="------------------------------------------------------";
    outStream<<tempStr<<endl;

    tempStr="Brs";
    outStream<<tempStr<<endl;

    tempStr="小王子团队";
    outStream<<tempStr<<endl;

    tempStr="Tel: 何畅022-65303756 OR 周朝彬010-56602399";
    outStream<<tempStr<<endl;

    tempStr="Email: changhe@sohu-inc.com OR chaobinzhou@sohu-inc.com";
    outStream<<tempStr<<endl;

    tempStr="QQ: 周朝彬1787072341";
    outStream<<tempStr<<endl;
    file.close();

    QString filename=gConfigDir + QDir::separator() + "emailsender.txt";
    QStringList list=Helper::getList(filename);

    if(list.size()>=2)
    {
        QString senderName=list.at(0);
        QString senderKey=list.at(1);
        QString cmdLine;

        QString strdate=Helper::getTime2("yyyyMMdd_hhmmss");
        QString subject=brand + model + "-" +appName + "-crash报告-"+strdate;
        QString mailContent=gConfigDir + QDir::separator() + "mailContent_crash.txt";
        mailContent=QDir::toNativeSeparators(mailContent);

        QString receiver=gConfigDir + QDir::separator() + "email.txt";

        cmdLine="cmd /c java -jar " + gConfigDir + QDir::separator() + "sendmail2.jar" + " " + senderName + " " + senderKey + " " + receiver +  " " + subject + " " + mailContent + " " + crashTargetXlsx + " " + sss;
        cmdLine=cmdLine.trimmed();
        //Helper::runCmd(cmdLine);

        AdbShellController::Controller *controller=new AdbShellController::Controller;
        controller->setMark("getAll","sendmail");
        controller->setCmdLine(cmdLine);
        //connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
        controller->doController();
    }

    /*
    qDebug()<<"-----------------------";
    qDebug()<<arg_deviceId;
    qDebug()<<arg_secondLevelDirName;
    qDebug()<<arg_moduleBaseName;
    qDebug()<<arg_scriptBaseName;
    qDebug()<<arg_appInfo;
    qDebug()<<arg_mobileInfoList;
    qDebug()<<logcatPath;
    qDebug()<<"-----------------------";
    */
}

void CreateCrashReport::writeStyle(Document &arg_xlsx,const QString &arg_cell, const QString &arg_text,Format arg_format)
{
    arg_xlsx.write(arg_cell,arg_text,arg_format);
}

Format CreateCrashReport::getNormalFormat()
{
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);
    return format;
}

QString CreateCrashReport::getMinMaxMemory(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    double doubleValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    doubleValue=intValue/1024;
    strValue=QString::number(doubleValue,'f',2);
    return strValue;
}


QString CreateCrashReport::getMinMaxCpu(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    strValue=QString::number(intValue);
    return strValue;
}


QString CreateCrashReport::getMinMaxBatterytemp(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    strValue=QString::number(intValue*0.1);
    return strValue;
}

QString CreateCrashReport::getMinMaxCputemp(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    strValue=QString::number(intValue);
    return strValue;
}


QString CreateCrashReport::getMemoryAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue/1024;
        d=d/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString CreateCrashReport::getCpuAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString CreateCrashReport::getBatterytempAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue*0.1;
        d=d/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString CreateCrashReport::getCputempAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString CreateCrashReport::getWifiMobile(const QString &arg_deviceId)
{
    QString lrxWifi;
    QString lrxMobile;
    QString tempStr;
    QString trafficValue;
    QStringList splitResult;

    if(gData_Traffic_Hash.isEmpty())
    {
        lrxWifi="";
        lrxMobile="";
    }
    else
    {
        tempStr=arg_deviceId + "=" + "currentStateWifi";
        if(gData_Traffic_Hash.keys().contains(tempStr))
        {
            trafficValue=gData_Traffic_Hash.value(tempStr);
            splitResult=trafficValue.split("=");
            lrxWifi=splitResult.at(0);
            double d=lrxWifi.toDouble();
            d=d/1024;
            lrxWifi=QString::number(d,'f',2);
        }
        else
        {
            lrxWifi="";
        }


        tempStr=arg_deviceId + "=" + "currentStateMobile";
        if(gData_Traffic_Hash.keys().contains(tempStr))
        {
            trafficValue=gData_Traffic_Hash.value(tempStr);
            splitResult=trafficValue.split("=");
            lrxMobile=splitResult.at(2);

            double d=lrxMobile.toDouble();
            d=d/1024;
            lrxMobile=QString::number(d,'f',2);
        }
        else
        {
            lrxMobile="";
        }
    }
    return lrxWifi+"="+lrxMobile;
}

QImage CreateCrashReport::getImage()
{
    QImage icon(gIconPath);

    QImage fitimgpic;
    if(icon.width()>200)
    {
        fitimgpic=icon.scaled(icon.width()/2,icon.height()/2, Qt::KeepAspectRatioByExpanding);
    }
    else
    {
        fitimgpic=icon.scaled(icon.width(),icon.height(), Qt::KeepAspectRatioByExpanding);
    }
    return fitimgpic;
}

Format CreateCrashReport::getHyperlinkFormat()
{
    QColor color(220,230,241,255);
    Format format;
    format.setFontColor(Qt::red);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setPatternBackgroundColor(color);
    format.setBorderStyle(Format::BorderThin);
    format.setFontUnderline(Format::FontUnderlineSingle);
    format.setFontName("Verdana");
    return format;
}
