#include "worker.h"
#include "windows.h"
#include "helper.h"
#include <string>
#include "test.h"
using namespace std;

int m=0;
Worker::Worker()
{
}

void Worker::setCmdLine(const QString &arg_cmdLine)
{
    cmdLine=arg_cmdLine;
}

void Worker::setLoopNumber(int arg_loopNumber)
{
    loopNumber=arg_loopNumber;
}

void Worker::setCompileMore(QStringList arg_javacList,QStringList arg_fileNameList)
{
    javacList=arg_javacList;
    fileNameList=arg_fileNameList;
}

void Worker::setCompileMore2(const QString &arg_cmdline)
{
    compile2cmdline=arg_cmdline;
}


void Worker::setPackCmdList(QStringList arg_cmdList)
{
    packCmdList=arg_cmdList;
}


void Worker::setDeviceId(const QString &arg_deviceId)
{
    deviceId=arg_deviceId;
}


void Worker::setMemoryDeviceId(const QString &arg_deviceId)
{
    memoryID=arg_deviceId;
}




void Worker::setErrortag(const QString &arg_errtag)
{
    errorTag=arg_errtag;
}

void Worker::setScript(QStringList arg_scriptList)
{
    scriptList.clear();
    scriptList=arg_scriptList;
}


void Worker::setArgList(QStringList arg_argList)
{
    argList=arg_argList;


}

void Worker::setArgList2(QStringList arg_argList)
{
    argList2=arg_argList;
}




void Worker::startCompile()
{
    proc=new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->closeReadChannel(QProcess::StandardOutput);
    proc->closeReadChannel(QProcess::StandardError);
    proc->closeWriteChannel();

    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveCompileResult()));

    connect(proc, SIGNAL(finished(int)),this,SLOT(receiveCompileFinished()));
    connect(proc, SIGNAL(finished(int)),proc,SLOT(deleteLater()));

    proc->start(cmdLine);
    proc->waitForStarted();
    if(proc->waitForFinished(-1))
    {
        if(proc->isOpen())
        {
            proc->close();
        }
        delete proc;
    }
}

void Worker::receiveCompileResult()
{
    while(proc->canReadLine())
    {
        dataLine=proc->readLine();

        emit sendCompileResult(tc->toUnicode(dataLine));
    }
}
void Worker::receiveCompileFinished()
{
    emit sendCompileFinished();
    emit sendCompileResult("编译结束");
}




void Worker::startCompileMore()
{
    QMutex mutex1;
    isStopCompile=false;
    int mCount=javacList.count();
    for(int i=0;i<mCount;i++)
    {
        mutex1.lock();
        if(isStopCompile)
        {
            mutex1.unlock();
            emit sendCompileMoreResult("停止编译");
            emit sendCompileMoreFinished();
            return;
        }
        fileName=fileNameList.at(i);
        proc=new QProcess(this);
        proc->setReadChannelMode(QProcess::MergedChannels);
        proc->closeReadChannel(QProcess::StandardOutput);
        proc->closeReadChannel(QProcess::StandardError);
        proc->closeWriteChannel();

        connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveCompileOneResult()));
        connect(proc, SIGNAL(finished(int)),this,SLOT(receiveCompileOneFinished()));
        connect(proc, SIGNAL(finished(int)),proc,SLOT(deleteLater()));

        proc->start(javacList.at(i));
        proc->waitForStarted();

        if(proc->waitForFinished(-1))
        {
            if(proc->isOpen())
            {
                proc->close();
            }
            delete proc;
        }
        mutex1.unlock();
    }
    emit sendCompileMoreResult("编译完成");
    emit sendCompileMoreFinished();
}


void Worker::startCompileMore2()
{

    isStopCompile=false;
    if(isStopCompile)
    {

        emit sendCompileMoreResult("停止编译");
        emit sendCompileMoreFinished();
        return;
    }

    proc=new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->closeReadChannel(QProcess::StandardOutput);
    proc->closeReadChannel(QProcess::StandardError);
    proc->closeWriteChannel();

    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveCompileOneResult()));
    connect(proc, SIGNAL(finished(int)),this,SLOT(receiveCompileOneFinished()));
    connect(proc, SIGNAL(finished(int)),proc,SLOT(deleteLater()));

    proc->start(compile2cmdline);
    proc->waitForStarted();

    if(proc->waitForFinished(-1))
    {
        if(proc->isOpen())
        {
            proc->close();
        }
        delete proc;
    }

    emit sendCompileMoreResult("编译完成");
    emit sendCompileMoreFinished();
}



void Worker::receiveCompileOneResult()
{
    while(proc->canReadLine())
    {
        dataLine=proc->readLine();
        emit sendCompileMoreResult(tc->toUnicode(dataLine));
    }
}


void Worker::receiveCompileOneFinished()
{
    emit sendCompileMoreResult(fileName + "    编译结束");
}

void Worker::receiveStopCompileMore()
{
    isStopCompile=true;
}



void Worker::startPack()
{
    QMutex mutex1;

    int mCount=packCmdList.count();
    for(int i=0;i<mCount;i++)
    {
        mutex1.lock();
        proc=new QProcess(this);
        proc->setReadChannelMode(QProcess::MergedChannels);
        proc->closeReadChannel(QProcess::StandardOutput);
        proc->closeReadChannel(QProcess::StandardError);
        proc->closeWriteChannel();

        connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(receivePackResult()));
        connect(proc, SIGNAL(finished(int)),this,SLOT(receivePackProcFinished()));
        connect(proc, SIGNAL(finished(int)),proc,SLOT(deleteLater()));

        QStringList mCmdList;
        mCmdList<<"/c";
        mCmdList<<packCmdList.at(i);

        proc->start("cmd",mCmdList);
        proc->waitForStarted();

        if(proc->waitForFinished(-1))
        {
            if(proc->isOpen())
            {
                proc->close();
            }
            delete proc;
        }
        mutex1.unlock();
    }//for(int i=0;i<loopNumber;i++)
    emit sendPackResult("打包结束");
    emit sendPackFinished();
}


void Worker::receivePackResult()
{
    while(proc->canReadLine())
    {
        dataLine=proc->readLine();
        emit sendPackResult(tc->toUnicode(dataLine));
    }
}


void Worker::receivePackProcFinished()
{
    //emit sendCompileMoreResult(fileName + "    编译结束");
}









void Worker::createLogFile()
{
    QString mLogFile=gLogDir + QDir::separator() + deviceId + QDir::separator() + "log.txt";
    //QFile mFile(mLogFile);
    logFile.setFileName(mLogFile);
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        logFile.close();
        return;
    }
    stream=new QTextStream(&logFile);

}


void Worker::startRunScript()
{
    gHashCrash.insert(deviceId,false);
    //createLogFile();
    int mScriptCount;
    QString s2;
    QString mAdbDevice;
    QString mCmdLine;
    QString mJarName;
    QMutex mutex1;
    QMutex mutex2;


    isMonitorError=false;
    isScriptRunError=false;
    isStopRunScript=false;
    tempScriptName="";

    mAdbDevice="adb -s " + deviceId;

    mScriptCount=scriptList.count();



    for(int i=0;i<loopNumber;i++)
    {
        if(isScriptRunError)
        {
            return;
        }
        if(isStopRunScript)
        {
            return;
        }
        if(isMonitorError)
        {
            return;
        }


        currentLoopNumber=i+1;
        for(int j=0;j<mScriptCount;j++)
        {
            tempList=scriptList.at(j).split("=");
            tempScriptPos=i*mScriptCount + j;
            gAllScriptNum[deviceId]=tempScriptPos+1;
            tempModuleName=tempList.at(0);
            tempScriptName=tempList.at(1) + ".txt";
            gHashCurrentScriptName.insert(deviceId,tempScriptName);

            if(isScriptRunError)
            {
                return;
            }
            if(isStopRunScript)
            {
                return;
            }
            if(isMonitorError)
            {
                return;
            }

            proc=new QProcess(this);
            gHashProc.insert(deviceId,proc);

            gHashProc.value(deviceId)->setReadChannelMode(QProcess::MergedChannels);
            gHashProc.value(deviceId)->closeReadChannel(QProcess::StandardOutput);
            gHashProc.value(deviceId)->closeReadChannel(QProcess::StandardError);
            gHashProc.value(deviceId)->closeWriteChannel();

            connect(gHashProc.value(deviceId), SIGNAL(readyReadStandardOutput()), this, SLOT(receiveRunScriptResult()));
            connect(gHashProc.value(deviceId), SIGNAL(finished(int)),this,SLOT(receiveRunOneScriptFinished()));
            connect(gHashProc.value(deviceId), SIGNAL(finished(int)),gHashProc.value(deviceId),SLOT(deleteLater()));





            mJarName=tempList.at(0) + ".jar";

            cmdLine=mAdbDevice + " shell uiautomator runtest /sdcard/" + mJarName + " -c com.sohu.test." + tempList.at(1);

            gHashProc.value(deviceId)->start(cmdLine);
            gHashProc.value(deviceId)->waitForStarted();

            if(gHashProc.value(deviceId)->waitForFinished(-1))
            {
                if(gHashProc.value(deviceId)->isOpen())
                {
                    gHashProc.value(deviceId)->close();
                    delete gHashProc.value(deviceId);
                }
                //delete gHashProc.value(deviceId);
            }
            gRightScriptNum[deviceId]=tempScriptPos-gErrorScriptNum.value(deviceId)+1;

            if(isScriptRunError)
            {
                gHashProc.remove(deviceId);
                emit sendRunAllScriptFinished();
                emit sendRemoveDevice(deviceId);
                return;
            }
            if(isStopRunScript)
            {
                gHashProc.remove(deviceId);
                emit sendRunAllScriptFinished();
                emit sendRemoveDevice(deviceId);
                return;
            }
            if(isMonitorError)
            {
                gHashProc.remove(deviceId);
                emit sendRunAllScriptFinished();
                emit sendRemoveDevice(deviceId);
                return;
            }

        }
    }//for(int i=0;i<loopNumber;i++)

    if(gErrorScriptNum[deviceId]>0)
    {
        emit sendRunScriptResult("*** Error ! ***\r\n\r\n"  );
    }
    else
    {
        emit sendRunScriptResult("*** Success ! ***\r\n\r\n" );
    }

    emit sendRunAllScriptFinished();
    emit sendRemoveDevice(deviceId);
    gHashProc.remove(deviceId);
    gStopMemoryHash.insert(deviceId,true);

}


void Worker::receiveRunOneScriptFinished()
{
    QString numStr="";
    if((tempScriptPos+1)%10==1&&((tempScriptPos+1)%100!=11))
        numStr=QString::number(tempScriptPos+1)+"st";
    else if((tempScriptPos+1)%10==2&&((tempScriptPos+1)%100!=12))
        numStr=QString::number(tempScriptPos+1)+"nd";
    else if((tempScriptPos+1)%10==3&&((tempScriptPos+1)%100!=13))
        numStr=QString::number(tempScriptPos+1)+"rd";
    else
        numStr=QString::number(tempScriptPos+1)+"th";
    emit sendRunScriptResult("The "+numStr+" script     "+ tempModuleName+"/" + tempScriptName + "    " + "finished\r\n\r\n");
}

void Worker::receiveStopRunScriptCommand()
{
    gStopMemoryHash.insert(deviceId,true);
    isStopRunScript=true;
    emit sendRunScriptResult(deviceId + "    Stoped\r\n\r\n");
}


void Worker::receiveRunScriptResult()
{
    while(gHashProc.value(deviceId)->canReadLine())
    {
        dataLine=gHashProc.value(deviceId)->readLine();
        if(dataLine.trimmed()!="")
        {
            tempStr=deviceId + "    " + tempModuleName+"/" +tempScriptName + "    " + dataLine;
            if(!(dataLine.contains("INSTRUMENT")||dataLine.contains("OK (1 test)")))
                emit sendRunScriptResult(tempStr);
        }

        if(dataLine.contains("princekinfalse"))
        {
            gErrorScriptNum[deviceId]+=1;
            emit sendReport(QString::number(tempScriptPos)+"$"+ QString::number( scriptList.count()*loopNumber)+"$"+ tempStr,deviceId);
        }

        else if(dataLine.contains("java.lang.RuntimeException")||(dataLine.contains("Error")&&dataLine.contains("java.lang."))||dataLine.contains("error"))
        {
            gErrorScriptNum[deviceId]+=1;
            emit sendReport(QString::number(tempScriptPos)+"$"+ QString::number( scriptList.count()*loopNumber)+"$"+ tempStr,deviceId);
            isScriptRunError=true;
            gStopMemoryHash.insert(deviceId,true);
            break;
        }

       /* else if(dataLine.contains("princekin_notExitfalse"))
        {
            emit sendReport(tempStr,deviceId);
            isScriptRunError=true;
            gStopMemoryHash.insert(deviceId,true);
        }*/


        else if(dataLine.contains("monitorError"))
        {
            emit sendReport(QString::number(tempScriptPos)+"$"+ QString::number( scriptList.count())+"$"+ tempStr,deviceId);
            isMonitorError=true;
            gStopMemoryHash.insert(deviceId,true);

            doMonitorCrash();
        }

        else if(dataLine.contains("fastBack"))
        {
            fastBack();
        }
        else if(dataLine.contains("princekinTakeScreen"))
        {
            pullTakeScreen2();
        }

        else if(dataLine.contains("pushApk="))
        {
            QStringList mList=tc->toUnicode(dataLine).split("=");
            pushApk(mList.at(1));
        }

        Sleep(100);
    }
}


void Worker::doMonitorCrash()
{
    QString mCmdLine;
    QString mAdbDevice;
    QString mCurrentTime;
    QString mMonitorLogFile;
    QString mCurrentScriptName;

    QString mStr;

    mAdbDevice="adb -s " + deviceId;
    mCurrentTime=getTime();

    mCmdLine=" shell ps uiautomator";
    ExeCmd::killLinux(mAdbDevice,mCmdLine,"uiautomator");

    mStr=gDeviceDir.value(deviceId);
    mMonitorLogFile=mStr + QDir::separator()  + "logcat" + mCurrentTime + ".txt";
    mCmdLine=mAdbDevice + " logcat -n 10000 -d";
    ExeCmd::runCmd(mCmdLine,mMonitorLogFile);



    mCmdLine=mAdbDevice + " shell screencap /sdcard/temppng/" + tempScriptName + "_" + mCurrentTime + ".png";
    ExeCmd::runCmd(mCmdLine);

    mStr=gDeviceDir.value(deviceId);
    QString mPngDir=mStr + QDir::separator() + "png" + QDir::separator();
    mCmdLine=mAdbDevice + " pull /sdcard/temppng/ " + mPngDir;
    ExeCmd::runCmd(mCmdLine);

    mCmdLine=mAdbDevice + " shell rm /sdcard/temppng/*.*";
    ExeCmd::runCmd(mCmdLine);


}



void Worker::startReplay()
{
    proc=new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->closeReadChannel(QProcess::StandardOutput);
    proc->closeReadChannel(QProcess::StandardError);
    proc->closeWriteChannel();

    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveReplayResult()));

    connect(proc, SIGNAL(finished(int)),this,SLOT(receiveReplayFinished()));


    proc->start(cmdLine);
    proc->waitForStarted();
}

void Worker::receiveReplayResult()
{
    while(proc->canReadLine())
    {
        dataLine=proc->readLine();
        emit sendReplayResult(dataLine);

        if(dataLine.contains("fastBack"))
        {
            fastBack();
        }
        else if(dataLine.contains("princekinTakeScreen"))
        {
            pullTakeScreen();
        }
        else if(dataLine.contains("pushApk="))
        {
            QStringList mList=tc->toUnicode(dataLine).split("=");
            pushApk(mList.at(1));
        }
        Sleep(100);
    }
}



void Worker::receiveReplayFinished()
{
    emit sendReplayFinished();
    emit sendReplayResult("replayFinished");
    if(proc->isOpen())
    {
        proc->close();
    }
    delete proc;
}


/*
void Worker::slot_output()
{
    while(proc->canReadLine())
    {
        dataLine=proc->readLine();
        emit sendSignal(tc->toUnicode(dataLine));
        Sleep(100);
    }
}
*/

QString Worker::getTime()
{
    QDateTime mCurrentTime=QDateTime::currentDateTime();
    QString mTime=mCurrentTime.toString("yyyy-MM-dd-hh-mm-ss");
    return mTime;
}


void Worker::pushApk(const QString &arg_apkName)
{
    QString mCmdLine;
    QString mAdbDevice;
    //QString mResult;

    mAdbDevice="adb -s " + deviceId;

    QString mStr=mAdbDevice + " push " + gApkDir + QDir::separator() + arg_apkName;
    mStr=QDir::toNativeSeparators(mStr);

    mCmdLine=mStr + " /sdcard/";
    ExeCmd::runCmd(mCmdLine);
}
void Worker::fastBack()
{
    QString mCmdLine;
    QString mAdbDevice;
    mAdbDevice="adb -s " + deviceId;

    mCmdLine=mAdbDevice + " shell input keyevent 4";
    ExeCmd::runCmd(mCmdLine);
    ExeCmd::runCmd(mCmdLine);
}

void Worker::pullTakeScreen()
{
    QString mCmdLine;
    QString mAdbDevice;
    mAdbDevice="adb -s " + deviceId;


    mCmdLine=mAdbDevice + " pull /sdcard/temppng/ " + gDebugDir + QDir::separator() + deviceId + QDir::separator();
    ExeCmd::runCmd(mCmdLine);

    mCmdLine=mAdbDevice + " shell rm /sdcard/temppng/*.*";
    ExeCmd::runCmd(mCmdLine);
}

void Worker::pullTakeScreen2()
{
    QString mCmdLine;
    QString mAdbDevice;
    QString mStr;
    mAdbDevice="adb -s " + deviceId;

    mStr=gDeviceDir.value(deviceId);

    //QString mPngDir=mStr + QDir::separator() + "png" + QDir::separator() + deviceId + QDir::separator();
    //createPath(mPngDir);

    QString mPngDir=mStr + QDir::separator() + "png" + QDir::separator();

    mPngDir=QDir::toNativeSeparators(mPngDir);
    mCmdLine=mAdbDevice + " pull /sdcard/temppng/ " + mPngDir;

    ExeCmd::runCmd(mCmdLine);

    mCmdLine=mAdbDevice + " shell rm /sdcard/temppng/*.*";

    ExeCmd::runCmd(mCmdLine);
}

void Worker::createPath(const QString &arg_path)
{
    QDir mDir(arg_path);
    if(mDir.exists())
    {
    }
    else
    {
        mDir.mkpath(arg_path);
    }
}


//**************************memory*********************//
void Worker::initMemory(const QString &arg_deviceid, const QString &arg_cmdLine, int arg_interval)
{
    qMemoryDeviceId=arg_deviceid;
    qMemoryCmdLine=arg_cmdLine;
    qInterval=arg_interval;
}

void Worker::startMemory()
{

    Test *test=new Test;

    connect(test,SIGNAL(sendMemoryResult(const QString &,const QString &,int,int)), this, SLOT(receiveMemoryResult(const QString &,const QString &,int,int)), Qt::DirectConnection);
    test->getMemory(qMemoryDeviceId,qMemoryCmdLine,qInterval);
}

void Worker::receiveMemoryResult(const QString &arg_deviceID,const QString &arg_currentMemoryValue,int arg_min,int arg_max)
{
    emit sendMemoryResult(arg_deviceID,arg_currentMemoryValue,arg_min,arg_max);
}
//**************************memory*********************//




