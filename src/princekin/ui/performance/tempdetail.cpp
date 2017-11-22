#include "tempdetail.h"
#include "ui_tempdetail.h"

tempDetail::tempDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tempDetail)
{
    ui->setupUi(this);

    ui->batteryTextEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                            "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->cpuTempTextEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                        "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
}

tempDetail::~tempDetail()
{
    delete ui;
}

void tempDetail::WriteEdit()
{
    ui->batteryTextEdit->clear();
    for(int i=0;i<batteryNum.length();i++)
    {
        ui->batteryTextEdit->append(QString::number( batteryNum.at(i))+"     "+batteryTime.at(i));
    }

    ui->cpuTempTextEdit->clear();
    for(int i=0;i<cpuTempNum.length();i++)
    {
        ui->cpuTempTextEdit->append(QString::number( cpuTempNum.at(i))+"     "+cpuTempTime.at(i));
    }
}

void tempDetail::RecieveBatteryResult(QString numStr, QString time)
{
    if(this->isActiveWindow())
    {
        ui->batteryTextEdit->append(QString::number( numStr.toDouble()/10 )+"     "+time);
    }
}

void tempDetail::RecieveCpuTempResult(QString numStr, QString time)
{
    if(this->isActiveWindow())
    {
        ui->cpuTempTextEdit->append(numStr+"     "+time);
    }
}
