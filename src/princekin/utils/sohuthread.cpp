#include "sohuthread.h"

QString hc="\r\n";
QTime currentTime;
int startSecond;
int currentSecond;

int startMsec;
int currentMsec;
int sec;
SohuThread::SohuThread()
{
}

void SohuThread::setXXX(const QString &arg_XXX)
{
    strXXX=arg_XXX;
}

void SohuThread::setProjectName(const QString &arg_projectName)
{
    projectName=arg_projectName;
}

void SohuThread::setModelName(const QString &arg_modelName)
{
    modelName=arg_modelName;
}

void SohuThread::setJavaFileSaveDir(const QString &arg_javaFileSaveDir)
{
    javaFileSaveDir=arg_javaFileSaveDir;
}

void SohuThread::setCmdLine(const QString &arg_cmdLine)
{
    cmdLine=arg_cmdLine;
}

void SohuThread::setCmdList(QStringList arg_cmdList)
{
    cmdList=arg_cmdList;
}

void SohuThread::setDeviceId(const QString &arg_deviceid)
{
    deviceId=arg_deviceid;
}

void SohuThread::setN(int arg_n)
{
    firstn=arg_n;
}


void SohuThread::slot_createProject()
{
    proc=new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->closeReadChannel(QProcess::StandardOutput);
    proc->closeReadChannel(QProcess::StandardError);
    proc->closeWriteChannel();

    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_output()));
    connect(proc,SIGNAL(finished(int)),this,SLOT(slot_createProjectFinished()));

    proc->start(cmdLine);
    proc->waitForStarted();
}

void SohuThread::slot_createProjectFinished()
{
    emit mySignal("createProjectFinished");
    if(proc->isOpen())
    {
        proc->close();
    }
    delete proc;
}

void SohuThread::slot_buildProject()
{
    proc=new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->closeReadChannel(QProcess::StandardOutput);
    proc->closeReadChannel(QProcess::StandardError);
    proc->closeWriteChannel();

    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_output()));
    connect(proc,SIGNAL(finished(int)),this,SLOT(slot_buildProjectFinished()));

    proc->start(cmdLine);
    proc->waitForStarted();
}

void SohuThread::slot_buildProjectFinished()
{
    emit mySignal("buildXmlFinished");
    if(proc->isOpen())
    {
        proc->close();
    }
    delete proc;
}



void SohuThread::slot_compileOneUiautomator()
{
    proc=new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->closeReadChannel(QProcess::StandardOutput);
    proc->closeReadChannel(QProcess::StandardError);
    proc->closeWriteChannel();

    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_output()));
    connect(proc,SIGNAL(finished(int)),this,SLOT(slot_compileOneUiautomatorFinished()));

    proc->start(cmdLine);
    proc->waitForStarted();
}

void SohuThread::slot_compileOneUiautomatorFinished()
{
    emit mySignal("编译结束");
    if(proc->isOpen())
    {
        proc->close();
    }
    delete proc;
}






void SohuThread::slot_compileFinished()
{
    //emit mySignal("finished");
}



void SohuThread::slot_output()
{
    while(proc->canReadLine())
    {
        dataLine=proc->readLine();
        emit mySignal(tc->toUnicode(dataLine));
    }
}


void SohuThread::slot_replay()
{
    proc=new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveReplayResult()));
    //connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slot_procFinished(int,QProcess::ExitStatus)));
    //connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), proc, SLOT(deleteLater()) );
    proc->closeReadChannel(QProcess::StandardOutput);
    proc->closeReadChannel(QProcess::StandardError);
    proc->closeWriteChannel();

    proc->start(cmdLine);
    proc->waitForStarted();



}

void SohuThread::slot_closeProc()
{
    if(!proc->waitForFinished(3000))
    {
    }
    if(proc->isOpen())
    {
        proc->close();
    }
    proc->kill();
    //标记问题
}
void SohuThread::slot_procFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
    {
    }
    else
    {
    }
}


void SohuThread::slot_getEvent()
{
    dump();//录制一开始先dump当前页面信息，然后解析，否则会漏下第一次点击的事件   hechang
    Helper::readConfigXml(gConfigDir + QDir::separator() + "dump.xml");
    startSec=0;
    geteventProc=new QProcess(this);
    geteventProc->setReadChannelMode(QProcess::MergedChannels);
    connect(geteventProc, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_outputEvent()));
    connect(geteventProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slot_geteventProcFinished(int,QProcess::ExitStatus)));
    //connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), geteventProc, SLOT(deleteLater()) );
    geteventProc->closeReadChannel(QProcess::StandardOutput);
    geteventProc->closeReadChannel(QProcess::StandardError);
    geteventProc->closeWriteChannel();


    geteventProc->start(cmdLine);
    geteventProc->waitForStarted();
}
void SohuThread::slot_outputEvent()
{
    while(geteventProc->canReadLine())
    {
        dataLine=geteventProc->readLine();
        parseEventLine(dataLine);
    }
}



void SohuThread::parseEventLine(const QString &arg_eventLine)
{

    if(arg_eventLine.trimmed().contains("ABS_MT_POSITION_X"))
    {

        if(isX)
        {

        }
        else
        {
            isX=true;
            getABS_X(arg_eventLine.trimmed());
        }
    }
    if(arg_eventLine.trimmed().contains("ABS_MT_POSITION_Y"))
    {

        if(isY)
        {

        }
        else
        {

            isY=true;
            getABS_Y(arg_eventLine.trimmed());

            Helper::readConfigXml(gConfigDir + QDir::separator() + "dump.xml");
            getEndTime(arg_eventLine.trimmed());

            fenxi();

            dump();
            qDebug()<<"ABS_MT_POSITION_Y dump";

        }
    }

    if(arg_eventLine.trimmed().endsWith("UP"))
    {

        getEndTime(arg_eventLine.trimmed());
        parseKey(arg_eventLine.trimmed());

    }
    if(arg_eventLine.trimmed().endsWith("ffffffff"))//一个操作点下去，如果手指未抬起，则不会有ffff输出，isX是区别拖动情况，只识别第一次点下去的坐标 hechang
    {

        isX=false;
        isY=false;
    }
}


void SohuThread::parseKey(const QString &arg_eventLine)
{
    foreach (QString s, gEventList)
    {
        if(arg_eventLine.contains(s))
        {
            isBHM=true;

            emit mySignal("sleep " + strSleepSec);
            emit mySignal(hc);

            emit mySignal(gHashEvent.value(s));
            emit mySignal(hc);
            emit mySignal(hc);

            dump();
            qDebug()<<"parsekey dump";
            return;
        }
    }


}

void SohuThread::dump()
{
    QString mCmdLine="adb -s " + deviceId  + " shell uiautomator runtest /sdcard/script_Dump.jar -c com.sohu.test.Dump";
    QString res = ExeCmd::runCmd(mCmdLine);

    mCmdLine="adb -s " + deviceId  + " pull /data/local/tmp/local/tmp/dump.xml " + gConfigDir;// + QDir::separator();  这里不能再加separator，pull时报错  hechang
    res = ExeCmd::runCmd(mCmdLine);
    qDebug()<<"pull res: "+res;

    int dumpNum=0;
    while(res.contains("not exist"))
    {
        mCmdLine="adb -s " + deviceId  + " shell uiautomator runtest /sdcard/script_Dump.jar -c com.sohu.test.Dump";
        ExeCmd::runCmd(mCmdLine);

        mCmdLine="adb -s " + deviceId  + " pull /data/local/tmp/local/tmp/dump.xml " + gConfigDir;// + QDir::separator();  这里不能再加separator，pull时报错  hechang
        res = ExeCmd::runCmd(mCmdLine);
        qDebug()<<"pull res: "+res;
        dumpNum++;
        if(dumpNum>5)
            break;
    }
}

void SohuThread::slot_geteventProcFinished(int exitCode,QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
    {
    }
    else
    {
    }

    if(!geteventProc->waitForFinished(3000))
    {
    }
    if(geteventProc->isOpen())
    {
        geteventProc->close();
    }
    geteventProc->kill();
}

void SohuThread::receiveReplayResult()
{
    while(proc->canReadLine())
    {
        dataLine=proc->readLine();
        emit mySignal(tc->toUnicode(dataLine));
        if(dataLine.contains("foxtestStartApp="))
        {
            QStringList mList=tc->toUnicode(dataLine).split("=");
            ExeCmd::runCmd("adb -s " + deviceId + " shell am start -n " + mList.at(1));
        }

    }
}
void SohuThread::getABS_X(const QString &arg_x)
{
    QStringList tempList=arg_x.split(" ");
    QString test=tempList.at(tempList.size()-1);
    int result=test.toInt(0,16);
    absX=result;
}
void SohuThread::getABS_Y(const QString &arg_y)
{
    QStringList tempList=arg_y.split(" ");
    QString test=tempList.at(tempList.size()-1);
    int result=test.toInt(0,16);
    absY=result;
}


void SohuThread::getEndTime(const QString &arg_eventLine)
{
    int mIndex=arg_eventLine.indexOf(".");

    endSec=arg_eventLine.mid(1,mIndex-1).trimmed().toInt();
    if(startSec==0)
    {
        strSleepSec="0";
    }
    else
    {
        strSleepSec=QString::number(endSec-startSec);
    }
    startSec=endSec;
}

void SohuThread::fenxi()
{
    QHash<int,int> mHash;
    QList<int> dList;

    QString mBoundsValue="";
    QString mBoundsValue2="";
    QString mText="";
    QString mResourceId="";
    QString mContentDesc="";
    QString mClass="";


    QStringList mListXY;


    QRegExp rx("(\\d+)");

    int x1;
    int y1;
    int x2;
    int y2;
    int chaX=0;
    int pos=0;
    int mIndex;
    if(gBounds.isEmpty())
    {

    }
    else
    {
        int len=gBounds.size();

        int ii;
        for(int i=0;i<len;i++)
        {
            pos=0;
            mListXY.clear();
            mBoundsValue=gBounds.at(i);//[0,0][1080,1920]

            while ((pos = rx.indexIn(mBoundsValue, pos)) != -1)
            {
                mListXY.append(rx.cap(0));
                pos += rx.matchedLength();
            }
            x1=mListXY.at(0).toInt();
            x2=mListXY.at(2).toInt();
            y1=mListXY.at(1).toInt();
            y2=mListXY.at(3).toInt();


            if(x1<=absX && absX<=x2 && y1<=absY && absY<=y2)
            {
                chaX=absX-x1;
                dList.append(chaX);
                mHash.insert(chaX,i);

                //从这开始
                mBoundsValue2=mBoundsValue;
                mText=gText.at(i);


                if(gResourceId.size()==0)
                {
                    mResourceId=gResourceId.at(i);
                }
                else
                {
                    mResourceId="";
                }
                //mResourceId=gResourceId.at(i);
                mContentDesc=gContentDesc.at(i);
                mClass=gClass.at(i);
            }
        }//for

        if(dList.count()==0)
            return;

        qSort(dList.begin(), dList.end(), qLess<int>());
        mIndex=mHash.value(dList.at(0));

        mBoundsValue2=gBounds.at(mIndex);
        mText=gText.at(mIndex);

        if(gResourceId.size()==0)
        {
            mResourceId=gResourceId.at(mIndex);
        }
        else
        {
            mResourceId="";
        }
        //mResourceId=gResourceId.at(mIndex);


        mClass=gClass.at(mIndex);
        mContentDesc=gContentDesc.at(mIndex);


        emit mySignal("sleep " + strSleepSec);
        emit mySignal(hc);

        int m1=0;
        int m2=0;
        int m3=0;
        int m4=0;
        //int num=gBounds.indexOf(mBoundsValue2)+1;
        int num=mIndex+1;


        if(!mText.isEmpty())
        {
            QVector<QString> vectText(num);
            qCopy(gText.begin(), gText.begin()+num, vectText.begin());

            QStringList mTempList=vectText.toList();
            QStringList mResult=mTempList.filter(mText);
            for(int i=0;i<mResult.size();i++)
            {
                if(mResult.at(i)==mText)
                {
                    m1++;
                }
            }
        }
        if(!mResourceId.isEmpty())
        {
            QVector<QString> vectResourceId(num);
            qCopy(gResourceId.begin(), gResourceId.begin()+num, vectResourceId.begin());

            QStringList mTempList=vectResourceId.toList();
            QStringList mResult=mTempList.filter(mResourceId);

            for(int i=0;i<mResult.size();i++)
            {
                if(mResult.at(i)==mResourceId)
                {
                    m2++;
                }
            }
        }

        if(!mClass.isEmpty())
        {
            QVector<QString> vectClass(num);
            qCopy(gClass.begin(), gClass.begin()+num, vectClass.begin());

            QStringList mTempList=vectClass.toList();
            QStringList mResult=mTempList.filter(mClass);

            for(int i=0;i<mResult.size();i++)
            {
                if(mResult.at(i)==mClass)
                {
                    m3++;
                }
            }
        }

        if(!mContentDesc.isEmpty())
        {
            QVector<QString> vectContentDesc(num);
            qCopy(gContentDesc.begin(), gContentDesc.begin()+num, vectContentDesc.begin());

            QStringList mTempList=vectContentDesc.toList();
            QStringList mResult=mTempList.filter(mContentDesc);

            for(int i=0;i<mResult.size();i++)
            {
                if(mResult.at(i)==mContentDesc)
                {
                    m4++;
                }
            }
        }

        if(!mText.isEmpty())
        {
            if(m1==1)
            {
                if(mText.contains(" "))
                {
                    QStringList splitResult=mText.split(" ",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0));
                        return;
                    }
                }
                else if(mText.contains(","))
                {
                    QStringList splitResult=mText.split(",",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0));
                        return;
                    }
                }
                else if(mText.contains("，"))
                {
                    QStringList splitResult=mText.split("，",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0));
                        return;
                    }
                }
                else if(mText.contains("*"))
                {
                    QStringList splitResult=mText.split("*",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0));
                        return;
                    }
                }
                else
                {
                    emit mySignal("clickText " + mText);
                }

                //emit mySignal("clickText " + mText);
            }
            else
            {
                int m=m1-1;

                if(mText.contains(" "))
                {
                    QStringList splitResult=mText.split(" ",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0) + " " + QString::number(m));
                        return;
                    }
                }

                else if(mText.contains(","))
                {
                    QStringList splitResult=mText.split(",",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0) + " " + QString::number(m));
                        return;
                    }
                }
                else if(mText.contains("，"))
                {
                    QStringList splitResult=mText.split("，",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0) + " " + QString::number(m));
                        return;
                    }
                }
                else if(mText.contains("*"))
                {
                    QStringList splitResult=mText.split("*",QString::SkipEmptyParts);
                    if(splitResult.count()>=1)
                    {
                        emit mySignal("clickTextStartWith " + splitResult.at(0) + " " + QString::number(m));
                        return;
                    }
                }
                else
                {
                    emit mySignal("clickText " + mText + " " + QString::number(m));
                }
                //emit mySignal("clickText " + mText + " " + QString::number(m));
            }
            emit mySignal(hc);
            emit mySignal(hc);

            return;
        }

        if(!mResourceId.isEmpty())
        {
            if(m2==1)
            {
                emit mySignal("clickIdOrClass " + mResourceId);
            }
            else
            {
                int m=m2-1;
                emit mySignal("clickIdOrClass " + mResourceId + " " + QString::number(m));
            }
            emit mySignal(hc);
            emit mySignal(hc);

            return;
        }

        if(!mClass.isEmpty())
        {
            if(m3==1)
            {
                emit mySignal("clickIdOrClass " + mClass);
            }
            else
            {
                int m=m3-1;
                emit mySignal("clickIdOrClass " + mClass + " " + QString::number(m));
            }
            emit mySignal(hc);
            emit mySignal(hc);

            return;
        }

        if(!mContentDesc.isEmpty())
        {
            if(m4==1)
            {
                emit mySignal("clickDescription " + mContentDesc);
            }
            else
            {
                int m=m4-1;
                emit mySignal("clickDescription " + mContentDesc + " " + QString::number(m));
            }
            emit mySignal(hc);
            emit mySignal(hc);

            return;
        }

    }//if(gBounds.isEmpty())

}
