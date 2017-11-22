#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "commonhead.h"
#include "worker.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();

public:

    void setDeviceId(const QString &);
    void setScript(QStringList);
    void setLoopNumber(int);
    void setErrortag(const QString &);


    void setArgList(QStringList);
    void setArgList2(QStringList);

    void setPackCmdList(QStringList);


    void startPack(QStringList);
    void startCompile(const QString&);
    void startCompileMore(QStringList,QStringList);
    void startCompileMore2(const QString&);



    void startRunScript();


    void startReplay(const QString&);

    //memory
    void startMemory(const QString&,const QString&,int);
    void setMemoryDeviceId(const QString&);

    //cpu
    void startCpu(const QString&,const QString&,int);
    void setCpuDeviceId(const QString&);




signals:

    void sendCompileResult(const QString&);
    void sendCompileMoreResult(const QString&);
    void sendStopCompileMore();


    void sendPackResult(const QString&);

    void sendReplayResult(const QString&);




    void sendRunScriptResult(const QString&);
    void sendStopRunScriptCommand();


    void sendMonitorResult(const QString&);
    void sendStopMonitorCommand();


    void sendRemoveDevice(const QString&);
    void sendRemoveMonitorDevice(const QString&);

    void sendReport(const QString&,const QString&);

    void sendAllScriptFinishedResult();

    //void sig(const QString&);


    //内存
    void sendMemoryResult(const QString &,const QString &,int,int);
    //cpu
    //void sendCpuResult(const QString &,const QString &,int,int);



private slots:
    void receiveCompileResult(const QString&);
    void receiveCompileMoreResult(const QString&);
    void receiveStopCompileMore();


    void receivePackResult(const QString&);


    void receiveRunScriptResult(const QString&);

    void receiveStopRunScriptCommand();
    void receiveReplayResult(const QString&);

    void receiveMonitorResult(const QString&);

    void receiveAllScriptFinishedResult();



    void receiveRemoveDevice(const QString&);
    void receiveRemoveMonitorDevice(const QString&);
    void finishedTest();

    void receiveReport(const QString&,const QString&);


    //memory
    void receiveMemoryResult(const QString &,const QString &,int,int);


private:
    int loopNumber;
    QString deviceId;
    QString memoryID;
    QString cpuID;
    QString packageName;
    QString errorTag;

    QStringList deviceList;
    QStringList scriptList;
    QStringList packageList;
    QStringList argList;
    QStringList argList2;
    QStringList packCmdList;


    QThread ThreadInstance;
    //Worker *worker;



};

#endif // CONTROLLER_H
