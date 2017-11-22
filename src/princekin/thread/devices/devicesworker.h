#ifndef DEVICESWORKER_H
#define DEVICESWORKER_H

#include "globalvar.h"
#include <QDebug>
#include <QString>
#include <QRegExp>
#include <QProcess>
#include <QTimerEvent>
#include <QBasicTimer>
#include <QTextCodec>
#include <QByteArray>


class DevicesWorker : public QObject
{
    Q_OBJECT
public:
    DevicesWorker();

public:
    void initVar();
    //void workerStart();

protected:
      void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

public slots:
      void workerStart();

signals:
    void sendResult(QStringList);

public:
    bool qIsStop=false;


private:
    int qMin;
    int qMax;
    bool qIsFirstValue;
    QString qCmdLine;
    QString qDeviceId;
    QRegExp qRx;
    QBasicTimer qTimer;
    QTextCodec *qTc=QTextCodec::codecForName("gb2312");

};

#endif // DEVICESWORKER_H
