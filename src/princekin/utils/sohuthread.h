#ifndef SOHUTHREAD_H
#define SOHUTHREAD_H

#include <QObject>
#include <QEventLoop>
#include <QTime>
#include <QVector>
#include <QRegExp>
#include <QList>

#include "commonhead.h"
#include "globalvar.h"
#include "helper.h"
#include "execmd.h"



class SohuThread : public QObject
{
    Q_OBJECT
public:
    SohuThread();

    //void setObj(QObject *);
    void setProjectName(const QString &);
    void setModelName(const QString &);
    void setJavaFileSaveDir(const QString &);
    void setCmdLine(const QString &);
    void setDeviceId(const QString &);
    void setCmdList(QStringList);
    void setXXX(const QString &);

    void setN(int arg_n);

    void closeProc();
    void parseEventLine(const QString &);
    void getStartTime(const QString &);
    void getEndTime(const QString &);
    void parseKey(const QString &);

    void getABS_X(const QString &);
    void getABS_Y(const QString &);
    void dump();

    void kk();

    void fenxi();
private slots:
    void slot_output();
    void slot_replay();
    void slot_closeProc();
    void slot_procFinished(int,QProcess::ExitStatus);
    void slot_geteventProcFinished(int,QProcess::ExitStatus);


    void slot_compileFinished();
    void slot_getEvent();

    void slot_outputEvent();


    void slot_createProject();
    void slot_createProjectFinished();

    void slot_buildProject();
    void slot_buildProjectFinished();
    void slot_compileOneUiautomator();
    void slot_compileOneUiautomatorFinished();
    void receiveReplayResult();




signals:
    void mySignal(const QString&);

public:
    QProcess *proc;
private:
    //QObject *obj;

    QProcess *geteventProc;
    QProcess *serviceProc;




    QString projectName;
    QString modelName;
    QString javaFileSaveDir;
    QString strXXX;
    QString tempStr;


    QByteArray dataLine;
    QString cmdLine;
    QString deviceId;
    QStringList cmdList;

    int firstn=0;
    int x1;
    int y1;
    bool isXY=false;
    bool isX=false;
    bool isY=false;

    bool isBHM=false;
    int absX;
    int absY;


    int startSec=0;
    int endSec=0;

    QString strSleepSec="";
    //QTextCodec *tc=QTextCodec::codecForName("GBK");
    QTextCodec *tc=QTextCodec::codecForName("GB18030");

};

#endif // SOHUTHREAD_H
