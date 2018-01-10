#include "performancedetail.h"
#include "ui_performancedetail.h"

performanceDetail::performanceDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::performanceDetail)
{
    ui->setupUi(this);

    ui->memTextEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                            "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->cpuTextEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                        "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");

    ui->cpuAveLabel->setText("");
    ui->cpuMaxLabel->setText("");
    ui->cpuMinLabel->setText("");
    ui->memAveLabel->setText("");
    ui->memMaxLabel->setText("");
    ui->memMinLabel->setText("");
}

performanceDetail::~performanceDetail()
{
    delete ui;
}

void performanceDetail::WriteEdit()
{
    ui->memTextEdit->clear();
    for(int i=0;i<memNum.length();i++)
    {
        ui->memTextEdit->append(QString::number( memNum.at(i))+"     "+memTime.at(i));
    }

    ui->cpuTextEdit->clear();
    for(int i=0;i<cpuNum.length();i++)
    {
        ui->cpuTextEdit->append(QString::number( cpuNum.at(i))+"     "+cpuTime.at(i));
    }
}

void performanceDetail::RecieveMemResult(QString numStr, QString time,QString max,QString min,QString ave)
{
    if(this->isActiveWindow())
    {
        ui->memTextEdit->append(QString::number(numStr.toDouble()/1024,'f',1 )+"     "+time);
        ui->memAveLabel->setText(ave);
        ui->memMaxLabel->setText(max);
        ui->memMinLabel->setText(min);
    }
}

void performanceDetail::RecieveCpuResult(QString numStr, QString time,QString max,QString min,QString ave)
{
    if(this->isActiveWindow())
    {
        ui->cpuTextEdit->append(numStr+"     "+time);
        ui->cpuAveLabel->setText(ave);
        ui->cpuMaxLabel->setText(max);
        ui->cpuMinLabel->setText(min);
    }
}
