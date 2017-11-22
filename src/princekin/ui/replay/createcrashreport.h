#ifndef CREATECRASHREPORT_H
#define CREATECRASHREPORT_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QStringList>
#include <QtXlsx>
#include <QImage>
#include <QUrl>

#include "helper.h"
#include "globalvar.h"
#include "xlsxformat.h"
#include "xlsxdocument.h"
#include "xlsxabstractsheet.h"
#include "adbshellcontroller.h"
using namespace QXlsx;


class CreateCrashReport : public QObject
{
    Q_OBJECT
public:
    CreateCrashReport();

private:
    void createExcel();
    void writeStyle(Document &,const QString &, const QString &,Format);

    QString getMinMaxMemory(const QString &,QHash<QString,int>);
    QString getMinMaxCpu(const QString &,QHash<QString,int>);
    QString getMinMaxBatterytemp(const QString &,QHash<QString,int>);
    QString getMinMaxCputemp(const QString &,QHash<QString,int>);

    QString getMemoryAveValue(const QString &,QStringList);
    QString getCpuAveValue(const QString &,QStringList);
    QString getBatterytempAveValue(const QString &,QStringList);
    QString getCputempAveValue(const QString &,QStringList);

    QString getWifiMobile(const QString &);

    QImage getImage();
    Format getNormalFormat();
    Format getHyperlinkFormat();

private slots:
    void receiveCrashResult(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,QStringList);

};

#endif // CREATECRASHREPORT_H
