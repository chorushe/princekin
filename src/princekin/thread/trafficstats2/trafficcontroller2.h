#ifndef TRAFFICCONTROLLER2_H
#define TRAFFICCONTROLLER2_H

#include "trafficworker2.h"

#include <QObject>
#include <QRunnable>

#include <QDebug>
#include <QString>

using namespace TrafficWorker2;

namespace TrafficController2
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
    void setSec(int);

signals:
    void sendStopWorker();

    void sendResult_wifi1(const QString &,const QString &);
    void sendResult_wifi2(const QString &,const QString &);
    void sendResult_mobile1(const QString &,const QString &);
    void sendResult_mobile2(const QString &,const QString &);

public slots:
   void receiveResult_wifi1(const QString &,const QString &);
   void receiveResult_wifi2(const QString &,const QString &);
   void receiveResult_mobile1(const QString &,const QString &);
   void receiveResult_mobile2(const QString &,const QString &);


private:
    QString qCmdLine;
    QString qDeviceId;
    int qSec;
};

}

#endif // TRAFFICCONTROLLER2_H
