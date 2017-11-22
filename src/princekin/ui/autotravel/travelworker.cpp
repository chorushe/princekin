#include "travelworker.h"

TravelWorker::TravelWorker(QObject *parent) : QObject(parent)
{
    p=new QProcess(this);
    connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(ReadOutput()));
    connect(p,SIGNAL(readyReadStandardError()),this,SLOT(ReadError()));
    connect(p,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(travelerNotRunning()));
}

void TravelWorker::startTravel()
{
    if(p->state())
        p->write(cmdStr.toLatin1());
    else
    {
        p->start("cmd", QStringList()<<"/c"<< cmdStr );
    }
}

void TravelWorker::ReadOutput()
{
    QString recieveStr=QString::fromLocal8Bit(p->readAllStandardOutput());
    emit readOutputFromWorker(recieveStr);
}

void TravelWorker::ReadError()
{
    QString recieveStr=QString::fromLocal8Bit(p->readAllStandardError());
    emit readErrorFromWorker(recieveStr);
}

void TravelWorker::travelerNotRunning()
{
    if(p->state()==QProcess::NotRunning)
    {
        emit travelStop();
        p->close();
    }
}
