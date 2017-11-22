#include "cmdworker.h"
#include <QProcess>
#include <QDebug>

CmdWorker::CmdWorker()
{

}


void CmdWorker::pullPicture()
{
    QProcess proc;
    QString mStr;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(arg_cmdLine);
    proc.waitForStarted();

    if(proc.waitForFinished())
    {
        mStr=proc.readAll();
        proc.close();
        qDebug()<<mStr;
        emit pullPictureStop();
    }

}
