#include "trafficstatdetail.h"
#include "ui_trafficstatdetail.h"

TrafficStatDetail::TrafficStatDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrafficStatDetail)
{
    ui->setupUi(this);

    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                            "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->tableWidget_2->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                        "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
}

TrafficStatDetail::~TrafficStatDetail()
{
    delete ui;
}

void TrafficStatDetail::drawTable()
{
    QStringList splistResult;

    // ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    for(int i=0;i<wifiData.length();i++)
    {
        int row=ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        splistResult=wifiData.at(i).split("=");

        if(splistResult.size()==6)
        {
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(splistResult.at(0)));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(splistResult.at(1)));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(splistResult.at(2)));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(splistResult.at(3)));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(splistResult.at(4)));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(splistResult.at(5)));
        }
        ui->tableWidget->scrollToBottom();
    }

    ui->tableWidget_2->setRowCount(0);
    for(int i=0;i<mobileData.length();i++)
    {
        int row=ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(row);

        splistResult=mobileData.at(i).split("=");

        if(splistResult.size()==6)
        {
            ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(splistResult.at(0)));
            ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(splistResult.at(1)));
            ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(splistResult.at(2)));
            ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(splistResult.at(3)));
            ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(splistResult.at(4)));
            ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(splistResult.at(5)));
        }
        ui->tableWidget_2->scrollToBottom();
    }
}

void TrafficStatDetail::receiveTrafficResult_wifi1(const QString &arg_receiveAll,const QString &arg_upAll )
{
    /*ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(arg_receiveAll));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(arg_upAll));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("0.00"));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem("0.00"));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem("0.00"));
    ui->tableWidget->setItem(0, 5, new QTableWidgetItem("0.00"));*/
    QString temp=arg_receiveAll+"="+arg_upAll+"="+"0.00=0.00=0.00=0.00";
    wifiData.append(temp);
}

void TrafficStatDetail::receiveTrafficResult_wifi2(const QString &arg_,const QString &arg_all )
{
    QStringList splistResult;

    wifiData.append(arg_all);

    if(this->isActiveWindow())
    {
        int row=ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        splistResult=arg_all.split("=");

        if(splistResult.size()==6)
        {
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(splistResult.at(0)));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(splistResult.at(1)));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(splistResult.at(2)));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(splistResult.at(3)));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(splistResult.at(4)));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(splistResult.at(5)));
        }
        ui->tableWidget->scrollToBottom();
    }
}

void TrafficStatDetail::receiveTrafficResult_mobile1(const QString &arg_receiveAll,const QString &arg_upAll )
{
    /* ui->tableWidget_2->insertRow(0);
    ui->tableWidget_2->setItem(0, 0, new QTableWidgetItem(arg_receiveAll));
    ui->tableWidget_2->setItem(0, 1, new QTableWidgetItem(arg_upAll));
    ui->tableWidget_2->setItem(0, 2, new QTableWidgetItem("0.00"));
    ui->tableWidget_2->setItem(0, 3, new QTableWidgetItem("0.00"));
    ui->tableWidget_2->setItem(0, 4, new QTableWidgetItem("0.00"));
    ui->tableWidget_2->setItem(0, 5, new QTableWidgetItem("0.00"));*/
    QString temp=arg_receiveAll+"="+arg_upAll+"="+"0.00=0.00=0.00=0.00";
    mobileData.append(temp);
}


void TrafficStatDetail::receiveTrafficResult_mobile2(const QString &arg_,const QString &arg_all )
{
    QStringList splistResult;

    mobileData.append(arg_all);
    if(this->isActiveWindow())
    {

        int row=ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(row);

        splistResult=arg_all.split("=");

        if(splistResult.size()==6)
        {
            ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(splistResult.at(0)));
            ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(splistResult.at(1)));
            ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(splistResult.at(2)));
            ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(splistResult.at(3)));
            ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(splistResult.at(4)));
            ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(splistResult.at(5)));
        }
        ui->tableWidget_2->scrollToBottom();
    }
}

