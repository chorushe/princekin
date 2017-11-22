#ifndef WRITEINFO_H
#define WRITEINFO_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QtXlsx>
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QSet>
#include <QMap>
#include <QStringList>
#include <QRegExp>

#include "xlsxformat.h"
#include "xlsxdocument.h"
#include "xlsxabstractsheet.h"
#include "globalvar.h"

using namespace QXlsx;

class WriteInfo
{

public:
    WriteInfo();
public:
    static void setIconPath(const QString &);
    static void setAppChineseName(const QString &);
    static void setAppVersion(const QString &);
    static void setAppSize(const QString &);
    static void setLaunchTime(const QString &);
    static void setPlatform(const QString &);
    static void setTestDate(const QString &);


    static void setMobileBrand(const QString &);
    static void setMobileModel(const QString &);
    static void setMobileVersion(const QString &);
    static void setMobileVmsize(const QString &);

    static void setAppSize2(const QString &);
    static void setFrames(const QString &);
    static void setXlsxSaveDir(const QString &);



    static void setMonkeyTime(const QString &);

    static void setTravelStartTime(const QString &);
    static void setTravelEndTime(const QString &);
    static void setTravelTime(const QString &);
    static void setCheckNumber(const QString &);
    static void setClickNumber(const QString &);
    static void setErrorNumber(const QString &);
    static void setTravelActivityNumber(const QString &);


    static void writeUniversal(const QString &,const QString &,const QString &);
    static void writeIcon(Document &,const QString &);
    static void writeAppInfo(Document &,const QString &);

    static void writeMobileInfo_Monkey(Document &,const QString &);
    static void writeMobileInfo(Document &);
    static void writeMobileInfo_Travel(Document &,const QString &);

    static void writeMobileInfo_behaviour(Document &,const QString &);

    static void writeStyle(Document &,const QString &, const QString &,Format);
    static Format getNormalFormat();
    static Format getHyperlinkFormat();
    static Format getErrorFormat();

    static void mergeCells(Document &,const QString &);
    static void mergeCells(Document &,const QString &,Format);
    static void writeHyperlink(Document &,int,int,QUrl,Format,const QString &,const QString &);

    //*****************20170331*****************//
    static void setUrlTotalNum(int);
    static void setUrlErrorNum(int);
    static void setUrlSuccessNum(int);
    static void setStatiList(QStringList);
    //*****************20170331*****************//

    //*****************20170606*****************//
    static void writeUrl(Document &);
    static void setMem(QStringList);
    static void setCpu(QStringList);
    static void setBattery(QStringList);
    static void setCpuTemp(QStringList);
    static void setAllWifi(double);
    static void setAllMobile(double);


    static void writeMobileInfo_Interface(Document &);
    static void writeMobileInfo_Performance(Document &);
    static void statisticsReport(const QString &);
    static void writeTitle(Document &);
    static void mergeModule(Document &);
    static void writeData(Document &arg_xlsx);
    //*****************20170606*****************//

};

#endif // WRITEINFO_H
