#ifndef BLOCKWORKER_H
#define BLOCKWORKER_H

#include "commonhead.h"
#include "globalvar.h"
#include <QObject>
#include <QEventLoop>
#include <QMutex>
#include <QDateTime>
#include "windows.h"
#include "helper.h"
#include <string>


class BlockWorker : public QObject
{
    Q_OBJECT
public:
    BlockWorker();

public:
    void setCmdLine(const QString&);


private slots:
    void startTrafficService_slot();
    void receiveTrafficServiceResult();



private:
    QString qCmdLine;
    QProcess *qProc;
    QByteArray qDataLine;
    QTextCodec *tc=QTextCodec::codecForName("gb2312");
    //QTextCodec *tc=QTextCodec::codecForName("UTF-8");
};

#endif // BLOCKWORKER_H
