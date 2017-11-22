#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include "windows.h"

#include <QEventLoop>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QRegExp>
#include <QProcess>
#include <QTextCodec>


namespace ChartWorker
{
class Worker  : public QObject
{
    Q_OBJECT
public:
    Worker();

public:
    void startWorker();
    void setCmdList(QStringList);

signals:
    void sendWorkerResult(const QString &);

private slots:
    void receiveRunOneScriptResult();
    void receiveProcFinished(int);


private:
    QStringList qCmdList;

    QByteArray qByte;
    QString  qDataLine;
    QProcess *qProc;
    //QTextCodec *qTc=QTextCodec::codecForName("gb2312");
    QTextCodec *qTc=QTextCodec::codecForName("UTF-8");
};
}

#endif // CHARTWORKER_H
