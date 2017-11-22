#ifndef REPLAYHTMLCONTROLLER_H
#define REPLAYHTMLCONTROLLER_H

#include <QString>
#include <QStringList>

#include <QObject>
#include <QRunnable>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include "globalvar.h"
#include <QTextCodec>
#include <QByteArray>
namespace ReplayHtmlController
{
class Controller : public QObject,public QRunnable
{
    Q_OBJECT
public:
    Controller();
protected:
    void run();

public:
    void setSaveDir(const QString&,const QString&);
    void setWarningValue(const QString&,const QString&,const QString&,const QString&,const QString&,const QString&);
    void setTime_vector(QVector<QString>,QVector<QString>,QVector<QString>,QVector<QString>,QVector<QString>,QVector<QString>);
    void setValue_vector(QVector<double>,QVector<double>,QVector<double>,QVector<double>,QVector<double>,QVector<double>);
    void createHtml();
    void createHtml2(QStringList);
signals:
    void sendControllerfinished(const QString&);
public:
    QString qHtmlSaveDir;
    QString qJsSaveDir;

    QVector<double> qMemValue_vector;
    QVector<double> qCpuValue_vector;
    QVector<double> qCpuTempValue_vector;
    QVector<double> qBatteryTempValue_vector;
    QVector<double> qWifiValue_vector;
    QVector<double> qMobileValue_vector;

    QVector<QString> qMemTime_vector;
    QVector<QString> qCpuTime_vector;
    QVector<QString> qCpuTempTime_vector;
    QVector<QString> qBatteryTempTime_vector;
    QVector<QString> qWifiTime_vector;
    QVector<QString> qMobileTime_vector;

    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qCpuTempWarningValue;
    QString qBatteryTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;

    QTextCodec *qTc=QTextCodec::codecForName("UTF-8");
};
}
#endif // REPLAYHTMLCONTROLLER_H
