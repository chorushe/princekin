#include "replayworker.h"
#include "replayworker.h"

#include "windows.h"
#include "helper.h"
#include <string>
#include "test.h"
using namespace std;

namespace ReplayWorker
{

Worker::Worker()
{
}

void Worker::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void Worker::setFirstLevelDirName(const QString &arg_text)
{
    qFirstLevelDirName=arg_text;
}

void Worker::setSecondLevelDirName(const QString &arg_text)
{
    qSecondLevelDirName=arg_text;
}

void Worker::setScriptList(QStringList arg_list)
{
    qScriptList=arg_list;
}

void Worker::setVarValue()
{
    qAdbDevice="adb -s " + qDeviceId;

    qPngDir=gDetailedPath + QDir::separator() + qSecondLevelDirName + QDir::separator() + "png";
    qPngDir=QDir::toNativeSeparators(qPngDir);

    qLogcatDir=gDetailedPath + QDir::separator() + qSecondLevelDirName + QDir::separator() + "logcat";
    qLogcatDir=QDir::toNativeSeparators(qLogcatDir);

    qLooping=0;
    qScriptLine="";
    qErrorHash.clear();
    qActualHash.clear();
    qMarkList.clear();

    qActualList.clear();
    qErrorList.clear();
    qCrashModuleList.clear();
    qSec=0;
    isStopRunScript=false;

}




void Worker::startWorker()
{
    int scriptCount;
    QString jarName;
    QString cmdLine;

    QString startRunTime;
    QStringList tempList;
    QStringList splitResult;
    QEventLoop eventLoop;

    setVarValue();

    startRunTime=Helper::getTime2("yyyy-MM-dd hh:mm:ss");
    foreach(QString var,qScriptList)
    {
        qActualHash.insert(qDeviceId + "=" + var,0);
        qErrorHash.insert(qDeviceId + "=" + var,0);
    }

    scriptCount=qScriptList.count();

    for(int i=0;i<gdLoop;i++)
    {
        qLooping=i+1;
        if(isStopRunScript)
        {
            break;
        }

        for(int j=0;j<scriptCount;j++)
        {
            qProc=new QProcess(this);

            connect(qProc,SIGNAL(finished(int)), &eventLoop,SLOT(quit()));

            if(isStopRunScript)
            {
                break;
            }

            qScriptLine=qScriptList.at(j);
            splitResult=qScriptLine.split("=");
            qModuleBaseName=splitResult.at(0);
            qScriptBaseName=splitResult.at(1);

            jarName=qModuleBaseName + ".jar";
            cmdLine=qAdbDevice + " shell uiautomator runtest /sdcard/" + jarName + " -c com.sohu.test." + qScriptBaseName + " -e checkStat no -e appname " + gPackageName;

            qProc->setReadChannelMode(QProcess::MergedChannels);
            qProc->closeWriteChannel();

            connect(qProc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveRunOneScriptResult()));
            connect(qProc, SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(receiveProcFinished(int,QProcess::ExitStatus)));
            connect(qProc, SIGNAL(finished(int)),qProc,SLOT(deleteLater()));

            qProc->start(cmdLine);

            tempList=qActualList.filter(qScriptLine + "=");
            for(QString var:tempList)
            {
                qActualList.removeOne(var);
            }
            qActualList.append(qScriptLine + "=" + QString::number(qLooping) + "=" + qDeviceId);

            qProc->waitForStarted();

            /*
            tempList=qActualList.filter(qScriptLine + "=");
            for(QString var:tempList)
            {
                qActualList.removeOne(var);
            }
            qActualList.append(qScriptLine + "=" + QString::number(qLooping) + "=" + qDeviceId);
            */
            eventLoop.exec();
        }
    }//for(int i=0;i<loopNumber;i++)
    emit sendReplayResult(qDeviceId,qSecondLevelDirName,qActualList,qErrorList,qSec,qCrashModuleList,startRunTime);
}

void Worker::receiveProcFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    emit sendProcFinished(qDeviceId);
    closeP();
}


void Worker::receiveStopReplay()
{
    isStopRunScript=true;
    closeP();
}

void Worker::stopReplay()
{
    isStopRunScript=true;
    closeP();
}

void Worker::closeP()
{
    if(qProc->isOpen())
    {
        qProc->close();
    }
}

void Worker::receiveRunOneScriptResult()
{
    QString tempStr;
    while(qProc->canReadLine())
    {
        qByte=qProc->readLine();
        qDataLine=qTc->toUnicode(qByte);
        tempStr=qDeviceId + "    " + qModuleBaseName + "=" + qScriptBaseName + ".txt" + "    " + qDataLine;
        emit sendRunOneScriptResult(tempStr);

        if(qDataLine.contains("princekinfalse"))
        {
            QString ss1=QString::number(qLooping)+"="+qDeviceId + "=" + qScriptLine;
            if(qMarkList.indexOf(ss1)==-1)
            {
                QStringList tempList=qErrorList.filter(qScriptLine + "=");
                for(QString var:tempList)
                {
                    qErrorList.removeOne(var);
                }
                int errorNumber=qErrorHash.value(qDeviceId + "=" + qScriptLine);
                qErrorHash.insert(qDeviceId + "=" + qScriptLine,errorNumber+1);
                qErrorList.append(qScriptLine + "=" + QString::number(errorNumber+1) + "=" + qDeviceId);
                qMarkList.append(ss1);
            }
            sendErrorResult(qSecondLevelDirName,tempStr);
        }
        else if(qDataLine.contains("java.lang.RuntimeException",Qt::CaseInsensitive) || qDataLine.contains("error:" ,Qt::CaseInsensitive) )
        {
            QString ss1=QString::number(qLooping)+"="+qDeviceId + "=" + qScriptLine;
            if(qMarkList.indexOf(ss1)==-1)
            {
                QStringList tempList=qErrorList.filter(qScriptLine + "=");
                for(QString var:tempList)
                {
                    qErrorList.removeOne(var);
                }
                int errorNumber=qErrorHash.value(qDeviceId + "=" + qScriptLine);
                qErrorHash.insert(qDeviceId + "=" + qScriptLine,errorNumber+1);
                qErrorList.append(qScriptLine + "=" + QString::number(errorNumber+1) + "=" + qDeviceId);
                qMarkList.append(ss1);
            }
            sendErrorResult(qSecondLevelDirName,tempStr);
        }
        else if(qDataLine.contains("monitorError"))
        {
            QString ss1=QString::number(qLooping)+"="+qDeviceId + "=" + qScriptLine;
            if(qMarkList.indexOf(ss1)==-1)
            {
                QStringList tempList=qErrorList.filter(qScriptLine + "=");
                for(QString var:tempList)
                {
                    qErrorList.removeOne(var);
                }

                sendErrorResult(qSecondLevelDirName,tempStr);

                int errorNumber=qErrorHash.value(qDeviceId + "=" + qScriptLine);
                qErrorHash.insert(qDeviceId + "=" +qScriptLine,errorNumber+1);

                qErrorList.append(qScriptLine + "=" + QString::number(errorNumber+1) + "=" + qDeviceId);
                qMarkList.append(ss1);
            }
            doMonitorCrash();

        }
        else if(qDataLine.contains("fastBack"))
        {
            fastBack();
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


void Worker::doMonitorCrash()
{
    QString cmdLine;
    QString currentTime;
    QString monitorLogFile;

    stopReplay();


    currentTime=Helper::getTime();
    monitorLogFile=qLogcatDir + QDir::separator()  + "monitor-" + currentTime + ".txt";
    monitorLogFile=QDir::toNativeSeparators(monitorLogFile);

    cmdLine=qAdbDevice + " logcat -n 10000 -d";
    ExeCmd::runCmd(cmdLine,monitorLogFile);


    cmdLine=qAdbDevice + " shell screencap /sdcard/temppng/" + qModuleBaseName + "-" + qScriptBaseName + "_" + currentTime + ".png";
    ExeCmd::runCmd(cmdLine);

    pullTakeScreen2();

    qCrashModuleList.append(qDeviceId+"="+qModuleBaseName);
    emit sendCrashResult(qDeviceId,qSecondLevelDirName,qModuleBaseName,qScriptBaseName);

}

void Worker::pullTakeScreen2()
{
    QString cmdLine;

    cmdLine=qAdbDevice + " pull /sdcard/temppng/. " + qPngDir;
    ExeCmd::runCmd(cmdLine);

    cmdLine=qAdbDevice + " shell rm /sdcard/temppng/*.*";
    ExeCmd::runCmd(cmdLine);
}

void Worker::fastBack()
{
    QString cmdLine;

    cmdLine=qAdbDevice + " shell input keyevent 4";
    ExeCmd::runCmd(cmdLine);
    ExeCmd::runCmd(cmdLine);
}


void Worker::pushApk(const QString &arg_apkName)
{
    QString str;
    QString cmdLine;

    str=qAdbDevice + " push " + gApkDir + QDir::separator() + arg_apkName;
    str=QDir::toNativeSeparators(str);

    cmdLine=str + " /sdcard/";
    ExeCmd::runCmd(cmdLine);
}


}
