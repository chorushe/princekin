#ifndef TEST_H
#define TEST_H

#include "globalvar.h"
#include "commonhead.h"
#include "windows.h"

class Test : public QObject
{
    Q_OBJECT
public:
    Test();

    void getMemory(const QString&,const QString&,int);
    void getCpu(const QString&,const QString&,int);
    void getBattery(const QString&,const QString&,int);

    void getCputemp(const QString&,const QString&,int);


signals:
    void sendMemoryResult(const QString &,const QString &,int,int);
    void sendCpuResult(const QString &,const QString &,float,float);
    void sendBatteryResult(const QString &,const QString &,float,float);

    void sendCputempResult(const QString &,const QString &,int,int);

};

#endif // TEST_H
