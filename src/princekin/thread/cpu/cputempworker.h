#ifndef CPUTEMPWORKER_H
#define CPUTEMPWORKER_H

#include "windows.h"
#include "globalvar.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVector>
#include <QProcess>
#include <QTextCodec>

namespace CpuTempWorker
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
    void startWorker();

signals:
    void sendResult(const QString &,const QString &,int,int);

private slots:
    void receiveStopWorker();

public:
    bool qIsStop;

private:
    QString qDeviceId;
    QString qCmdLine;
    QTextCodec *qTc=QTextCodec::codecForName("gb2312");
};

}

#endif // CPUTEMPWORKER_H
