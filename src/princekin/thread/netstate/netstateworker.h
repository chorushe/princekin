#ifndef NETSTATEWORKER_H
#define NETSTATEWORKER_H

#include "windows.h"
#include "globalvar.h"

#include <QDebug>
#include <QString>

#include <QByteArray>
#include <QProcess>
#include <QTextCodec>

namespace NetStateWorker
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
signals:
    void sendProcFinished(const QString &);

private slots:
    void startWorker();
    void receiveResult();
    void receiveProcFinished(int,QProcess::ExitStatus);
    void receiveStopWorker();

private:
    QString qStr;
    QString qNetState;
    QString qDeviceId;
    QString qCmdLine;
    QByteArray qByteArray;
    QProcess *qProc;
    QTextCodec *qTc=QTextCodec::codecForName("gb2312");
};


}

#endif // NETSTATEWORKER_H
