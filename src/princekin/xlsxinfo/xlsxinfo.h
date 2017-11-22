#ifndef XLSXINFO_H
#define XLSXINFO_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include "globalvar.h"
#include "execmd.h"
#include "helper.h"

class XlsxInfo
{

public:
    XlsxInfo();

public:
    static void copyFile(const QString &,const QString &);
    static QString getApkName();
    static QString getIconResPath(const QString &);
    static QString getIconPath(const QString &,const QString &);

    static QString getAppChineseName(const QString &);
    static QString getAppVersion(const QString &);
    static QString getAppSize(const QString &);
    static QString getMobileBrand(const QString &);
    static QString getMobileModel(const QString &);
    static QString getMobileVersion(const QString &);
    static QString getMobileVmsize(const QString &);
    static QString getDate();
    static QString getPlatform();

};

#endif // XLSXINFO_H
