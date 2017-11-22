#include "chartworker.h"

#include "windows.h"
#include "helper.h"
#include <string>


using namespace std;

namespace ChartWorker
{

Worker::Worker()
{
}

void Worker::setCmdList(QStringList arg_cmdList)
{
    qCmdList=arg_cmdList;
}


void Worker::startWorker()
{
    QEventLoop eventLoop;

    foreach(QString cmdLine,qCmdList)
    {
        qProc=new QProcess(this);

        connect(qProc,SIGNAL(finished(int)), &eventLoop,SLOT(quit()));

        qProc->setReadChannelMode(QProcess::MergedChannels);
        qProc->closeWriteChannel();

        connect(qProc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveRunOneScriptResult()));
        connect(qProc, SIGNAL(finished(int)),this,SLOT(receiveProcFinished(int)));
        connect(qProc, SIGNAL(finished(int)),qProc,SLOT(deleteLater()));

        qProc->start(cmdLine);
        qProc->waitForStarted();

        eventLoop.exec();
    }
    emit sendWorkerResult("");
}


void Worker::receiveRunOneScriptResult()
{
    QString tempStr;
    while(qProc->canReadLine())
    {
        qByte=qProc->readLine();
        qDataLine=qTc->toUnicode(qByte);
    }
}

void Worker::receiveProcFinished(int x)
{
}

}
