#include "initenv.h"

InitEnv::InitEnv()
{
}

void InitEnv::setDeviceIdList(QStringList arg_deviceIdList)
{
    qDeviceIdList=arg_deviceIdList;
}

void InitEnv::setModuleNameList(QStringList arg_moduleNameList)
{
    qModuleNameList=arg_moduleNameList;
}


void InitEnv::run()
{

    int count1;
    int count2;

    QString strValue;
    QString cmdLine;
    QString deviceName;
    QString moduleName;

    count1=qDeviceIdList.count();
    count2=qModuleNameList.count();

    for(int i=0;i<count1;i++)
    {
        deviceName=qDeviceIdList.at(i);

        ExeCmd::runCmd("adb -s " + deviceName + " logcat -c");
        ExeCmd::runCmd("adb -s " + deviceName + " shell mkdir /sdcard/temppng/");

        for(int j=0;j<count2;j++)
        {
            moduleName=qModuleNameList.at(j);
            cmdLine="adb -s " + deviceName + " push " + gUiautomatorDir2 + QDir::separator() + moduleName + QDir::separator() + "bin" + QDir::separator() + moduleName + ".jar" + " /sdcard/" + moduleName + ".jar";
            qDebug()<<cmdLine;
            ExeCmd::runCmd(cmdLine);
        }
    }

    foreach(QString deviceId,qDeviceIdList)
    {
        cmdLine="cmd /c adb -s " + deviceId + " shell pm list packages -3 | findstr com.sohu.trafficservice";
        strValue=ExeCmd::runCmd_getAll(cmdLine);
        if(!strValue.contains("com.sohu.trafficservice"))
        {
            cmdLine="adb -s " + deviceId + " install -r " + gConfigDir + QDir::separator() + "apk" + QDir::separator() + "trafficService.apk";
            ExeCmd::runCmd(cmdLine);
        }

        cmdLine="adb -s " + deviceId + " shell am startservice com.sohu.trafficservice/.TrafficService";
        ExeCmd::runCmd(cmdLine);

        cmdLine="cmd /c adb -s " + deviceId + " shell dumpsys package " + gPackageName + " | findstr userId=";
        QString uid=ExeCmd::runCmd_getUid(cmdLine);

        if(!uid.isEmpty())
        {
            gUidHash.insert(deviceId,uid);
        }
    }

    emit sendFinished();
}
