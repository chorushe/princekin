#ifndef TRAVELWORKER_H
#define TRAVELWORKER_H

#include <QObject>
#include <QProcess>

class TravelWorker : public QObject
{
    Q_OBJECT
public:
    explicit TravelWorker(QObject *parent = 0);
    QString cmdStr;
private:
    QProcess *p=NULL;

signals:
    void readOutputFromWorker(QString recieveStr);
    void readErrorFromWorker(QString recieveStr);
    void travelStop();

public slots:
    void startTravel();

    void ReadOutput();
    void ReadError();
    void travelerNotRunning();
};

#endif // TRAVELWORKER_H
