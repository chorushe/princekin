#include "batterytempworker.h"

namespace BatteryTempWorker
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
    int count;
    int intTemp;
    int intMinValue=0;
    int intMaxValue=0;
    bool ok;
    bool isFirstValue=true;

    QString strLine;
    QString currentValue;
    QStringList splitResult;

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

        while(proc.canReadLine())
        {
            byteArray=proc.readLine();
            strLine=qTc->toUnicode(byteArray).trimmed();

            if(strLine.contains("temperature:"))
            {
                splitResult=strLine.split(":");
                count=splitResult.size();
                if(count>=2)
                {
                    currentValue=splitResult.at(1).trimmed();
                    intTemp=currentValue.toInt(&ok,10);
                    if(ok)
                    {
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
                    else
                    {
                        //没有启动应用或设备断开,暂不处理
                    }
                }

            }//if(strLine.contains("temperature:"))
            else
            {
                //没有启动应用或设备断开,暂不处理
            }

        }//while(proc.canReadLine())
        proc.close();

        Sleep(gdInterval);
    }//while(true)

}


}


