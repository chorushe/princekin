#include "servercontrol.h"
#include <QCoreApplication>
#include <QDir>
#include "globalvar.h"

ServerControl::ServerControl(QWidget *parent) : QWidget(parent)
{
    p=new QProcess(this);

    connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(ReadOutput()));

    //cdCmd="cd /opt/DATA/logs/pv\n";
    //getCmd="tail -f access.log|grep uid=48bfaff5f2d2db2b0277d3d1a06d003e\n";
}

void ServerControl::StartControl()
{
    QString cmdStr=gConfigDir +QDir::separator() +"behavior_cmd.bat";
    if(p->state())
        p->write(cmdStr.toLatin1());
    else
    {
        p->start("cmd", QStringList()<<"/c"<< cmdStr);
        p->write(cdCmd.toLatin1());
        p->write(getCmd.toLatin1());
    }
}

void ServerControl::StopControl()
{
    p->close();
}


void ServerControl::ReadOutput()
{
    QString str=p->readAllStandardOutput();
    emit SendReadOutput(str);
}


