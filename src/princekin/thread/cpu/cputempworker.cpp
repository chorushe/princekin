#include "cputempworker.h"

namespace CpuTempWorker
{

Worker::Worker()
{
}

void Worker::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void Worker::setCmdLine(const QString &arg_text)
{
    qCmdLine=arg_text;
}

void Worker::initVar()
{
    qIsStop=false;
}

void Worker::receiveStopWorker()
{
    qIsStop=true;
}


void Worker::startWorker()
{
    int intTemp;
    int intMinValue=0;
    int intMaxValue=0;
    bool ok;
    bool isFirstValue=true;

    QString strLine;
    QString currentValue;

    QVector<int> intVector;

    QByteArray byteArray;

    while(true)
    {
        if(qIsStop)
        {
            return;
        }

        QProcess proc;
        proc.setReadChannelMode(QProcess::MergedChannels);
        proc.start(qCmdLine);
        proc.waitForStarted();

        while(proc.waitForFinished()==false)
        {
        }

        intVector.clear();

        while(proc.canReadLine())
        {
            byteArray=proc.readLine();
            strLine=qTc->toUnicode(byteArray).trimmed();

            intTemp=strLine.toInt(&ok,10);
            if(ok)
            {
                if(intTemp>0 && intTemp<150)
                {
                    intVector.append(intTemp);
                }
                if(intTemp>1000)
                {
                    intVector.append(intTemp/1000);
                }
            }
            else
            {
                //没有启动应用或设备断开,暂不处理
            }
        }

        if(intVector.isEmpty())
        {
            //没有启动应用或设备断开,暂不处理
        }
        else
        {
            qSort(intVector.begin(), intVector.end());
            intTemp=intVector.last();
            currentValue=QString::number(intTemp);
            if(isFirstValue)
            {
                intMinValue=intTemp;
                intMaxValue=intTemp;
                isFirstValue=false;
            }

            if(intTemp<=intMinValue)
            {
                intMinValue=intTemp;
            }
            if(intTemp>=intMaxValue)
            {
                intMaxValue=intTemp;
            }
            emit sendResult(qDeviceId,currentValue,intMinValue,intMaxValue);
        }
        proc.close();

        Sleep(gdInterval);

    }//while(true)

}


}


