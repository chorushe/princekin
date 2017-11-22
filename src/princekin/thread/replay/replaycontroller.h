#ifndef REPLAYCONTROLLER_H
#define REPLAYCONTROLLER_H

#include "replayworker.h"

#include <QString>
#include <QStringList>
#include <QObject>
#include <QRunnable>

namespace ReplayController
{

class Controller : public QObject,public QRunnable
{
    Q_OBJECT
public:
    Controller();

protected:
    void run();

public:
   void setDeviceId(const QString &);
   void setFirstLevelDirName(const QString &);
   void setSecondLevelDirName(const QString &);
   void setScriptList(QStringList);

signals:
   void sendProcFinished(const QString &);
   void sendRunOneScriptResult(const QString&);
   void sendReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &);
   void sendCrashResult(const QString &,const QString &,const QString &,const QString &);
   void sendStopReplay();
   void sendErrorResult(const QString &,const QString &);

public slots:
   void receiveProcFinished(const QString &);
   void receiveRunScriptResult(const QString &);
   void receiveReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &);
   void receiveCrashResult(const QString &,const QString &,const QString &,const QString &);
   void receiveStopReplay();
   void receiveErrorResult(const QString &,const QString &);



private:
   QString qDeviceId;
   QString qFirstLevelDirName;
   QString qSecondLevelDirName;
   QStringList qScriptList;
};

}

#endif // REPLAYCONTROLLER_H
