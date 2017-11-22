#include "execmd.h"

static QTextCodec *tc=QTextCodec::codecForName("gb2312");
static QTextCodec *tcUtf8=QTextCodec::codecForName("utf-8");
ExeCmd::ExeCmd()
{
}




QString ExeCmd::runCmd_getAll(const QString &arg_cmdLine)
{
    QString str;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    byteArray=proc.readAll();
    str=tc->toUnicode(byteArray).trimmed();
    proc.close();

    return str;
}




QString ExeCmd::runCmd_getUid(const QString &arg_cmdLine)
{
    QString uid;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    QString tag;
    QString str;
    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        str=tc->toUnicode(byteArray).trimmed();
        if(str.startsWith("userId="))
        {
            tag=str;
        }
    }
    proc.close();


    if(tag.isEmpty())
    {
    }
    else
    {
        QStringList list=tag.split(" ");
        str=list.at(0);
        list=str.split("=");
        uid=list.at(1);
    }
    return uid;
}

QStringList ExeCmd::runCmd_getPid(const QString &arg_cmdLine)
{
    QString pid;
    QString strLine;
    QStringList strList;
    QStringList tempList;

    QRegExp regExp;
    QProcess proc;
    QByteArray byteArray;

    regExp.setPattern("(\\d+)");

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tc->toUnicode(byteArray).trimmed();
        if(strLine.startsWith("shell"))
        {
            int pos=0;
            while((pos=regExp.indexIn(strLine, pos)) != -1)
            {
                tempList << regExp.cap(1);
                pos += regExp.matchedLength();
            }
            pid=tempList.at(0);
            strList.append(pid);
        }
    }//while(proc.canReadLine())
    proc.close();

    return strList;
}



QStringList ExeCmd::runCmd_getDevice(const QString &arg_cmdLine)
{
    QString tempStr;
    QString strLine;

    QStringList strList;
    QStringList splitResult;

    QByteArray byteArray;

    QProcess proc;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(arg_cmdLine);
    proc.waitForStarted();
    while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tc->toUnicode(byteArray).trimmed();
        if(strLine.endsWith("device"))
        {
            splitResult=strLine.split("device");
            tempStr=splitResult.at(0);
            tempStr=tempStr.replace("\t","");
            strList.append(tempStr);
        }
        else
        {
            //没有启动应用或设备断开,暂不处理
        }
    }
    proc.close();
    return strList;
}

QString ExeCmd::runCmd_getModel(const QString &arg_cmdLine)
{
    QString strLine;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }
    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tc->toUnicode(byteArray).trimmed();
    }
    proc.close();

    if(strLine.startsWith("error:") || strLine.contains("not found"))
    {
        strLine="";
    }
    else
    {
        strLine.replace(" ","_");
    }

    return strLine;
}


QString ExeCmd::runCmd_getStr(const QString &arg_cmdLine,const QString &arg_flag)
{
    QString strLine;
    QString strValue;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tcUtf8->toUnicode(byteArray).trimmed();
        if(strLine.contains(arg_flag,Qt::CaseInsensitive))
        {
            strValue=strLine;
        }
    }
    proc.close();

    return strValue;
}


QStringList ExeCmd::runCmd_getAppName(const QString &arg_cmdLine,const QString &arg_flag)
{
    QString appName;
    QString strLine;
    QString strValue;
    QStringList strList;
    QStringList splitResult;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        //"application-label:'搜狐视频'"
        byteArray=proc.readLine();
        strLine=tcUtf8->toUnicode(byteArray).trimmed();
        if(strLine.contains(arg_flag,Qt::CaseInsensitive))
        {
            /*
            strValue=strLine.trimmed();
            splitResult=strValue.split(":");
            strValue=splitResult.at(1);
            splitResult=strValue.split("'");
            appName=splitResult.at(1);

            strList.append(appName);
            */

            strValue=strLine.trimmed();
            splitResult=strValue.split("'");
            qDebug()<<splitResult;
        }
    }
    proc.close();

    return splitResult;
}


QString ExeCmd::runCmd_getVersionName(const QString &arg_cmdLine,const QString &arg_flag)
{
    QString versionName;
    QString strLine;
    QString strValue;
    QStringList splitResult;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tcUtf8->toUnicode(byteArray).trimmed();
        if(strLine.contains(arg_flag,Qt::CaseInsensitive))
        {
            strValue=strLine.trimmed();
            splitResult=strValue.split(" ");

            for(QString var:splitResult)
            {
                if(var.trimmed().startsWith(arg_flag,Qt::CaseInsensitive))
                {
                    splitResult=var.trimmed().split("=");
                    strValue=splitResult.at(1);
                    splitResult=strValue.split("'");
                    versionName=splitResult.at(1);
                }
            }
        }
    }
    proc.close();

    return versionName;
}


QString ExeCmd::runCmd_getProp(const QString &arg_cmdLine)
{
    QString strLine;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }
    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tc->toUnicode(byteArray).trimmed();
    }
    proc.close();

    if(strLine.startsWith("error:") || strLine.contains("not found") )
    {
        strLine="";
    }

    return strLine;
}


QString ExeCmd::runCmd_getWmsize(const QString &arg_cmdLine)
{
    QString strLine;
    QString vmsize;
    QProcess proc;
    QStringList splitResult;

    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }
    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tc->toUnicode(byteArray).trimmed();
        if(strLine.contains("Physical size:",Qt::CaseInsensitive))
        {
            splitResult=strLine.split(":");
            vmsize=splitResult.at(1);
            vmsize=vmsize.trimmed();
        }
    }
    proc.close();

    return vmsize;
}


QStringList ExeCmd::runCmd_getIconList(const QString &arg_cmdLine,const QString &arg_flag)
{
    QString appName;
    QString strLine;
    QString strValue;
    QStringList splitResult;
    QStringList strList;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        //application-icon-120:'res/drawable-ldpi-v4/launcher_sohu.png'
        byteArray=proc.readLine();
        strLine=tcUtf8->toUnicode(byteArray).trimmed();
        if(strLine.contains(arg_flag,Qt::CaseInsensitive))
        {
            strValue=strLine.trimmed();
            splitResult=strValue.split(":");
            strValue=splitResult.at(1);


            splitResult=strValue.split("'");
            strValue=splitResult.at(1);

            strList.append(strValue);
        }
    }
    proc.close();

    return strList;
}

void ExeCmd::openFolder(const QString &path)
{
    QString ss=QDir::toNativeSeparators(path);
    QProcess::startDetached("explorer " + ss);
}


QString ExeCmd::runCmd_getLunchTime(const QString &arg_cmdLine)
{
    QString strLine;
    QString strValue;
    QString lunchTime;
    QStringList splitResult;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

   while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tcUtf8->toUnicode(byteArray).trimmed();
        if(strLine.contains("ThisTime:",Qt::CaseInsensitive))
        {
            strValue=strLine.trimmed();
            splitResult=strValue.split(":");
            lunchTime=splitResult.at(1).trimmed();
        }
    }
    proc.close();

    return lunchTime;
}

//**************2017-06-06**************//
QString ExeCmd::runCmd_getPackageActivity(const QString &arg_cmdLine,const QString &arg_flag)
{
    QString strLine;
    QString strValue;

    QStringList splitResult;
    QByteArray byteArray;

    QProcess proc;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    while(proc.canReadLine())
    {
        byteArray=proc.readLine();
        strLine=tcUtf8->toUnicode(byteArray).trimmed();

        if(strLine.contains(arg_flag,Qt::CaseInsensitive))
        {
            splitResult=strLine.split("'");
        }
    }
    proc.close();

    if(splitResult.count()>2)
    {
        strValue=splitResult.at(1);
    }

    return strValue;
}
//**************2017-06-06**************//


QStringList ExeCmd::getDevice()
{
    QProcess proc;
    QString cmdLine="adb devices";
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(cmdLine);
    proc.waitForStarted();
    while(proc.waitForFinished()==false)
    {
    }

    QString mStr;
    QStringList mSplitResult;
    QStringList mDeviceList;

    while(proc.canReadLine())
    {
        mStr=proc.readLine();
        if(mStr.contains("device") && !mStr.contains("devices"))
        {
            mSplitResult=mStr.split("device");
            mDeviceList.append(mSplitResult.at(0).trimmed());
        }
    }
    proc.close();
    return mDeviceList;
}


QStringList ExeCmd::getPackages(const QString &arg_cmdLine)
{
    QProcess proc;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(arg_cmdLine);
    proc.waitForStarted();
    while(proc.waitForFinished()==false)
    {
    }

    QString mStr;
    QStringList mSplitResult;
    QStringList mDeviceList;

    while(proc.canReadLine())
    {
        mStr=proc.readLine();
        if(mStr.contains("package:"))
        {
            mSplitResult=mStr.split(":");
            mDeviceList.append(mSplitResult.at(1).trimmed());
        }
    }
    proc.close();
    return mDeviceList;
}


QString ExeCmd::getIPAddress(const QString &arg_res)
{
    QString mStr=arg_res;
    int num=mStr.indexOf("IPv4");
    if(num!=-1)
    {
        mStr=mStr.mid(num);
        mStr=mStr.mid(mStr.indexOf(":")+1);
        mStr=mStr.left(mStr.indexOf("\r\n")).trimmed();
        qDebug()<<mStr;
    }
    return mStr;
}


QString ExeCmd::runCmd(const QString &arg_cmdLine)
{
    QProcess proc;
    QString mStr;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    if(proc.waitForFinished())
    {
        mStr=proc.readAll();
        proc.close();
    }
    return mStr;
}

QString ExeCmd::runCmd2(const QString &arg_cmdLine)
{
    QProcess proc;
    QString mStr;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    if(proc.waitForFinished())
    {
        mStr=proc.readLine();
        proc.close();
    }
    QStringList list=mStr.split("\r");
    mStr=list.at(0);

    if(mStr.startsWith("error:"))
    {
        mStr="";
    }
    else
    {
        mStr.replace(" ","_");
    }

    return mStr;

}

QString ExeCmd::runCmd3(const QString &arg_cmdLine)
{
    QProcess proc;
    QString mStr;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    if(proc.waitForFinished())
    {
        mStr=proc.readAll();
        proc.close();
    }
    QStringList list=mStr.split("\r");
    return list.at(0);
}

QString ExeCmd::runCmd(const QString &arg_cmdLine,const QString &arg_logcatfile)
{
    QProcess proc;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.setStandardOutputFile(arg_logcatfile);
    proc.start(arg_cmdLine);
    proc.waitForStarted();
    while(proc.waitForFinished()==false)
    {
    }

    QString mStr;
    proc.close();
    return mStr;
}




QString ExeCmd::getLinuxPid(const QString &arg_cmdline,const QString &arg_tag)
{
    QString mResult;
    QProcess mProc;
    mProc.setReadChannelMode(QProcess::MergedChannels);
    mProc.closeReadChannel(QProcess::StandardOutput);
    mProc.closeReadChannel(QProcess::StandardError);
    mProc.closeWriteChannel();
    mProc.start(arg_cmdline);
    mProc.waitForStarted();
    mProc.waitForFinished();

    mResult=mProc.readAll();
    mProc.close();

    QString mPid;
    if(mResult.contains(arg_tag))
    {

        QStringList mList=mResult.split(" ");
        for(int i=1;i<mList.count();i++)
        {
            if(!mList.at(i).isEmpty())
            {
                mPid=mList.at(i);
                break;
            }
        }
    }
    return mPid;
}




void ExeCmd::killLinux(const QString &arg_adbdevice,const QString &arg_cmdline,const QString &arg_tag)
{
    QString mPid=getLinuxPid(arg_adbdevice,arg_cmdline,arg_tag);
    if(mPid.isEmpty())
    {
    }
    else
    {
        runCmd(arg_adbdevice + " shell kill " + mPid);
    }
}


QString ExeCmd::getLinuxPid(const QString &arg_adbdevice,const QString &arg_cmdline,const QString &arg_tag)
{
    QString mPid;
    QProcess proc;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_adbdevice + arg_cmdline);
    proc.waitForStarted();

    if(proc.waitForFinished(-1))
    {
        QString mTag;
        QString mStrLine;
        QTextStream mStream(&proc);
        while(!mStream.atEnd())
        {
            mStrLine=mStream.readLine();
            if(mStrLine.contains(arg_tag))
            {
                mTag=mStrLine;
            }
        }
        proc.close();


        QStringList mList=mTag.split(" ");
        for(int i=1;i<mList.count();i++)
        {
            if(!mList.at(i).isEmpty())
            {
                mPid=mList.at(i);
                break;
            }
        }
    }

    return mPid;
}

QString ExeCmd::getLinuxPid2(const QString &arg_cmdline,const QString &arg_tag)
{
    QString mPid;
    QProcess proc;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdline);
    proc.waitForStarted();

    if(proc.waitForFinished(-1))
    {
        QString mTag;
        QString mStrLine;
        QTextStream mStream(&proc);
        while(!mStream.atEnd())
        {
            mStrLine=mStream.readLine();
            if(mStrLine.contains(arg_tag))
            {
                mTag=mStrLine;
            }
        }
        proc.close();


        QStringList mList=mTag.split(" ");
        for(int i=1;i<mList.count();i++)
        {
            if(!mList.at(i).isEmpty())
            {
                mPid=mList.at(i);
                break;
            }
        }
    }

    return mPid;
}


QString ExeCmd::getVersion(const QString &arg_cmdline)
{
    QString mPid;
    QProcess proc;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdline);
    proc.waitForStarted();

    if(proc.waitForFinished(-1))
    {
        QString mTag;
        QString mStrLine;
        QTextStream mStream(&proc);
        while(!mStream.atEnd())
        {
            mStrLine=mStream.readLine();
            if(mStrLine.contains("ro.build.version.release"))
            {
                mTag=mStrLine;
            }
        }
        proc.close();


        QStringList mList=mTag.split("[");

        mList=mList.at(2).split("]");
        mPid=mList.at(0);
    }

    return mPid;
}

QString ExeCmd::getUid(const QString &arg_cmdline)
{
    QString mUid;
    QProcess proc;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(arg_cmdline);
    proc.waitForStarted();

    if(proc.waitForFinished(-1))
    {
        QString mTag;
        QString mStrLine;
        QTextStream mStream(&proc);
        while(!mStream.atEnd())
        {
            mStrLine=mStream.readLine().trimmed();
            if(mStrLine.startsWith("userId="))
            {
                mTag=mStrLine;
            }
        }
        proc.close();


        if(mTag.isEmpty())
        {
        }
        else
        {
            QStringList mList=mTag.split(" ");
            QString s1=mList.at(0);
            mList=s1.split("=");
            mUid=mList.at(1);
        }
    }
    return mUid;
}

QStringList ExeCmd::getDevices()
{
    QString cmdLine="adb devices";
    QProcess p(0);
    p.setReadChannelMode(QProcess::MergedChannels);
    p.start(cmdLine);
    p.waitForStarted();
    while(p.waitForFinished()==false)
    {
    }

    QString mStr;
    QStringList mTempList;
    QStringList mDevicesResult;

    while(p.canReadLine())
    {
        mStr=p.readLine();

        if(mStr.contains("device") && !mStr.contains("devices"))
        {
            mTempList=mStr.split("device");
            QString tempStr=GetDeviceModel(mTempList.at(0).trimmed());
            //qDebug()<<mStr<<"   "<<tempStr;
            if((!tempStr.contains("error"))&&tempStr!="")
                mDevicesResult.append(tempStr+" + "+mTempList.at(0).trimmed());
            else
            {
                mDevicesResult.clear();
                mDevicesResult.append("");
                return mDevicesResult;
            }
        }
    }
    p.close();

    return mDevicesResult;
}

QString ExeCmd::GetDeviceModel(QString sn)
{
    QString getModel="";
    getModel="adb -s "+sn +" shell getprop ro.product.model\n";
    QString result=runCmd(getModel).trimmed();
    result=result.replace(" ","_");
    return result;
}

QString ExeCmd::getDeviceSN(QString deviceModel)
{
    QString tmp="+";
    int num=deviceModel.indexOf(tmp)+1;
    return deviceModel.mid(num).trimmed();
}

void ExeCmd::StopLogcat(QString deviceName)
{
    QString cmdMomkey="adb -s "+deviceName+ " shell ps | grep logcat";
    QString result=runCmd(cmdMomkey);

    qDebug()<<result;

    //"root      22124 22122 1208   688   c0128a38 4025210c S logcat\r\r\nroot      22295 22293 1340   688   c0128a38 402dd10c S logcat\r\r\n"
    if(result.contains("logcat"))
    {
        QStringList logcatList = result.split("\n");
        for(int i=0;i<logcatList.count();i++)
        {
            if(logcatList.at(i).contains("logcat"))
            {
                QString pid="";
                QStringList mList=logcatList.at(i).split(" ");
                for(int i=1;i<mList.count();i++)
                {
                    if(!mList.at(i).isEmpty())
                    {
                        pid=mList.at(i);
                        break;
                    }
                }
                cmdMomkey="adb -s "+deviceName+" shell kill "+pid;
                runCmd(cmdMomkey);
            }
        }
    }

}

