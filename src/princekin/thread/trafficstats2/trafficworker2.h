#ifndef TRAFFICWORKER2_H
#define TRAFFICWORKER2_H

#include "windows.h"
#include "globalvar.h"

#include <QDebug>
#include <QString>

#include <QByteArray>
#include <QProcess>
#include <QTextCodec>
#include <QTimer>

namespace TrafficWorker2
{
class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();

public:
    void initVar();
    void setDeviceId(const QString &);
    void setCmdLine(const QString &);
    void setSec(int);
    void startWorker();

signals:
    void sendResult_wifi1(const QString &,const QString &);
    void sendResult_wifi2(const QString &,const QString &);

    void sendResult_mobile1(const QString &,const QString &);
    void sendResult_mobile2(const QString &,const QString &);

private slots:
    void receiveStopWorker();

public:
    bool qIsStop;

private:
    int c;
    QString qDeviceId;
    QString qCmdLine;
    int qSec;
    QTimer *testTimer;
    QTextCodec *qTc=QTextCodec::codecForName("gb2312");
};

}

#endif // TRAFFICWORKER2_H
