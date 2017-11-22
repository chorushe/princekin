#include "datacontroller.h"

namespace DataController
{

Controller::Controller()
{
}

void Controller::setXlsx(const QString &arg_text)
{
    qXlsx=arg_text;
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

void Controller::setMemTime_Vector(QVector<QString> arg_vector)
{
    qMemTime_vector=arg_vector;
}

void Controller::setCpuTime_Vector(QVector<QString> arg_vector)
{
    qCpuTime_vector=arg_vector;
}

void Controller::setCpuTempTime_Vector(QVector<QString> arg_vector)
{
    qCpuTempTime_vector=arg_vector;
}

void Controller::setBatteryTempTime_Vector(QVector<QString> arg_vector)
{
    qBatteryTempTime_vector=arg_vector;
}

void Controller::setWifiTime_Vector(QVector<QString> arg_vector)
{
    qWifiTime_vector=arg_vector;
}

void Controller::setMobileTime_Vector(QVector<QString> arg_vector)
{
    qMobileTime_vector=arg_vector;
}

void Controller::setMemory_Vector(QVector<double> arg_vector)
{
    qMemValue_vector=arg_vector;
}

void Controller::setCpu_Vector(QVector<double> arg_vector)
{
    qCpuValue_vector=arg_vector;
}

void Controller::setCpuTemp_Vector(QVector<double> arg_vector)
{
    qCpuTempValue_vector=arg_vector;
}

void Controller::setBatteryTemp_Vector(QVector<double> arg_vector)
{
    qBatteryTempValue_vector=arg_vector;
}

void Controller::setWifi_Vector(QVector<double> arg_vector)
{
    qWifiValue_vector=arg_vector;
}

void Controller::setMobile_Vector(QVector<double> arg_vector)
{
    qMobileValue_vector=arg_vector;
}





void Controller::run()
{
    using DataWorker::Worker;

    Worker *workerInstance=new Worker();
    connect(workerInstance,SIGNAL(sendWorkerResult(const QString &)),this,SLOT(receiveWorkerResult(const QString &))  ,Qt::DirectConnection  );


    workerInstance->setXlsx(qXlsx);

    workerInstance->setMemWarningValue(qMemWarningValue);
    workerInstance->setCpuWarningValue(qCpuWarningValue);
    workerInstance->setCpuTempWarningValue(qCpuTempWarningValue);
    workerInstance->setBatteryTempWarningValue(qBatteryTempWarningValue);
    workerInstance->setWifiWarningValue(qWifiWarningValue);
    workerInstance->setMobileWarningValue(qMobileWarningValue);

    workerInstance->setMemTime_Vector(qMemTime_vector);
    workerInstance->setCpuTime_Vector(qCpuTime_vector);
    workerInstance->setCpuTempTime_Vector(qCpuTempTime_vector);
    workerInstance->setBatteryTempTime_Vector(qBatteryTempTime_vector);
    workerInstance->setWifiTime_Vector(qWifiTime_vector);
    workerInstance->setMobileTime_Vector(qMobileTime_vector);

    workerInstance->setMemory_Vector(qMemValue_vector);
    workerInstance->setCpu_Vector(qCpuValue_vector);
    workerInstance->setCpuTemp_Vector(qCpuTempValue_vector);
    workerInstance->setBatteryTemp_Vector(qBatteryTempValue_vector);
    workerInstance->setWifi_Vector(qWifiValue_vector);
    workerInstance->setMobile_Vector(qMobileValue_vector);

    workerInstance->startWorker();
}

void Controller::receiveWorkerResult(const QString &arg_workerResult)
{
    emit sendWorkerResult(arg_workerResult);
}

}
