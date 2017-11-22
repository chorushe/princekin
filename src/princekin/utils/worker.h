#ifndef WORKER_H
#define WORKER_H
#include "commonhead.h"
#include "globalvar.h"
#include <QObject>
#include <QEventLoop>
#include <QMutex>

#include "helper.h"
#include <QDateTime>
#include "execmd.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();


public:
    void setCmdLine(const QString &);
    void setDeviceId(const QString &);
    void setErrortag(const QString &);
    void setScript(QStringList);
    void setLoopNumber(int);
    void setCompileMore(QStringList,QStringList);
    void setCompileMore2(const QString &);


    void setPackCmdList(QStringList);


    void setArgList(QStringList);
    void setArgList2(QStringList);



    void createLogFile();

    QString getTime();
    void doMonitorCrash();


    void deleteOkError();
    void deleteError();
    void fastBack();
    void pullTakeScreen();
    void pullTakeScreen2();

    void startApp(const QString &);
    void pushApk(const QString &);
    void installApk(const QString &);
    void uninstallApk(const QString &);

    void createPath(const QString &);



    //*****************************************************//
    //memory
    void setMemoryDeviceId(const QString&);
    void initMemory(const QString&,const QString&,int);
    //cpu
    //void initCpu(const QString&,const QString&,int);


signals:
    void sendSignal(const QString&);

    void sendCompileResult(const QString&);
    void sendCompileFinished();


    void sendCompileMoreResult(const QString&);
    void sendCompileMoreFinished();

    void sendPackResult(const QString&);
    void sendPackFinished();

    void sendRunScriptResult(const QString&);
    void sendRunAllScriptFinished();


    void sendReplayResult(const QString&);
    void sendReplayFinished();

    void sendMonitorResult(const QString&);
    void sendMonitorFinished();

    void sendRemoveDevice(const QString&);
    void sendRemoveMonitorDevice(const QString&);

    void sendReport(const QString&,const QString&);

    void sendMemoryResult(const QString &,const QString &,int,int);


private slots:
    //void slot_output();
    //void outPutRunScriptResult();


    void startCompile();
    void receiveCompileResult();
    void receiveCompileFinished();


    void startCompileMore();
    void startCompileMore2();
    void receiveCompileOneResult();
    void receiveCompileOneFinished();
    void receiveStopCompileMore();


    void startPack();
    void receivePackResult();
    void receivePackProcFinished();


    void startRunScript();
    void receiveRunScriptResult();
    void receiveRunOneScriptFinished();
    void receiveStopRunScriptCommand();



    void startReplay();
    void receiveReplayResult();
    void receiveReplayFinished();



    //memory
    void startMemory();
    void receiveMemoryResult(const QString &,const QString &,int,int);



private:
    bool isStopRunScript;
    bool isStopCompile;
    bool isScriptRunError;
    bool isMonitorError;

    bool isCrashed=false;
    bool isAnr=false;
    bool isCon=false;

    int k=10;
    int loopNumber;
    int currentLoopNumber;
    QString cmdLine;
    QString deviceId;
    QString memoryID;
    QString tempStr;
    QString tempScriptName;
    QString tempModuleName;//hechang 当前模块值
    int tempScriptPos;//hechang 当前脚本所在位置
    QStringList scriptList;
    QStringList packageList;
    QStringList errorTagList;
    QStringList argList;
    QStringList argList2;
    QStringList javacList;
    QStringList fileNameList;
    QString fileName;
    QString compile2cmdline;

    QString errorTag;
    QString strReport;

    QString packageName;
    QStringList tempList;
    QStringList pushedJarList;
    QByteArray dataLine;



    QFile logFile;

    QStringList packCmdList;
    QProcess *proc;
    QTextStream *stream;
    QTextCodec *tc=QTextCodec::codecForName("gb2312");
    //QTextCodec *tc=QTextCodec::codecForName("GBK");
    //QTextCodec *tc=QTextCodec::codecForName("UTF-8");

    //memory
    QString qMemoryCmdLine;
    QString qMemoryDeviceId;
    int qInterval;

    //cpu
    QString qCpuCmdLine;
    QString qCpuDeviceId;



};

#endif // WORKER_H
