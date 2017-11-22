#include "blockworker.h"

BlockWorker::BlockWorker()
{
}

void BlockWorker::setCmdLine(const QString &arg_cmdLine)
{
    qCmdLine=arg_cmdLine;
}

void BlockWorker::startTrafficService_slot()
{
    qProc=new QProcess(this);
    qProc->setReadChannelMode(QProcess::MergedChannels);
    qProc->closeReadChannel(QProcess::StandardOutput);
    qProc->closeReadChannel(QProcess::StandardError);
    qProc->closeWriteChannel();

    connect(qProc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveTrafficServiceResult()));
    qProc->start(qCmdLine);
    qProc->waitForStarted();
}


void BlockWorker::receiveTrafficServiceResult()
{
    while(qProc->canReadLine())
    {
        if(gIsStopTrafficService)
        {
            return;
        }
        qDataLine=qProc->readLine();
    }
}

