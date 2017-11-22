#ifndef MONKEYWORKER_H
#define MONKEYWORKER_H

#include <QObject>
#include <QProcess>

class MonkeyWorker : public QObject
{
    Q_OBJECT
public:
    explicit MonkeyWorker(QObject *parent = 0);
    QString cmdStr;
private:
    QProcess *p=NULL;

signals:
    void readOutputFromWorker(QString recieveStr);
    void readErrorFromWorker(QString recieveStr);
    void monkeyStop();

public slots:
    void startMonkey();

    void ReadOutput();
    void ReadError();
    void monkeyNotRunning();
};

#endif // MONKEYWORKER_H
