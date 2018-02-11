#ifndef SENDSIGNALCLASS_H
#define SENDSIGNALCLASS_H

#include <QObject>

class SendSignalClass : public QObject
{
    Q_OBJECT
public:
    explicit SendSignalClass(QObject *parent = 0);

    void emitCrash();
signals:
    void sendCrashSignal();

public slots:
};

#endif // SENDSIGNALCLASS_H
