#include "adbshellcontroller.h"

namespace AdbShellController
{

Controller::Controller()
{
}

Controller::~Controller()
{
    threadInstance.quit();
    threadInstance.wait();
}

void Controller::setMark(const QString &arg_mark1,const QString &arg_mark2)
{
    qMark1=arg_mark1;
    qMark2=arg_mark2;
}

void Controller::setCmdLine(const QString &arg_text)
{
    qCmdLine=arg_text;
}

void Controller::doController()
{
    AdbShellWorker::Worker *worker = new AdbShellWorker::Worker;
    worker->moveToThread(&threadInstance);

    worker->setMark(qMark1,qMark2);
    worker->setCmdLine(qCmdLine);

    connect(&threadInstance,SIGNAL(started()),  worker, SLOT(doWork()),Qt::DirectConnection);
    connect(&threadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);
    connect(&threadInstance,SIGNAL(finished()), this,   SLOT(deleteLater()), Qt::DirectConnection);

    connect(worker,SIGNAL(sendWorkerResult(const QString&,const QString&)), this, SLOT(receiveWorkerResult(const QString&,const QString&)), Qt::DirectConnection);

    threadInstance.start();
}

void Controller::receiveWorkerResult(const QString& arg_str,const QString& arg_mark)
{
    emit sendWorkerResult(arg_str,arg_mark);
}

}
