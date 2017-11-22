#ifndef ADBSHELLCONTROLLER_H
#define ADBSHELLCONTROLLER_H

#include <QObject>
#include <QThread>
#include "adbshellworker.h"
namespace AdbShellController
{

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();

public:
    void setMark(const QString &,const QString &);
    void setCmdLine(const QString &);
    void doController();


signals:
    void sendWorkerResult(const QString&,const QString&);

public slots:
    void receiveWorkerResult(const QString&,const QString&);

private:
    QString qMark1;
    QString qMark2;
    QString qCmdLine;
    QString qStartTime;
    QString qTargetXlsx;

    QThread threadInstance;
};
}
#endif // ADBSHELLCONTROLLER_H
