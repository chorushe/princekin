#ifndef NETSTATEeCONTROLLER_H
#define NETSTATEeCONTROLLER_H

#include "netstateworker.h"

#include <QObject>
#include <QRunnable>
#include <QString>
#include <QStringList>
#include <QThread>

using namespace NetStateWorker;

namespace NetStateController
{
class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();

public:
   void setDeviceId(const QString &);
   void setCmdLine(const QString &);
   void startController();
   void stopController();

signals:
   void sendProcFinished(const QString &);
   void sendStopWorker();

private slots:
   void receiveProcFinished(const QString &);

private:
    QString qCmdLine;
    QString qDeviceId;
    QThread ThreadInstance;
};

}

#endif // NETSTATEeCONTROLLER_H
