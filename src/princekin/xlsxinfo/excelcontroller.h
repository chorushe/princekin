#ifndef EXCELCONTROLLER_H
#define EXCELCONTROLLER_H

#include <QObject>
#include "excelworker.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QObject>
#include <QRegExp>
#include <QMessageBox>

#include "globalvar.h"
#include "measurereport.h"
#include "xlsxinfo.h"
#include "writeinfo.h"
#include "chartcontroller.h"
#include "datacontroller.h"
#include "appsize.h"
#include "htmlinfo.h"
#include "string.h"

using namespace ExcelWorker;

namespace ExcelController
{
class Controller : public QObject
{
    Q_OBJECT
public:    
    Controller();
    ~Controller();

public:
    void doController();

    //*******************通用*******************//
    void setMark(const QString &);
    void setXlsxSaveDir(const QString &);
    void setStartTime(const QString &);
    void setDeviceId(const QString &);
    void setFrames(const QString&);
    void setLunchTime(const QString&);
    void setMemWarningValue(const QString &);
    void setCpuWarningValue(const QString &);
    void setCpuTempWarningValue(const QString &);
    void setBatteryTempWarningValue(const QString &);
    void setWifiWarningValue(const QString &);
    void setMobileWarningValue(const QString &);
    //*******************通用*******************//

    //*******************遍历*******************//
    void setTravelStartTime(const QString &);
    void setTravelEndTime(const QString &);
    void setTravelTime(const QString &);
    void setCheckNumber(const QString &);
    void setClickNumber(const QString &);
    void setErrorNumber(const QString &);
    void setTravelActivityNumber(const QString &);
    //*******************遍历*******************//


    //*******************统计*******************//
    void setUrlTotalNum(int);
    void setUrlErrorNum(int);
    void setUrlSuccessNum(int);
    //*******************统计*******************//



    //*******************monkey*******************//
    void setMonkeyTime(const QString &);
    //*******************monkey*******************//


    void setPackageName(const QString &);
    void setMeasureVector(QVector<MeasureReport>);
    void setStatiList(QStringList);



    void getAppInfo(const QString &);
    void getAppInfo1(const QString &);
    void getAppInfo2(const QString &);

    void setAppInfo(const QString &);
    void setAppInfo_travel();
    void setAppInfo_universal();
    void setAppInfo_behaviour();
    void setAppInfo_monkey();

    void writeInfo(const QString &);
    void getValues(const QString &);
    void chart();
    void chart2();
    void createExcel(const QString &);


    //*****************20170505*************************//
    void getXXX();
    void getAppSize();


    void setAppInfo_universal_behaviour();
    void sendMail();


    void performanceHtml();
    void createBaseExcel(const QString &);
    void setPerformance(ExcelWorker::Worker *worker);
    void setTravel(ExcelWorker::Worker *worker);
    void setBaseTravel(ExcelWorker::Worker *worker);
    void setBehaviour(ExcelWorker::Worker *worker);
    void setBaseBehaviour(ExcelWorker::Worker *worker);
    void setInterface(ExcelWorker::Worker *worker);
    void setBaseInterface(ExcelWorker::Worker *worker);
    void setMonkey(ExcelWorker::Worker *worker);
    void setBaseMonkey(ExcelWorker::Worker *worker);

signals:
    void sendWorkerFinished();

public slots:
    void receiveWorkerFinished();




private:
    //****************************//
    QString qXlsxSaveDir;
    QString qStartTime;
    QString qTargetXlsx;
    QString qDeviceId;
    //****************************//

    QString qPackageName;

    //****************************//
    QString qAppChineseName;
    QString qAppVersion;
    QString qAppSize;
    QString qIconResPath;
    QString qIconPath;
    QString qLaunchTime;
    QString qPlatform;
    QString qTestDate;
    //****************************//

    //****************************//
    QString qMobileBrand;
    QString qMobileModel;
    QString qMobileVersion;
    QString qMobileVmsize;
    //****************************//

    //****************************//
    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qCpuTempWarningValue;
    QString qBatteryTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;
    //****************************//

    //****************************//
    QString qTravelStartTime;
    QString qTravelEndTime;
    QString qTravelTime;
    QString qCheckNumber;
    QString qClickNumber;
    QString qErrorNumber;
    QString qTravelActivityNumber;
    //****************************//

    //****************************//
    QString qMonkeyTime;
    //****************************//

    //****************************//
    int qUrlTotalNum;
    int qUrlErrorNum;
    int qUrlSuccessNum;
    //****************************//

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

    QVector<MeasureReport> qMeasureVector;

    //*****************20170505*************************//
    QString qAppSize2;
    QString qFrames;
    QStringList qStatisticsDataList;
    //*****************20170505*************************//

    //*****************20170606*************************//
    QStringList qMemInfoList;
    QStringList qCpuInfoList;
    QStringList qBatteryInfoList;
    QStringList qCpuTempInfoList;
    double qAllWifi;
    double qAllMobile;
    QString qMark;
    //*****************20170606*************************//
    QThread threadInstance;
};
}


#endif // EXCELCONTROLLER_H


