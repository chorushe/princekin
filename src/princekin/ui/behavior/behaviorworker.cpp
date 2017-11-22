#include "behaviorworker.h"

BehaviorWorker::BehaviorWorker(QObject *parent) : QObject(parent)
{
    p=new QProcess(this);
    connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(ReadOutput()));
    connect(p,SIGNAL(readyReadStandardError()),this,SLOT(ReadError()));
    connect(p,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(processNotRunning()));
}

void BehaviorWorker::startStat()
{
    if(p->state())
        p->write(cmdStr.toLatin1());
    else
    {
        p->start("cmd", QStringList()<<"/c"<< cmdStr );
    }
}

void BehaviorWorker::ReadOutput()
{
    while(p->canReadLine())
    {
        QByteArray res=p->readLine();
        QTextCodec *BianMa = QTextCodec::codecForName ( "GBK" );
        QString result=BianMa->toUnicode(res);
        emit readOutputFromWorker(result);
    }
}

void BehaviorWorker::ReadError()
{
    QByteArray res=p->readAllStandardError();
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QString result = codec->toUnicode(res);
    emit readErrorFromWorker(result);
}

void BehaviorWorker::recieveStopStatSignal()//外边传回来的停止统计信号
{
    if(p->state()!=QProcess::NotRunning)
    {
      //  emit statStop();
        p->close();
    }
}

void BehaviorWorker::processNotRunning()//外边传回来的停止统计信号
{
    if(p->state()==QProcess::NotRunning)
    {
        emit statStop();
        p->close();
    }
}
