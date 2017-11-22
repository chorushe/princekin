#include "chartcontroller.h"

namespace ChartController
{

Controller::Controller()
{
}

void Controller::setCmdList(QStringList arg_cmdList)
{
    qCmdList=arg_cmdList;
}

void Controller::run()
{
    using ChartWorker::Worker;

    Worker *workerInstance=new Worker();
    connect(workerInstance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveWorkerResult(const QString &))  ,Qt::DirectConnection  );
    workerInstance->setCmdList(qCmdList);
    workerInstance->startWorker();
}

void Controller::receiveWorkerResult(const QString &arg_result)
{
    emit sendChartWorkerResult(arg_result);
}

}
