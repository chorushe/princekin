#ifndef UPLOAD_H
#define UPLOAD_H
#include <QObject>
#include "commonhead.h"
#include "componenthead.h"
#include "globalvar.h"
#include "execmd.h"
class Upload : public QObject
{
    Q_OBJECT
public:
    Upload();
    ~Upload();
public:
    static void createFolder(const QString &,const QString &,const QString &);
    static void createFolder(const QString &,const QString &,const QString &,const QString &);
    static void createTempFolder(const QString &,const QString &,const QString &);

    static void writeStr(const QString &,const QString &,const QString &);
    static void writeStr(const QString &,const QString &,const QString &,const QString &);
    static void writePackage(const QString &);
    static QString getHttp(const QString &);
    static QString getDataPath(const QString &);
    static void putScriptlog(const QString &,const QString &,const QString &);
    static void writePut(const QString &,const QString &,const QString &);

    static void putLogcat(const QString &,const QString &,const QString &);
    static void writeLogcat(const QString &,const QString &,const QString &);

    static void createFtpMonkeyFolder(const QString &,const QString &,const QString &);
    static void putMonkey(const QString &,const QString &,const QString &);

    static void createFtpTravelFolder(const QString &,const QString &,const QString &);
    static void putTravel(const QString &,const QString &,const QString &);

    static void createFtpbehaviourFolder(const QString &,const QString &,const QString &);
    static void putBehaviour(const QString &,const QString &,const QString &);


    static void createFtpinterfaceFolder(const QString &,const QString &,const QString &);
    static void putInterface(const QString &,const QString &,const QString &);

    static void createFtpperformanceFolder(const QString &,const QString &,const QString &);
    static void putPerformance(const QString &,const QString &,const QString &);

    static void putDaily(const QString &);

};

#endif // UPLOAD_H
