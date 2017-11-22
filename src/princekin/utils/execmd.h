#ifndef EXECMD_H
#define EXECMD_H

#include <QDebug>
#include <QString>
#include <QProcess>
#include <QByteArray>
#include <QTextCodec>
#include <QRegExp>
#include <QDir>
#include <QCoreApplication>
#include "globalvar.h"
class ExeCmd
{
public:
    ExeCmd();
public:
    static QString runCmd_getAll(const QString &);
    static QString runCmd_getUid(const QString &);
    static QString runCmd_getModel(const QString &);
    static QString runCmd_getProp(const QString &);
    static QString runCmd_getWmsize(const QString &);
    static QStringList runCmd_getDevice(const QString &);
    static QStringList runCmd_getPid(const QString &);
    static QStringList runCmd_getIconList(const QString &,const QString &);
    static QString runCmd_getStr(const QString &,const QString &);
    static QStringList runCmd_getAppName(const QString &,const QString &);
    static QString runCmd_getVersionName(const QString &,const QString &);
    static void openFolder(const QString &);
    static QString runCmd_getLunchTime(const QString &);
    static QString runCmd_getPackageActivity(const QString &,const QString &);
    static QStringList getDevice();
    static QStringList getPackages(const QString &);
    static QString runCmd(const QString &);
    static QString runCmd2(const QString &);
    static QString runCmd3(const QString &);
    static QString runCmd(const QString &,const QString &);
    static QString getLinuxPid(const QString &,const QString &);
    static void killLinuxPid(const QString &);
    static QString getLinuxPid2(const QString &,const QString &);
    static QString getLinuxPid(const QString &,const QString &,const QString &);
    static void killLinux(const QString &,const QString &,const QString &);
    static QString getVersion(const QString &);
    static QString getUid(const QString &);
    static QString GetDeviceModel(QString sn);
    static QStringList getDevices();
    static QString getDeviceSN(QString deviceModel);
    static void StopLogcat(QString deviceName);
    static QString getIPAddress(const QString &);
};

#endif // EXECMD_H
