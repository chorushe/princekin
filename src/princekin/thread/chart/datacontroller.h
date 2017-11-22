#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include "dataworker.h"

#include <QString>
#include <QStringList>

#include <QObject>
#include <QRunnable>

namespace DataController
{

class Controller : public QObject,public QRunnable
{
    Q_OBJECT
public:
    Controller();

protected:
    void run();

public:
   void setXlsx(const QString &);

   void setMemWarningValue(const QString &);
   void setCpuWarningValue(const QString &);
   void setCpuTempWarningValue(const QString &);
   void setBatteryTempWarningValue(const QString &);
   void setWifiWarningValue(const QString &);
   void setMobileWarningValue(const QString &);


   void setMemTime_Vector(QVector<QString>);
   void setCpuTime_Vector(QVector<QString>);
   void setCpuTempTime_Vector(QVector<QString>);
   void setBatteryTempTime_Vector(QVector<QString>);
   void setWifiTime_Vector(QVector<QString>);
   void setMobileTime_Vector(QVector<QString>);


   void setMemory_Vector(QVector<double>);
   void setCpu_Vector(QVector<double>);
   void setCpuTemp_Vector(QVector<double>);
   void setBatteryTemp_Vector(QVector<double>);
   void setWifi_Vector(QVector<double>);
   void setMobile_Vector(QVector<double>);

signals:
    void sendWorkerResult(const QString &);

private slots:
    void receiveWorkerResult(const QString &);

private:
   QString qXlsx;
   QString qMemWarningValue;
   QString qCpuWarningValue;
   QString qCpuTempWarningValue;
   QString qBatteryTempWarningValue;
   QString qWifiWarningValue;
   QString qMobileWarningValue;

   QVector<QString> qMemTime_vector;
   QVector<QString> qCpuTime_vector;
   QVector<QString> qCpuTempTime_vector;
   QVector<QString> qBatteryTempTime_vector;
   QVector<QString> qWifiTime_vector;
   QVector<QString> qMobileTime_vector;

   QVector<double> qMemValue_vector;
   QVector<double> qCpuValue_vector;
   QVector<double> qCpuTempValue_vector;
   QVector<double> qBatteryTempValue_vector;
   QVector<double> qWifiValue_vector;
   QVector<double> qMobileValue_vector;

};

}

#endif // DATACONTROLLER_H
