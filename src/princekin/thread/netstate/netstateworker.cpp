#include "netstateworker.h"

namespace NetStateWorker
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
    if(qProc->isOpen())
    {
        qProc->close();
    }
}


void Worker::startWorker()
{
    qProc=new QProcess();
    qProc->setReadChannelMode(QProcess::MergedChannels);
    qProc->closeWriteChannel();

    connect(qProc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveResult()),Qt::DirectConnection );//,Qt::QueuedConnection
    connect(qProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(receiveProcFinished(int,QProcess::ExitStatus) ));
    connect(qProc, SIGNAL(finished(int)),qProc,SLOT(deleteLater()));

    qProc->start(qCmdLine);
    qProc->waitForStarted();
}

void Worker::receiveProcFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    emit sendProcFinished(qDeviceId);
    if(qProc->isOpen())
    {
        qProc->close();
    }

}

void Worker::receiveResult()
{
    while(qProc->canReadLine())
    {
        qByteArray=qProc->readLine();
        qStr=qTc->toUnicode(qByteArray);

        if(qStr.contains("currentStateWifi"))
        {
            qNetState="currentStateWifi";
            gNetStateHash.insert(qDeviceId,qNetState);
        }
        else if(qStr.contains("currentStateMobile"))
        {
            qNetState="currentStateMobile";
            gNetStateHash.insert(qDeviceId,qNetState);
        }
        else if(qStr.contains("currentStateNoNet"))
        {
            qNetState="currentStateNoNet";
            gNetStateHash.insert(qDeviceId,qNetState);
        }
        else
        {
            qNetState="currentStateError";
            gNetStateHash.insert(qDeviceId,qNetState);
        }

    }
}
}
