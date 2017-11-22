#include "writeresult.h"

WriteResult::WriteResult()
{
    qFontSize=12;
    qStartRow_mem_Index=3;
    qStartRow_cpu_Index=3;
    qStartRow_battery_Index=3;
    qStartRow_cpuTemp_Index=3;
    qStartRow_traffic_Index=3;
    qStartRow_base_Index=6;
    maxWidth=0;
}


void WriteResult::setScriptList(QStringList arg_scriptList)
{
    qScriptList=arg_scriptList;
}

void WriteResult::setTargetXlsx(const QString &arg_xlsx)
{
    qTargetXlsx=arg_xlsx;
}

void WriteResult::setDevice_TopIndex_Hash(QHash<QString,QString> arg_device_TopIndex_Hash)
{
    qDevice_TopIndex_Hash=arg_device_TopIndex_Hash;
}

void WriteResult::setDir(const QString &arg_dir)
{
    qDir=arg_dir;
}

void WriteResult::receiveReplayResult(const QString &arg_deviceId,const QString &arg_two,QStringList arg_actualList,QStringList arg_errorList,const QString &arg_detailedPath)
{
    bool isOK;
    int intRowIndex;
    int intActualLoop;
    int intErrorLoop;
    int intSuccessLoop;


    QString strLine;
    QString tempStr;
    QString strTopIndex;
    QString strRowIndex;

    QString strActual;
    QString strActualLoop;
    QString strErrorLoop;
    QString strSuccessLoop;

    QStringList splitResult;
    QStringList actualFilterList;
    QStringList errorFilterList;


    Format successFormat;
    Format errorFormat;


    successFormat=getNormalFormat();
    errorFormat=getErrorFormat();

    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("自动化测试报告");

    writeTitle(xlsxDoc,arg_deviceId,arg_two,arg_detailedPath);



    if(arg_actualList.isEmpty())
    {
        intRowIndex=4;
        tempStr="0/0";
        for(QString var:qScriptList)
        {
            strTopIndex=qDevice_TopIndex_Hash.value(arg_deviceId);
            strRowIndex=strTopIndex + QString::number(intRowIndex);
            writeStyle(xlsxDoc,strRowIndex,tempStr,successFormat);
            intRowIndex++;
        }
    }
    else
    {

        intRowIndex=4;
        for(QString var:qScriptList)
        {

            strTopIndex=qDevice_TopIndex_Hash.value(arg_deviceId);
            strRowIndex=strTopIndex + QString::number(intRowIndex);

            strLine=var + "=";
            actualFilterList=arg_actualList.filter(strLine);

            if(actualFilterList.isEmpty())
            {
                tempStr="0/0";
                writeStyle(xlsxDoc,strRowIndex,tempStr,successFormat);
            }
            else
            {

                strActual=actualFilterList.at(0);
                splitResult=strActual.split("=");

                strActualLoop=splitResult.at(2);
                intActualLoop=strActualLoop.toInt(&isOK,10);

                errorFilterList=arg_errorList.filter(strLine);
                if(errorFilterList.isEmpty())
                {
                    tempStr=strActualLoop + "/" + strActualLoop;
                    writeStyle(xlsxDoc,strRowIndex,tempStr,successFormat);
                }
                else
                {
                    tempStr=errorFilterList.at(0);
                    splitResult=tempStr.split("=");

                    strErrorLoop=splitResult.at(2);
                    intErrorLoop=strErrorLoop.toInt(&isOK,10);

                    intSuccessLoop=intActualLoop-intErrorLoop;
                    strSuccessLoop=QString::number(intSuccessLoop);
                    tempStr=strSuccessLoop + "/" + strActualLoop;
                    writeStyle(xlsxDoc,strRowIndex,tempStr,errorFormat);
                }
            }
            intRowIndex++;
        }//for(QString var:qScriptList)
    }

    setxxx(xlsxDoc,false);
    xlsxDoc.save();
}


void WriteResult::receiveSaveXlsx()
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("性能测试报告");
    xlsxDoc.setColumnWidth(1,maxWidth+5);
    xlsxDoc.save();


    Document xlsxDoc2(qTargetXlsx);
    xlsxDoc2.selectSheet("总体报告");
    xlsxDoc2.save();
}


void WriteResult::receiveMinMax_memory(const QString &arg_deviceId, int arg_min, int arg_max)
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("性能测试报告");

    QString minValue;
    QString maxValue;
    QString startRowIndex;
    QString deviceIdCell;
    QString minMemCell;
    QString maxMemCell;

    Format format=getNormalFormat();


    int width1=arg_deviceId.length();
    if(maxWidth<=width1)
    {
        maxWidth=width1;
    }

    minValue=QString::number(arg_min);
    maxValue=QString::number(arg_max);
    startRowIndex=QString::number(qStartRow_mem_Index);

    deviceIdCell="A" + startRowIndex;
    writeStyle(xlsxDoc,deviceIdCell,arg_deviceId,format);

    minMemCell="B" + startRowIndex;
    writeStyle(xlsxDoc,minMemCell,minValue,format);

    maxMemCell="C" + startRowIndex;
    writeStyle(xlsxDoc,maxMemCell,maxValue,format);

    setxxx(xlsxDoc,false);
    xlsxDoc.save();


    qStartRow_mem_Index++;
}

void WriteResult::receiveMinMax_cpu(const QString &arg_deviceId, int arg_min, int arg_max)
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("性能测试报告");

    QString minValue;
    QString maxValue;
    QString startRowIndex;
    QString deviceIdCell;
    QString minMemCell;
    QString maxMemCell;

    Format format=getNormalFormat();

    int width1=arg_deviceId.length();
    if(maxWidth<=width1)
    {
        maxWidth=width1;
    }

    minValue=QString::number(arg_min);
    maxValue=QString::number(arg_max);
    startRowIndex=QString::number(qStartRow_cpu_Index);

    deviceIdCell="A" + startRowIndex;
    writeStyle(xlsxDoc,deviceIdCell,arg_deviceId,format);

    minMemCell="D" + startRowIndex;
    writeStyle(xlsxDoc,minMemCell,minValue,format);

    maxMemCell="E" + startRowIndex;
    writeStyle(xlsxDoc,maxMemCell,maxValue,format);

    setxxx(xlsxDoc,false);
    xlsxDoc.save();

    qStartRow_cpu_Index++;
}

void WriteResult::receiveMinMax_battery(const QString &arg_deviceId, int arg_min, int arg_max)
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("性能测试报告");

    QString minValue;
    QString maxValue;
    QString startRowIndex;
    QString deviceIdCell;
    QString minMemCell;
    QString maxMemCell;

    Format format=getNormalFormat();

    int width1=arg_deviceId.length();
    if(maxWidth<=width1)
    {
        maxWidth=width1;
    }

    minValue=QString::number(arg_min*0.1);
    maxValue=QString::number(arg_max*0.1);


    startRowIndex=QString::number(qStartRow_battery_Index);

    deviceIdCell="A" + startRowIndex;
    writeStyle(xlsxDoc,deviceIdCell,arg_deviceId,format);

    minMemCell="J" + startRowIndex;
    writeStyle(xlsxDoc,minMemCell,minValue,format);

    maxMemCell="K" + startRowIndex;
    writeStyle(xlsxDoc,maxMemCell,maxValue,format);

    setxxx(xlsxDoc,false);
    xlsxDoc.save();


    qStartRow_battery_Index++;
}


void WriteResult::receiveMinMax_cpuTemp(const QString &arg_deviceId, int arg_min, int arg_max)
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("性能测试报告");

    QString minValue;
    QString maxValue;
    QString startRowIndex;
    QString deviceIdCell;
    QString minMemCell;
    QString maxMemCell;

    Format format=getNormalFormat();

    int width1=arg_deviceId.length();
    if(maxWidth<=width1)
    {
        maxWidth=width1;
    }

    minValue=QString::number(arg_min);
    maxValue=QString::number(arg_max);
    startRowIndex=QString::number(qStartRow_cpuTemp_Index);

    deviceIdCell="A" + startRowIndex;
    writeStyle(xlsxDoc,deviceIdCell,arg_deviceId,format);

    minMemCell="L" + startRowIndex;
    writeStyle(xlsxDoc,minMemCell,minValue,format);

    maxMemCell="M" + startRowIndex;
    writeStyle(xlsxDoc,maxMemCell,maxValue,format);

    setxxx(xlsxDoc,false);

    xlsxDoc.save();


    qStartRow_cpuTemp_Index++;
}



void WriteResult::writeTitle(Document &arg_xlsx,const QString &arg_deviceId,const QString &arg_two,const QString &arg_detailedPath)
{
    int letterIndex;

    QString topIndex;
    QString rowIndex;

    //QUrl url;

    Format titleFormat;
    Format hyperlinkFormat;

    titleFormat=getTitleFormat();
    hyperlinkFormat=getHyperlinkFormat();

    topIndex=qDevice_TopIndex_Hash.value(arg_deviceId);

    rowIndex=topIndex + "1";
    writeStyle(arg_xlsx,rowIndex,arg_deviceId,titleFormat);

    //rowIndex=topIndex + "2";
    //writeStyle(arg_xlsx,rowIndex,"具体详情",format);


    //QUrl url("http://www.sohu.com");

    //url.setUrl("http://www.sohu.com");

    letterIndex=gLetterHash.value(topIndex);

    QUrl url = QUrl::fromLocalFile(arg_detailedPath + QDir::separator() + arg_two);

    writeHyperlink(arg_xlsx,2,letterIndex,url,hyperlinkFormat,"具体详情","");

    rowIndex=topIndex + "3";
    writeStyle(arg_xlsx,rowIndex,"测试结果",titleFormat);

    int maxWidth1=arg_deviceId.length();
    arg_xlsx.setColumnWidth(letterIndex,maxWidth1+5);

}


void WriteResult::receiveTraffic(const QString &arg_deviceId,QHash<QString,QString> arg_hash)
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("性能测试报告");


    QString tempStr;
    QString trafficValue;

    QString lrxWifi="0";
    QString ltxWifi="0";

    QString lrxMobile="0";
    QString ltxMobile="0";

    QString startRowIndex;
    QString deviceIdCell;

    QString lrxWifiCell;
    QString ltxWifiCell;
    QString lrxMobileCell;
    QString ltxMobileCell;

    QStringList splitResult;

    Format format=getNormalFormat();

    int width1=arg_deviceId.length();
    if(maxWidth<=width1)
    {
        maxWidth=width1;
    }

    if(arg_hash.isEmpty())
    {
        lrxWifi="0.00";
        ltxWifi="0.00";
        lrxMobile="0.00";
        ltxMobile="0.00";
    }
    else
    {
        tempStr=arg_deviceId + "=" + "currentStateWifi";
        if(tempStr.isEmpty())
        {
            lrxWifi="0.00";
            ltxWifi="0.00";
        }
        else
        {
            if(arg_hash.keys().contains(tempStr))
            {
                trafficValue=arg_hash.value(tempStr);
                splitResult=trafficValue.split("=");

                lrxWifi=splitResult.at(0);
                ltxWifi=splitResult.at(1);
            }
            else
            {
                lrxWifi="0.00";
                ltxWifi="0.00";
            }
        }

        tempStr=arg_deviceId + "=" + "currentStateMobile";
        if(tempStr.isEmpty())
        {
            lrxMobile="0.00";
            ltxMobile="0.00";
        }
        else
        {
            if(arg_hash.keys().contains(tempStr))
            {
                trafficValue=arg_hash.value(tempStr);
                splitResult=trafficValue.split("=");

                lrxMobile=splitResult.at(2);
                ltxMobile=splitResult.at(3);
            }
            else
            {
                lrxMobile="0.00";
                ltxMobile="0.00";
            }
        }
    }


    startRowIndex=QString::number(qStartRow_traffic_Index);

    deviceIdCell="A" + startRowIndex;
    writeStyle(xlsxDoc,deviceIdCell,arg_deviceId,format);

    lrxWifiCell="F" + startRowIndex;
    writeStyle(xlsxDoc,lrxWifiCell,lrxWifi,format);


    ltxWifiCell="G" + startRowIndex;
    writeStyle(xlsxDoc,ltxWifiCell,ltxWifi,format);

    lrxMobileCell="H" + startRowIndex;
    writeStyle(xlsxDoc,lrxMobileCell,lrxMobile,format);

    ltxMobileCell="I" + startRowIndex;
    writeStyle(xlsxDoc,ltxMobileCell,ltxMobile,format);

    setxxx(xlsxDoc,false);

    xlsxDoc.save();


    qStartRow_traffic_Index++;
}

void WriteResult::writeStyle(Document &arg_xlsx,const QString &arg_cell, const QString &arg_text,Format arg_format)
{
    arg_xlsx.write(arg_cell,arg_text,arg_format);
}

void WriteResult::writeHyperlink(Document &arg_xlsx,int arg_row,int arg_column,QUrl arg_url,Format arg_format,const QString &arg_text,const QString &arg_tip)
{

    arg_xlsx.currentWorksheet()->writeHyperlink(arg_row,arg_column,arg_url,arg_format,arg_text,"");
}

Format WriteResult::getTitleFormat()
{
    QColor color(220,230,241,255);
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setPatternBackgroundColor(color);
    format.setBorderStyle(Format::BorderThin);
    return format;
}

Format WriteResult::getNormalFormat()
{
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);
    return format;
}

Format WriteResult::getErrorFormat()
{
    Format format;
    format.setFontColor(Qt::red);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);
    return format;
}

Format WriteResult::getHyperlinkFormat()
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

void WriteResult::receiveBaseInfoResult(QString arg_info)
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("总体报告");

    QString appName;
    QString appVersion;
    QString fileSize;
    //QString deviceNumber;
    QString platform;
    QString testDate;

    Format format=getNormalFormat();

    QStringList splistResult=arg_info.split("=");
    if(splistResult.size()==6)
    {
        appName=splistResult.at(0);
        appVersion=splistResult.at(1);
        fileSize=splistResult.at(2);
        //deviceNumber=splistResult.at(3);
        platform=splistResult.at(4);
        testDate=splistResult.at(5);
    }

    QString strCell;

    strCell="C1";
    writeStyle(xlsxDoc,strCell,appName,format);

    strCell="C2";
    writeStyle(xlsxDoc,strCell,fileSize,format);

    strCell="C3";
    writeStyle(xlsxDoc,strCell,platform,format);

    strCell="F1";
    writeStyle(xlsxDoc,strCell,appVersion,format);

    //strCell="F2";
    //writeStyle(xlsxDoc,strCell,deviceNumber,format);

    strCell="F3";
    writeStyle(xlsxDoc,strCell,testDate,format);

    setxxx(xlsxDoc,false);

    xlsxDoc.save();


}

void WriteResult::receiveSec(int arg_sec)
{
    //double dsec=(double)arg_sec/1000;
    double dsec=(double)arg_sec*0.001;
    //QString secStr=QString::number(dsec,'f',2);
    QString secStr=QString::number(dsec);

    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("总体报告");

    Format format=getNormalFormat();

    QString strCell;

    strCell="F2";
    writeStyle(xlsxDoc,strCell,secStr,format);

    setxxx(xlsxDoc,false);

    xlsxDoc.save();
}

void WriteResult::receiveIcon(QString arg_iconfile)
{
    QImage icon(arg_iconfile);

    QImage fitimgpic;
    if(icon.width()>200)
    {
        fitimgpic=icon.scaled(icon.width()/2,icon.height()/2, Qt::KeepAspectRatioByExpanding);
    }
    else
    {
        fitimgpic=icon.scaled(icon.width(),icon.height(), Qt::KeepAspectRatioByExpanding);
    }

    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("总体报告");
    xlsxDoc.insertImage(0,0,fitimgpic);


    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);

    QString index1="1";
    QString index2="3";

    QString cell="A" + index1 + ":" + "A" + index2;
    xlsxDoc.mergeCells(cell, format);

    setxxx(xlsxDoc,false);
    xlsxDoc.save();
}



void WriteResult::receiveBaseInfoResult(QStringList arg_infoList)
{
    Document xlsxDoc(qTargetXlsx);
    xlsxDoc.selectSheet("总体报告");

    QString brand;
    QString model;
    QString version;
    QString wmsize;

    QString strIndex;
    QString strCell;

    QString cell;
    Format format=getNormalFormat();

    QStringList splitResult;
    for(QString var:arg_infoList)
    {
        splitResult=var.split("=");
        if(splitResult.size()==5)
        {
            brand=splitResult.at(1);
            strIndex=QString::number(qStartRow_base_Index);
            strCell="A"+strIndex;
            writeStyle(xlsxDoc,strCell,brand,format);

            model=splitResult.at(2);
            strIndex=QString::number(qStartRow_base_Index);
            strCell="B"+strIndex;
            writeStyle(xlsxDoc,strCell,model,format);

            version=splitResult.at(3);
            strIndex=QString::number(qStartRow_base_Index);
            strCell="C"+strIndex;
            writeStyle(xlsxDoc,strCell,version,format);

            cell="C"+strIndex+":"+"D"+strIndex;
            mergeCells(xlsxDoc,cell,format);

            wmsize=splitResult.at(4);
            strIndex=QString::number(qStartRow_base_Index);
            strCell="E"+strIndex;
            writeStyle(xlsxDoc,strCell,wmsize,format);
            cell="E"+strIndex+":"+"F"+strIndex;
            mergeCells(xlsxDoc,cell,format);
        }
        qStartRow_base_Index++;
    }
    setxxx(xlsxDoc,false);
    xlsxDoc.save();
}

void WriteResult::mergeCells(Document &arg_xlsx,const QString &arg_cell,Format arg_format)
{
    arg_xlsx.mergeCells(arg_cell, arg_format);
}


void WriteResult::setxxx(Document &arg_xlsx,bool b)
{
    arg_xlsx.currentWorksheet()->setSelected(b);
}
