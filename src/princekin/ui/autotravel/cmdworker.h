#ifndef CMDWORKER_H
#define CMDWORKER_H

#include <QObject>

class CmdWorker:public QObject
{
    Q_OBJECT
public:
    CmdWorker();
public:
    QString arg_cmdLine;
public slots:
    void pullPicture();
signals:
    void pullPictureStop();
};

#endif // CMDWORKER_H
