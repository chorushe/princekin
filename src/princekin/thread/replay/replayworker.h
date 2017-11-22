#ifndef REPLAYWORKER_H
#define REPLAYWORKER_H

#include "commonhead.h"
#include "globalvar.h"
#include <QObject>
#include <QEventLoop>
#include <QMutex>
#include "helper.h"
#include <QDateTime>
#include <QSharedPointer>
#include <QRunnable>
#include <replayform.h>
#include "execmd.h"


namespace ReplayWorker
{
class Worker  : public QObject
{
    Q_OBJECT
public:
    Worker();
public:
    void setDeviceId(const QString &);
    void setFirstLevelDirName(const QString &);
    void setSecondLevelDirName(const QString &);
    void setScriptList(QStringList);
    void startWorker();

private:
    void doMonitorCrash();
    void fastBack();
    void pullTakeScreen2();
    void pushApk(const QString &);
    void setVarValue();
    void stopReplay();
    void closeP();

signals:
    void sendProcFinished(const QString &);
    void sendRunOneScriptResult(const QString &);
    void sendReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &);
    void sendCrashResult(const QString &,const QString &,const QString &,const QString &);
    void sendErrorResult(const QString &,const QString &);



public slots:
    void receiveStopReplay();
    void receiveRunOneScriptResult();
    void receiveProcFinished(int,QProcess::ExitStatus);


private:
    int qSec;
    int qLooping;
    bool isStopRunScript;

    QString qDeviceId;
    QString qFirstLevelDirName;
    QString qSecondLevelDirName;

    QString qAdbDevice;
    QString qPngDir;
    QString qLogcatDir;
    QString qScriptLine;
    QString qScriptBaseName;
    QString qModuleBaseName;


    QHash<QString,int> qActualHash;
    QHash<QString,int> qErrorHash;

    QStringList qScriptList;
    QStringList qActualList;//实际运行的次数
    QStringList qErrorList;//运行出错的次数
    QStringList qCrashModuleList;
    QStringList qMarkList;

    int kkk=0;
    QByteArray qByte;
    QString  qDataLine;
    QProcess *qProc;
    //QTextCodec *qTc=QTextCodec::codecForName("gb2312");
    QTextCodec *qTc=QTextCodec::codecForName("UTF-8");



};
}

#endif // REPLAYWORKER_H
