#include "monkeyworker.h"

MonkeyWorker::MonkeyWorker(QObject *parent) : QObject(parent)
{
    p=new QProcess(this);
    connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(ReadOutput()));
    connect(p,SIGNAL(readyReadStandardError()),this,SLOT(ReadError()));
    connect(p,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(monkeyNotRunning()));
}

void MonkeyWorker::startMonkey()
{
    if(p->state())
        p->write(cmdStr.toLatin1());
    else
    {
        p->start("cmd", QStringList()<<"/c"<< cmdStr );
    }
}

void MonkeyWorker::ReadOutput()
{
    QString recieveStr=QString::fromLocal8Bit(p->readAllStandardOutput());
    emit readOutputFromWorker(recieveStr);
}

void MonkeyWorker::ReadError()
{
    QString recieveStr=QString::fromLocal8Bit(p->readAllStandardError());
    emit readErrorFromWorker(recieveStr);
}

void MonkeyWorker::monkeyNotRunning()
{
    if(p->state()==QProcess::NotRunning)
    {
        emit monkeyStop();
        p->close();
    }
}
