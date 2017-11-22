#ifndef WRITERESULT_H
#define WRITERESULT_H

#include "xlsxformat.h"
#include "xlsxdocument.h"
#include "xlsxabstractsheet.h"
#include "globalvar.h"

#include <QtXlsx>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QUrl>
#include <QImage>

using namespace QXlsx;

class WriteResult : public QWidget
{
    Q_OBJECT
public:
    WriteResult();

public:
    void setTargetXlsx(const QString &);
    void setScriptList(QStringList);
    void setDevice_TopIndex_Hash(QHash<QString,QString>);
    void setDir(const QString &);

private:
    void writeTitle(Document &,const QString &,const QString &,const QString &);
    void writeStyle(Document &,const QString &, const QString &,Format);
    void writeHyperlink(Document &,int,int,QUrl,Format,const QString &,const QString &);

    void mergeCells(Document &,const QString &,Format);
    void setxxx(Document &,bool);

    Format getTitleFormat();
    Format getErrorFormat();
    Format getHyperlinkFormat();


    Format getNormalFormat();



public slots:
    void receiveMinMax_memory(const QString &,int,int);
    void receiveMinMax_cpu(const QString &,int,int);
    void receiveMinMax_battery(const QString &,int,int);
    void receiveMinMax_cpuTemp(const QString &,int,int);
    void receiveTraffic(const QString &,QHash<QString,QString>);


    void receiveReplayResult(const QString &,const QString &,QStringList,QStringList,const QString &);
    void receiveSec(int);
    void receiveSaveXlsx();

    void receiveBaseInfoResult(QStringList);
    void receiveBaseInfoResult(QString);
    void receiveIcon(QString);

private:
    int qFontSize;
    int qStartRow_mem_Index;
    int qStartRow_cpu_Index;
    int qStartRow_battery_Index;
    int qStartRow_cpuTemp_Index;
    int qStartRow_traffic_Index;

    int qStartRow_base_Index;
    int maxWidth=0;

    QString qDir;
    QString qTargetXlsx;
    QStringList qScriptList;
    QHash<QString,QString> qDevice_TopIndex_Hash;

    //Document *qXlsx;
};

#endif // WRITERESULT_H
