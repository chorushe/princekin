#include "replayhtml.h"

ReplayHtml::ReplayHtml()
{

}

void ReplayHtml::setSaveDir(const QString &arg_saveDir,const QString &arg_jssaveDir)
{
    qHtmlSaveDir=arg_saveDir;
    qJsSaveDir=arg_jssaveDir;
}

void ReplayHtml::setWarningValue(const QString &arg_warning1, const QString &arg_warning2, const QString &arg_warning3, const QString &arg_warning4, const QString &arg_warning5, const QString &arg_warning6)
{
    qMemWarningValue=arg_warning1;
    qCpuWarningValue=arg_warning2;
    qCpuTempWarningValue=arg_warning3;
    qBatteryTempWarningValue=arg_warning4;
    qWifiWarningValue=arg_warning5;
    qMobileWarningValue=arg_warning6;
}


void ReplayHtml::setTime_vector(QVector<QString> arg_1,QVector<QString> arg_2,QVector<QString> arg_3,QVector<QString> arg_4,QVector<QString> arg_5,QVector<QString> arg_6)
{
    qMemTime_vector=arg_1;
    qCpuTime_vector=arg_2;
    qCpuTempTime_vector=arg_3;
    qBatteryTempTime_vector=arg_4;
    qWifiTime_vector=arg_5;
    qMobileTime_vector=arg_6;
}

void ReplayHtml::setValue_vector(QVector<double> arg_1,QVector<double> arg_2,QVector<double> arg_3,QVector<double> arg_4,QVector<double> arg_5,QVector<double> arg_6)
{
    qMemValue_vector=arg_1;
    qCpuValue_vector=arg_2;
    qCpuTempValue_vector=arg_3;
    qBatteryTempValue_vector=arg_4;
    qWifiValue_vector=arg_5;
    qMobileValue_vector=arg_6;
}


void ReplayHtml::createHtml()
{
    int len;
    QString ss1;
    QString ss2="];";
    QString ss3;
    QString filePath;
    QStringList splitResult;
    QFile file;

    QStringList templist;

    len=qMemValue_vector.count();
    if(len!=0)
    {
        templist.append("memory");

        filePath=qJsSaveDir + QDir::separator() + "memory_value.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_value(&file);
        ss1="var memoryValueArr=[";
        outStream_value<<ss1;
        foreach (double var, qMemValue_vector)
        {
            outStream_value<<var;
            outStream_value<<",";
        }
        outStream_value<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "memory_time.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_time(&file);
        ss1="var memoryTimeArr=[";
        outStream_time<<ss1;
        foreach (QString var, qMemTime_vector)
        {
            splitResult=var.split(" ");
            outStream_time<<"\"" + splitResult.at(1) + "\"";
            outStream_time<<",";
        }
        outStream_time<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "memory_warning.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }


        QTextStream outStream_warning(&file);
        ss1="var memoryWarningArr=[";
        outStream_warning<<ss1;

        if(qMemWarningValue.isEmpty())
        {
            ss3="0";
        }
        else
        {
            ss3=qMemWarningValue;
        }
        for(int i=0;i<len;i++)
        {
            outStream_warning<<ss3;
            outStream_warning<<",";
        }
        outStream_warning<<ss2;
        file.close();
    }

    //*************************************************//
    len=qCpuValue_vector.count();
    if(len!=0)
    {
        templist.append("cpu");

        filePath=qJsSaveDir + QDir::separator() + "cpu_value.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_value(&file);
        ss1="var cpuValueArr=[";
        outStream_value<<ss1;
        foreach (double var, qCpuValue_vector)
        {
            outStream_value<<var;
            outStream_value<<",";
        }
        outStream_value<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "cpu_time.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_time(&file);
        ss1="var cpuTimeArr=[";
        outStream_time<<ss1;
        foreach (QString var, qCpuTime_vector)
        {
            splitResult=var.split(" ");
            outStream_time<<"\"" + splitResult.at(1) + "\"";
            outStream_time<<",";
        }
        outStream_time<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "cpu_warning.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_warning(&file);
        ss1="var cpuWarningArr=[";
        outStream_warning<<ss1;

        if(qCpuWarningValue.isEmpty())
        {
            ss3="0";
        }
        else
        {
            ss3=qCpuWarningValue;
        }
        for(int i=0;i<len;i++)
        {
            outStream_warning<<ss3;
            outStream_warning<<",";
        }
        outStream_warning<<ss2;
        file.close();
    }

    //*************************************************//
    len=qCpuTempValue_vector.count();
    if(len!=0)
    {
        templist.append("cpuTemp");

        filePath=qJsSaveDir + QDir::separator() + "cpuTemp_value.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_value(&file);
        ss1="var cpuTempValueArr=[";
        outStream_value<<ss1;
        foreach (double var, qCpuTempValue_vector)
        {
            outStream_value<<var;
            outStream_value<<",";
        }
        outStream_value<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "cpuTemp_time.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }


        QTextStream outStream_time(&file);
        ss1="var cpuTempTimeArr=[";
        outStream_time<<ss1;
        foreach (QString var, qCpuTempTime_vector)
        {
            splitResult=var.split(" ");
            outStream_time<<"\"" + splitResult.at(1) + "\"";
            outStream_time<<",";
        }
        outStream_time<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "cpuTemp_warning.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_warning(&file);
        ss1="var cpuTempWarningArr=[";
        outStream_warning<<ss1;

        if(qCpuTempWarningValue.isEmpty())
        {
            ss3="0";
        }
        else
        {
            ss3=qCpuTempWarningValue;
        }
        for(int i=0;i<len;i++)
        {
            outStream_warning<<ss3;
            outStream_warning<<",";
        }
        outStream_warning<<ss2;
        file.close();
    }

    //*************************************************//
    len=qBatteryTempValue_vector.count();
    if(len!=0)
    {
        templist.append("batteryTemp");

        filePath=qJsSaveDir + QDir::separator() + "batteryTemp_value.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_value(&file);
        ss1="var batteryTempValueArr=[";
        outStream_value<<ss1;
        foreach (double var, qBatteryTempValue_vector)
        {
            outStream_value<<var;
            outStream_value<<",";
        }
        outStream_value<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "batteryTemp_time.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }


        QTextStream outStream_time(&file);
        ss1="var batteryTempTimeArr=[";
        outStream_time<<ss1;
        foreach (QString var, qBatteryTempTime_vector)
        {
            splitResult=var.split(" ");
            outStream_time<<"\"" + splitResult.at(1) + "\"";
            outStream_time<<",";
        }
        outStream_time<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "batteryTemp_warning.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_warning(&file);
        ss1="var batteryTempWarningArr=[";
        outStream_warning<<ss1;

        if(qBatteryTempWarningValue.isEmpty())
        {
            ss3="0";
        }
        else
        {
            ss3=qBatteryTempWarningValue;
        }
        for(int i=0;i<len;i++)
        {
            outStream_warning<<ss3;
            outStream_warning<<",";
        }
        outStream_warning<<ss2;
        file.close();
    }

    //*************************************************//
    len=qWifiValue_vector.count();
    if(len!=0)
    {
        templist.append("wifi");

        filePath=qJsSaveDir + QDir::separator() + "wifi_value.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_value(&file);
        ss1="var wifiValueArr=[";
        outStream_value<<ss1;
        foreach (double var, qWifiValue_vector)
        {
            outStream_value<<var;
            outStream_value<<",";
        }
        outStream_value<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "wifi_time.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }


        QTextStream outStream_time(&file);
        ss1="var wifiTimeArr=[";
        outStream_time<<ss1;
        foreach (QString var, qWifiTime_vector)
        {
            splitResult=var.split(" ");
            outStream_time<<"\"" + splitResult.at(1) + "\"";
            outStream_time<<",";
        }
        outStream_time<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "wifi_warning.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_warning(&file);
        ss1="var wifiWarningArr=[";
        outStream_warning<<ss1;

        if(qWifiWarningValue.isEmpty())
        {
            ss3="0";
        }
        else
        {
            ss3=qWifiWarningValue;
        }
        for(int i=0;i<len;i++)
        {
            outStream_warning<<ss3;
            outStream_warning<<",";
        }
        outStream_warning<<ss2;
        file.close();
    }

    //*************************************************//
    len=qMobileValue_vector.count();
    if(len!=0)
    {
        templist.append("mobile");

        filePath=qJsSaveDir + QDir::separator() + "mobile_value.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_value(&file);
        ss1="var mobileValueArr=[";
        outStream_value<<ss1;
        foreach (double var, qMobileValue_vector)
        {
            outStream_value<<var;
            outStream_value<<",";
        }
        outStream_value<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "mobile_time.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }


        QTextStream outStream_time(&file);
        ss1="var mobileTimeArr=[";
        outStream_time<<ss1;
        foreach (QString var, qMobileTime_vector)
        {
            splitResult=var.split(" ");
            outStream_time<<"\"" + splitResult.at(1) + "\"";
            outStream_time<<",";
        }
        outStream_time<<ss2;
        file.close();


        filePath=qJsSaveDir + QDir::separator() + "mobile_warning.js";
        file.setFileName(filePath);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示",  "文件异常");
            return;
        }

        QTextStream outStream_warning(&file);
        ss1="var mobileWarningArr=[";
        outStream_warning<<ss1;

        if(qMobileWarningValue.isEmpty())
        {
            ss3="0";
        }
        else
        {
            ss3=qMobileWarningValue;
        }
        for(int i=0;i<len;i++)
        {
            outStream_warning<<ss3;
            outStream_warning<<",";
        }
        outStream_warning<<ss2;
        file.close();
    }
    createHtml2(templist);
}


void ReplayHtml::createHtml2(QStringList templist)
{
    QString filePath=qHtmlSaveDir + QDir::separator() + "web.html";
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        file.close();
        QMessageBox::information(NULL,"提示",  "文件异常");
        return;
    }
    QTextStream outStream_web(&file);
    outStream_web.setCodec(qTc);

    QString strLine;

    QFile file2(gConfigDir + QDir::separator() + "replay_web1.html");
    if(file2.open(QIODevice::ReadOnly))
    {
        QTextStream inStream(&file2);
        while(!inStream.atEnd())
        {
            strLine=inStream.readLine();
            outStream_web<<strLine<<endl;
        }
    }
    file2.close();

    QString sss;
    for(QString var:templist)
    {
        sss="    <script src=\"html/" + var + "_value.js" + "\"></script>";
        outStream_web<<sss<<endl;

        sss="    <script src=\"html/" + var + "_time.js" + "\"></script>";
        outStream_web<<sss<<endl;

        sss="    <script src=\"html/" + var + "_warning.js" + "\"></script>";
        outStream_web<<sss<<endl;
    }

    for(QString var:templist)
    {
        sss="    <div id=\"" + var + "\" style=\"height:400px;width: 20000px\"></div>";
        outStream_web<<sss<<endl;
    }

    file2.setFileName(gConfigDir + QDir::separator() + "web2.html");
    if(file2.open(QIODevice::ReadOnly))
    {
        QTextStream inStream(&file2);
        while(!inStream.atEnd())
        {
            strLine=inStream.readLine();
            outStream_web<<strLine<<endl;
        }
    }
    file2.close();



    for(QString var:templist)
    {
        sss="            draw"+var+"(ec);";
        outStream_web<<sss<<endl;
    }



    file2.setFileName(gConfigDir + QDir::separator() + "web3.html");
    if(file2.open(QIODevice::ReadOnly))
    {
        QTextStream inStream(&file2);
        while(!inStream.atEnd())
        {
            strLine=inStream.readLine();
            outStream_web<<strLine<<endl;
        }
    }
    file2.close();

    file.close();
}
