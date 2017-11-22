#ifndef BEHAVIORWORKER_H
#define BEHAVIORWORKER_H

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QTextCodec>
#include <QByteArray>

class BehaviorWorker : public QObject
{
    Q_OBJECT
public:
    explicit BehaviorWorker(QObject *parent = 0);
    QString cmdStr;

private:
    QProcess * p=NULL;

signals:
    void readOutputFromWorker(QString recieveStr);
    void readErrorFromWorker(QString recieveStr);

    void statStop();

public slots:
    void startStat();

    void ReadOutput();
    void ReadError();
    void recieveStopStatSignal();
    void processNotRunning();
};

#endif // BEHAVIORWORKER_H
