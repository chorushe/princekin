#include "devicesworker.h"

DevicesWorker::DevicesWorker()
{
}

void DevicesWorker::initVar()
{
    qCmdLine="adb devices";
}

void DevicesWorker::workerStart()
{
    qTimer.start(2000, this);
}


void DevicesWorker::timerEvent(QTimerEvent *event)
{
    if(qIsStop)
    {
        qTimer.stop();
    }
    else
    {
        if(event->timerId()==qTimer.timerId())
        {
            QString tempStr;
            QString strLine;
            QStringList strList;
            QStringList splitResult;

            QByteArray byteArray;

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
                if(strLine.endsWith("device"))
                {
                    splitResult=strLine.split("device");
                    tempStr=splitResult.at(0);
                    tempStr=tempStr.replace("\t","");
                    strList.append(tempStr);
                }
                else
                {
                    //没有启动应用或设备断开,暂不处理
                }
            }

            emit sendResult(strList);
            proc.close();
        }
    }
}
