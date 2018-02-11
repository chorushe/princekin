#include "writeinfo.h"

//************************通用************************//
static QString qIconPath;
static QString qAppChineseName;
static QString qAppVersion;
static QString qAppSize;
static QString qLaunchTime;
static QString qPlatform;
static QString qTestDate;
static QString qStartTime;
static QString qDeviceId;

static QString qMobileBrand;
static QString qMobileModel;
static QString qMobileVersion;
static QString qMobileVmsize;
static QString qAppSize2;
static QString qFrames;
static QString qXlsxSaveDir;
static QStringList qStatiList;
//************************通用************************//

static QString qMonkeyTime;

static QString qTravelStartTime;
static QString qTravelEndTime;
static QString qTravelTime;
static QString qCheckNumber;
static QString qClickNumber;
static QString qErrorNumber;
static QString qTravelActivityNumber;


static int qUrlTotalNum;
static int qUrlErrorNum;
static int qUrlSuccessNum;

//*****************20170606*****************//
static QStringList qMemList;
static QStringList qCpuList;
static QStringList qBatteryList;
static QStringList qCpuTempList;
static double qAllWifi;
static double qAllMobile;
//*****************20170606*****************//

WriteInfo::WriteInfo()
{
}

//************************通用************************//
void WriteInfo::setIconPath(const QString &arg_text)
{
    qIconPath=arg_text;
    if(qIconPath.trimmed().isEmpty())
    {
        qIconPath="-";
    }
}
void WriteInfo::setAppChineseName(const QString &arg_text)
{
    qAppChineseName=arg_text;
    if(qAppChineseName.trimmed().isEmpty())
    {
        qAppChineseName="-";
    }
}

void WriteInfo::setAppVersion(const QString &arg_text)
{
    qAppVersion=arg_text;
    if(qAppVersion.trimmed().isEmpty())
    {
        qAppVersion="-";
    }
}

void WriteInfo::setAppSize(const QString &arg_text)
{
    qAppSize=arg_text;
    if(qAppSize.trimmed().isEmpty())
    {
        qAppSize="-";
    }
}

void WriteInfo::setLaunchTime(const QString &arg_text)
{
    qLaunchTime=arg_text;
    if(qLaunchTime.trimmed().isEmpty())
    {
        qLaunchTime="-";
    }
}

void WriteInfo::setPlatform(const QString &arg_text)
{
    qPlatform=arg_text;
    if(qPlatform.trimmed().isEmpty())
    {
        qPlatform="-";
    }
}

void WriteInfo::setTestDate(const QString &arg_text)
{
    qTestDate=arg_text;
    if(qTestDate.trimmed().isEmpty())
    {
        qTestDate="-";
    }
}

void WriteInfo::setMobileBrand(const QString &arg_text)
{
    qMobileBrand=arg_text;
    if(qMobileBrand.trimmed().isEmpty())
    {
        qMobileBrand="-";
    }
}

void WriteInfo::setMobileModel(const QString &arg_text)
{
    qMobileModel=arg_text;
    if(qMobileModel.trimmed().isEmpty())
    {
        qMobileModel="-";
    }
}

void WriteInfo::setMobileVersion(const QString &arg_text)
{
    qMobileVersion=arg_text;
    if(qMobileVersion.trimmed().isEmpty())
    {
        qMobileVersion="-";
    }
}

void WriteInfo::setMobileVmsize(const QString &arg_text)
{
    qMobileVmsize=arg_text;
    if(qMobileVmsize.trimmed().isEmpty())
    {
        qMobileVmsize="-";
    }
}

void WriteInfo::setAppSize2(const QString &arg_text)
{
    qAppSize2=arg_text;
    if(qAppSize2.trimmed().isEmpty())
    {
        qAppSize2="-";
    }
}

void WriteInfo::setFrames(const QString &arg_text)
{
    qFrames=arg_text;
    if(qFrames.trimmed().isEmpty())
    {
        qFrames="-";
    }
}

void WriteInfo::setXlsxSaveDir(const QString &arg_text)
{
    qXlsxSaveDir=arg_text;
}

void WriteInfo::setStatiList(QStringList arg_list)
{
    qStatiList=arg_list;
}

//************************通用************************//

void WriteInfo::setMonkeyTime(const QString &arg_text)
{
    qMonkeyTime=arg_text;
}


void WriteInfo::setTravelStartTime(const QString &arg_text)
{
    qTravelStartTime=arg_text;
}

void WriteInfo::setTravelEndTime(const QString &arg_text)
{
    qTravelEndTime=arg_text;
}

void WriteInfo::setTravelTime(const QString &arg_text)
{
    qTravelTime=arg_text;
}

void WriteInfo::setCheckNumber(const QString &arg_text)
{
    qCheckNumber=arg_text;
}

void WriteInfo::setClickNumber(const QString &arg_text)
{
    qClickNumber=arg_text;
}

void WriteInfo::setErrorNumber(const QString &arg_text)
{
    qErrorNumber=arg_text;
}

void WriteInfo::setTravelActivityNumber(const QString &arg_text)
{
    qTravelActivityNumber=arg_text;
}


void WriteInfo::setUrlTotalNum(int arg_num)
{
    qUrlTotalNum=arg_num;
}

void WriteInfo::setUrlErrorNum(int arg_num)
{
    qUrlErrorNum=arg_num;
}

void WriteInfo::setUrlSuccessNum(int arg_num)
{
    qUrlSuccessNum=arg_num;
}


void WriteInfo::writeUniversal(const QString &arg_targetXlsx,const QString &arg_tag,const QString &arg_deviceId,const QString &arg_startTime)
{
    qStartTime=arg_startTime;
    qDeviceId=arg_deviceId;
    Document xlsxDoc(arg_targetXlsx);
    xlsxDoc.selectSheet("总体报告");

    QFileInfo info(arg_targetXlsx);
    QString paretnDir=info.absolutePath();
    info.setFile(paretnDir);
    paretnDir=info.absolutePath();

    writeIcon(xlsxDoc,arg_tag);

    writeAppInfo(xlsxDoc,arg_tag);

    if(arg_tag=="performance")
    {
        writeMobileInfo(xlsxDoc);
        writeMobileInfo_Performance(xlsxDoc);
    }
    if(arg_tag=="monkey")
    {
        writeMobileInfo_Monkey(xlsxDoc,paretnDir);
    }
    if(arg_tag=="behaviour")
    {
        writeMobileInfo(xlsxDoc);
        writeMobileInfo_behaviour(xlsxDoc,paretnDir);
    }
    if(arg_tag=="travel")
    {
        writeMobileInfo(xlsxDoc);
        if(qTravelStartTime.isEmpty())
        {
        }
        else
        {
            QFileInfo infos(arg_targetXlsx);
            QString sdir=infos.absolutePath();
            writeMobileInfo_Travel(xlsxDoc,sdir);
        }

    }
    if(arg_tag=="interface")
    {
        //***************20170606****************//
        writeMobileInfo(xlsxDoc);
        writeMobileInfo_Interface(xlsxDoc);
        //***************20170606****************//
    }
    //xlsxDoc.save();


    //***************20170606****************//
    if(arg_tag=="performance")
    {
        if(gPerformanceNumber!=0)
        {
            xlsxDoc.selectSheet("url");
            writeUrl(xlsxDoc);
        }
    }
    if(arg_tag=="monkey")
    {
        if(gMonkeyNumber!=0)
        {
            xlsxDoc.selectSheet("url");
            writeUrl(xlsxDoc);
        }
    }
    if(arg_tag=="behaviour")
    {
        xlsxDoc.selectSheet("url");
        writeUrl(xlsxDoc);
    }
    if(arg_tag=="travel")
    {
        if(gTravelNumber!=0)
        {
            xlsxDoc.selectSheet("url");
            writeUrl(xlsxDoc);
        }
    }


    xlsxDoc.save();

    if(arg_tag=="behaviour")
    {
        qDeviceId=arg_deviceId;
        if(!gmap.isEmpty())
        {
            statisticsReport(arg_targetXlsx);
        }
        //xlsxDoc.selectSheet("url");
        //writeUrl(xlsxDoc);
    }

    Document xlsxDoc2(arg_targetXlsx);
    xlsxDoc2.selectSheet("总体报告");
    xlsxDoc2.save();
    //***************20170606****************//
}

void WriteInfo::writeIcon(Document &arg_xlsxDoc,const QString &arg_tag)
{
    Format format=getNormalFormat();
    if(arg_tag=="behaviour")
    {
    }
    else
    {
        if(qIconPath=="手机")
        {
            QString strCell="A1";
            writeStyle(arg_xlsxDoc,strCell,qIconPath,format);
        }
        else if(qIconPath.isEmpty())
        {
            QString strCell="A1";

            //***********20170717***********//
            /*
            writeStyle(arg_xlsxDoc,strCell,"未发现apk",format);//20170717
            */
            //***********20170717***********//

            //***********20170717***********//
            writeStyle(arg_xlsxDoc,strCell,"",format);
            //***********20170717***********//
        }
        else
        {
            QImage icon(qIconPath);
            QImage fitimgpic;
            if(icon.width()>200)
            {
                fitimgpic=icon.scaled(icon.width()/2,icon.height()/2, Qt::KeepAspectRatioByExpanding);
            }
            else
            {
                fitimgpic=icon.scaled(icon.width(),icon.height(), Qt::KeepAspectRatioByExpanding);
            }
            arg_xlsxDoc.insertImage(0,0,fitimgpic);
        }
    }


    QString index1="1";
    QString index2="4";

    QString cell="A" + index1 + ":" + "A" + index2;
    arg_xlsxDoc.mergeCells(cell, format);
}


void WriteInfo::writeAppInfo(Document &arg_xlsxDoc,const QString &arg_tag)
{
    Format format=getNormalFormat();

    QString strCell;

    if(arg_tag=="behaviour")
    {
        strCell="C1";
        writeStyle(arg_xlsxDoc,strCell,"-",format);

        strCell="C2";
        writeStyle(arg_xlsxDoc,strCell,"-",format);

        strCell="C3";
        if(qFrames=="手机")
        {
            writeStyle(arg_xlsxDoc,strCell,"-",format);
        }
        else
        {
            writeStyle(arg_xlsxDoc,strCell,"-",format);
        }


        strCell="C4";
        writeStyle(arg_xlsxDoc,strCell,qPlatform,format);



        strCell="F1";
        writeStyle(arg_xlsxDoc,strCell,"-",format);

        strCell="F2";
        writeStyle(arg_xlsxDoc,strCell,"-",format);

        strCell="F3";
        if(qLaunchTime.trimmed().isEmpty())
        {
            qLaunchTime="-";
        }
        writeStyle(arg_xlsxDoc,strCell,qLaunchTime,format);

        strCell="F4";
        writeStyle(arg_xlsxDoc,strCell,qTestDate,format);
    }
    else
    {
        if(arg_tag=="performance")
        {
            if(gIsThread)
            {
                qAppChineseName=qAppChineseName+"("+gPackageThreadName+")";
            }
        }

        strCell="C1";
        writeStyle(arg_xlsxDoc,strCell,qAppChineseName,format);

        strCell="C2";
        writeStyle(arg_xlsxDoc,strCell,qAppSize,format);

        strCell="C3";
        if(qFrames=="手机")
        {
            writeStyle(arg_xlsxDoc,strCell,"-",format);
        }
        else
        {
            if(qFrames.trimmed().isEmpty())
            {
                writeStyle(arg_xlsxDoc,strCell,"-",format);
            }
            else
            {
                writeStyle(arg_xlsxDoc,strCell,qFrames,format);
            }

        }


        strCell="C4";
        writeStyle(arg_xlsxDoc,strCell,qPlatform,format);



        strCell="F1";
        writeStyle(arg_xlsxDoc,strCell,qAppVersion,format);

        strCell="F2";
        writeStyle(arg_xlsxDoc,strCell,qAppSize2,format);

        strCell="F3";
        if(qLaunchTime=="手机")
        {
            writeStyle(arg_xlsxDoc,strCell,"-",format);
        }
        else
        {
            if(qLaunchTime.trimmed().isEmpty())
            {
                writeStyle(arg_xlsxDoc,strCell,"-",format);
            }
            else
            {
                writeStyle(arg_xlsxDoc,strCell,qLaunchTime,format);
            }

        }


        strCell="F4";
        writeStyle(arg_xlsxDoc,strCell,qTestDate,format);
    }
}

void WriteInfo::writeMobileInfo_Monkey(Document &arg_xlsxDoc,const QString &arg_path)
{
    QString strCell;
    QString rowIndexStr;

    Format format=getNormalFormat();
    Format hyperlinkFormat=getHyperlinkFormat();

    rowIndexStr="7";

    strCell="A"+rowIndexStr;
    if(qMobileBrand.trimmed().isEmpty())
    {
        qMobileBrand="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileBrand,format);




    strCell="B"+rowIndexStr;
    if(qMobileModel.trimmed().isEmpty())
    {
        qMobileModel="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileModel,format);

    strCell="C"+rowIndexStr;
    if(qMobileVersion.trimmed().isEmpty())
    {
        qMobileVersion="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileVersion,format);

    strCell="D"+rowIndexStr;
    if(qMobileVmsize.trimmed().isEmpty())
    {
        qMobileVmsize="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileVmsize,format);

    strCell="E"+rowIndexStr;
    if(qMonkeyTime.trimmed().isEmpty())
    {
        qMonkeyTime="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMonkeyTime,format);

    strCell="F"+rowIndexStr;
    writeStyle(arg_xlsxDoc,strCell,"查看详情",format);


    //*****************20170606*****************//
    QString tempValue;
    rowIndexStr="9";
    strCell="B"+rowIndexStr;
    tempValue=qMemList.at(0);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="C"+rowIndexStr;
    tempValue=qMemList.at(1);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);




    strCell="E"+rowIndexStr;
    tempValue=qMemList.at(2);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMemList.at(2),format);

    rowIndexStr="10";
    strCell="B"+rowIndexStr;
    tempValue=qCpuList.at(0);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="C"+rowIndexStr;
    tempValue=qCpuList.at(1);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="E"+rowIndexStr;
    tempValue=qCpuList.at(2);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="11";
    strCell="B"+rowIndexStr;
    tempValue=qBatteryList.at(0);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="C"+rowIndexStr;
    tempValue=qBatteryList.at(1);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);




    strCell="E"+rowIndexStr;
    tempValue=qBatteryList.at(2);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="12";
    strCell="B"+rowIndexStr;
    tempValue=qCpuTempList.at(0);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="C"+rowIndexStr;
    tempValue=qCpuTempList.at(1);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=qCpuTempList.at(2);
    tempValue=tempValue.trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    rowIndexStr="13";
    strCell="B"+rowIndexStr;
    if(qAllWifi==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllWifi),format);
    }

    strCell="E"+rowIndexStr;
    if(qAllMobile==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllMobile),format);
    }
    //*****************20170606*****************//


    QString httppath="http://10.10.53.117/report/monkey/"+qDeviceId+"/"+qStartTime;

    QUrl url = QUrl::fromLocalFile(qXlsxSaveDir + QDir::separator() + "monkey" + QDir::separator() + "MonkeyLog.txt");
    url.setUrl(httppath);

    writeHyperlink(arg_xlsxDoc,7,6,url,hyperlinkFormat,"查看详情","");

}

void WriteInfo::writeMobileInfo(Document &arg_xlsxDoc)
{
    QString strCell;
    QString rowIndexStr;

    Format format=getNormalFormat();

    rowIndexStr="7";

    strCell="A"+rowIndexStr;
    if(qMobileBrand.trimmed().isEmpty())
    {
        qMobileBrand="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileBrand,format);

    strCell="B"+rowIndexStr;
    if(qMobileModel.trimmed().isEmpty())
    {
        qMobileModel="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileModel,format);

    strCell="C"+rowIndexStr;
    if(qMobileVersion.trimmed().isEmpty())
    {
        qMobileVersion="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileVersion,format);
    //strCell="C"+rowIndexStr+":"+"D"+rowIndexStr;
    //mergeCells(arg_xlsxDoc,strCell,format);

    strCell="E"+rowIndexStr;
    if(qMobileVmsize.trimmed().isEmpty())
    {
        qMobileVmsize="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qMobileVmsize,format);
    //strCell="E"+rowIndexStr+":"+"F"+rowIndexStr;
    //mergeCells(arg_xlsxDoc,strCell,format);
}


void WriteInfo::writeMobileInfo_Travel(Document &arg_xlsxDoc,const QString &arg_path)
{
    QString strCell;
    QString rowIndexStr;

    Format format=getNormalFormat();
    Format hyperlinkFormat=getHyperlinkFormat();


    rowIndexStr="9";

    strCell="B"+rowIndexStr;
    if(qTravelStartTime.trimmed().isEmpty())
    {
        qTravelStartTime="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qTravelStartTime,format);


    strCell="D"+rowIndexStr;
    if(qTravelEndTime.trimmed().isEmpty())
    {
        qTravelEndTime="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qTravelEndTime,format);

    strCell="F"+rowIndexStr;
    if(qTravelTime.trimmed().isEmpty())
    {
        qTravelTime="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qTravelTime,format);

    rowIndexStr="10";
    strCell="B"+rowIndexStr;
    if(qCheckNumber.trimmed().isEmpty())
    {
        qCheckNumber="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qCheckNumber,format);


    strCell="D"+rowIndexStr;
    if(qClickNumber.trimmed().isEmpty())
    {
        qClickNumber="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qClickNumber,format);

    strCell="F"+rowIndexStr;
    if(qErrorNumber.trimmed().isEmpty())
    {
        qErrorNumber="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qErrorNumber,format);


    rowIndexStr="11";
    strCell="B"+rowIndexStr;
    if(qTravelActivityNumber.trimmed().isEmpty())
    {
        qTravelActivityNumber="-";
    }
    writeStyle(arg_xlsxDoc,strCell,qTravelActivityNumber,format);


    strCell="F"+rowIndexStr;
    writeStyle(arg_xlsxDoc,strCell,"查看详情",format);
    //strCell="B"+rowIndexStr+":"+"D"+rowIndexStr;
    //mergeCells(arg_xlsxDoc,strCell,format);


    //*****************20170606*****************//
    rowIndexStr="13";
    QString tempValue;

    strCell="B"+rowIndexStr;
    tempValue=qMemList.at(0).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qMemList.at(1).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="E"+rowIndexStr;
    tempValue=qMemList.at(2).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="14";
    strCell="B"+rowIndexStr;
    tempValue=qCpuList.at(0).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="C"+rowIndexStr;
    tempValue=qCpuList.at(1).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=qCpuList.at(2).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="15";
    strCell="B"+rowIndexStr;
    tempValue=qBatteryList.at(0).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qBatteryList.at(1).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue.at(1),format);


    strCell="E"+rowIndexStr;
    tempValue=qBatteryList.at(2).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="16";
    strCell="B"+rowIndexStr;
    tempValue=qCpuTempList.at(0).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qCpuTempList.at(1).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="E"+rowIndexStr;
    tempValue=qCpuTempList.at(2).trimmed();
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    rowIndexStr="17";
    strCell="B"+rowIndexStr;
    if(qAllWifi==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllWifi),format);
    }

    strCell="E"+rowIndexStr;
    if(qAllMobile==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllMobile),format);
    }

    //*****************20170606*****************//
    QString httppath="http://10.10.53.117/report/travel/"+qDeviceId+"/"+qStartTime;

    QUrl url = QUrl::fromLocalFile(arg_path);
    url.setUrl(httppath);
    writeHyperlink(arg_xlsxDoc,11,6,url,hyperlinkFormat,"查看详情","");

}

void WriteInfo::writeMobileInfo_behaviour(Document &arg_xlsxDoc,const QString &arg_path)
{
    QString strCell;
    QString rowIndexStr;
    QString tempValue;

    Format format=getNormalFormat();
    Format hyperlinkFormat=getHyperlinkFormat();

    rowIndexStr="9";
    strCell="B"+rowIndexStr;
    tempValue=QString::number(qUrlTotalNum);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=QString::number(qUrlSuccessNum);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="10";
    strCell="B"+rowIndexStr;
    tempValue=QString::number(qUrlErrorNum);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    writeStyle(arg_xlsxDoc,strCell,"查看详情",format);


    //*****************20170606*****************//
    rowIndexStr="12";
    strCell="B"+rowIndexStr;
    tempValue=qMemList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="C"+rowIndexStr;
    tempValue=qMemList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="E"+rowIndexStr;
    tempValue=qMemList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    rowIndexStr="13";
    strCell="B"+rowIndexStr;
    tempValue=qCpuList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qCpuList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="E"+rowIndexStr;
    tempValue=qCpuList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="14";
    strCell="B"+rowIndexStr;
    tempValue=qBatteryList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qBatteryList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=qBatteryList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="15";
    strCell="B"+rowIndexStr;
    tempValue=qCpuTempList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="C"+rowIndexStr;
    tempValue=qCpuTempList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=qCpuTempList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="16";
    strCell="B"+rowIndexStr;
    if(qAllWifi==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllWifi),format);
    }

    strCell="E"+rowIndexStr;
    if(qAllMobile==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllMobile),format);
    }

    //*****************20170606*****************//

    QDir dir(qXlsxSaveDir);
    QStringList fileList=dir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    QString linkFile;
    foreach (QString var, fileList)
    {
        if(var.startsWith("url"))
        {
            linkFile=var;
        }
    }

    QString httppath="http://10.10.53.117/report/behaviour/"+qDeviceId+"/"+qStartTime;
    QUrl url = QUrl::fromLocalFile(qXlsxSaveDir + QDir::separator() +  linkFile);

    url.setUrl(httppath);
    writeHyperlink(arg_xlsxDoc,10,5,url,hyperlinkFormat,"查看详情","");
}


void WriteInfo::writeStyle(Document &arg_xlsxDoc,const QString &arg_cell, const QString &arg_text,Format arg_format)
{
    arg_xlsxDoc.write(arg_cell,arg_text,arg_format);
}


Format WriteInfo::getNormalFormat()
{
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);
    return format;
}

Format WriteInfo::getHyperlinkFormat()
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

Format WriteInfo::getBlueformat()
{
    QColor color(220,230,241,255);
    Format format;
    format.setFontColor(Qt::blue);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setPatternBackgroundColor(color);
    format.setBorderStyle(Format::BorderThin);
    format.setFontUnderline(Format::FontUnderlineSingle);
    format.setFontName("Verdana");
    return format;
}

Format WriteInfo::getErrorFormat()
{
    Format format;
    format.setFontColor(Qt::red);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);
    return format;
}


void WriteInfo::mergeCells(Document &arg_xlsxDoc,const QString &arg_cell,Format arg_format)
{
    arg_xlsxDoc.mergeCells(arg_cell, arg_format);
}
void WriteInfo::mergeCells(Document &arg_xlsxDoc,const QString &arg_cell)
{
    arg_xlsxDoc.mergeCells(arg_cell);
}
void WriteInfo::writeHyperlink(Document &arg_xlsx,int arg_row,int arg_column,QUrl arg_url,Format arg_format,const QString &arg_text,const QString &arg_tip)
{
    arg_xlsx.currentWorksheet()->writeHyperlink(arg_row,arg_column,arg_url,arg_format,arg_text,"");
}


void WriteInfo::writeUrl(Document &arg_xlsx)
{
    int rowIndex=2;
    int len;
    int y;
    QStringList splitResult1;
    QStringList splitResult2;

    QString fileName;
    QString state;
    QString tempStr;

    foreach (QString var, qStatiList)
    {
        splitResult1=var.split("&");
        len=splitResult1.count();
        if(len>=2)
        {
            fileName=splitResult1.at(0);
            arg_xlsx.write("A"+QString::number(rowIndex),fileName);

            state=splitResult1.at(1);
            arg_xlsx.write("B"+QString::number(rowIndex),state);

            y=rowIndex;
            for(int i=2;i<len;i++)
            {
                tempStr=splitResult1.at(i);
                splitResult2=tempStr.split("=");

                if(splitResult2.count()==2)
                {
                    arg_xlsx.write("C"+QString::number(y),splitResult2.at(0));
                    arg_xlsx.write("D"+QString::number(y),splitResult2.at(1));
                    arg_xlsx.write("E"+QString::number(y),"?");
                }
                if(splitResult2.count()==3)
                {
                    arg_xlsx.write("C"+QString::number(y),splitResult2.at(0));
                    arg_xlsx.write("D"+QString::number(y),splitResult2.at(1));
                    arg_xlsx.write("E"+QString::number(y),splitResult2.at(2));
                }
                y++;
            }
        }
        else
        {
        }
        if(len==2)
        {
            y++;
        }
        rowIndex=y;
    }
    //behaviour_pv_4001.xml&×&cv=6.6.0=5.8.0&sim=0=;
    //behaviour_pv_4001.xml&×&cv=6.6.0=5.8.0&sim=0=1;
    //behaviour_pv_4001.xml&×&cv=6.6.0=5.8.0&sim==1;

    y=y+5;
    if(gxmlNum.count()==0)
    {

    }
    else
    {
        Format blue=getBlueformat();
        arg_xlsx.write("A"+QString::number(y),"文件名",blue);
        arg_xlsx.write("B"+QString::number(y),"次数",blue);
        y++;
        int dnum;
        QString tempValue;
        QStringList keyList=gxmlNum.keys();
        for(QString var:keyList)
        {
            dnum=gxmlNum.value(var);
            tempValue=QString::number(dnum);

            arg_xlsx.write("A"+QString::number(y),var);
            arg_xlsx.write("B"+QString::number(y),tempValue);
            y++;
        }
    }
}

//********************20170606***************//
void WriteInfo::setMem(QStringList info)
{
    qMemList=info;
}
void WriteInfo::setCpu(QStringList info)
{
    qCpuList=info;
}
void WriteInfo::setBattery(QStringList info)
{
    qBatteryList=info;
}
void WriteInfo::setCpuTemp(QStringList info)
{
    qCpuTempList=info;
}

void WriteInfo::setAllWifi(double arg_)
{
    qAllWifi=arg_;
}

void WriteInfo::setAllMobile(double arg_)
{
    qAllMobile=arg_;
}

void WriteInfo::writeMobileInfo_Interface(Document &arg_xlsxDoc)
{
    QString strCell;
    QString rowIndexStr;
    QString tempValue;

    Format format=getNormalFormat();

    rowIndexStr="9";
    strCell="B"+rowIndexStr;
    tempValue=qMemList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qMemList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=qMemList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="10";
    strCell="B"+rowIndexStr;
    tempValue=qCpuList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="C"+rowIndexStr;
    tempValue=qCpuList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=qCpuList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="11";
    strCell="B"+rowIndexStr;
    tempValue=qBatteryList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qBatteryList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="E"+rowIndexStr;
    tempValue=qBatteryList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="12";
    strCell="B"+rowIndexStr;
    tempValue=qCpuTempList.at(0);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=qCpuTempList.at(1);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="E"+rowIndexStr;
    tempValue=qCpuTempList.at(2);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="13";
    strCell="B"+rowIndexStr;
    if(qAllWifi==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllWifi),format);
    }

    strCell="E"+rowIndexStr;
    if(qAllMobile==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(qAllMobile),format);
    }


    rowIndexStr="15";
    strCell="A"+rowIndexStr;
    tempValue=QString::number(gInterfaceTotalNum);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="B"+rowIndexStr;
    tempValue=QString::number(gInterfaceAveReponseTime);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="C"+rowIndexStr;
    tempValue=QString::number(gInterfaceMinTime);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="D"+rowIndexStr;
    tempValue=QString::number(gInterfaceMaxTime);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="E"+rowIndexStr;
    tempValue=QString::number(gInterfaceTimeWarning);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="F"+rowIndexStr;
    tempValue=QString::number(gInterfaceTimeOutRate);
    if(tempValue.trimmed().isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);
}

void WriteInfo::writeMobileInfo_Performance(Document &arg_xlsxDoc)
{
    QString strCell;
    QString rowIndexStr;
    QString tempValue;

    Format format=getNormalFormat();

    rowIndexStr="9";
    strCell="A"+rowIndexStr;
    if(gMemOption==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"mem_total(M)",format);
    }
    else if(gMemOption==1)
    {
        writeStyle(arg_xlsxDoc,strCell,"mem_heap(M)",format);
    }


    rowIndexStr="9";
    strCell="B"+rowIndexStr;
    tempValue=QString::number(memMax,10,2);
    if(tempValue.isEmpty())
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,tempValue,format);
    }



    strCell="C"+rowIndexStr;
    tempValue=QString::number(memMin,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="E"+rowIndexStr;
    tempValue=QString::number(gMemAve,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    rowIndexStr="10";
    strCell="B"+rowIndexStr;
    tempValue=QString::number(cpuMax,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);




    strCell="C"+rowIndexStr;
    tempValue=QString::number(cpuMin,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="E"+rowIndexStr;
    tempValue=QString::number(gCpuAve,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    rowIndexStr="11";
    strCell="B"+rowIndexStr;
    tempValue=QString::number(batteryMax,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);

    strCell="C"+rowIndexStr;
    tempValue=QString::number(batteryMin,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="E"+rowIndexStr;
    tempValue=QString::number(gBatteryAve,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    rowIndexStr="12";
    strCell="B"+rowIndexStr;
    tempValue=QString::number(cpuTempMax,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);



    strCell="C"+rowIndexStr;
    tempValue=QString::number(cpuTempMin,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    strCell="E"+rowIndexStr;
    tempValue=QString::number(gCpuTempAve,10,2);
    if(tempValue.isEmpty())
    {
        tempValue="-";
    }
    writeStyle(arg_xlsxDoc,strCell,tempValue,format);


    rowIndexStr="13";
    strCell="B"+rowIndexStr;
    if(gPerformanceWifi==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(gPerformanceWifi),format);
    }

    strCell="E"+rowIndexStr;
    if(gPerformanceMobile==0)
    {
        writeStyle(arg_xlsxDoc,strCell,"-",format);
    }
    else
    {
        writeStyle(arg_xlsxDoc,strCell,QString::number(gPerformanceMobile),format);
    }

}

//********************20170606***************//
void WriteInfo::statisticsReport(const QString &arg_targetXlsx)
{

    Document xlsxDoc(arg_targetXlsx);
    xlsxDoc.selectSheet("埋点报告");

    writeTitle(xlsxDoc);
    writeData(xlsxDoc);
    mergeModule(xlsxDoc);

    xlsxDoc.selectSheet("url");
    writeUrl(xlsxDoc);


    xlsxDoc.save();



    /*
    arg_xlsxDoc.selectSheet("埋点报告");

    writeTitle(arg_xlsxDoc);
    writeData(arg_xlsxDoc);
    mergeModule(arg_xlsxDoc);
    //xlsxDoc.save();
    */
}

void WriteInfo::writeTitle(Document &arg_xlsx)
{
    int fontSize=12;

    QString strName;
    QString strCell;

    Format format=getNormalFormat();
    Format hyperlinkFormat=getHyperlinkFormat();

    strCell="A1";
    strName="机型";
    writeStyle(arg_xlsx,strCell,strName,format);
    mergeCells(arg_xlsx,"A1:B1",format);


    strCell="A2";
    strName="详情";
    writeStyle(arg_xlsx,strCell,strName,format);
    mergeCells(arg_xlsx,"A2:B2",format);


    strCell="C2";
    strName="具体详情";
    writeStyle(arg_xlsx,strCell,strName,format);

    QUrl url = QUrl::fromLocalFile(qXlsxSaveDir);
    writeHyperlink(arg_xlsx,2,3,url,hyperlinkFormat,"具体详情","");


    strCell="A3";
    strName="模块";
    writeStyle(arg_xlsx,strCell,strName,format);

    strCell="B3";
    strName="用例";
    writeStyle(arg_xlsx,strCell,strName,format);


    strCell="C1";
    strName=qDeviceId;
    writeStyle(arg_xlsx,strCell,strName,format);

    strCell="C3";
    strName="测试结果";
    writeStyle(arg_xlsx,strCell,strName,format);

}


void WriteInfo::writeData(Document &arg_xlsx)
{
    int number=4;
    int fontSize=12;

    int width1=0;
    int maxWidth1=0;

    int width2=0;
    int maxWidth2=0;

    QString rowIndex;

    QString moduleColumn;
    QString scriptColumn;

    QString moduleName;
    QString scriptName;

    QStringList splitResult;

    Format format=getNormalFormat();
    Format errorformat=getErrorFormat();

    bool isOk;
    QList<QString> list=gmap.keys();
    foreach(QString var,list)
    {
        splitResult=var.split("=");

        moduleName=splitResult.at(0);
        width1=moduleName.length();
        if(maxWidth1<=width1)
        {
            maxWidth1=width1;
        }


        scriptName=splitResult.at(1) + ".txt";

        isOk=gmap.value(var);

        width2=scriptName.length();
        if(maxWidth2<=width2)
        {
            maxWidth2=width2;
        }


        rowIndex=QString::number(number);

        moduleColumn="A" + rowIndex;
        writeStyle(arg_xlsx,moduleColumn,moduleName,format);


        scriptColumn="B" + rowIndex;
        writeStyle(arg_xlsx,scriptColumn,scriptName,format);

        scriptColumn="C" + rowIndex;

        if(isOk)
        {
            arg_xlsx.write(scriptColumn,isOk,format);
        }
        else
        {
            arg_xlsx.write(scriptColumn,isOk,errorformat);
        }


        number++;
    }

    arg_xlsx.setColumnWidth(1,maxWidth1+5);
    arg_xlsx.setColumnWidth(2,maxWidth2+5);
}

void WriteInfo::mergeModule(Document &arg_xlsx)
{
    QString str;
    QRegExp regExp;
    QSet<QString> set;
    QStringList splitResult;
    QStringList templist;
    QStringList scriptnumList;

    QStringList keyList=gmap.keys();

    for(QString var:keyList)
    {
        splitResult=var.split("=");
        set.insert(splitResult.at(0));
    }

    for(QString var:set)
    {
        str=var+"=";
        regExp.setPattern("^"+str);
        templist=keyList.filter(regExp);
        scriptnumList.append(var+"="+QString::number(templist.count()));
    }

    int count;
    int scriptCount;
    int startIndex;
    int endMergeIndex;

    bool ok;


    QString s1;
    QString s2;

    endMergeIndex=4;
    count=scriptnumList.size();

    QString startStr;
    startIndex=4;
    int j=3;
    startStr=arg_xlsx.cellAt(startIndex,1)->value().toString();

    for(int i=0;i<gmap.count();i++)
    {
        j++;
        str=arg_xlsx.cellAt(j,1)->value().toString();
        if(str!=startStr)
        {
            s1=QString::number(startIndex);
            s2=QString::number(j-1);
            mergeCells(arg_xlsx,"A" + s1 + ":" + "A" + s2);
            startIndex=j;
            startStr=arg_xlsx.cellAt(j,1)->value().toString();
        }
        else
        {
            if(i==gmap.count()-1)
            {
                s1=QString::number(startIndex);
                s2=QString::number(j);
                mergeCells(arg_xlsx,"A" + s1 + ":" + "A" + s2);
            }
        }
    }
}
