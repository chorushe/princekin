#include "adbshellworker.h"

namespace AdbShellWorker
{

Worker::Worker()
{
}

Worker::~Worker()
{
}

void Worker::setMark(const QString &arg_mark1,const QString &arg_mark2)
{
    qMark1=arg_mark1;
    qMark2=arg_mark2;
}

void Worker::setCmdLine(const QString &arg_text)
{
    qCmdLine=arg_text;
}

void Worker::doWork()
{
    if(qMark1=="getAll")
    {
        getAll();
    }
    /*
    QProcess proc;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(qCmdLine);
    proc.waitForStarted();

    if(proc.waitForFinished())
    {
        proc.close();
    }
    */
}

void Worker::getAll()
{
    QString str;
    QProcess proc;
    QByteArray byteArray;

    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(qCmdLine);
    proc.waitForStarted();

    while(proc.waitForFinished()==false)
    {
    }

    byteArray=proc.readAll();
    str=qTc->toUnicode(byteArray).trimmed();
    proc.close();

    emit sendWorkerResult(str,qMark2);
}

}

