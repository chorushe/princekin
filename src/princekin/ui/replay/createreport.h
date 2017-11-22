#ifndef CREATEREPORT_H
#define CREATEREPORT_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QtXlsx>

#include "helper.h"
#include "globalvar.h"
#include "xlsxformat.h"
#include "xlsxdocument.h"
#include "xlsxabstractsheet.h"
using namespace QXlsx;

class CreateReport : public QObject
{
    Q_OBJECT
public:
    CreateReport();

public:

    void setFirstLevelDirName(const QString &);
    void setScriptList(QStringList);
    void setSecondLevelDirNameList(QStringList);
    void setModule_FileCount_List(QStringList);

    void createReportSaveDir();
    void writeExcel(const QString &);

private:
    void writeTitle(Document &);
    void writeData(Document &);

    void mergeModule(Document &);
    void mergeCells(Document &,const QString &,Format);

    void writeStyle(Document &,const QString &, const QString &,Format);

private:

    QString qSourceXlsx;
    QString qTargetXlsx;
    QString qFirstLevelDirName;
    QStringList qScriptList;
    QStringList qSecondLevelDirNameList;
    QStringList qModule_FileCount_List;
};

#endif // CREATEREPORT_H
