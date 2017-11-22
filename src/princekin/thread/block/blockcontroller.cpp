#include "blockcontroller.h"

BlockController::BlockController()
{
}

BlockController::~BlockController()
{
    ThreadInstance.quit();
    ThreadInstance.wait();
}

void BlockController::startWorkerForTrafficService(const QString &arg_cmdLine)
{
    BlockWorker *worker = new BlockWorker;
    worker->setCmdLine(arg_cmdLine);
    worker->moveToThread(&ThreadInstance);

    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startTrafficService_slot()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);


    //connect(worker,SIGNAL(sendWorkerFinished(const QString&)), worker, SLOT(deleteLater()), Qt::DirectConnection);
    //connect(worker,SIGNAL(sendWorkerFinished(const QString&)), this,   SLOT(deleteLater()), Qt::DirectConnection);


    //connect(worker,SIGNAL(sendTrafficServiceResult(const QString&)),       this,   SLOT(receiveReadData(const QString&)),Qt::DirectConnection);


    ThreadInstance.start();
}
