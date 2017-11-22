#include "cputempcontroller.h"

namespace CpuTempController
{

Controller::Controller()
{
}

void Controller::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void Controller::setCmdLine(const QString &arg_text)
{
    qCmdLine=arg_text;
}

void Controller::receiveResult(const QString &arg_deviceId,const QString &arg_currentValue,int arg_minValue,int arg_maxValue)
{
    emit sendResult(arg_deviceId,arg_currentValue,arg_minValue,arg_maxValue);
}

void Controller::stopController()
{
    emit sendStopWorker();
}


void Controller::run()
{
    Worker *workerInstance=new Worker;

    connect(workerInstance,SIGNAL(sendResult(const QString &,const QString &,int,int) ),this,SLOT(receiveResult(const QString &,const QString &,int,int) ) ,Qt::DirectConnection );
    connect(this,SIGNAL(sendStopWorker() ),workerInstance,SLOT(receiveStopWorker()) ,Qt::DirectConnection );

    workerInstance->initVar();
    workerInstance->setDeviceId(qDeviceId);
    workerInstance->setCmdLine(qCmdLine);
    workerInstance->startWorker();
}
}
