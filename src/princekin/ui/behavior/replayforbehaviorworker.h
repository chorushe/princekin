#ifndef REPLAYFORBEHAVIORWORKER_H
#define REPLAYFORBEHAVIORWORKER_H

#include <QObject>
#include <QMainWindow>
#include <QRunnable>
#include <QEventLoop>
#include <QProcess>
#include <QMutex>
#include <QDateTime>
#include <QMetaType>
#include <QSharedPointer>
#include <QThread>
#include <QThreadPool>
#include <QMetaType>

#include "commonhead.h"
#include "helper.h"
#include "globalvar.h"
#include "replayform.h"


typedef QMap<QString,bool> StringMap;

class ReplayForBehaviorWorker : public QObject
{
    Q_OBJECT
public:
    explicit ReplayForBehaviorWorker(QObject *parent = 0);

    void setDeviceId(const QString &);
    void setFirstLevelDirName(const QString &);
    void setSecondLevelDirName(const QString &);
    void setScriptList(QStringList);

    void startWorker();

 private:
     QProcess *qProc;
     QStringList qScriptList;
     QStringList qActualList;
     QStringList qErrorList;

     bool isStopRunScript;//外界停止脚步运行时标志位
     QString qScriptLine;
     QString qScriptBaseName;
     QString qModuleBaseName;

     int qSec=0;
     QString qPngDir;
     QString qAdbDevice;

     QString qDeviceId;
     QString qFirstLevelDirName;
     QString qSecondLevelDirName;

     QByteArray qByte;
     QString  qDataLine;
     QTextCodec *qTc=QTextCodec::codecForName("UTF-8");

     QMap<QString,bool> resMap;

     void pullTakeScreen2();
     void pushApk(const QString &arg_apkName);

     void setVarValue();

 signals:
     void sendProcFinished(const QString &);
     void sendRunOneScriptResult(const QString &);

     void sendReplayResult(const QString &,const QString &,const StringMap & ,int);
     void sendErrorResult(const QString &,const QString &);

     void oneScriptFinish(const QString &,bool flag);//当一个脚本执行结束后发送一个信号

 public slots:
     void receiveRunOneScriptResult();
     void receiveProcFinished(int exitCode);
     void receiveStopReplay();
};

#endif // REPLAYFORBEHAVIORWORKER_H
