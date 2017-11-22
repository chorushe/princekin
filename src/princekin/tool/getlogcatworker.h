#ifndef GETLOGCATWORKER_H
#define GETLOGCATWORKER_H

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QTextCodec>
#include <QByteArray>

class getLogcatWorker : public QObject
{
    Q_OBJECT
public:
    explicit getLogcatWorker(QObject *parent = 0);
    QString cmdStr;

private:
    QProcess * p=NULL;

signals:
    void readOutputFromWorker(QString recieveStr);

    void logcatStop();

public slots:
    void startLogcat();

    void ReadOutput();
    void recieveStopLogcatSignal();
    void processNotRunning();
};

#endif // GETLOGCATWORKER_H
