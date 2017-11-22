#include "replayforbehaviorcontroller.h"

ReplayForBehaviorController::ReplayForBehaviorController(QObject *parent) : QObject(parent)
{

}
void ReplayForBehaviorController::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void ReplayForBehaviorController::setFirstLevelDirName(const QString &arg_text)
{
    qFirstLevelDirName=arg_text;
}

void ReplayForBehaviorController::setSecondLevelDirName(const QString &arg_text)
{
    qSecondLevelDirName=arg_text;
}

void ReplayForBehaviorController::setScriptList(QStringList arg_list)
{
    qScriptList=arg_list;
}

void ReplayForBehaviorController::receiveRunScriptResult(const QString &arg_text)
{
   emit sendRunOneScriptResult(arg_text);
}

void ReplayForBehaviorController::receiveReplayResult(const QString &arg_deviceId, const QString &arg_two, QMap<QString,bool> arg_resmap, int arg_sec)
{
    emit sendReplayResult(arg_deviceId,arg_two,arg_resmap,arg_sec);
}

void ReplayForBehaviorController::receiveStopReplay()
{
    emit sendStopReplay();
}

void ReplayForBehaviorController::receiveErrorResult(const QString &arg_deviceId,const QString &arg_error)
{
    emit sendErrorResult(arg_deviceId,arg_error);
}

void ReplayForBehaviorController::recieveOneScriptFinish(const QString &arg_scriptline,bool flag)
{
    emit sendOneScriptFinish(arg_scriptline,flag);
}


void ReplayForBehaviorController::run()
{
    QThread::msleep(4000);//开始脚本时先等4s，因为代理启动较慢，等等他
    ReplayForBehaviorWorker *workerInstance=new ReplayForBehaviorWorker();

    connect(workerInstance,SIGNAL(sendRunOneScriptResult(const QString &)),this,SLOT(receiveRunScriptResult(const QString &))  ,Qt::DirectConnection  );
    connect(workerInstance,SIGNAL(sendReplayResult(const QString &,const QString &,QMap<QString,bool> ,int) ),this,SLOT(receiveReplayResult(const QString &,const QString &,QMap<QString,bool> ,int)) ,Qt::DirectConnection );
    connect(workerInstance,SIGNAL(sendErrorResult(const QString &,const QString &)),this,SLOT(receiveErrorResult(const QString &,const QString &))  ,Qt::DirectConnection  );
    connect(this,SIGNAL(sendStopReplay() ),workerInstance,SLOT(receiveStopReplay())  );
    connect(workerInstance,SIGNAL(oneScriptFinish(const QString &,bool)),this,SLOT(recieveOneScriptFinish(const QString &,bool)));

    workerInstance->setDeviceId(qDeviceId);
    workerInstance->setFirstLevelDirName(qFirstLevelDirName);
    workerInstance->setSecondLevelDirName(qSecondLevelDirName);
    workerInstance->setScriptList(qScriptList);
    workerInstance->startWorker();
}
