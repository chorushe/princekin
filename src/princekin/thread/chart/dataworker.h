#ifndef DATAWORKER_H
#define DATAWORKER_H


#include "commonhead.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"
#include "xlsxworksheet.h"
#include <QtXlsx>
#include <QObject>

using namespace QXlsx;
QTXLSX_USE_NAMESPACE

namespace DataWorker
{
class Worker  : public QObject
{
    Q_OBJECT
public:
    Worker();

public:
    void startWorker();

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


private:
    Format getNormalFormat();

    void writeTime  (Document &,Format,const QString &,const QString &,QVector<QString>,int,int);
    void writeWarning(Document &,Format,const QString &,const QString &,const QString &,QVector<QString>,int,int);
    void writeData  (Document &,Format,const QString &,const QString &,QVector<double>,int,int);

signals:
    void sendWorkerResult(const QString &);

private:
    //*********************************************
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

    bool isTime;
    //*********************************************



    QByteArray qByte;
    QString  qDataLine;
    QProcess *qProc;
    //QTextCodec *qTc=QTextCodec::codecForName("gb2312");
    QTextCodec *qTc=QTextCodec::codecForName("UTF-8");
};
}

#endif // DATAWORKER_H
