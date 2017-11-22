#ifndef INITENV_H
#define INITENV_H


#include "execmd.h"
#include "helper.h"
#include "globalvar.h"

#include <QObject>
#include <QRunnable>

class InitEnv : public QObject,public QRunnable
{
    Q_OBJECT
public:
    InitEnv();

public:
    void setDeviceIdList(QStringList);
    void setModuleNameList(QStringList);

protected:
    void run();

signals:
    void sendFinished();

private:
    QStringList qDeviceIdList;
    QStringList qModuleNameList;

};

#endif // INITENV_H
