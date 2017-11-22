#include "test.h"

#include <QRegExp>

Test::Test()
{
}

void Test::getMemory(const QString &arg_deviceid,const QString &arg_cmdLine,int arg_interval)
{
    int dtemp;
    int min=0;
    int max=0;

    bool isFirstValue=true;

    QRegExp rx("(\\d+)");

    while(true)
    {
        if(gStopMemoryHash.value(arg_deviceid))
        {
            return;
        }
        QProcess proc;
        proc.setReadChannelMode(QProcess::MergedChannels);
        proc.start(arg_cmdLine);
        proc.waitForStarted();
        while(proc.waitForFinished()==false)
        {
        }

        bool ok;
        QString mLine;
        QString mCurrentMemoryValue;

        while(proc.canReadLine())
        {
            mLine=proc.readLine();
            if(mLine.contains("TOTAL"))
            {
                QStringList list;
                int pos=0;

                while((pos = rx.indexIn(mLine, pos)) != -1)
                {
                    list << rx.cap(1);
                    pos += rx.matchedLength();
                }

                mCurrentMemoryValue=list.at(0);


                dtemp=mCurrentMemoryValue.toInt(&ok,10);

                if(isFirstValue)
                {
                    min=dtemp;
                    max=dtemp;
                    isFirstValue=false;
                }

                if(dtemp<=min)
                {
                    min=dtemp;
                }
                if(dtemp>=max)
                {
                    max=dtemp;
                }
                emit sendMemoryResult(arg_deviceid,mCurrentMemoryValue,min,max);
            }
        }
        proc.close();
        Sleep(gdInterval);
    }//while(true)
}


void Test::getCpu(const QString &arg_deviceid,const QString &arg_cmdLine,int arg_interval)
{
    float ftemp;
    float min=0.0;
    float max=0.0;

    bool isFirstValue=true;

    QRegExp rx("(\\d+)");

    while(true)
    {
        if(gStopMemoryHash.value(arg_deviceid))
        {
            return;
        }
        QProcess proc;
        proc.setReadChannelMode(QProcess::MergedChannels);
        proc.start(arg_cmdLine);
        proc.waitForStarted();
        while(proc.waitForFinished()==false)
        {
        }

        bool ok;
        QString mStr;
        QString mLine;
        QString mCurrentCpuValue;
        QStringList mSplitList;

        while(proc.canReadLine())
        {
            mLine=proc.readLine().trimmed();
            if(mLine.contains(gPackageName))
            {
                mSplitList=mLine.split(": ");
                mStr=mSplitList.at(0);
                if(mStr.endsWith(gPackageName))
                {
                    mSplitList=mStr.split("%");
                    mCurrentCpuValue=mSplitList.at(0);

                    ftemp=mCurrentCpuValue.toFloat(&ok);
                    if(isFirstValue)
                    {
                        min=ftemp;
                        max=ftemp;
                        isFirstValue=false;
                    }

                    if(ftemp<=min)
                    {
                        min=ftemp;
                    }
                    if(ftemp>=max)
                    {
                        max=ftemp;
                    }
                }

                emit sendCpuResult(arg_deviceid,mCurrentCpuValue,min,max);
            }
        }
        proc.close();
        Sleep(gdInterval);
    }//while(true)
}

void Test::getBattery(const QString &arg_deviceid,const QString &arg_cmdLine,int arg_interval)
{
    int dtemp;
    float ftemp;
    float min=0.0;
    float max=0.0;

    bool isFirstValue=true;

    QRegExp rx("(\\d+)");

    while(true)
    {
        if(gStopMemoryHash.value(arg_deviceid))
        {
            return;
        }
        QProcess proc;
        proc.setReadChannelMode(QProcess::MergedChannels);
        proc.start(arg_cmdLine);
        proc.waitForStarted();
        while(proc.waitForFinished()==false)
        {
        }

        bool ok;
        QString mStr;
        QString mLine;
        QString mCurrentTemperatureValue;
        QStringList mSplitList;

        while(proc.canReadLine())
        {
            mLine=proc.readLine().trimmed();
            if(mLine.contains("temperature:"))
            {
                mSplitList=mLine.split(":");
                mStr=mSplitList.at(1);
                mCurrentTemperatureValue=mStr.trimmed();

                dtemp=mCurrentTemperatureValue.toInt(&ok,10);
                ftemp=dtemp*0.1;
                if(isFirstValue)
                {
                    min=ftemp;
                    max=ftemp;
                    isFirstValue=false;
                }

                if(ftemp<=min)
                {
                    min=ftemp;
                }
                if(ftemp>=max)
                {
                    max=ftemp;
                }

                emit sendBatteryResult(arg_deviceid,mCurrentTemperatureValue,min,max);
            }
        }
        proc.close();
        Sleep(gdInterval);
    }//while(true)
}

void Test::getCputemp(const QString &arg_deviceid,const QString &arg_cmdLine,int arg_interval)
{
    QString strTemperature;
    int tempValue;
    int min=0;
    int max=0;

    bool isFirstValue=true;
    bool ok;
    QString mLine;

    QVector<int> integerVector;

    while(true)
    {
        if(gStopMemoryHash.value(arg_deviceid))
        {
            return;
        }
        QProcess proc;
        proc.setReadChannelMode(QProcess::MergedChannels);
        proc.start(arg_cmdLine);
        proc.waitForStarted();
        while(proc.waitForFinished()==false)
        {
        }

        integerVector.clear();

        while(proc.canReadLine())
        {
            mLine=proc.readLine().trimmed();
            tempValue=mLine.toInt(&ok,10);
            if(ok)
            {
                if(tempValue>150 || tempValue<0)
                {
                }
                else
                {
                    integerVector.append(tempValue);
                }
            }
        }

        qSort(integerVector.begin(), integerVector.end());

        if(integerVector.isEmpty())
            tempValue=integerVector.last();
        else
        {
            tempValue=10;
            qDebug()<<"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh  "<<tempValue;
            return;
        }
        if(isFirstValue)
        {
            min=tempValue;
            max=tempValue;
            isFirstValue=false;
        }
        if(tempValue<=min)
        {
            min=tempValue;
        }
        if(tempValue>=max)
        {
            max=tempValue;
        }

        strTemperature=QString::number(tempValue);
        emit sendCputempResult(arg_deviceid,strTemperature,min,max);

        proc.close();
        Sleep(gdInterval);
    }//while(true)
}

