#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include "chartworker.h"

#include <QString>
#include <QStringList>

#include <QObject>
#include <QRunnable>

namespace ChartController
{

class Controller : public QObject,public QRunnable
{
    Q_OBJECT
public:
    Controller();

protected:
    void run();

public:
   void setCmdList(QStringList);

signals:
    void sendChartWorkerResult(const QString &);

private slots:
    void receiveWorkerResult(const QString &);

private:
   QStringList qCmdList;

};


}

#endif // CHARTCONTROLLER_H
