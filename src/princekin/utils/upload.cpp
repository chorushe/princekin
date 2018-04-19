#include "upload.h"

Upload::Upload()
{

}
Upload::~Upload()
{

}

void Upload::createFolder(const QString &arg_date,const QString &arg_packageName,const QString &arg_detailedFolder)
{
    writeStr(arg_date,arg_packageName,arg_detailedFolder);

    QString cmdLine;
    //cmdLine=gConfigDir+QDir::separator()+"FileZilla"+QDir::separator()+"ftp -s:"+gConfigDir+QDir::separator()+"upload.txt";
    cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"upload.txt";
    ExeCmd::runCmd(cmdLine);

    //createTempFolder(arg_date,arg_packageName,arg_detailedFolder);
}

void Upload::createFolder(const QString &arg_date,const QString &arg_packageName,const QString &arg_detailedFolder,const QString &arg_modelid)
{
    QString cmdLine;

    writeStr(arg_date,arg_packageName,arg_detailedFolder,arg_modelid);

    cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"upload.txt";
    ExeCmd::runCmd(cmdLine);
}






void Upload::writeStr(const QString &arg_date,const QString &arg_packageName,const QString &arg_detailedFolder)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"upload.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    paramStr="mkdir "+arg_date;
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_date;
    outStream<<paramStr<<endl;

    paramStr="mkdir "+arg_packageName;
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_packageName;
    outStream<<paramStr<<endl;

    paramStr="mkdir "+arg_detailedFolder;
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();


}

void Upload::writeStr(const QString &arg_date,const QString &arg_packageName,const QString &arg_detailedFolder,const QString &arg_modelid)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"upload.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_date;
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_packageName;
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_detailedFolder;
    outStream<<paramStr<<endl;

    paramStr="mkdir "+arg_modelid;
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_modelid;
    outStream<<paramStr<<endl;

    paramStr="mkdir logcat";
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();


}


void Upload::writePackage(const QString &arg_package)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"upload.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    //paramStr="mkdir "+arg_date;
    //outStream<<paramStr<<endl;

    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();
}

void Upload::createTempFolder(const QString &arg_date,const QString &arg_packageName,const QString &arg_detailedFolder)
{

    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"tempuploadPath.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    paramStr="mkdir "+arg_date;
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_date;
    outStream<<paramStr<<endl;

    paramStr="mkdir "+arg_packageName;
    outStream<<paramStr<<endl;

    paramStr="cd "+arg_packageName;
    outStream<<paramStr<<endl;

    paramStr="mkdir "+arg_detailedFolder;
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

}

QString Upload::getHttp(const QString &arg_detailedPath)
{
    int len;
    QString httppath="";
    QStringList splitResult;

    splitResult=arg_detailedPath.split(QDir::separator());
    len=splitResult.length();

    /*
    for(int i=2;i<len;i++)
    {
        httppath=httppath+splitResult.at(i)+"/";
    }
    */
    int index=0;
    for(int i=0;i<len;i++)
    {
        if(splitResult.at(i)=="report")
        {
            index=i;
        }
    }

    for(int i=index;i<len;i++)
    {
        httppath=httppath+splitResult.at(i)+"/";
    }

    httppath="http://10.10.53.117/"+httppath;

    return httppath;
}

QString Upload::getDataPath(const QString &arg_detailedPath)
{
    int len;
    QString httppath="";
    QStringList splitResult;

    splitResult=arg_detailedPath.split(QDir::separator());
    len=splitResult.length();

    /*
    for(int i=2;i<len;i++)
    {
        httppath=httppath+splitResult.at(i)+"/";
    }
    */

    int index=0;
    for(int i=0;i<len;i++)
    {
        if(splitResult.at(i)=="report")
        {
            index=i;
        }
    }

    for(int i=index;i<len;i++)
    {
        httppath=httppath+splitResult.at(i)+"/";
    }
    return httppath;
}


void Upload::putScriptlog(const QString &arg_localPath,const QString &arg_datePath,const QString &arg_modelid)
{
    QString cmdLine;
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+ arg_modelid + ".txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="put " + gDetailedPath + QDir::separator() + "detail-report.xlsx " +  arg_datePath +  "/detail-report.xlsx";
    outStream<<paramStr<<endl;


    paramStr="put " + arg_localPath + QDir::separator() + "script-log.txt " +  arg_datePath +  "/script-log.txt";
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();


    cmdLine="ftp -s:"+gConfigDir+QDir::separator()+arg_modelid+".txt";
    ExeCmd::runCmd(cmdLine);
}

void Upload::writePut(const QString &arg_localPath,const QString &arg_datePath,const QString &arg_modelid)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+ arg_modelid + ".txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="put "+arg_localPath+QDir::separator() + "script-log.txt " +  arg_datePath +  "/script-log.txt";
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();
}



void Upload::putLogcat(const QString &arg_localPath,const QString &arg_dataPath,const QString &arg_logfile)
{
    QString cmdLine;

    writeLogcat(arg_localPath,arg_dataPath,arg_logfile);
    cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"logcat.txt";
    ExeCmd::runCmd(cmdLine);
}

void Upload::writeLogcat(const QString &arg_localPath,const QString &arg_dataPath,const QString &arg_logfile)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator() + "logcat.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="put "+arg_localPath+QDir::separator() + arg_logfile + " " +  arg_dataPath +  "/"+arg_logfile;
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();
}

void Upload::createFtpMonkeyFolder(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"monkeyfolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    paramStr="mkdir monkey";
    outStream<<paramStr<<endl;

    paramStr="cd monkey";
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_deviceId;
    outStream<<paramStr<<endl;


    paramStr="cd "+arg_deviceId;
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_startTime;
    outStream<<paramStr<<endl;


    paramStr="cd "+arg_startTime;
    outStream<<paramStr<<endl;
    paramStr="mkdir monkey";
    outStream<<paramStr<<endl;
    paramStr="mkdir logcat";
    outStream<<paramStr<<endl;
    paramStr="mkdir bugreport";
    outStream<<paramStr<<endl;


    paramStr="cd bugreport";
    outStream<<paramStr<<endl;
    paramStr="mkdir anr";
    outStream<<paramStr<<endl;
    paramStr="mkdir anr_1";
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"monkeyfolder.txt";
    ExeCmd::runCmd(cmdLine);
}

void Upload::putMonkey(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"monkeyfolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="prompt";
    outStream<<paramStr<<endl;



    paramStr="cd /data/ftpuser/test/report/monkey/"+arg_deviceId+"/"+arg_startTime;
    outStream<<paramStr<<endl;

    paramStr="mput "+arg_xlsSaveDir+QDir::separator()+"*.xlsx";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report/monkey/"+arg_deviceId+"/"+arg_startTime+"/"+"monkey";
    outStream<<paramStr<<endl;

    paramStr="mput "+arg_xlsSaveDir+QDir::separator()+"monkey\\*.txt";
    outStream<<paramStr<<endl;


    paramStr="cd ..";
    outStream<<paramStr<<endl;
    paramStr="cd logcat";
    outStream<<paramStr<<endl;
    paramStr="mput "+arg_xlsSaveDir+QDir::separator()+"logcat\\*.txt";
    outStream<<paramStr<<endl;


    paramStr="cd ..";
    outStream<<paramStr<<endl;
    paramStr="cd bugreport";
    outStream<<paramStr<<endl;
    paramStr="mput "+arg_xlsSaveDir+QDir::separator()+"bugreport\\*.txt";
    outStream<<paramStr<<endl;


    paramStr="cd anr";
    outStream<<paramStr<<endl;
    paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "bugreport" + QDir::separator() + "anr" +QDir::separator() + "anr" +QDir::separator() + "*.txt";
    outStream<<paramStr<<endl;



    paramStr="cd ..";
    outStream<<paramStr<<endl;
    paramStr="cd anr_1";
    outStream<<paramStr<<endl;
    paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "bugreport" + QDir::separator() + "anr_1" + QDir::separator() + "anr_1" + QDir::separator() + "*.txt";
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"monkeyfolder.txt";
    ExeCmd::runCmd(cmdLine);
}

void Upload::createFtpTravelFolder(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"travelfolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    paramStr="mkdir travel";
    outStream<<paramStr<<endl;

    paramStr="cd travel";
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_deviceId;
    outStream<<paramStr<<endl;


    paramStr="cd "+arg_deviceId;
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_startTime;
    outStream<<paramStr<<endl;


    paramStr="cd "+arg_startTime;
    outStream<<paramStr<<endl;
    paramStr="mkdir log";
    outStream<<paramStr<<endl;

    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"travelfolder.txt";
    ExeCmd::runCmd(cmdLine);
}

void Upload::putTravel(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"travelfolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="prompt";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report/travel/"+arg_deviceId+"/"+arg_startTime;
    outStream<<paramStr<<endl;
    paramStr="mput "+arg_xlsSaveDir+QDir::separator()+"*.xlsx";
    outStream<<paramStr<<endl;




    paramStr="cd /data/ftpuser/test/report/travel/"+arg_deviceId+"/"+arg_startTime+"/log";
    outStream<<paramStr<<endl;

    paramStr="mput "+arg_xlsSaveDir+QDir::separator()+"log\\*.txt";
    outStream<<paramStr<<endl;

    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"travelfolder.txt";
    ExeCmd::runCmd(cmdLine);
}


void Upload::createFtpbehaviourFolder(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    if(gMobileOS=="android")
    {
        QString paramStr;
        QString filename=gConfigDir+QDir::separator()+"behaviourfolder.txt";
        QFile file(filename);
        QTextStream outStream(&file);
        file.open(QIODevice::WriteOnly);


        paramStr="open 10.10.53.117";
        outStream<<paramStr<<endl;

        paramStr="ftpuser";
        outStream<<paramStr<<endl;

        paramStr="djs1#sjm";
        outStream<<paramStr<<endl;


        paramStr="cd /data/ftpuser/test/report";
        outStream<<paramStr<<endl;

        paramStr="mkdir behaviour";
        outStream<<paramStr<<endl;

        paramStr="cd behaviour";
        outStream<<paramStr<<endl;
        paramStr="mkdir "+arg_deviceId;
        outStream<<paramStr<<endl;


        paramStr="cd "+arg_deviceId;
        outStream<<paramStr<<endl;
        paramStr="mkdir "+arg_startTime;
        outStream<<paramStr<<endl;


        paramStr="bye";
        outStream<<paramStr<<endl;

        paramStr="quit";
        outStream<<paramStr<<endl;

        paramStr="exit";
        outStream<<paramStr<<endl;

        file.close();

        QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"behaviourfolder.txt";
        ExeCmd::runCmd(cmdLine);
    }
    else
    {
        QString paramStr;
        QString filename=gConfigDir+QDir::separator()+"behaviourfolder.txt";
        QFile file(filename);
        QTextStream outStream(&file);
        file.open(QIODevice::WriteOnly);


        paramStr="open 10.10.53.117";
        outStream<<paramStr<<endl;

        paramStr="ftpuser";
        outStream<<paramStr<<endl;

        paramStr="djs1#sjm";
        outStream<<paramStr<<endl;


        paramStr="cd /data/ftpuser/test/report";
        outStream<<paramStr<<endl;

        paramStr="mkdir behaviour";
        outStream<<paramStr<<endl;

        paramStr="cd behaviour";
        outStream<<paramStr<<endl;
        paramStr="mkdir ios";
        outStream<<paramStr<<endl;


        paramStr="cd ios";
        outStream<<paramStr<<endl;
        paramStr="mkdir "+arg_startTime;
        outStream<<paramStr<<endl;


        paramStr="bye";
        outStream<<paramStr<<endl;

        paramStr="quit";
        outStream<<paramStr<<endl;

        paramStr="exit";
        outStream<<paramStr<<endl;

        file.close();

        QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"behaviourfolder.txt";
        ExeCmd::runCmd(cmdLine);
    }

}

void Upload::putBehaviour(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    if(gMobileOS=="android")
    {
        QString paramStr;
        QString filename=gConfigDir+QDir::separator()+"behaviourfolder.txt";
        QFile file(filename);
        QTextStream outStream(&file);
        file.open(QIODevice::WriteOnly);


        paramStr="open 10.10.53.117";
        outStream<<paramStr<<endl;

        paramStr="ftpuser";
        outStream<<paramStr<<endl;

        paramStr="djs1#sjm";
        outStream<<paramStr<<endl;


        paramStr="prompt";
        outStream<<paramStr<<endl;


        paramStr="cd /data/ftpuser/test/report/behaviour/"+arg_deviceId+"/"+arg_startTime;
        outStream<<paramStr<<endl;

        paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "*.xlsx";
        outStream<<paramStr<<endl;

        paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "*.txt";
        outStream<<paramStr<<endl;


        paramStr="bye";
        outStream<<paramStr<<endl;

        paramStr="quit";
        outStream<<paramStr<<endl;

        paramStr="exit";
        outStream<<paramStr<<endl;

        file.close();

        QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"behaviourfolder.txt";
        ExeCmd::runCmd(cmdLine);
    }
    else
    {
        QString paramStr;
        QString filename=gConfigDir+QDir::separator()+"behaviourfolder.txt";
        QFile file(filename);
        QTextStream outStream(&file);
        file.open(QIODevice::WriteOnly);


        paramStr="open 10.10.53.117";
        outStream<<paramStr<<endl;

        paramStr="ftpuser";
        outStream<<paramStr<<endl;

        paramStr="djs1#sjm";
        outStream<<paramStr<<endl;


        paramStr="prompt";
        outStream<<paramStr<<endl;


        QString ssss="ios";
        paramStr="cd /data/ftpuser/test/report/behaviour/"+ssss+"/"+arg_startTime;
        outStream<<paramStr<<endl;

        paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "*.xlsx";
        outStream<<paramStr<<endl;

        paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "*.txt";
        outStream<<paramStr<<endl;


        paramStr="bye";
        outStream<<paramStr<<endl;

        paramStr="quit";
        outStream<<paramStr<<endl;

        paramStr="exit";
        outStream<<paramStr<<endl;

        file.close();

        QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"behaviourfolder.txt";
        ExeCmd::runCmd(cmdLine);
    }

}



void Upload::createFtpinterfaceFolder(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"interfacefolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    paramStr="mkdir interface";
    outStream<<paramStr<<endl;

    paramStr="cd interface";
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_deviceId;
    outStream<<paramStr<<endl;


    paramStr="cd "+arg_deviceId;
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_startTime;
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"interfacefolder.txt";
    ExeCmd::runCmd(cmdLine);
}

void Upload::putInterface(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"interfacefolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="prompt";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report/interface/"+arg_deviceId+"/"+arg_startTime;
    outStream<<paramStr<<endl;

    paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "*.xlsx";
    outStream<<paramStr<<endl;

    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"interfacefolder.txt";
    ExeCmd::runCmd(cmdLine);
}


void Upload::createFtpperformanceFolder(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"performancefolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report";
    outStream<<paramStr<<endl;

    paramStr="mkdir performance";
    outStream<<paramStr<<endl;

    paramStr="cd performance";
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_deviceId;
    outStream<<paramStr<<endl;


    paramStr="cd "+arg_deviceId;
    outStream<<paramStr<<endl;
    paramStr="mkdir "+arg_startTime;
    outStream<<paramStr<<endl;


    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"performancefolder.txt";
    ExeCmd::runCmd(cmdLine);
}

void Upload::putPerformance(const QString &arg_deviceId,const QString &arg_startTime,const QString &arg_xlsSaveDir)
{
    QString paramStr;
    QString filename=gConfigDir+QDir::separator()+"performancefolder.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="prompt";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report/performance/"+arg_deviceId+"/"+arg_startTime;
    outStream<<paramStr<<endl;

    paramStr="mput " + arg_xlsSaveDir + QDir::separator() + "*.xlsx";
    outStream<<paramStr<<endl;

    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();

    QString cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"performancefolder.txt";
    ExeCmd::runCmd(cmdLine);
}


void Upload::putDaily(const QString &arg_dataPath)
{
    QString cmdLine;
    QString paramStr;
    QString filename=gConfigDir + QDir::separator() + "putdaily.txt";
    QFile file(filename);
    QTextStream outStream(&file);
    file.open(QIODevice::WriteOnly);


    paramStr="open 10.10.53.117";
    outStream<<paramStr<<endl;

    paramStr="ftpuser";
    outStream<<paramStr<<endl;

    paramStr="djs1#sjm";
    outStream<<paramStr<<endl;


    paramStr="prompt";
    outStream<<paramStr<<endl;


    paramStr="cd /data/ftpuser/test/report/"+gStartTextDate+"/"+gPackageName;
    outStream<<paramStr<<endl;


    paramStr="put " + gPackagePath + QDir::separator() + "daily"+gStartTextDate+".xlsx ";
    outStream<<paramStr<<endl;

    paramStr="bye";
    outStream<<paramStr<<endl;

    paramStr="quit";
    outStream<<paramStr<<endl;

    paramStr="exit";
    outStream<<paramStr<<endl;

    file.close();


    cmdLine="ftp -s:"+gConfigDir+QDir::separator()+"putdaily.txt";
    ExeCmd::runCmd(cmdLine);
}

