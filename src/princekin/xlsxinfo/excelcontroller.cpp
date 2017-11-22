#include "excelcontroller.h"

namespace ExcelController
{

Controller::Controller()
{
}
Controller::~Controller()
{
    threadInstance.quit();
    threadInstance.wait();
}

//*******************通用********************//
void Controller::setMark(const QString &arg_text)
{
    qMark=arg_text;
}

void Controller::setXlsxSaveDir(const QString &arg_text)
{
    qXlsxSaveDir=arg_text;
}

void Controller::setStartTime(const QString &arg_text)
{
    qStartTime=arg_text;
}

void Controller::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void Controller::setLunchTime(const QString &arg_text)
{
    qLaunchTime=arg_text;
}

void Controller::setFrames(const QString &arg_text)
{
    qFrames=arg_text;
}

void Controller::setMemWarningValue(const QString &arg_text)
{
    qMemWarningValue=arg_text;
}

void Controller::setCpuWarningValue(const QString &arg_text)
{
    qCpuWarningValue=arg_text;
}

void Controller::setCpuTempWarningValue(const QString &arg_text)
{
    qCpuTempWarningValue=arg_text;
}

void Controller::setBatteryTempWarningValue(const QString &arg_text)
{
    qBatteryTempWarningValue=arg_text;
}

void Controller::setWifiWarningValue(const QString &arg_text)
{
    qWifiWarningValue=arg_text;
}

void Controller::setMobileWarningValue(const QString &arg_text)
{
    qMobileWarningValue=arg_text;
}
//*******************通用*******************//


//*******************遍历*******************//
void Controller::setTravelStartTime(const QString &arg_text)
{
    qTravelStartTime=arg_text;
}

void Controller::setTravelEndTime(const QString &arg_text)
{
    qTravelEndTime=arg_text;
}

void Controller::setTravelTime(const QString &arg_text)
{
    qTravelTime=arg_text;
}

void Controller::setCheckNumber(const QString &arg_text)
{
    qCheckNumber=arg_text;
}

void Controller::setClickNumber(const QString &arg_text)
{
    qClickNumber=arg_text;
}

void Controller::setErrorNumber(const QString &arg_text)
{
    qErrorNumber=arg_text;
}

void Controller::setTravelActivityNumber(const QString &arg_text)
{
    qTravelActivityNumber=arg_text;
}
//*******************遍历*******************//


//*******************统计*******************//
void Controller::setUrlTotalNum(int arg_num)
{
    qUrlTotalNum=arg_num;
}

void Controller::setUrlErrorNum(int arg_num)
{
    qUrlErrorNum=arg_num;
}

void Controller::setUrlSuccessNum(int arg_num)
{
    qUrlSuccessNum=arg_num;
}
//*******************统计*******************//

//*******************monkey*******************//
void Controller::setMonkeyTime(const QString &arg_text)
{
    qMonkeyTime=arg_text;
}
//*******************monkey*******************//


//******************************************//
void Controller::setPackageName(const QString &arg_text)
{
    qPackageName=arg_text;
}

void Controller::setStatiList(QStringList arg_list)
{
    qStatisticsDataList=arg_list;
}

void Controller::setMeasureVector(QVector<MeasureReport> arg_vector)
{
    qMeasureVector=arg_vector;
}
//******************************************//

void Controller::doController()
{
    ExcelWorker::Worker *worker = new ExcelWorker::Worker;
    worker->moveToThread(&threadInstance);

    if(qMark=="performance")
    {
        setPerformance(worker);
    }
    else if(qMark=="travel")
    {
        setTravel(worker);
    }
    else if(qMark=="base-travel")
    {
        setBaseTravel(worker);
    }
    else if(qMark=="behaviour")
    {
        setBehaviour(worker);
    }
    else if(qMark=="base-behaviour")
    {
        setBaseBehaviour(worker);
    }
    else if(qMark=="interface")
    {
        setInterface(worker);
    }
    else if(qMark=="base-interface")
    {
        setBaseInterface(worker);
    }
    else if(qMark=="monkey")
    {
        setMonkey(worker);
    }
    else if(qMark=="base-monkey")
    {
        setBaseMonkey(worker);
    }

    connect(&threadInstance,SIGNAL(started()),  worker, SLOT(doWork()), Qt::DirectConnection);
    connect(&threadInstance,SIGNAL(finished()), worker, SLOT(deleteLater()) , Qt::DirectConnection);
    connect(&threadInstance,SIGNAL(finished()), this,   SLOT(deleteLater()), Qt::DirectConnection);

    connect(worker,SIGNAL(sendWorkerFinished()), this, SLOT(receiveWorkerFinished()), Qt::DirectConnection);

    threadInstance.start();
}

void Controller::setPerformance(ExcelWorker::Worker *worker)
{
    worker->setMark("performance");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);
    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setPackageName(qPackageName);
    worker->setStatiList(qStatisticsDataList);
}


void Controller::setTravel(ExcelWorker::Worker *worker)
{
    worker->setMark("travel");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setTravelStartTime(qTravelStartTime);
    worker->setTravelEndTime(qTravelEndTime);
    worker->setTravelTime(qTravelTime);
    worker->setCheckNumber(qCheckNumber);
    worker->setClickNumber(qClickNumber);
    worker->setErrorNumber(qErrorNumber);
    worker->setTravelActivityNumber(qTravelActivityNumber);

    worker->setPackageName(qPackageName);
    worker->setStatiList(qStatisticsDataList);
    worker->setMeasureVector(qMeasureVector);
}


void Controller::setBaseTravel(ExcelWorker::Worker *worker)
{
    worker->setMark("base-travel");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setTravelStartTime(qTravelStartTime);
    worker->setTravelEndTime(qTravelEndTime);
    worker->setTravelTime(qTravelTime);
    worker->setCheckNumber(qCheckNumber);
    worker->setClickNumber(qClickNumber);
    worker->setErrorNumber(qErrorNumber);
    worker->setTravelActivityNumber(qTravelActivityNumber);

    worker->setPackageName(qPackageName);
    worker->setStatiList(qStatisticsDataList);
}


void Controller::setBehaviour(ExcelWorker::Worker *worker)
{
    worker->setMark("behaviour");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setUrlTotalNum(qUrlTotalNum);
    worker->setUrlErrorNum(qUrlErrorNum);
    worker->setUrlSuccessNum(qUrlTotalNum-qUrlErrorNum);

    worker->setPackageName(qPackageName);
    worker->setStatiList(qStatisticsDataList);
    worker->setMeasureVector(qMeasureVector);
}

void Controller::setBaseBehaviour(ExcelWorker::Worker *worker)
{
    worker->setMark("base-behaviour");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setUrlTotalNum(qUrlTotalNum);
    worker->setUrlErrorNum(qUrlErrorNum);
    worker->setUrlSuccessNum(qUrlTotalNum-qUrlErrorNum);

    worker->setPackageName(qPackageName);
    worker->setStatiList(qStatisticsDataList);
}

void Controller::setInterface(ExcelWorker::Worker *worker)
{
    worker->setMark("interface");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setPackageName(qPackageName);
    worker->setMeasureVector(qMeasureVector);
}

void Controller::setBaseInterface(ExcelWorker::Worker *worker)
{
    worker->setMark("base-interface");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setPackageName(qPackageName);
}

void Controller::setMonkey(ExcelWorker::Worker *worker)
{
    worker->setMark("monkey");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setMonkeyTime(qMonkeyTime);
    worker->setPackageName(qPackageName);
    worker->setStatiList(qStatisticsDataList);
    worker->setMeasureVector(qMeasureVector);
}

void Controller::setBaseMonkey(ExcelWorker::Worker *worker)
{
    worker->setMark("monkey");
    worker->setXlsxSaveDir(qXlsxSaveDir);
    worker->setStartTime(qStartTime);
    worker->setDeviceId(qDeviceId);
    worker->setLunchTime(qLaunchTime);
    worker->setFrames(qFrames);

    worker->setMemWarningValue(qMemWarningValue);
    worker->setCpuWarningValue(qCpuWarningValue);
    worker->setCpuTempWarningValue(qCpuTempWarningValue);
    worker->setBatteryTempWarningValue(qBatteryTempWarningValue);
    worker->setWifiWarningValue(qWifiWarningValue);
    worker->setMobileWarningValue(qMobileWarningValue);

    worker->setMonkeyTime(qMonkeyTime);
    worker->setPackageName(qPackageName);
    worker->setStatiList(qStatisticsDataList);
}

void Controller::receiveWorkerFinished()
{
    emit sendWorkerFinished();
}


}
