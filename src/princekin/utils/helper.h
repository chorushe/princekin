#ifndef HELPER_H
#define HELPER_H

#include "commonhead.h"
#include "componenthead.h"
#include <QXmlStreamReader>
#include "globalvar.h"
#include "parsexml.h"
#include <QStandardPaths>


class Helper : public QObject
{
    Q_OBJECT
public:
    Helper();
    ~Helper();

public:
    static void initVar();
    static void initEvent();
    static void initConfig();
    static void initCurrentRepository();
    static void createRepository(const QString &);
    static void loadRepository(const QString &);
    static void loadRepository2(const QString &);
    static void createPath(const QString &);
    static void createfile(const QString &);
    static void writeStr(const QString &,const QString &);
    static void createDir(const QString &);
    static void getWorkspace();
    static void copyXml();
    static void copyXml(const QString &,const QString &,const QString &);

    static bool readConfigXml(const QString &);
    static bool isPackageName1(const QString &);
    static bool copyDir(const QString &, const QString &, bool);
    static bool isCopyConfig(const QString &);
    static bool copyNormal(const QString &, const QString &, bool);
    static bool copyModify(const QString &, const QString &, bool);

    static QString getPackageName(const QString &);
    static QString getTime();
    static QString getTime2(const QString&);    
    static QString getFirstLine(const QString &);
    static QString getAllText(const QString &);

    static QStringList getList(const QString &);
    static QHash<QString,QString> getBrand(const QString &);

};

#endif // HELPER_H
