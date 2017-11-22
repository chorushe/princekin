#include "chartdetail.h"
#include "ui_chartdetail.h"

ChartDetail::ChartDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartDetail)
{
    ui->setupUi(this);




    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(30);
    ui->customPlot->rescaleAxes();
    //设置坐标轴标签名称
    ui->customPlot->xAxis->setLabel("时  间");
}

void ChartDetail::setFlag(QString flag)
{
    chartFlag=flag;
    qDebug()<<chartFlag;
}

void ChartDetail::startTimer()
{
    ui->customPlot->clearGraphs();
    for(int i=0;i<2;i++)
    {
        ui->customPlot->addGraph();
    }
    ui->customPlot->graph(0)->setPen(QPen(QColor("#c1904d"),2));
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));

    ui->customPlot->graph(0)->setName("数值");
    ui->customPlot->graph(1)->setName("阈值");
    ui->customPlot->legend->setVisible(true);

    if(chartFlag.compare("mem")==0)
        ui->titleLabel->setText("内存(M)");
    if(chartFlag.compare("cpu")==0)
        ui->titleLabel->setText("CPU(%)");
    if(chartFlag.compare("battery")==0)
        ui->titleLabel->setText("电池温度(℃)");
    if(chartFlag.compare("cputemp")==0)
        ui->titleLabel->setText("CPU温度(℃)");
    if(chartFlag.compare("wifi")==0)
        ui->titleLabel->setText("Wifi流量(M)");
    if(chartFlag.compare("mobile")==0)
        ui->titleLabel->setText("Mobile流量(M)");

    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(StartDrawChart()));
    timer->start(2000);


}

void ChartDetail::StartDrawChart()
{
    if(chartFlag.compare("mem")==0)
    {
        int min,max;
        if( memMin > strMemThres.toDouble())
            min=strMemThres.toDouble();
        else
            min=memMin;
        if(memMax< strMemThres.toDouble())
            max=strMemThres.toDouble();
        else
            max=memMax;


        ui->customPlot->yAxis->setRange(min*0.9,max*1.25);
        if(memTimeInt.count()>0)
        {
            ui->customPlot->xAxis->setRange(memTimeInt[0],memTimeInt[memTimeInt.length()-1]+60);
            ui->customPlot->graph(0)->setData(memTimeInt,memNum);
            ui->customPlot->graph(1)->setData(memTimeInt,memThres);

            double internal=memTimeInt[memTimeInt.length()-1]-memTimeInt[0];
            if(internal>60*1.5)
            {
                int num=(int)(internal/4)/30;
                internal=(num+1)*30;
                ui->customPlot->xAxis->setTickStep(internal); // 一天的秒数
            }
        }

    }
    else if(chartFlag.compare("cpu")==0)
    {
        int min,max;
        if( cpuMin > strCpuThres.toDouble())
            min=strCpuThres.toDouble();
        else
            min=cpuMin;
        if(cpuMax< strCpuThres.toDouble())
            max=strCpuThres.toDouble();
        else
            max=cpuMax;


        ui->customPlot->yAxis->setRange(min*0.9,max*1.25);
        if(cpuTimeInt.count()>0)
        {
            ui->customPlot->xAxis->setRange(cpuTimeInt[0],cpuTimeInt[cpuTimeInt.length()-1]+60);
            ui->customPlot->graph(0)->setData(cpuTimeInt,cpuNum);
            ui->customPlot->graph(1)->setData(cpuTimeInt,cpuThres);

            double internal=cpuTimeInt[cpuTimeInt.length()-1]-memTimeInt[0];
            if(internal>60*1.5)
            {
                int num=(int)(internal/4)/30;
                internal=(num+1)*30;
                ui->customPlot->xAxis->setTickStep(internal); // 一天的秒数
            }
        }
    }
    else if(chartFlag.compare("battery")==0)
    {
        int min,max;
        if( batteryMin > strBatteryThres.toDouble())
            min=strBatteryThres.toDouble();
        else
            min=batteryMin;
        if(batteryMax< strBatteryThres.toDouble())
            max=strBatteryThres.toDouble();
        else
            max=batteryMax;


        ui->customPlot->yAxis->setRange(min*0.9,max*1.25);
        if(batteryTimeInt.count()>0)
        {
            ui->customPlot->xAxis->setRange(batteryTimeInt[0],batteryTimeInt[batteryTimeInt.length()-1]+60);
            ui->customPlot->graph(0)->setData(batteryTimeInt,batteryNum);
            ui->customPlot->graph(1)->setData(batteryTimeInt,batteryThres);

            double internal=batteryTimeInt[batteryTimeInt.length()-1]-batteryTimeInt[0];
            if(internal>60*1.5)
            {
                int num=(int)(internal/4)/30;
                internal=(num+1)*30;
                ui->customPlot->xAxis->setTickStep(internal); // 一天的秒数
            }
        }
    }
    else if(chartFlag.compare("cputemp")==0)
    {
        int min,max;
        if( cpuTempMin > strCpuTempThres.toDouble())
            min=strCpuTempThres.toDouble();
        else
            min=cpuTempMin;
        if(cpuTempMax< strCpuTempThres.toDouble())
            max=strCpuTempThres.toDouble();
        else
            max=cpuTempMax;


        ui->customPlot->yAxis->setRange(min*0.9,max*1.25);
        if(cpuTempTimeInt.count()>0)
        {
            ui->customPlot->xAxis->setRange(cpuTempTimeInt[0],cpuTempTimeInt[cpuTempTimeInt.length()-1]+60);
            ui->customPlot->graph(0)->setData(cpuTempTimeInt,cpuTempNum);
            ui->customPlot->graph(1)->setData(cpuTempTimeInt,cpuTempThres);

            double internal=cpuTempTimeInt[cpuTempTimeInt.length()-1]-cpuTempTimeInt[0];
            if(internal>60*1.5)
            {
                int num=(int)(internal/4)/30;
                internal=(num+1)*30;
                ui->customPlot->xAxis->setTickStep(internal); // 一天的秒数
            }
        }
    }
    else if(chartFlag.compare("wifi")==0)
    {
        int min,max;
        if( wifiMin > strWifiThres.toDouble())
            min=strWifiThres.toDouble();
        else
            min=wifiMin;
        if(wifiMax< strWifiThres.toDouble())
            max=strWifiThres.toDouble();
        else
            max=wifiMax;


        ui->customPlot->yAxis->setRange(min*0.9,max*1.25);
        if(wifiTimeInt.count()>0)
        {
            ui->customPlot->xAxis->setRange(wifiTimeInt[0],wifiTimeInt[wifiTimeInt.length()-1]+60);
            ui->customPlot->graph(0)->setData(wifiTimeInt,wifiNum);
            ui->customPlot->graph(1)->setData(wifiTimeInt,wifiThres);

            double internal=wifiTimeInt[wifiTimeInt.length()-1]-wifiTimeInt[0];
            if(internal>60*1.5)
            {
                int num=(int)(internal/4)/30;
                internal=(num+1)*30;
                ui->customPlot->xAxis->setTickStep(internal); // 一天的秒数
            }
        }
    }
    else if(chartFlag.compare("mobile")==0)
    {
        int min,max;
        if( mobileMin > strMobileThres.toDouble())
            min=strMobileThres.toDouble();
        else
            min=mobileMin;
        if(mobileMax< strMobileThres.toDouble())
            max=strMobileThres.toDouble();
        else
            max=mobileMax;


        ui->customPlot->yAxis->setRange(min*0.9,max*1.25);
        if(mobileTimeInt.count()>0)
        {
            ui->customPlot->xAxis->setRange(mobileTimeInt[0],mobileTimeInt[mobileTimeInt.length()-1]+60);
            ui->customPlot->graph(0)->setData(mobileTimeInt,mobileNum);
            ui->customPlot->graph(1)->setData(mobileTimeInt,mobileThres);

            double internal=mobileTimeInt[mobileTimeInt.length()-1]-mobileTimeInt[0];
            if(internal>60*1.5)
            {
                int num=(int)(internal/4)/30;
                internal=(num+1)*30;
                ui->customPlot->xAxis->setTickStep(internal); // 一天的秒数
            }
        }
    }
    ui->customPlot->replot();
}

void ChartDetail::RecieveThresMax(QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres)
{
    this->strMemThres=memThres;
    this->strCpuThres=cpuThres;
    this->strBatteryThres=batteryThres;
    this->strCpuTempThres=cpuTempThres;
    this->strWifiThres=wifiThres;
    this->strMobileThres=mobileThres;
}

void ChartDetail::closeEvent()
{
    if(timer->isActive())
        timer->stop();
}

ChartDetail::~ChartDetail()
{
    delete ui;
}

void ChartDetail::RecieveCompareData(QVector<QVector<QString>> qMemory_data_Vector,QVector<QVector<QString>> qMemory_time_Vector,
                                     QVector<QVector<QString>> qCpu_data_Vector,QVector<QVector<QString>> qCpu_time_Vector,
                                     QVector<QVector<QString>> qCputemp_data_Vector,QVector<QVector<QString>> qCputemp_time_Vector,
                                     QVector<QVector<QString>> qBatterytemp_data_Vector,QVector<QVector<QString>> qBatterytemp_time_Vector,
                                     QVector<QVector<QString>> qWifi_data_Vector,QVector<QVector<QString>> qWifi_time_Vector,
                                     QVector<QVector<QString>> qMobile_data_Vector,QVector<QVector<QString>> qMobile_time_Vector)
{
    this->qMemory_data_Vector=qMemory_data_Vector;
    this->qMemory_time_Vector=qMemory_time_Vector;
    this->qCpu_data_Vector=qCpu_data_Vector;
    this->qCpu_time_Vector=qCpu_time_Vector;
    this->qBatterytemp_data_Vector=qBatterytemp_data_Vector;
    this->qBatterytemp_time_Vector=qBatterytemp_time_Vector;
    this->qCputemp_data_Vector=qCputemp_data_Vector;
    this->qCputemp_time_Vector=qCputemp_time_Vector;
    this->qMobile_data_Vector=qMobile_data_Vector;
    this->qMobile_time_Vector=qMobile_time_Vector;
    this->qWifi_data_Vector=qWifi_data_Vector;
    this->qWifi_time_Vector=qWifi_time_Vector;
}

void ChartDetail::plotChart()
{
    ui->customPlot->clearGraphs();

    QStringList colorList;
    colorList.append("#c1904d");
    colorList.append("#9a5acc");
    colorList.append("#ee4f71");
    colorList.append("#7fda85");
    colorList.append("#25a4d6");
    colorList.append("#98d7ed");
    colorList.append("#fadf97");


   // ui->customPlot->graph(0)->setName("数值");
   // ui->customPlot->graph(1)->setName("阈值");
    for(int i=0;i<gExcelFilePath.length();i++)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(i)->setPen(QPen(QColor(colorList.at(i)),1));
        QFileInfo  info;
        info = QFileInfo(gExcelFilePath.at(i));
        ui->customPlot->graph(i)->setName(info.fileName());
    }

    ui->customPlot->legend->setVisible(true);
    if(chartFlag.compare("mem")==0)
        ui->titleLabel->setText("内存(M)");
    if(chartFlag.compare("cpu")==0)
        ui->titleLabel->setText("CPU(%)");
    if(chartFlag.compare("battery")==0)
        ui->titleLabel->setText("电池温度(℃)");
    if(chartFlag.compare("cputemp")==0)
        ui->titleLabel->setText("CPU温度(℃)");
    if(chartFlag.compare("wifi")==0)
        ui->titleLabel->setText("Wifi流量(M)");
    if(chartFlag.compare("mobile")==0)
        ui->titleLabel->setText("Mobile流量(M)");

    //初始化
    double mem_min=0,cpu_min=0,battery_min=0,cputemp_min=0,wifi_min=0,mobile_min=0;
    double mem_max=0,cpu_max=0,battery_max=0,cputemp_max=0,wifi_max=0,mobile_max=0;
    double mem_time_max=0,cpu_time_max=0,battery_time_max=0,cputemp_time_max=0,wifi_time_max=0,mobile_time_max=0;
    if(qMemory_data_Vector.count()>0)
    {
        if(qMemory_data_Vector.at(0).count()>0)
        {
            mem_min=qMemory_data_Vector.at(0).at(0).toDouble();
            mem_max=qMemory_data_Vector.at(0).at(0).toDouble();
        }
    }
    if(qCpu_data_Vector.count()>0)
    {
        if(qCpu_data_Vector.at(0).count()>0)
        {
            cpu_min=qCpu_data_Vector.at(0).at(0).toDouble();
            cpu_max=qCpu_data_Vector.at(0).at(0).toDouble();
        }
    }
    if(qBatterytemp_data_Vector.count()>0)
    {
        if(qBatterytemp_data_Vector.at(0).count()>0)
        {
            battery_min=qBatterytemp_data_Vector.at(0).at(0).toDouble();
            battery_max=qBatterytemp_data_Vector.at(0).at(0).toDouble();
        }
    }
    if(qCputemp_data_Vector.count()>0)
    {
        if(qCputemp_data_Vector.at(0).count()>0)
        {
            cputemp_min=qCputemp_data_Vector.at(0).at(0).toDouble();
            cputemp_max=qCputemp_data_Vector.at(0).at(0).toDouble();
        }
    }
    if(qWifi_data_Vector.count()>0)
    {
        if(qWifi_data_Vector.at(0).count()>0)
        {
            wifi_min=qWifi_data_Vector.at(0).at(0).toDouble();
            wifi_max=qWifi_data_Vector.at(0).at(0).toDouble();
        }
    }
    if(qMobile_data_Vector.count()>0)
    {
        if(qMobile_data_Vector.at(0).count()>0)
        {
            mobile_min=qMobile_data_Vector.at(0).at(0).toDouble();
            mobile_max=qMobile_data_Vector.at(0).at(0).toDouble();
        }
    }

    //清除原来数据
    for(int i=0;i<ui->customPlot->graphCount();i++)
    {
        ui->customPlot->graph(i)->clearData();
    }
    ui->customPlot->replot();

    if(chartFlag.compare("mem")==0)
    {
        //内存数据整理
        for(int i=0;i<qMemory_data_Vector.count();i++)
        {
            //计算time int 差值
            QVector<double> Memory_time_int_Vector;
            QVector<double> Memory_data_Vector;
            int internal=0;
            QString temp="2017-03-28 ";
            QString temp0="2017-03-28 00:00:00";
            if(qMemory_time_Vector.at(i).count()>0)
            {
                QString firstOne = temp+qMemory_time_Vector.at(i).at(0);
                qDebug()<<"firstone  "<<firstOne;
                internal=QDateTime::fromString(firstOne,"yyyy-MM-dd hh:mm:ss").toTime_t()-QDateTime::fromString(temp0,"yyyy-MM-dd hh:mm:ss").toTime_t();
            }

            for(int j=0;j<qMemory_time_Vector.at(i).count();j++)
            {
                Memory_time_int_Vector.append(QDateTime::fromString(temp+qMemory_time_Vector.at(i).at(j),"yyyy-MM-dd hh:mm:ss").toTime_t()-internal);
                Memory_data_Vector.append(qMemory_data_Vector.at(i).at(j).toDouble());
                //计算数据轴的最大最小值
                if(mem_min>qMemory_data_Vector.at(i).at(j).toDouble())
                    mem_min=qMemory_data_Vector.at(i).at(j).toDouble();
                if(mem_max<qMemory_data_Vector.at(i).at(j).toDouble())
                    mem_max=qMemory_data_Vector.at(i).at(j).toDouble();
            }
            //设置轴范围，如不设置，不显示图
            if(Memory_time_int_Vector.length()>0)
            {
                if(mem_time_max<Memory_time_int_Vector[Memory_time_int_Vector.length()-1])
                    mem_time_max=Memory_time_int_Vector[Memory_time_int_Vector.length()-1];
                ui->customPlot->yAxis->setRange(mem_min*0.9,mem_max*1.25);
                ui->customPlot->xAxis->setRange(Memory_time_int_Vector[0],mem_time_max+60);
                //设置时间轴的间隔显示，大概四五个时间点
                double internalForTime=mem_time_max-Memory_time_int_Vector[0];
                if(internalForTime>60*1.5)
                {
                    int num=(int)(internalForTime/4)/30;
                    internalForTime=(num+1)*30;
                    ui->customPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
                }

                ui->customPlot->graph(i)->setData(Memory_time_int_Vector,Memory_data_Vector);
                ui->customPlot->replot();
            }
        }
    }
    else if(chartFlag.compare("cpu")==0)
    {
        //cpu数据整理
        for(int i=0;i<qCpu_data_Vector.count();i++)
        {
            //计算time int 差值
            QVector<double> Cpu_time_int_Vector;
            QVector<double> Cpu_data_Vector;
            int internal=0;
            QString temp="2017-03-28 ";
            QString temp0="2017-03-28 00:00:00";
            if(qCpu_time_Vector.at(i).count()>0)
            {
                QString firstOne = temp+qCpu_time_Vector.at(i).at(0);
                qDebug()<<"firstone  "<<firstOne;
                internal=QDateTime::fromString(firstOne,"yyyy-MM-dd hh:mm:ss").toTime_t()-QDateTime::fromString(temp0,"yyyy-MM-dd hh:mm:ss").toTime_t();
            }

            for(int j=0;j<qCpu_time_Vector.at(i).count();j++)
            {
                Cpu_time_int_Vector.append(QDateTime::fromString(temp+qCpu_time_Vector.at(i).at(j),"yyyy-MM-dd hh:mm:ss").toTime_t()-internal);
                Cpu_data_Vector.append(qCpu_data_Vector.at(i).at(j).toDouble());
                //计算数据轴的最大最小值
                if(cpu_min>qCpu_data_Vector.at(i).at(j).toDouble())
                    cpu_min=qCpu_data_Vector.at(i).at(j).toDouble();
                if(cpu_max<qCpu_data_Vector.at(i).at(j).toDouble())
                    cpu_max=qCpu_data_Vector.at(i).at(j).toDouble();
            }
            //设置轴范围，如不设置，不显示图
            if(Cpu_time_int_Vector.length()>0)
            {
                if(cpu_time_max<Cpu_time_int_Vector[Cpu_time_int_Vector.length()-1])
                    cpu_time_max=Cpu_time_int_Vector[Cpu_time_int_Vector.length()-1];
                ui->customPlot->yAxis->setRange(cpu_min*0.9,cpu_max*1.25);
                ui->customPlot->xAxis->setRange(Cpu_time_int_Vector[0],cpu_time_max+60);
                //设置时间轴的间隔显示，大概四五个时间点
                double internalForTime=cpu_time_max-Cpu_time_int_Vector[0];
                if(internalForTime>60*1.5)
                {
                    int num=(int)(internalForTime/4)/30;
                    internalForTime=(num+1)*30;
                    ui->customPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
                }
                ui->customPlot->graph(i)->setData(Cpu_time_int_Vector,Cpu_data_Vector);
                ui->customPlot->replot();
            }
        }
    }
    else if(chartFlag.compare("battery")==0)
    {
        //battery数据整理
        for(int i=0;i<qBatterytemp_data_Vector.count();i++)
        {
            //计算time int 差值
            QVector<double> Batterytemp_time_int_Vector;
            QVector<double> Batterytemp_data_Vector;
            int internal=0;
            QString temp="2017-03-28 ";
            QString temp0="2017-03-28 00:00:00";
            if(qBatterytemp_time_Vector.at(i).count()>0)
            {
                QString firstOne = temp+qBatterytemp_time_Vector.at(i).at(0);
                qDebug()<<"firstone  "<<firstOne;
                internal=QDateTime::fromString(firstOne,"yyyy-MM-dd hh:mm:ss").toTime_t()-QDateTime::fromString(temp0,"yyyy-MM-dd hh:mm:ss").toTime_t();
            }

            for(int j=0;j<qBatterytemp_time_Vector.at(i).count();j++)
            {
                Batterytemp_time_int_Vector.append(QDateTime::fromString(temp+qBatterytemp_time_Vector.at(i).at(j),"yyyy-MM-dd hh:mm:ss").toTime_t()-internal);
                Batterytemp_data_Vector.append(qBatterytemp_data_Vector.at(i).at(j).toDouble());
                //计算数据轴的最大最小值
                if(battery_min>qBatterytemp_data_Vector.at(i).at(j).toDouble())
                    battery_min=qBatterytemp_data_Vector.at(i).at(j).toDouble();
                if(battery_max<qBatterytemp_data_Vector.at(i).at(j).toDouble())
                    battery_max=qBatterytemp_data_Vector.at(i).at(j).toDouble();
            }
            //设置轴范围，如不设置，不显示图
            if(Batterytemp_time_int_Vector.length()>0)
            {
                if(battery_time_max<Batterytemp_time_int_Vector[Batterytemp_time_int_Vector.length()-1])
                    battery_time_max=Batterytemp_time_int_Vector[Batterytemp_time_int_Vector.length()-1];
                ui->customPlot->yAxis->setRange(battery_min*0.9,battery_max*1.25);
                ui->customPlot->xAxis->setRange(Batterytemp_time_int_Vector[0],battery_time_max+60);
                //设置时间轴的间隔显示，大概四五个时间点
                double internalForTime=battery_time_max-Batterytemp_time_int_Vector[0];
                if(internalForTime>60*1.5)
                {
                    int num=(int)(internalForTime/4)/30;
                    internalForTime=(num+1)*30;
                    ui->customPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
                }
                ui->customPlot->graph(i)->setData(Batterytemp_time_int_Vector,Batterytemp_data_Vector);
                ui->customPlot->replot();
            }
        }
    }
    else if(chartFlag.compare("cputemp")==0)
    {
        for(int i=0;i<qCputemp_data_Vector.count();i++)
        {
            //计算time int 差值
            QVector<double> Cputemp_time_int_Vector;
            QVector<double> Cputemp_data_Vector;
            int internal=0;
            QString temp="2017-03-28 ";
            QString temp0="2017-03-28 00:00:00";
            if(qCputemp_time_Vector.at(i).count()>0)
            {
                QString firstOne = temp+qCputemp_time_Vector.at(i).at(0);
                qDebug()<<"firstone  "<<firstOne;
                internal=QDateTime::fromString(firstOne,"yyyy-MM-dd hh:mm:ss").toTime_t()-QDateTime::fromString(temp0,"yyyy-MM-dd hh:mm:ss").toTime_t();
            }

            for(int j=0;j<qCputemp_time_Vector.at(i).count();j++)
            {
                Cputemp_time_int_Vector.append(QDateTime::fromString(temp+qCputemp_time_Vector.at(i).at(j),"yyyy-MM-dd hh:mm:ss").toTime_t()-internal);
                Cputemp_data_Vector.append(qCputemp_data_Vector.at(i).at(j).toDouble());
                //计算数据轴的最大最小值
                if(cputemp_min>qCputemp_data_Vector.at(i).at(j).toDouble())
                    cputemp_min=qCputemp_data_Vector.at(i).at(j).toDouble();
                if(cputemp_max<qCputemp_data_Vector.at(i).at(j).toDouble())
                    cputemp_max=qCputemp_data_Vector.at(i).at(j).toDouble();
            }
            //设置轴范围，如不设置，不显示图
            if(Cputemp_time_int_Vector.length()>0)
            {
                if(cputemp_time_max<Cputemp_time_int_Vector[Cputemp_time_int_Vector.length()-1])
                    cputemp_time_max=Cputemp_time_int_Vector[Cputemp_time_int_Vector.length()-1];
                ui->customPlot->yAxis->setRange(cputemp_min*0.9,cputemp_max*1.25);
                ui->customPlot->xAxis->setRange(Cputemp_time_int_Vector[0],cputemp_time_max+60);
                //设置时间轴的间隔显示，大概四五个时间点
                double internalForTime=cputemp_time_max-Cputemp_time_int_Vector[0];
                if(internalForTime>60*1.5)
                {
                    int num=(int)(internalForTime/4)/30;
                    internalForTime=(num+1)*30;
                    ui->customPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
                }
                ui->customPlot->graph(i)->setData(Cputemp_time_int_Vector,Cputemp_data_Vector);
                ui->customPlot->replot();
            }
        }
    }
    else if(chartFlag.compare("wifi")==0)
    {
        for(int i=0;i<qWifi_data_Vector.count();i++)
        {
            //计算time int 差值
            QVector<double> Wifi_time_int_Vector;
            QVector<double> Wifi_data_Vector;
            int internal=0;
            QString temp="2017-03-28 ";
            QString temp0="2017-03-28 00:00:00";
            if(qWifi_time_Vector.at(i).count()>0)
            {
                QString firstOne = temp+qWifi_time_Vector.at(i).at(0);
                qDebug()<<"firstone  "<<firstOne;
                internal=QDateTime::fromString(firstOne,"yyyy-MM-dd hh:mm:ss").toTime_t()-QDateTime::fromString(temp0,"yyyy-MM-dd hh:mm:ss").toTime_t();
            }

            for(int j=0;j<qWifi_time_Vector.at(i).count();j++)
            {
                Wifi_time_int_Vector.append(QDateTime::fromString(temp+qWifi_time_Vector.at(i).at(j),"yyyy-MM-dd hh:mm:ss").toTime_t()-internal);
                Wifi_data_Vector.append(qWifi_data_Vector.at(i).at(j).toDouble());
                //计算数据轴的最大最小值
                if(wifi_min>qCputemp_data_Vector.at(i).at(j).toDouble())
                    wifi_min=qCputemp_data_Vector.at(i).at(j).toDouble();
                if(wifi_max<qCputemp_data_Vector.at(i).at(j).toDouble())
                    wifi_max=qCputemp_data_Vector.at(i).at(j).toDouble();
            }
            //设置轴范围，如不设置，不显示图
            if(Wifi_time_int_Vector.length()>0)
            {
                if(wifi_time_max<Wifi_time_int_Vector[Wifi_time_int_Vector.length()-1])
                    wifi_time_max=Wifi_time_int_Vector[Wifi_time_int_Vector.length()-1];
                ui->customPlot->yAxis->setRange(wifi_min*0.9,wifi_max*1.25);
                ui->customPlot->xAxis->setRange(Wifi_time_int_Vector[0],wifi_time_max+60);
                //设置时间轴的间隔显示，大概四五个时间点
                double internalForTime=wifi_time_max-Wifi_time_int_Vector[0];
                if(internalForTime>60*1.5)
                {
                    int num=(int)(internalForTime/4)/30;
                    internalForTime=(num+1)*30;
                    ui->customPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
                }
                ui->customPlot->graph(i)->setData(Wifi_time_int_Vector,Wifi_data_Vector);
                ui->customPlot->replot();
            }
        }
    }
    else if(chartFlag.compare("mobile")==0)
    {
        for(int i=0;i<qMobile_data_Vector.count();i++)
        {
            //计算time int 差值
            QVector<double> Mobile_time_int_Vector;
            QVector<double> Mobile_data_Vector;
            int internal=0;
            QString temp="2017-03-28 ";
            QString temp0="2017-03-28 00:00:00";
            if(qMobile_time_Vector.at(i).count()>0)
            {
                QString firstOne = temp+qMobile_time_Vector.at(i).at(0);
                qDebug()<<"firstone  "<<firstOne;
                internal=QDateTime::fromString(firstOne,"yyyy-MM-dd hh:mm:ss").toTime_t()-QDateTime::fromString(temp0,"yyyy-MM-dd hh:mm:ss").toTime_t();
            }

            for(int j=0;j<qMobile_time_Vector.at(i).count();j++)
            {
                Mobile_time_int_Vector.append(QDateTime::fromString(temp+qMobile_time_Vector.at(i).at(j),"yyyy-MM-dd hh:mm:ss").toTime_t()-internal);
                Mobile_data_Vector.append(qMobile_data_Vector.at(i).at(j).toDouble());
                //计算数据轴的最大最小值
                if(mobile_min>qMobile_data_Vector.at(i).at(j).toDouble())
                    mobile_min=qMobile_data_Vector.at(i).at(j).toDouble();
                if(mobile_max<qMobile_data_Vector.at(i).at(j).toDouble())
                    mobile_max=qMobile_data_Vector.at(i).at(j).toDouble();
            }
            //设置轴范围，如不设置，不显示图
            if(Mobile_time_int_Vector.length()>0)
            {
                if(mobile_time_max<Mobile_time_int_Vector[Mobile_time_int_Vector.length()-1])
                    mobile_time_max=Mobile_time_int_Vector[Mobile_time_int_Vector.length()-1];
                ui->customPlot->yAxis->setRange(mobile_min*0.9,mobile_max*1.25);
                ui->customPlot->xAxis->setRange(Mobile_time_int_Vector[0],mobile_time_max+60);
                //设置时间轴的间隔显示，大概四五个时间点
                double internalForTime=mobile_time_max-Mobile_time_int_Vector[0];
                if(internalForTime>60*1.5)
                {
                    int num=(int)(internalForTime/4)/30;
                    internalForTime=(num+1)*30;
                    ui->customPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
                }
                ui->customPlot->graph(i)->setData(Mobile_time_int_Vector,Mobile_data_Vector);
                ui->customPlot->replot();
            }
        }
    }

    else
    {
        for(int i=0;i<ui->customPlot->graphCount();i++)
        {
            ui->customPlot->graph(i)->clearData();
        }
        ui->customPlot->replot();
    }
}
