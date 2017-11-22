#include "netstatecontroller.h"

namespace NetStateController
{

Controller::Controller()
{
}

Controller::~Controller()
{
    ThreadInstance.quit();
    ThreadInstance.wait();
}

void Controller::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void Controller::setCmdLine(const QString &arg_text)
{
    qCmdLine=arg_text;
}


void Controller::stopController()
{
    emit sendStopWorker();
}

void Controller::receiveProcFinished(const QString &arg_text)
{
    emit sendProcFinished(arg_text);
}


void Controller::startController()
{
    Worker *workerInstance=new Worker;

    workerInstance->initVar();
    workerInstance->setDeviceId(qDeviceId);
    workerInstance->setCmdLine(qCmdLine);
    workerInstance->moveToThread(&ThreadInstance);

    connect(&ThreadInstance,SIGNAL(started()),  workerInstance, SLOT(startWorker()) ,Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), workerInstance, SLOT(deleteLater()) ,Qt::DirectConnection);

    connect(this,SIGNAL(sendStopWorker() ),workerInstance,SLOT(receiveStopWorker())  );//,Qt::DirectConnection
    connect(workerInstance,SIGNAL(sendProcFinished(const QString &) ),this,SLOT(receiveProcFinished(const QString &) )  );//,Qt::DirectConnection

    ThreadInstance.start();
}


}
