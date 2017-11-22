#include "replayforbehaviorworker.h"

ReplayForBehaviorWorker::ReplayForBehaviorWorker(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<StringMap>("StringMap");
}

void ReplayForBehaviorWorker::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void ReplayForBehaviorWorker::setFirstLevelDirName(const QString &arg_text)
{
    qFirstLevelDirName=arg_text;
}

void ReplayForBehaviorWorker::setSecondLevelDirName(const QString &arg_text)
{
    qSecondLevelDirName=arg_text;
}

void ReplayForBehaviorWorker::setScriptList(QStringList arg_list)
{
    qScriptList=arg_list;
}

void ReplayForBehaviorWorker::setVarValue()
{
    qAdbDevice="adb -s " + qDeviceId;

    //qPngDir=qFirstLevelDirName + QDir::separator() + qSecondLevelDirName ;20170726
    qPngDir=qFirstLevelDirName;
    qPngDir=QDir::toNativeSeparators(qPngDir);
    QDir *temp = new QDir;
    bool exist = temp->exists(qPngDir);
    if(!exist)
    {
        bool ok = temp->mkdir(qPngDir);
    }
    qPngDir = qPngDir + QDir::separator() + "png";

    qScriptLine="";

    qActualList.clear();
    qErrorList.clear();

    qSec=0;
    isStopRunScript=false;
}

void ReplayForBehaviorWorker::startWorker()
{
    int scriptCount;
    QString jarName;
    QString cmdLine;

    QStringList splitResult;
    QEventLoop eventLoop;

    scriptCount=qScriptList.count();

    setVarValue();

    if(isStopRunScript)
    {
        return;
    }

    for(int j=0;j<scriptCount;j++)
    {
        if(isStopRunScript)
        {
            break;
        }
        qProc=new QProcess();
        connect(qProc,SIGNAL(finished(int)), &eventLoop,SLOT(quit()));

        qScriptLine=qScriptList.at(j);
        splitResult=qScriptLine.split("=");
        qModuleBaseName=splitResult.at(0);
        qScriptBaseName=splitResult.at(1);

        jarName=qModuleBaseName + ".jar";
        cmdLine=qAdbDevice + " shell uiautomator runtest /sdcard/" + jarName + " -c com.sohu.test." + qScriptBaseName + " -e checkStat yes";
        qDebug()<<"cmdline"<<cmdLine;

        qProc->setReadChannelMode(QProcess::MergedChannels);
        qProc->closeWriteChannel();

        connect(qProc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveRunOneScriptResult()));
        connect(qProc, SIGNAL(finished(int)),this,SLOT(receiveProcFinished(int)));
        connect(qProc, SIGNAL(finished(int)),qProc,SLOT(deleteLater()));

        qProc->start(cmdLine);
        qProc->waitForStarted();

        resMap.insert(qScriptLine,true);
        eventLoop.exec();

        QThread::msleep(2000);
        emit oneScriptFinish(qScriptLine,resMap[qScriptLine]);//当一个脚本执行结束后发送一个信号
    }
    QThread::msleep(2000);
    emit sendReplayResult(qDeviceId,qSecondLevelDirName,resMap,qSec);


}

void ReplayForBehaviorWorker::receiveRunOneScriptResult()
{
    QString tempStr;
    while(qProc->canReadLine())
    {
        qByte=qProc->readLine();
        qDataLine=qTc->toUnicode(qByte);
        qDebug()<<"=="<<qDataLine;
        tempStr=qDeviceId + "    " + qModuleBaseName + "=" + qScriptBaseName + ".txt" + "    " + qDataLine;
        emit sendRunOneScriptResult(tempStr);

        if(qDataLine.contains("princekinfalse"))
        {
            resMap[qScriptLine]=false;
            sendErrorResult(qSecondLevelDirName,tempStr);
        }
        else if(qDataLine.contains("java.lang.RuntimeException",Qt::CaseInsensitive) || qDataLine.contains("error:" ,Qt::CaseInsensitive) )
        {
            resMap[qScriptLine]=false;
            sendErrorResult(qSecondLevelDirName,tempStr);

        }
        else if(qDataLine.contains("princekinTakeScreen"))
        {
            pullTakeScreen2();
        }
        else if(qDataLine.contains("pushApk="))
        {
            QStringList list=qDataLine.split("=");
            pushApk(list.at(1));
        }
        else if(qDataLine.contains("startAppTime="))
        {
            QStringList list=qDataLine.split("=");

            QString sss=list.at(1);

            bool ok;
            int x=sss.toInt(&ok,10);
            if(ok)
            {
                qSec=qSec+x;
            }
        }
        Sleep(200);
    }
}

void ReplayForBehaviorWorker::receiveProcFinished(int exitCode)
{
    qDebug()<<"receiveProcFinished";
    if(qProc->isOpen())
    {
        qProc->close();
    }
}

void ReplayForBehaviorWorker::receiveStopReplay()
{
    qDebug()<<"zhongtu break";
    isStopRunScript=true;
    if(qProc->isOpen())
    {
        qProc->close();
    }
}

void ReplayForBehaviorWorker::pullTakeScreen2()
{
    QString cmdLine;

    cmdLine=qAdbDevice + " pull /sdcard/temppng/. " + qPngDir;
    ExeCmd::runCmd(cmdLine);

    QString cmdline=qAdbDevice + " shell rm /sdcard/temppng/*.*";
    qDebug()<<"takecreen"<<cmdline;
    ExeCmd::runCmd(cmdline);
}


void ReplayForBehaviorWorker::pushApk(const QString &arg_apkName)
{
    QString str;
    QString cmdLine;

    str=qAdbDevice + " push " + gApkDir + QDir::separator() + arg_apkName;
    str=QDir::toNativeSeparators(str);

    cmdLine=str + " /sdcard/";
    ExeCmd::runCmd(cmdLine);
}
