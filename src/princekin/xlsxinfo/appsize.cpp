#include "appsize.h"

AppSize::AppSize()
{
}

QString AppSize::getSize(const QString &arg_deviceId)
{
    QString cmdLine;
    QString filePath;

    filePath=gConfigDir + QDir::separator() + "packageName.txt";
    cmdLine="adb -s " + arg_deviceId + " push " + filePath + " /sdcard/";
    qDebug()<<cmdLine;
    ExeCmd::runCmd(cmdLine);

    filePath=gConfigDir + QDir::separator() + "apk" + QDir::separator() + "appsize.apk";
    cmdLine="adb -s " + arg_deviceId + " install -r " + filePath;
    qDebug()<<cmdLine;
    ExeCmd::runCmd(cmdLine);


    cmdLine="adb -s " + arg_deviceId + " shell am startservice appsize.sohu.com.appsize/.GetSizeService";
    qDebug()<<cmdLine;
    ExeCmd::runCmd(cmdLine);

    cmdLine="adb -s " + arg_deviceId + " shell am stopservice appsize.sohu.com.appsize/.GetSizeService";
    qDebug()<<cmdLine;
    ExeCmd::runCmd(cmdLine);



    filePath=gConfigDir + QDir::separator() + "appsize.txt";
    cmdLine="adb -s " + arg_deviceId + " pull /sdcard/appsize.txt " + filePath;
    qDebug()<<cmdLine;
    ExeCmd::runCmd(cmdLine);


    QFile file;
    file.setFileName(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return "";
    }
    QTextStream inStream(&file);
    QString appSizeStr=inStream.readLine();
    file.close();

    return appSizeStr;
}
