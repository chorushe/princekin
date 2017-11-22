#include "createreport.h"

CreateReport::CreateReport()
{
}

void CreateReport::setFirstLevelDirName(const QString &arg_firstLevelDirName)
{
    qFirstLevelDirName=arg_firstLevelDirName;
}

void CreateReport::setSecondLevelDirNameList(QStringList arg_secondLevelDirNameList)
{
    qSecondLevelDirNameList=arg_secondLevelDirNameList;
}

void CreateReport::setScriptList(QStringList arg_scriptList)
{
    qScriptList=arg_scriptList;
}

void CreateReport::setModule_FileCount_List(QStringList arg_)
{
    qModule_FileCount_List=arg_;
}


void CreateReport::createReportSaveDir()
{
    QString strDir;
    QString strFile;
    QString firstLevelDir;
    QString secondLevelDir;
    QString secondLevelDirName;
    QString performanceDir;
    QString crashTargetXlsx;
    QString crashSourceXlsx;
    QString firstLevelPath;
    QString secondLevelPath;



    gStartTextDate=Helper::getTime2("yyyyMMdd");

    //D:\princekinWorkspace\report\20170927
    gDatePath=gWorkSpace +  QDir::separator() + "report" +  QDir::separator() + gStartTextDate;
    gDatePath=QDir::toNativeSeparators(gDatePath);
    Helper::createPath(gDatePath);

    //D:\princekinWorkspace\report\20170927\com.sohu.qianfan
    gPackagePath=gDatePath + QDir::separator() + gPackageName;
    gPackagePath=QDir::toNativeSeparators(gPackagePath);
    Helper::createPath(gPackagePath);


    //D:\princekinWorkspace\report\20170927\com.sohu.qianfan\sohuvideoreport_20170927_155913
    firstLevelPath=gPackagePath + QDir::separator() + qFirstLevelDirName;
    firstLevelPath=QDir::toNativeSeparators(firstLevelPath);
    Helper::createPath(firstLevelPath);


    crashSourceXlsx=gConfigDir + QDir::separator() + "crash.xlsx";

    //D:\princekinWorkspace\report\20170927\com.sohu.qianfan\sohuvideoreport_20170927_155913

    firstLevelDir=gWorkSpace +  QDir::separator() + "report" +  QDir::separator() + qFirstLevelDirName;
    firstLevelDir=QDir::toNativeSeparators(firstLevelDir);
    Helper::createPath(firstLevelDir);


    int count=qSecondLevelDirNameList.count();
    for(int i=0;i<count;i++)
    {
        secondLevelDirName=qSecondLevelDirNameList.at(i);

        secondLevelPath=firstLevelPath + QDir::separator() + secondLevelDirName;
        secondLevelPath=QDir::toNativeSeparators(secondLevelPath);
        Helper::createPath(secondLevelPath);

        strDir=secondLevelPath + QDir::separator() + "png";
        Helper::createPath(strDir);

        strDir=secondLevelPath + QDir::separator() + "logcat";
        Helper::createPath(strDir);

        performanceDir=secondLevelPath + QDir::separator() + "performance";
        Helper::createPath(performanceDir);






        secondLevelDir=firstLevelDir + QDir::separator() + secondLevelDirName;
        secondLevelDir=QDir::toNativeSeparators(secondLevelDir);
        Helper::createPath(secondLevelDir);

        crashTargetXlsx=secondLevelDir + QDir::separator() + secondLevelDirName + "_crash.xlsx";
        //QFile::copy(crashSourceXlsx,crashTargetXlsx);

        strDir=secondLevelDir + QDir::separator() + "png";
        Helper::createPath(strDir);

        strDir=secondLevelDir + QDir::separator() + "logcat";
        Helper::createPath(strDir);


        performanceDir=secondLevelDir + QDir::separator() + "performance";
        Helper::createPath(performanceDir);


        /*
        strFile=performanceDir + QDir::separator() + "memory.txt";
        Helper::createfile(strFile);


        strFile=performanceDir + QDir::separator() + "cpu.txt";
        Helper::createfile(strFile);



        strFile=performanceDir + QDir::separator() + "cpu-temperature.txt";
        Helper::createfile(strFile);

        strFile=performanceDir + QDir::separator() + "battery-temperature.txt";
        Helper::createfile(strFile);


        strFile=secondLevelDir + QDir::separator() + "performance-report.txt";
        Helper::createfile(strFile);

        strFile=secondLevelDir + QDir::separator() + "script-report.txt";
        Helper::createfile(strFile);
        */
    }//for
}


void CreateReport::writeExcel(const QString &arg_xlsx)
{
    Document xlsx(arg_xlsx);
    xlsx.selectSheet("自动化测试报告");

    writeTitle(xlsx);
    writeData(xlsx);
    mergeModule(xlsx);

    xlsx.save();
}

void CreateReport::writeTitle(Document &arg_xlsx)
{
    int fontSize=12;

    QString strName;
    QString strCell;
    QColor color(220,230,241,255);

    Format format;
    //format.setFontSize(fontSize);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setPatternBackgroundColor(color);
    format.setBorderStyle(Format::BorderThin);



    strCell="A1";
    strName="机型";
    writeStyle(arg_xlsx,strCell,strName,format);
    mergeCells(arg_xlsx,"A1:B1",format);


    strCell="A2";
    strName="详情";
    writeStyle(arg_xlsx,strCell,strName,format);
    mergeCells(arg_xlsx,"A2:B2",format);



    strCell="A3";
    strName="模块";
    writeStyle(arg_xlsx,strCell,strName,format);

    strCell="B3";
    strName="用例";
    writeStyle(arg_xlsx,strCell,strName,format);
}


void CreateReport::writeData(Document &arg_xlsx)
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

    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);


    foreach(QString var,qScriptList)
    {
        splitResult=var.split("=");

        moduleName=splitResult.at(0);
        width1=moduleName.length();
        if(maxWidth1<=width1)
        {
            maxWidth1=width1;
        }


        scriptName=splitResult.at(1) + ".txt";

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

        number++;
    }

    arg_xlsx.setColumnWidth(1,maxWidth1+5);
    arg_xlsx.setColumnWidth(2,maxWidth2+5);
}

void CreateReport::writeStyle(Document &arg_xlsx,const QString &arg_cell, const QString &arg_text,Format arg_format)
{
    arg_xlsx.write(arg_cell,arg_text,arg_format);
}


void CreateReport::mergeModule(Document &arg_xlsx)
{
    int count;
    int scriptCount;
    int startIndex;
    int endMergeIndex;

    bool ok;


    QString s1;
    QString s2;
    QString str;

    QStringList splitResult;

    Format format;

    format.setFontSize(16);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);


    endMergeIndex=3;

    QStringList tempList;
    for(QString var:qModule_FileCount_List)
    {
        if(tempList.indexOf(var)==-1)
        {
            tempList.append(var);
        }
    }

    for(QString var:tempList)
    {
        splitResult=var.split("=");
        scriptCount=splitResult.at(1).toInt(&ok,10);

        startIndex=endMergeIndex+1;
        endMergeIndex=startIndex+scriptCount-1;

        s1=QString::number(startIndex);
        s2=QString::number(endMergeIndex);

        mergeCells(arg_xlsx,"A" + s1 + ":" + "A" + s2, format);
    }

    /*
    count=qModule_FileCount_List.size();
    for(int i=0;i<count;i++)
    {
        str=qModule_FileCount_List.at(i);
        splitResult=str.split("=");
        scriptCount=splitResult.at(1).toInt(&ok,10);


        startIndex=endMergeIndex+1;
        endMergeIndex=startIndex+scriptCount-1;

        s1=QString::number(startIndex);
        s2=QString::number(endMergeIndex);

        mergeCells(arg_xlsx,"A" + s1 + ":" + "A" + s2, format);
    }
    */



}

void CreateReport::mergeCells(Document &arg_xlsx,const QString &arg_cell,Format arg_format)
{
    arg_xlsx.mergeCells(arg_cell, arg_format);
}
