#ifndef SERVERCONTROL_H
#define SERVERCONTROL_H

#include <QWidget>
#include <QProcess>

class ServerControl : public QWidget
{
    Q_OBJECT
public:
    explicit ServerControl(QWidget *parent = 0);

    QProcess *p;
    QString cdCmd;
    QString getCmd;
    void StartControl();
    void StopControl();

signals:
    void SendReadOutput(QString str);

public slots:
    void ReadOutput();
};

#endif // SERVERCONTROL_H
