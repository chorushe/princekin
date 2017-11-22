#include "controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
    ThreadInstance.quit();
    ThreadInstance.wait();
}


void Controller::setScript(QStringList arg_scriptList)
{
    scriptList=arg_scriptList;
}

void Controller::setDeviceId(const QString &arg_deviceId)
{
    deviceId=arg_deviceId;
}

void Controller::setMemoryDeviceId(const QString &arg_deviceid)
{
    memoryID=arg_deviceid;
}

void Controller::setCpuDeviceId(const QString &arg_deviceid)
{
    cpuID=arg_deviceid;
}



void Controller::setErrortag(const QString &arg_errtag)
{
    errorTag=arg_errtag;
}

void Controller::setLoopNumber(int arg_loopNumber)
{
    loopNumber=arg_loopNumber;
}


void Controller::setArgList(QStringList arg_argList)
{
    argList=arg_argList;
}

void Controller::setArgList2(QStringList arg_argList)
{
    argList2=arg_argList;
}

void Controller::setPackCmdList(QStringList arg_argList)
{
    packCmdList=arg_argList;
}








void Controller::startCompile(const QString &arg_cmdLine)
{
    Worker *worker = new Worker;
    worker->setCmdLine(arg_cmdLine);
    worker->moveToThread(&ThreadInstance);

    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startCompile()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);    
    connect(worker,SIGNAL(sendCompileFinished()), this,SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileFinished()), worker, SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileResult(const QString&)),this, SLOT(receiveCompileResult(const QString&)),Qt::DirectConnection);
    ThreadInstance.start();
}

void Controller::receiveCompileResult(const QString &arg_line)
{
    emit sendCompileResult(arg_line);
}



//********************************************************************************************//
void Controller::startCompileMore(QStringList arg_javacList,QStringList arg_fileNameList)//这个函数是不是不用了
{
    Worker *worker = new Worker;
    worker->setCompileMore(arg_javacList,arg_fileNameList);
    worker->moveToThread(&ThreadInstance);

    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startCompileMore()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileMoreFinished()), this,SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileMoreFinished()), worker, SLOT(deleteLater()), Qt::DirectConnection);
    connect(this,SIGNAL(sendStopCompileMore()), worker, SLOT(receiveStopCompileMore()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileMoreResult(const QString&)),this, SLOT(receiveCompileMoreResult(const QString&)),Qt::DirectConnection);
    ThreadInstance.start();
}
void Controller::startCompileMore2(const QString &arg_cmdline)
{
    Worker *worker = new Worker;
    worker->setCompileMore2(arg_cmdline);
    worker->moveToThread(&ThreadInstance);

    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startCompileMore2()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileMoreFinished()), this,SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileMoreFinished()), worker, SLOT(deleteLater()), Qt::DirectConnection);
    connect(this,SIGNAL(sendStopCompileMore()), worker, SLOT(receiveStopCompileMore()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendCompileMoreResult(const QString&)),this, SLOT(receiveCompileMoreResult(const QString&)),Qt::DirectConnection);
    ThreadInstance.start();
}

void Controller::receiveCompileMoreResult(const QString &arg_line)
{
    emit sendCompileMoreResult(arg_line);
}
void Controller::receiveStopCompileMore()
{
    emit sendStopCompileMore();
}
//********************************************************************************************//









void Controller::startPack(QStringList arg_cmdList)
{
    Worker *worker = new Worker;
    worker->setPackCmdList(arg_cmdList);
    worker->moveToThread(&ThreadInstance);

    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startPack()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);
    connect(worker,SIGNAL(sendPackFinished()), this,SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendPackFinished()), worker, SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendPackResult(const QString&)),this, SLOT(receivePackResult(const QString&)),Qt::DirectConnection);
    ThreadInstance.start();
}

void Controller::receivePackResult(const QString &arg_line)
{
    emit sendPackResult(arg_line);
}





void Controller::startRunScript()
{
    Worker *worker = new Worker();
    worker->setDeviceId(deviceId);
    worker->setScript(scriptList);
    worker->setLoopNumber(loopNumber);
    worker->moveToThread(&ThreadInstance);


    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startRunScript()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);

    connect(worker,SIGNAL(sendRunAllScriptFinished()), this,SLOT(deleteLater()), Qt::DirectConnection);

    connect(worker,SIGNAL(sendRunAllScriptFinished()), worker, SLOT(deleteLater()), Qt::DirectConnection);

    connect(worker,SIGNAL(sendRunScriptResult(const QString&)),this, SLOT(receiveRunScriptResult(const QString&)),Qt::DirectConnection);
    connect(worker,SIGNAL(sendReport(const QString&,const QString&)),this, SLOT(receiveReport(const QString&,const QString&)),Qt::DirectConnection);
    connect(worker,SIGNAL(sendRemoveDevice(const QString&)),this, SLOT(receiveRemoveDevice(const QString&)),Qt::DirectConnection);
    connect(this,SIGNAL(sendStopRunScriptCommand()), worker,SLOT(receiveStopRunScriptCommand()), Qt::DirectConnection);

    ThreadInstance.start();
}

void Controller::receiveAllScriptFinishedResult()
{
    emit sendAllScriptFinishedResult();
}



void Controller::receiveRunScriptResult(const QString &arg_result)
{
    emit sendRunScriptResult(arg_result);
}
void Controller::receiveRemoveDevice(const QString &arg_deviceId)
{
    emit sendRemoveDevice(arg_deviceId);
}

void Controller::receiveReport(const QString &arg_result,const QString &arg_deviceid)
{
    emit sendReport(arg_result,arg_deviceid);
}


void Controller::receiveStopRunScriptCommand()
{
    emit sendStopRunScriptCommand();
}


void Controller::finishedTest()
{
}


void Controller::startReplay(const QString &arg_cmdLine)
{
    Worker *worker = new Worker;
    worker->setCmdLine(arg_cmdLine);
    worker->setDeviceId(deviceId);
    worker->moveToThread(&ThreadInstance);
    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startReplay()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);    
    connect(worker,SIGNAL(sendReplayFinished()), this,SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendReplayResult(const QString&)),this, SLOT(receiveReplayResult(const QString&)),Qt::DirectConnection);
    ThreadInstance.start();

}

void Controller::receiveReplayResult(const QString &arg_result)
{
    emit sendReplayResult(arg_result);
}






void Controller::receiveMonitorResult(const QString &arg_result)
{
    emit sendMonitorResult(arg_result);
}
void Controller::receiveRemoveMonitorDevice(const QString &arg_deviceId)
{
    emit sendRemoveMonitorDevice(arg_deviceId);
}


void Controller::startMemory(const QString &arg_deviceid,const QString &arg_cmdLine,int arg_interval)
{
    Worker *worker = new Worker;

    worker->initMemory(arg_deviceid,arg_cmdLine,arg_interval);
    worker->moveToThread(&ThreadInstance);

    connect(&ThreadInstance,SIGNAL(started()),  worker, SLOT(startMemory()),Qt::DirectConnection);
    connect(&ThreadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) ,Qt::DirectConnection);
    //connect(worker,SIGNAL(sendPackFinished()), this,SLOT(deleteLater()), Qt::DirectConnection);
    //connect(worker,SIGNAL(sendPackFinished()), worker, SLOT(deleteLater()), Qt::DirectConnection);
    connect(worker,SIGNAL(sendMemoryResult(const QString &,const QString &,int,int)),this, SLOT(receiveMemoryResult(const QString &,const QString &,int,int)),Qt::DirectConnection);
    ThreadInstance.start();
}

void Controller::receiveMemoryResult(const QString &arg_deviceID,const QString &arg_currentMemoryValue,int arg_min,int arg_max)
{
    emit sendMemoryResult(arg_deviceID,arg_currentMemoryValue,arg_min,arg_max);
}
