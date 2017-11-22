#ifndef CPUTEMPCONTROLLER_H
#define CPUTEMPCONTROLLER_H

#include "cputempworker.h"

#include <QObject>
#include <QRunnable>

#include <QDebug>
#include <QString>

using namespace CpuTempWorker;

namespace CpuTempController
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
   void setCmdLine(const QString &);
   void stopController();

signals:
   void sendStopWorker();
   void sendResult(const QString &,const QString &,int,int);

public slots:
   void receiveResult(const QString &,const QString &,int,int);

private:
    QString qCmdLine;
    QString qDeviceId;
};

}

#endif // CPUTEMPCONTROLLER_H
