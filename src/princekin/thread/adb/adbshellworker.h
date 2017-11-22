#ifndef ADBSHELLWORKER_H
#define ADBSHELLWORKER_H

#include <QObject>
#include <QProcess>
#include <QByteArray>
#include <QTextCodec>

namespace AdbShellWorker
{
class Worker:public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();

    void getAll();
public:
    void setMark(const QString &,const QString &);
    void setCmdLine(const QString&);
signals:
    void sendWorkerResult(const QString&,const QString&);
private slots:
    void doWork();

private:
    QString qMark1;
    QString qMark2;
    QString qCmdLine;
    QTextCodec *qTc=QTextCodec::codecForName("gb2312");
};
}
#endif // ADBSHELLWORKER_H
