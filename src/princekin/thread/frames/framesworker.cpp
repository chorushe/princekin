#include "framesworker.h"
#include <QTextCodec>


namespace FramesWorker
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
}

void Worker::receiveStopWorker()
{
}


void Worker::startWorker()
{
    QString strLine;
    QByteArray byteArray;

    while(true)
    {
        QProcess proc;
        proc.setReadChannelMode(QProcess::MergedChannels);
        proc.start(qCmdLine);
        proc.waitForStarted();

        while(proc.waitForFinished()==false)
        {
        }

        bool isStart=false;
        QStringList strList;

        while(proc.canReadLine())
        {
            byteArray=proc.readLine();
            strLine=qTc->toUnicode(byteArray).trimmed();

            if(strLine.contains("Draw	Process	Execute") || strLine.contains("Draw	Prepare	Process	Execute"))
            {
                isStart=true;
            }
            if(strLine.contains("View hierarchy:"))
            {
                isStart=false;
            }

            if(isStart)
            {
                if(strLine.contains("Draw	Process	Execute") || strLine.contains("Draw	Prepare	Process	Execute"))
                {
                }
                else
                {
                    strList.append(strLine);
                }
            }
        }
        proc.close();

        float drawFloat=0;
        float perFloat=0;
        float processFloat=0;
        float executeFloat=0;
        float sum=0;

        int len=0;
        int x=0;

        QStringList tempList;

        if(strList.size()>5)
        {
            foreach (QString var, strList)
            {
                qDebug()<<var;
                tempList=var.split("\t");
                len=tempList.size();
                if(len==3)
                {
                    drawFloat=tempList.at(0).toFloat();
                    processFloat=tempList.at(1).toFloat();
                    executeFloat=tempList.at(2).toFloat();
                    sum=sum + drawFloat + processFloat + executeFloat;
                    x++;
                }
                if(len==4)
                {
                    drawFloat=tempList.at(0).toFloat();
                    perFloat=tempList.at(1).toFloat();
                    processFloat=tempList.at(2).toFloat();
                    executeFloat=tempList.at(3).toFloat();
                    sum=sum + drawFloat + perFloat + processFloat + executeFloat;
                    x++;
                }
            }
        }
        if(sum!=0)
        {
            float f=sum/x;
            QString strFrames=QString::number(f, 'f', 2);
            emit sendResult(qDeviceId,strFrames);
            break;
        }

        QThread::sleep(5);

    }//while(true)

}


}
