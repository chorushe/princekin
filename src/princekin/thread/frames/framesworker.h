#ifndef FRAMESWORKER_H
#define FRAMESWORKER_H

#include <QDebug>
#include <QString>

#include <QByteArray>
#include <QRegExp>
#include <QProcess>
#include <QTextCodec>
#include <QThread>

namespace FramesWorker
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
    void sendResult(const QString &,const QString &);

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


#endif // FRAMESWORKER_H
