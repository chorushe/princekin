#ifndef REPLAYFORBEHAVIORCONTROLLER_H
#define REPLAYFORBEHAVIORCONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRunnable>
#include "replayforbehaviorworker.h"

class ReplayForBehaviorController : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit ReplayForBehaviorController(QObject *parent = 0);

signals:

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
   void sendReplayResult(const QString &,const QString &,QMap<QString, bool> ,int);
   void sendStopReplay();
   void sendErrorResult(const QString &,const QString &);
   void sendOneScriptFinish(const QString &, bool flag);

public slots:
   void receiveRunScriptResult(const QString &);
   void receiveReplayResult(const QString &, const QString &, QMap<QString, bool> , int);
   void receiveStopReplay();
   void receiveErrorResult(const QString &,const QString &);
   void recieveOneScriptFinish(const QString &, bool flag);



private:
   QString qDeviceId;
   QString qFirstLevelDirName;
   QString qSecondLevelDirName;
   QStringList qScriptList;
};

#endif // REPLAYFORBEHAVIORCONTROLLER_H
