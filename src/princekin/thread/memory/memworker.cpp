#include "memworker.h"
#include <QTextCodec>
namespace MemWorker
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

    QByteArray byteArray;

    QRegExp regExp("(\\d+)");

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

            if(strLine.contains("TOTAL") && !strLine.contains("Dirty"))
            {
                QStringList list;
                int pos=0;

                while((pos = regExp.indexIn(strLine, pos)) != -1)
                {
                    list << regExp.cap(1);
                    pos += regExp.matchedLength();
                }

                if(!list.isEmpty())
                {
                    currentValue=list.at(0);
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
            }
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


