#include "dataworker.h"
#include "windows.h"
#include <string>


using namespace std;

namespace DataWorker
{

Worker::Worker()
{
}

void Worker::setXlsx(const QString &arg_text)
{
    qXlsx=arg_text;
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

void Worker::setMemTime_Vector(QVector<QString> arg_vector)
{
    qMemTime_vector=arg_vector;
}

void Worker::setCpuTime_Vector(QVector<QString> arg_vector)
{
    qCpuTime_vector=arg_vector;
}

void Worker::setCpuTempTime_Vector(QVector<QString> arg_vector)
{
    qCpuTempTime_vector=arg_vector;
}

void Worker::setBatteryTempTime_Vector(QVector<QString> arg_vector)
{
    qBatteryTempTime_vector=arg_vector;
}

void Worker::setWifiTime_Vector(QVector<QString> arg_vector)
{
    qWifiTime_vector=arg_vector;
}

void Worker::setMobileTime_Vector(QVector<QString> arg_vector)
{
    qMobileTime_vector=arg_vector;
}

void Worker::setMemory_Vector(QVector<double> arg_vector)
{
    qMemValue_vector=arg_vector;
}

void Worker::setCpu_Vector(QVector<double> arg_vector)
{
    qCpuValue_vector=arg_vector;
}

void Worker::setCpuTemp_Vector(QVector<double> arg_vector)
{
    qCpuTempValue_vector=arg_vector;
}

void Worker::setBatteryTemp_Vector(QVector<double> arg_vector)
{
    qBatteryTempValue_vector=arg_vector;
}

void Worker::setWifi_Vector(QVector<double> arg_vector)
{
    qWifiValue_vector=arg_vector;
}

void Worker::setMobile_Vector(QVector<double> arg_vector)
{
    qMobileValue_vector=arg_vector;
}


void Worker::startWorker()
{
    Document xlsx(qXlsx);
    Format format;
    format=getNormalFormat();

    if(qMemTime_vector.size()!=0 && qMemValue_vector.size()!=0)
    {
        writeTime(xlsx,format,"memory_time","A",qMemTime_vector,1,25);
        writeWarning(xlsx,format,"memory_waring","B",qMemWarningValue,qMemTime_vector,2,25);
        writeData(xlsx,format,"memory","C",qMemValue_vector,3,25);
    }

    if(qCpuTime_vector.size()!=0 && qCpuValue_vector.size()!=0)
    {
        writeTime(xlsx,format,"cpu_time","D",qCpuTime_vector,4,25);
        writeWarning(xlsx,format,"cpu_waring","E",qCpuWarningValue,qCpuTime_vector,5,25);
        writeData(xlsx,format,"cpu","F",qCpuValue_vector,6,25);
    }

    if(qCpuTempTime_vector.size()!=0 && qCpuTempValue_vector.size()!=0)
    {
        writeTime(xlsx,format,"cputemp_time","G",qCpuTempTime_vector,7,25);
        writeWarning(xlsx,format,"cputemp_waring","H",qCpuTempWarningValue,qCpuTempTime_vector,8,25);
        writeData(xlsx,format,"cputemp","I",qCpuTempValue_vector,9,25);
    }


    if(qBatteryTempTime_vector.size()!=0 && qBatteryTempValue_vector.size()!=0)
    {
        writeTime(xlsx,format,"batterytemp_time","J",qBatteryTempTime_vector,10,25);
        writeWarning(xlsx,format,"batterytemp_waring","K",qBatteryTempWarningValue,qBatteryTempTime_vector,11,25);
        writeData(xlsx,format,"batterytemp","L",qBatteryTempValue_vector,12,25);
    }

    if(qWifiTime_vector.size()!=0 && qWifiValue_vector.size()!=0)
    {
        writeTime(xlsx,format,"wifi_time","M",qWifiTime_vector,13,25);
        writeWarning(xlsx,format,"wifi_waring","N",qWifiWarningValue,qWifiTime_vector,14,25);
        writeData(xlsx,format,"wifi","O",qWifiValue_vector,15,25);
    }

    if(qMobileTime_vector.size()!=0 && qMobileValue_vector.size()!=0)
    {
        writeTime(xlsx,format,"mobile_time","P",qMobileTime_vector,16,25);
        writeWarning(xlsx,format,"mobile_waring","Q",qMobileWarningValue,qMobileTime_vector,17,25);
        writeData(xlsx,format,"mobile","R",qMobileValue_vector,18,25);
    }

    xlsx.selectSheet("总体报告");
    xlsx.save();

    emit sendWorkerResult("worker");
}


void Worker::writeTime(Document &arg_xlsx,Format arg_format,const QString &arg_markName,const QString &arg_columnName,QVector<QString> arg_timeVector,int arg_columnIndex,int arg_width)
{
    arg_xlsx.addSheet("sourceData");
    arg_xlsx.write(arg_columnName + "1", arg_markName,arg_format);

    int index=2;

    QString tempStr;
    QString timeStr;
    QString cellIndex;
    QStringList splitResult;

    foreach(QString var,arg_timeVector)
    {
        splitResult=var.split(" ");
        timeStr=splitResult.at(1);

        tempStr=QString::number(index);
        cellIndex=arg_columnName + tempStr;

        arg_xlsx.write(cellIndex, timeStr);

        index++;
        if(index>1048576)
        {
            return;
        }
    }

    arg_xlsx.setColumnWidth(arg_columnIndex,arg_width);
}


void Worker::writeWarning(Document &arg_xlsx,Format arg_format,const QString &arg_markName,const QString &arg_columnName,const QString &arg_waringValue,QVector<QString> arg_vector,int arg_columnIndex,int arg_width)
{
    arg_xlsx.selectSheet("sourceData");
    arg_xlsx.write(arg_columnName+"1", arg_markName,arg_format);

    int index=2;
    QString tempStr;
    QString cellIndex;

    int dValue=arg_waringValue.toInt();
    int count=arg_vector.size();
    for(int i=0;i<count;i++)
    {
        tempStr=QString::number(index);
        cellIndex=arg_columnName + tempStr;
        arg_xlsx.write(cellIndex, dValue);
        index++;
        if(index>1048576)
        {
            return;
        }
    }

    arg_xlsx.setColumnWidth(arg_columnIndex,arg_width);
}


void Worker::writeData(Document &arg_xlsx,Format arg_format,const QString &arg_markName,const QString &arg_columnName,QVector<double> arg_dataVector,int arg_columnIndex,int arg_width)
{
    arg_xlsx.selectSheet("sourceData");
    arg_xlsx.write(arg_columnName + "1", arg_markName,arg_format);

    int index=2;
    QString tempStr;
    QString cellIndex;

    foreach(double var,arg_dataVector)
    {
        tempStr=QString::number(index);
        cellIndex=arg_columnName + tempStr;
        arg_xlsx.write(cellIndex, var);
        index++;
        if(index>1048576)
        {
            return;
        }
    }

    arg_xlsx.setColumnWidth(arg_columnIndex,arg_width);
}


Format Worker::getNormalFormat()
{
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);
    return format;
}


}
