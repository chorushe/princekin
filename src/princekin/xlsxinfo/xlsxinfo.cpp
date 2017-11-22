#include "xlsxinfo.h"

XlsxInfo::XlsxInfo()
{

}

void XlsxInfo::copyFile(const QString &arg_source, const QString &arg_target)
{
    QFile::copy(arg_source,arg_target);
}

QString XlsxInfo::getApkName()
{
    QString apkName;
    QString filePath;
    QStringList strList;
    QStringList splitResult;

    apkName="";

    filePath=gConfigDir + QDir::separator() + "install-apk-name.txt";
    strList=Helper::getList(filePath);
    if(!strList.isEmpty())
    {
        apkName=strList.at(0);//sohuvideo.apk
        splitResult=apkName.split(".");
    }

    return apkName;
}

QString XlsxInfo::getIconResPath(const QString &arg_apkName)
{
    QString filePath;
    QString tagName;
    QString cmdLine;
    QString iconResPath;
    QStringList valueList;

    tagName="application:";
    filePath=gApkDir + QDir::separator() + arg_apkName;
    cmdLine="cmd /c aapt dump badging " + filePath + " | findstr application:";

    valueList=ExeCmd::runCmd_getAppName(cmdLine,tagName);
    if(valueList.size()>=4)
    {
        iconResPath=valueList.at(3);
    }
    return iconResPath;
}

QString XlsxInfo::getAppChineseName(const QString &arg_apkName)
{
    QString filePath;
    QString tagName;
    QString cmdLine;
    QString appChineseName;
    QStringList valueList;

    tagName="application:";
    filePath=gApkDir + QDir::separator() + arg_apkName;
    cmdLine="cmd /c aapt dump badging " + filePath + " | findstr application:";

    valueList=ExeCmd::runCmd_getAppName(cmdLine,tagName);
    if(valueList.size()>=4)
    {
        appChineseName=valueList.at(1);
    }
    return appChineseName;
}

QString XlsxInfo::getAppVersion(const QString &arg_apkName)
{
    QString filePath;
    QString tagName;
    QString cmdLine;
    QString appVersion;

    tagName="versionName=";
    filePath=gApkDir + QDir::separator() + arg_apkName;
    cmdLine="cmd /c aapt dump badging " + filePath + " | findstr versionName=";
    appVersion=ExeCmd::runCmd_getVersionName(cmdLine,tagName);

    return appVersion;
}

QString XlsxInfo::getAppSize(const QString &arg_apkName)
{
    QString filePath;
    QString fileSize;

    filePath=gApkDir + QDir::separator() + arg_apkName;
    QFileInfo info(filePath);
    qint64 m=info.size();
    int x=m/1024/1024;
    fileSize=QString::number(x);

    return fileSize;
}

QString XlsxInfo::getDate()
{
    QString time;
    QDateTime currentTime=QDateTime::currentDateTime();
    time=currentTime.toString("yyyy/MM/dd");
    return time;
}

QString XlsxInfo::getPlatform()
{
    return "android";
}

QString XlsxInfo::getMobileBrand(const QString &arg_deviceId)
{
    QString brand;
    QString chineseBrand;
    QString filePath;
    QString cmdLine;
    QHash<QString,QString> bradHash;

    //lenovo=联想
    filePath=gConfigDir + QDir::separator() + "brand.txt";
    bradHash=Helper::getBrand(filePath);

    cmdLine="adb -s " + arg_deviceId + " shell getprop ro.product.brand";
    brand=ExeCmd::runCmd_getProp(cmdLine);
    chineseBrand=bradHash.value(brand);//中国

    return chineseBrand;
}

QString XlsxInfo::getMobileModel(const QString &arg_deviceId)
{
    QString cmdLine;
    QString model;
    cmdLine="adb -s " + arg_deviceId + " shell getprop ro.product.model";
    model=ExeCmd::runCmd_getModel(cmdLine);//Lenovo_K920

    return model;
}

QString XlsxInfo::getMobileVersion(const QString &arg_deviceId)
{
    QString cmdLine;
    QString version;

    cmdLine="adb -s " + arg_deviceId + " shell getprop ro.build.version.release";
    version=ExeCmd::runCmd_getProp(cmdLine);//"4.4.2"

    return version;
}
QString XlsxInfo::getMobileVmsize(const QString &arg_deviceId)
{
    QString cmdLine;
    QString wmSize;

    cmdLine="adb -s " + arg_deviceId + " shell wm size";
    wmSize=ExeCmd::runCmd_getWmsize(cmdLine);

    return wmSize;
}

QString XlsxInfo::getIconPath(const QString &arg_apkName,const QString &arg_iconResPath)
{
    QString cmdLine;
    QString apkBaseName;
    QString iconPath;
    QString filePath;
    QString apkPngDir="";
    QStringList splitResult;

    splitResult=arg_apkName.split(".");
    apkBaseName=splitResult.at(0);

    splitResult=arg_iconResPath.split("/");
    int len=splitResult.size();
    for(int i=0;i<len-1;i++)
    {
        apkPngDir=apkPngDir+splitResult.at(i)+"\\";
    }
    apkPngDir=apkPngDir+splitResult.at(len-1);


    iconPath=gApkDir+QDir::separator() + apkBaseName + QDir::separator() + arg_iconResPath;
    iconPath=QDir::toNativeSeparators(iconPath);

    QFile f(iconPath);
    if(f.exists())
    {
        //如果存在，不用解压
    }
    else
    {
        QString saveDir=gApkDir + QDir::separator() + apkBaseName;
        Helper::createPath(saveDir);
        filePath=gApkDir + QDir::separator() + arg_apkName;
        cmdLine="WinRAR x -ibck -o+ " + filePath + " " + apkPngDir + " " + saveDir;
        ExeCmd::runCmd(cmdLine);
    }

    return iconPath;
}
