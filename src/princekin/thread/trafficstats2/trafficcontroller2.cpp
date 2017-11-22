#include "trafficcontroller2.h"

namespace TrafficController2
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

void Controller::setSec(int arg_sec)
{
    qSec=arg_sec;
}

void Controller::receiveResult_wifi1(const QString &arg_deviceId,const QString &arg_currentValue)
{
    emit sendResult_wifi1(arg_deviceId,arg_currentValue);
}
void Controller::receiveResult_wifi2(const QString &arg_deviceId,const QString &arg_currentValue)
{
    emit sendResult_wifi2(arg_deviceId,arg_currentValue);
}

void Controller::receiveResult_mobile1(const QString &arg_deviceId,const QString &arg_currentValue)
{
    emit sendResult_mobile1(arg_deviceId,arg_currentValue);
}
void Controller::receiveResult_mobile2(const QString &arg_deviceId,const QString &arg_currentValue)
{
    emit sendResult_mobile2(arg_deviceId,arg_currentValue);
}





void Controller::stopController()
{
    emit sendStopWorker();
}


void Controller::run()
{
    Worker *workerInstance=new Worker;

    connect(workerInstance,SIGNAL(sendResult_wifi1(const QString &,const QString &) ),this,SLOT(receiveResult_wifi1(const QString &,const QString &) ),Qt::DirectConnection  );
    connect(workerInstance,SIGNAL(sendResult_wifi2(const QString &,const QString &) ),this,SLOT(receiveResult_wifi2(const QString &,const QString &) ),Qt::DirectConnection  );

    connect(workerInstance,SIGNAL(sendResult_mobile1(const QString &,const QString &) ),this,SLOT(receiveResult_mobile1(const QString &,const QString &) ),Qt::DirectConnection  );
    connect(workerInstance,SIGNAL(sendResult_mobile2(const QString &,const QString &) ),this,SLOT(receiveResult_mobile2(const QString &,const QString &) ),Qt::DirectConnection  );

    connect(this,SIGNAL(sendStopWorker() ),workerInstance,SLOT(receiveStopWorker()) ,Qt::DirectConnection );

    workerInstance->initVar();
    workerInstance->setDeviceId(qDeviceId);
    workerInstance->setCmdLine(qCmdLine);
    workerInstance->setSec(qSec);
    workerInstance->startWorker();
}
}
