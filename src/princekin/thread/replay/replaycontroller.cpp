#include "replaycontroller.h"

namespace ReplayController
{

Controller::Controller()
{
}

void Controller::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}


void Controller::setFirstLevelDirName(const QString &arg_text)
{
    qFirstLevelDirName=arg_text;
}

void Controller::setSecondLevelDirName(const QString &arg_text)
{
    qSecondLevelDirName=arg_text;
}

void Controller::setScriptList(QStringList arg_list)
{
    qScriptList=arg_list;
}

void Controller::receiveRunScriptResult(const QString &arg_text)
{
   emit sendRunOneScriptResult(arg_text);
}

void Controller::receiveReplayResult(const QString &arg_deviceId,const QString &arg_two,QStringList arg_actual, QStringList arg_error,int arg_sec,QStringList arg_modulecrashlist,const QString&arg_startTime)
{
    emit sendReplayResult(arg_deviceId,arg_two,arg_actual,arg_error,arg_sec,arg_modulecrashlist,arg_startTime);
}

void Controller::receiveCrashResult(const QString &arg_deviceId, const QString &arg_secondLevelDirName, const QString &arg_moduleBaseName, const QString &arg_scriptBaseName)
{
    emit sendCrashResult(arg_deviceId,arg_secondLevelDirName,arg_moduleBaseName,arg_scriptBaseName);
}

void Controller::receiveProcFinished(const QString &arg_deviceId)
{
    emit sendProcFinished(arg_deviceId);
}

void Controller::receiveStopReplay()
{
    emit sendStopReplay();
}

void Controller::receiveErrorResult(const QString &arg_deviceId,const QString &arg_error)
{
    emit sendErrorResult(arg_deviceId,arg_error);
}


void Controller::run()
{
    using ReplayWorker::Worker;

    Worker *workerInstance=new Worker();

    connect(workerInstance,SIGNAL(sendRunOneScriptResult(const QString &)),this,SLOT(receiveRunScriptResult(const QString &))  ,Qt::DirectConnection  );
    connect(workerInstance,SIGNAL(sendReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &) ),this,SLOT(receiveReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &)) ,Qt::DirectConnection );
    connect(workerInstance,SIGNAL(sendErrorResult(const QString &,const QString &)),this,SLOT(receiveErrorResult(const QString &,const QString &))  ,Qt::DirectConnection  );
    connect(workerInstance,SIGNAL(sendProcFinished(const QString &) ),this,SLOT(receiveProcFinished(const QString &) ) ,Qt::DirectConnection );
    connect(workerInstance,SIGNAL(sendCrashResult(const QString &,const QString &,const QString &,const QString &) ),this,SLOT(receiveCrashResult(const QString &,const QString &,const QString &,const QString &) ) ,Qt::DirectConnection );

    connect(this,SIGNAL(sendStopReplay() ),workerInstance,SLOT(receiveStopReplay())  );

    workerInstance->setDeviceId(qDeviceId);
    workerInstance->setFirstLevelDirName(qFirstLevelDirName);
    workerInstance->setSecondLevelDirName(qSecondLevelDirName);
    workerInstance->setScriptList(qScriptList);
    workerInstance->startWorker();
}

}
