#ifndef FRAMESCONTROLLER_H
#define FRAMESCONTROLLER_H

#include "framesworker.h"

#include <QObject>
#include <QRunnable>

#include <QDebug>
#include <QString>

using namespace FramesWorker;

namespace FramesController
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
   void sendResult(const QString &,const QString &);

public slots:
   void receiveResult(const QString &,const QString &);

private:
    QString qCmdLine;
    QString qDeviceId;
};

}

#endif // FRAMESCONTROLLER_H
