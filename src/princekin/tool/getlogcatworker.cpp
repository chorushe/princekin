#include "getlogcatworker.h"

getLogcatWorker::getLogcatWorker(QObject *parent) : QObject(parent)
{
    p=new QProcess(this);
    connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(ReadOutput()));
    connect(p,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(processNotRunning()));
}

void getLogcatWorker::startLogcat()
{
    if(p->state())
        p->write(cmdStr.toLatin1());
    else
    {
        p->start("cmd", QStringList()<<"/c"<< cmdStr );
    }
}

void getLogcatWorker::ReadOutput()
{
    QString tempStr=QString::fromUtf8(p->readAllStandardOutput());
    emit readOutputFromWorker(tempStr);
}

void getLogcatWorker::recieveStopLogcatSignal()//外边传回来的停止信号
{
    if(p->state()!=QProcess::NotRunning)
    {
       // emit logcatStop();
        p->close();
    }
}

void getLogcatWorker::processNotRunning()//外边传回来的停止统计信号
{
    if(p->state()==QProcess::NotRunning)
    {
        emit logcatStop();
        p->close();
    }
}
