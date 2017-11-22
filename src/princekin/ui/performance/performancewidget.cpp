#include "performancewidget.h"
#include "ui_performancewidget.h"
#include <QScrollBar>

#define SHOWNUM 100
#define MEMSHOWMAX 32
#define CPUSHOWMAX 100

PerformanceWidget::PerformanceWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PerformanceWidget)
{
    ui->setupUi(this);

    createToolBar();

    setupDialog=new PerformanceSetup;
    setWindowTitle("性能测试");
    setupDialog->setAttribute(Qt::WA_QuitOnClose,false);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));//写入logcat的汉字是乱码，加这一条可改变编码方式

    ui->equipListView->setStyleSheet("QListView{font-size:12px;background:#ffffff;border:1px #ffffff;}"
                                     "QListView::item:selected:active{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                     "QListView::item:selected{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                     "QListView::item{height:30px;color:#4d4d4d}");

    ui->packageListView->setStyleSheet("QListView{font-size:12px;background:#ffffff;border:1px #ffffff;}"
                                       "QListView::item:selected:active{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                       "QListView::item:selected{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                       "QListView::item{height:30px;color:#4d4d4d}");

    ui->packageCombo->setStyleSheet("QComboBox QAbstractItemView::item{height:25px;}"
                                    "QComboBox {border: 1px solid #e4e4e4;  border-radius: 3px;padding: 1px 2px 1px 2px;min-width: 9em;}"
                                    "QComboBox::drop-down{ width: 40px; border-left-style: none;border-top-right-radius: 3px;  border-bottom-right-radius: 3px;}"
                                    "QComboBox::down-arrow{image:url(:/arrow.png);}");
    ui->packageCombo->setView(new QListView());

    ui->equipListView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                          "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->packageListView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                            "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->packageListView->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;width: 10px;}"
                                                              "QScrollBar::handle:horizontal {background: grey;border: 3px solid grey;border-radius:5px;}");
    dataInit();
    packageModel = new QStringListModel(packageList);
    equipModel = new QStringListModel(equipList);


    ui->splitter_2->setStretchFactor(0,4);
    ui->splitter_2->setStretchFactor(1,6);
    ui->splitter_3->setStretchFactor(0,3);
    ui->splitter_3->setStretchFactor(1,7);

    trafficDetail=new TrafficStatDetail;
    trafficDetail->setAttribute(Qt::WA_QuitOnClose,false);
    connect(ui->trafficLabel,SIGNAL(clicked()),trafficDetail,SLOT(show()));
    connect(ui->trafficLabel,SIGNAL(clicked()),trafficDetail,SLOT(drawTable()));

    perforDetail=new performanceDetail;
    perforDetail->setAttribute(Qt::WA_QuitOnClose,false);
    connect(ui->performanceLabel,SIGNAL(clicked()),perforDetail,SLOT(show()));
    connect(ui->performanceLabel,SIGNAL(clicked()),perforDetail,SLOT(WriteEdit()));

    tempdetail=new tempDetail;
    tempdetail->setAttribute(Qt::WA_QuitOnClose,false);
    connect(ui->tempLabel,SIGNAL(clicked()),tempdetail,SLOT(show()));
    connect(ui->tempLabel,SIGNAL(clicked()),tempdetail,SLOT(WriteEdit()));

    connect(ui->memPlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(MemPlotDetail()));
    connect(ui->cpuPlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(CpuPlotDetail()));
    connect(ui->batteryPlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(BatteryPlotDetail()));
    connect(ui->cpuTempPlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(CpuTempPlotDetail()));
    connect(ui->wifiPlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(WifiPlotDetail()));
    connect(ui->mobilePlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(MobilePlotDetail()));

    connect(settingButton,SIGNAL(clicked(bool)),this,SLOT(ShowSetupWindow()));
    connect(this,SIGNAL(SendMemPlotSignal(QString,double,double)),this,SLOT(MemPlotChart(QString,double,double)));
    connect(this,SIGNAL(SendMemStopSignal()),this,SLOT(RecieveMemStopSignal()));
    connect(this,SIGNAL(SendCpuPlotSignal(QString,double,double)),this,SLOT(CpuPlotChart(QString,double,double)));
    connect(this,SIGNAL(SendCpuStopSignal()),this,SLOT(RecieveMemStopSignal()));
    connect(this,SIGNAL(SendMemWriteEdit(QString,QString)),perforDetail,SLOT(RecieveMemResult(QString,QString)));
    connect(this,SIGNAL(SendCpuWriteEdit(QString,QString)),perforDetail,SLOT(RecieveCpuResult(QString,QString)));

    connect(ui->memPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(ui->cpuPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveEvent(QMouseEvent*)));

    connect(this,SIGNAL(SendBatteryPlotSignal(QString,double,double)),this,SLOT(BatteryPlotChart(QString,double,double)));
    connect(this,SIGNAL(SendBatteryStopSignal()),this,SLOT(RecieveBatteryStopSignal()));
    connect(this,SIGNAL(SendCPUTempPlotSignal(QString,double,double)),this,SLOT(CPUTempPlotChart(QString,double,double)));
    connect(this,SIGNAL(SendCPUTempStopSignal()),this,SLOT(RecieveBatteryStopSignal()));

    connect(this,SIGNAL(SendBatteryWriteEdit(QString,QString)),tempdetail,SLOT(RecieveBatteryResult(QString,QString)));
    connect(this,SIGNAL(SendCpuTempWriteEdit(QString,QString)),tempdetail,SLOT(RecieveCpuTempResult(QString,QString)));

    connect(this,SIGNAL(SendWifiPlotSignal(QString,double,double)),this,SLOT(WifiPlotChart(QString,double,double)));
    connect(this,SIGNAL(SendMobilePlotSignal(QString,double,double)),this,SLOT(MobilePlotChart(QString,double,double)));

    connect(ui->batteryPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(ui->cpuTempPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveEvent(QMouseEvent*)));

    connect(ui->wifiPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(ui->mobilePlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveEvent(QMouseEvent*)));

    connect(this,SIGNAL(sendSettingSig(QString,QString,QString,QString,QString,QString)),setupDialog,SLOT(recieveSettingSig(QString,QString,QString,QString,QString,QString)));

    connect(this,SIGNAL(SendTrafficStopSignal()),this,SLOT(RecieveTrafficStopSignal()));

    dataWidget=new DataCompareWidget();
    connect(dataWidget,SIGNAL(WindowDeactiveSignal()),this,SLOT(RecieveWindowDeactive()));
    connect(dataWidget,SIGNAL(SendCompareData(QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >)),
            this,SLOT(RecieveCompareData(QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >)));
    dataWidget->setAttribute(Qt::WA_QuitOnClose,false);

    chartDetail=new ChartDetail();
    chartDetail->setAttribute(Qt::WA_QuitOnClose,false);
    connect(dataWidget,SIGNAL(SendCompareData(QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >)),
            chartDetail,SLOT(RecieveCompareData(QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >,QVector<QVector<QString> >)));
}

PerformanceWidget::~PerformanceWidget()
{
    delete ui;
}

void PerformanceWidget::createToolBar()
{
    toolBar=addToolBar("setting");
    toolBar->setStyleSheet("QToolBar{background:#ffffff;}");
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spacer->setStyleSheet("QWidget{background:#ffffff;}");
    toolBar->addWidget(spacer);

    settingButton=new QToolButton(this);
    settingButton->setToolTip("设置");
    settingButton->setIcon(QIcon(":/setting.png"));
    settingButton->setFixedWidth(50);
    settingButton->setStyleSheet("QToolButton{background:#ffffff;}");
    toolBar->addWidget(settingButton);
}

void PerformanceWidget::showInit()
{
    emit sendSettingSig( gMemThres,gCpuThres,gBatteryThres,gCpuTempThres,gWifiThres,gMobileThres);
    if(ui->dataCompareBtn->text()==">")
    {
        dataWidget->show();
        int y1=ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height();
        int y2=ui->mobilePlot->mapToGlobal(QPoint(0,0)).y()+ui->mobilePlot->height();
        dataWidget->move(ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).x(), ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height());
        dataWidget->setFixedHeight(y2-y1);
    }
}

void PerformanceWidget::dataInit()
{
    //初始化
    time=2000;
    flag=0;

    strMemThres=gMemThres;
    strCpuThres=gCpuThres;
    strBatteryThres=gBatteryThres;
    strCpuTempThres=gCpuTempThres;
    strWifiThres=gWifiThres;
    strMobileThres=gMobileThres;

    //初始化图表的信息
    //向绘图区域QCustomPlot(从widget提升来的)添加一条曲线
    for(int i=0;i<9;i++)
    {
        ui->memPlot->addGraph();
        ui->cpuPlot->addGraph();
        ui->batteryPlot->addGraph();
        ui->cpuTempPlot->addGraph();
        ui->wifiPlot->addGraph();
        ui->mobilePlot->addGraph();
    }
    ui->memPlot->graph(0)->setPen(QPen(QColor("#c1904d"),2));
    ui->cpuPlot->graph(0)->setPen(QPen(QColor("#c1904d"),2));
    ui->batteryPlot->graph(0)->setPen(QPen(QColor("#c1904d"),2));
    ui->cpuTempPlot->graph(0)->setPen(QPen(QColor("#c1904d"),2));
    ui->wifiPlot->graph(0)->setPen(QPen(QColor("#c1904d"),2));
    ui->mobilePlot->graph(0)->setPen(QPen(QColor("#c1904d"),2));

    ui->cpuPlot->graph(1)->setPen(QPen(Qt::red));
    ui->memPlot->graph(1)->setPen(QPen(Qt::red));
    ui->batteryPlot->graph(1)->setPen(QPen(Qt::red));
    ui->cpuTempPlot->graph(1)->setPen(QPen(Qt::red));
    ui->wifiPlot->graph(1)->setPen(QPen(Qt::red));
    ui->mobilePlot->graph(1)->setPen(QPen(Qt::red));

    ui->memPlot->graph(2)->setPen(QPen(QColor("#c1904d"),1));
    ui->cpuPlot->graph(2)->setPen(QPen(QColor("#c1904d"),1));
    ui->batteryPlot->graph(2)->setPen(QPen(QColor("#c1904d"),1));
    ui->cpuTempPlot->graph(2)->setPen(QPen(QColor("#c1904d"),1));
    ui->wifiPlot->graph(2)->setPen(QPen(QColor("#c1904d"),1));
    ui->mobilePlot->graph(2)->setPen(QPen(QColor("#c1904d"),1));

    ui->memPlot->graph(3)->setPen(QPen(QColor("#9a5acc"),1));
    ui->cpuPlot->graph(3)->setPen(QPen(QColor("#9a5acc"),1));
    ui->batteryPlot->graph(3)->setPen(QPen(QColor("#9a5acc"),1));
    ui->cpuTempPlot->graph(3)->setPen(QPen(QColor("#9a5acc"),1));
    ui->wifiPlot->graph(3)->setPen(QPen(QColor("#9a5acc"),1));
    ui->mobilePlot->graph(3)->setPen(QPen(QColor("#9a5acc"),1));

    ui->memPlot->graph(4)->setPen(QPen(QColor("#ee4f71"),1));
    ui->cpuPlot->graph(4)->setPen(QPen(QColor("#ee4f71"),1));
    ui->batteryPlot->graph(4)->setPen(QPen(QColor("#ee4f71"),1));
    ui->cpuTempPlot->graph(4)->setPen(QPen(QColor("#ee4f71"),1));
    ui->wifiPlot->graph(4)->setPen(QPen(QColor("#ee4f71"),1));
    ui->mobilePlot->graph(4)->setPen(QPen(QColor("#ee4f71"),1));

    ui->memPlot->graph(5)->setPen(QPen(QColor("#7fda85"),1));
    ui->cpuPlot->graph(5)->setPen(QPen(QColor("#7fda85"),1));
    ui->batteryPlot->graph(5)->setPen(QPen(QColor("#7fda85"),1));
    ui->cpuTempPlot->graph(5)->setPen(QPen(QColor("#7fda85"),1));
    ui->wifiPlot->graph(5)->setPen(QPen(QColor("#7fda85"),1));
    ui->mobilePlot->graph(5)->setPen(QPen(QColor("#7fda85"),1));

    ui->memPlot->graph(6)->setPen(QPen(QColor("#25a4d6"),1));
    ui->cpuPlot->graph(6)->setPen(QPen(QColor("#25a4d6"),1));
    ui->batteryPlot->graph(6)->setPen(QPen(QColor("#25a4d6"),1));
    ui->cpuTempPlot->graph(6)->setPen(QPen(QColor("#25a4d6"),1));
    ui->wifiPlot->graph(6)->setPen(QPen(QColor("#25a4d6"),1));
    ui->mobilePlot->graph(6)->setPen(QPen(QColor("#25a4d6"),1));

    ui->memPlot->graph(7)->setPen(QPen(QColor("#98d7ed"),1));
    ui->cpuPlot->graph(7)->setPen(QPen(QColor("#98d7ed"),1));
    ui->batteryPlot->graph(7)->setPen(QPen(QColor("#98d7ed"),1));
    ui->cpuTempPlot->graph(7)->setPen(QPen(QColor("#98d7ed"),1));
    ui->wifiPlot->graph(7)->setPen(QPen(QColor("#98d7ed"),1));
    ui->mobilePlot->graph(7)->setPen(QPen(QColor("#98d7ed"),1));

    ui->memPlot->graph(8)->setPen(QPen(QColor("#fadf97"),1));
    ui->cpuPlot->graph(8)->setPen(QPen(QColor("#fadf97"),1));
    ui->batteryPlot->graph(8)->setPen(QPen(QColor("#fadf97"),1));
    ui->cpuTempPlot->graph(8)->setPen(QPen(QColor("#fadf97"),1));
    ui->wifiPlot->graph(8)->setPen(QPen(QColor("#fadf97"),1));
    ui->mobilePlot->graph(8)->setPen(QPen(QColor("#fadf97"),1));


    //设置坐标轴标签名称
    ui->memPlot->yAxis->setLabel("内存(M)");
    ui->cpuPlot->yAxis->setLabel("CPU(%)");
    ui->batteryPlot->yAxis->setLabel("电池温度(℃)");
    ui->cpuTempPlot->yAxis->setLabel("CPU温度(℃)");
    ui->wifiPlot->yAxis->setLabel("Wifi流量(M)");
    ui->mobilePlot->yAxis->setLabel("Phone流量(M)");/**/

    ui->memPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->memPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->memPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->memPlot->xAxis->setAutoTickStep(false);
    ui->memPlot->xAxis->setTickStep(30);
    ui->memPlot->rescaleAxes();

    ui->cpuPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->cpuPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->cpuPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->cpuPlot->xAxis->setAutoTickStep(false);
    ui->cpuPlot->xAxis->setTickStep(30);
    ui->cpuPlot->rescaleAxes();

    ui->batteryPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->batteryPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->batteryPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->batteryPlot->xAxis->setAutoTickStep(false);
    ui->batteryPlot->xAxis->setTickStep(30);
    ui->batteryPlot->rescaleAxes();

    ui->cpuTempPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->cpuTempPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->cpuTempPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->cpuTempPlot->xAxis->setAutoTickStep(false);
    ui->cpuTempPlot->xAxis->setTickStep(30);
    ui->cpuTempPlot->rescaleAxes();

    ui->wifiPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->wifiPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->wifiPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->wifiPlot->xAxis->setAutoTickStep(false);
    ui->wifiPlot->xAxis->setTickStep(30);
    ui->wifiPlot->rescaleAxes();

    ui->mobilePlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->mobilePlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->mobilePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->mobilePlot->xAxis->setAutoTickStep(false);
    ui->mobilePlot->xAxis->setTickStep(30);
    ui->mobilePlot->rescaleAxes();

    flagGetMem=0;
    flagGetCPU=0;

    gTrafficInterval="6";

}

void PerformanceWidget::RecieveDevicesSigal(QStringList devicesList)
{
    if(numForDeviceSignal<3)
        numForDeviceSignal++;
    QString currentDevice = ui->equipListView->currentIndex().data().toString();
    equipList.clear();
    equipList=devicesList;
    equipModel->setStringList(equipList);
    ui->equipListView->setModel(equipModel);

    int row=equipList.indexOf(currentDevice);
    if(row==-1)
    {
        if(ui->packageCombo->count()==4)
            ui->packageCombo->removeItem(0);//没有选中设备时，去掉第一项设备名称的显示，直接显示“第三方应用”
        packageModel->setStringList( QStringList());
        ui->packageListView->setModel(packageModel);
        deviceName="";//当前没有选中的设备时，设备名字清空，否则在点击设备时会出问题；
    }

    QModelIndex index=equipModel->index(row);
    if(equipList.contains(currentDevice))
    {
        ui->equipListView->setCurrentIndex(index);
    }
    if(numForDeviceSignal==1)
    {
        index=equipModel->index(0);
        ui->equipListView->setCurrentIndex(index);
        on_equipListView_clicked(index);
    }
}

void PerformanceWidget::on_equipBtn_clicked()//ShowEquip()
{
    //compareBcak();//对比对话框收回

    equipList.clear();
    equipModel = new QStringListModel(equipList);

    QString cmdLine="adb devices";
    QProcess p(0);
    p.setReadChannelMode(QProcess::MergedChannels);
    p.start(cmdLine);
    p.waitForStarted();
    while(p.waitForFinished()==false)
    {
    }

    QString mStr;
    QStringList mSplitResult;

    while(p.canReadLine())
    {
        mStr=p.readLine();
        if(mStr.contains("device") && !mStr.contains("devices"))
        {
            mSplitResult=mStr.split("device");
            if(!mSplitResult.at(0).contains("error"))
            {
                equipList.append(ExeCmd::GetDeviceModel(mSplitResult.at(0).trimmed())+" + "+mSplitResult.at(0).trimmed());
                equipModel->setStringList(equipList);
            }
        }
    }
    p.close();

    ui->equipListView->setModel(equipModel);
    deviceName="";//点击设备按钮后，还没选择设备，此时情况设备名

    QModelIndex indexEquip=ui->equipListView->model()->index(0,0);
    ui->equipListView->setCurrentIndex(indexEquip);

    deviceName=ExeCmd::getDeviceSN(ui->equipListView->currentIndex().data().toString());
    qDebug()<<deviceName;
    if(deviceName!="")
    {
        if(ui->packageCombo->count()==3)
            ui->packageCombo->insertItem(0,"");
        ui->packageCombo->setCurrentIndex(0);
        ui->packageCombo->setItemText(0,indexEquip.data().toString());
    }
}

void PerformanceWidget::on_packageCombo_currentIndexChanged(const QString &arg1)//ShowPackage()
{
    if(numForPackageIndexChange<3)
        numForPackageIndexChange++;
    if(deviceName=="")
    {
        if(numForPackageIndexChange!=1)
            QMessageBox::information(this,tr("提示"),tr("请选择一个设备"));
        return ;
    }

    if(ui->packageCombo->currentIndex()==0)
    {
        packageName="";
    }

    QString cmdStrAllPackages="adb -s "+deviceName+" shell pm list packages\n";
    QString cmdStrThirdPackages="adb -s "+deviceName+" shell pm list packages -3\n";
    QString cmdStrSystemPackages="adb -s "+deviceName+" shell pm list packages -s\n";

    int flag=0;//flag的作用是：选择包名时，第一项不需要process，为了区分第一项与其他三项

    QProcess p(0);
    p.setReadChannelMode(QProcess::MergedChannels);

    QString mStr;
    QStringList mSplitResult;

    packageList.clear();
    packageModel ->setStringList(packageList);

    switch(ui->packageCombo->currentIndex())
    {
    case 0:
        flag=0;
        break;
    case 2:
        p.start(cmdStrAllPackages);
        flag=1;
        break;
    case 1:
        p.start(cmdStrThirdPackages);
        flag=1;
        break;
    case 3:
        p.start(cmdStrSystemPackages);
        flag=1;
        break;
    default:
        flag=0;
        break;
    }
    if(flag==1)
    {
        p.waitForStarted();
        while(p.waitForFinished()==false)
        {
        }

        while(p.canReadLine())
        {
            mStr=p.readLine();
            if(mStr.contains("package:"))
            {
                mSplitResult=mStr.split("package:");
                packageList.append(mSplitResult.at(1).trimmed());
                packageModel->setStringList(packageList);
            }
        }
    }
    p.close();
    ui->packageListView->setModel(packageModel);
}

void PerformanceWidget::on_equipListView_clicked(const QModelIndex &index)//equipSelect()
{
    if((!ui->equipListView->currentIndex().data().toString().contains(deviceName))||deviceName=="")//如果没有换设备，不对当前界面操作
    {
        //更换设备时不需要开启监测其他参数，因为更换设备时，当前停止性能测试，一定要通过点击“开始”开启性能测试，但这里需要进行停止之前的其他参数监控操作。

        if(isStartFlag)
            emit SendMemStopSignal();

        QModelIndex indexEquip=ui->equipListView->currentIndex();
        deviceName=ExeCmd::getDeviceSN(indexEquip.data().toString());
        qDebug()<<deviceName;
        if(deviceName!="")
        {
            if(ui->packageCombo->count()==3)
                ui->packageCombo->insertItem(0,"");
            ui->packageCombo->setCurrentIndex(0);
            ui->packageCombo->setItemText(0,indexEquip.data().toString());
        }
        else
            ui->packageCombo->setCurrentIndex(0);
    }
}

void PerformanceWidget::on_packageListView_clicked(const QModelIndex &index)//packageSelect()
{
    QModelIndex indexPackage=ui->packageListView->currentIndex();
    packageName=indexPackage.data().toString();
}

void PerformanceWidget::on_startBtn_clicked()//StartMemMonitor()
{
    if(deviceName=="")
    {
        QMessageBox::information(this,"提示","请选择设备");
        return;
    }

    if(!isStartFlag)
    {
        //*****************20170717*****************//
        qOldPackageName=Helper::getFirstLine(gConfigDir + QDir::separator() + "packageName.txt");
        gOldPackageName=qOldPackageName;
        bool b=Helper::isPackageName1(packageName);
        if(!b)
        {
            return;
        }
        //*****************20170717*****************//

        ui->equipBtn->setEnabled(false);
        ui->equipListView->setEnabled(false);
        ui->packageCombo->setEnabled(false);
        ui->packageListView->setEnabled(false);
        ui->startBtn->setIcon(QIcon(":/stop.png"));
        ui->dataCompareBtn->setEnabled(false);
        ui->dataCpmpareBtn_2->setEnabled(false);
        ui->tempLabel->setEnabled(true);
        ui->trafficLabel->setEnabled(true);
        ui->performanceLabel->setEnabled(true);
        isStartFlag=true;
        flagError=false;

        startTime=QDateTime::currentDateTime();
        if(gWorkSpace=="")
            gWorkSpace=gNewDisk + QDir::separator() + "princekinWorkspace";

        //*****************20170315*****************//
        qStatisticsDataList.clear();
        qStartTime=Helper::getTime2("yyyyMMdd_hhmmss");
        fileNameForReport=gWorkSpace+QDir::separator()+ "report" + QDir::separator()+ "Performance_" + deviceName + "_" + qStartTime;
        Helper::createPath(fileNameForReport);

        if(xmlPaths.count()>0)
        {
            QStringList tempList=ParseXml::readConfigXmlForIP_Port(gConfigDir + QDir::separator() + "config.xml");
            IP=tempList[0];
            port=tempList[1];

            AssistBehaviorWorker *worker=new AssistBehaviorWorker;
            worker->xmlPaths=this->xmlPaths;
            worker->isDebugVersion=this->isDebug;
            worker->IP=this->IP;
            worker->Port=this->port;
            QThread *workerThread=new QThread(this);
            connect(workerThread,SIGNAL(started()),worker,SLOT(startStatistics()));
            connect(worker,SIGNAL(sendStopSignal()),workerThread,SLOT(quit()));
            connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));
            connect(this,SIGNAL(SendBatteryStopSignal()),worker,SLOT(stopStatistics()));

            connect(worker,SIGNAL(sendStatisticsData(QStringList)),this,SLOT(ReadStatisticsData(QStringList)));

            worker->moveToThread(workerThread);

            workerThread->start();
        }

        //开始之后就不是对比模式了
        compareModal=false;

        if(packageName=="")
        {
            qLunchTime="手机";
            time=4000;//测整个手机的内存时会卡一点，如果检测到测手机，把默认时间调慢一点
        }
        else
        {
            if(packageName!=qOldPackageName && packageName!="")
            {
                qLunchTime="";
                qFrames="";
            }
            else
            {
                getLunchTime();
            }
        }

        getMemoryCpuIndex();//CPU和内存的位置不是固定的，个别手机比较另类，需要提前提取位置信息

        timer = new QTimer;
        timerThread=new QThread;
        timer->start(time);
        timer->moveToThread(timerThread);
        connect( timer, SIGNAL(timeout()),this, SLOT(getMemoryCpu()),Qt::DirectConnection );
        connect( timer, SIGNAL(timeout()),this, SLOT(getBatteryTemp()),Qt::DirectConnection );
        connect( timer, SIGNAL(timeout()),this, SLOT(getCPUTemp()),Qt::DirectConnection );
        timerThread->start();


        if(packageName=="")
        {
            QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,"提示","测试整体手机时，流量统计暂不测量",QMessageBox::Close,this);
            QTimer::singleShot(2000,messageBox,SLOT(close()));
            messageBox->show();
        }
        else
        {
            //以下是流量测试
            QString mCmdLine;
            mCmdLine="cmd /c adb -s " + deviceName + " shell pm list packages -3 | findstr com.sohu.trafficservice";
            QString mStrService=ExeCmd::runCmd(mCmdLine);
            if(mStrService.isEmpty())
            {
                mCmdLine="adb -s " + deviceName + " install -r " + gConfigDir + QDir::separator() + "apk" + QDir::separator() + "trafficService.apk";
                ExeCmd::runCmd(mCmdLine);
            }

            bool isok;
            dSec=time/1000;
            QString uid;
            uid=getUid();
            if(uid.isEmpty())
            {
                //暂不提示
                return;
            }

            startService();
            getNetState();
            getTraffic(uid);
        }

        //*****************20170505*************************//
        getXXX();
        //*****************20170505*************************//


    }
    else if(isStartFlag)
    {
        emit SendMemStopSignal();
        emit SendBatteryStopSignal();
    }
}

void PerformanceWidget::ReadStatisticsData(QStringList statistics)
{    
    qStatisticsDataList=statistics;
    if(gPerformanceNumber!=0)
    {
        createExcel();
    }
}

void PerformanceWidget::RecieveTrafficStopSignal()
{
    if(packageName!="")
    {
        stopTraffic();
        stopService();
    }
}


void PerformanceWidget::exportReport()
{
    if(gPerformanceNumber==0)
    {
        createExcel();
    }
    //createBaseData();
}

void PerformanceWidget::on_clearBtn_clicked()
{
    for(int i=0;i<9;i++)
    {
        ui->memPlot->graph(i)->clearData();
        ui->cpuPlot->graph(i)->clearData();
        ui->batteryPlot->graph(i)->clearData();
        ui->cpuTempPlot->graph(i)->clearData();
        ui->wifiPlot->graph(i)->clearData();
        ui->mobilePlot->graph(i)->clearData();
    }

    memNum.clear();
    cpuNum.clear();
    memTime.clear();
    cpuTime.clear();
    memTimeInt.clear();
    cpuTimeInt.clear();
    memThres.clear();
    cpuThres.clear();

    ui->memPlot->replot();
    ui->cpuPlot->replot();

    batteryNum.clear();
    cpuTempNum.clear();
    batteryTime.clear();
    cpuTempTime.clear();
    batteryTimeInt.clear();
    cpuTempTimeInt.clear();
    batteryThres.clear();
    cpuTempThres.clear();

    ui->batteryPlot->replot();
    ui->cpuTempPlot->replot();

    wifiNum.clear();
    mobileNum.clear();
    wifiTime.clear();
    mobileTime.clear();
    wifiTimeInt.clear();
    mobileTimeInt.clear();
    wifiThres.clear();
    mobileThres.clear();

    ui->wifiPlot->replot();
    ui->mobilePlot->replot();

    trafficDetail->wifiData.clear();
    trafficDetail->mobileData.clear();

    wifiTrafficstats=0;
    mobileTrafficstats=0;
}

void PerformanceWidget::ShowSetupWindow()
{
    //setupDialog->setWindowModality(Qt::ApplicationModal);
    setupDialog->setWindowState(Qt::WindowActive);
    setupDialog->show();
    setupDialog->raise();
    setupDialog->move ((QApplication::desktop()->width() - setupDialog->width())/2,(QApplication::desktop()->height() - setupDialog->height())/2);
    connect(setupDialog,SIGNAL(SendData(QVariant)),this,SLOT(RecieveData(QVariant)));
}

void PerformanceWidget::RecieveData( QVariant var)
{
    signalData_s signalData;
    signalData=var.value<signalData_s>();
    this->time=signalData.time;
    this->strMemThres=signalData.memThres;
    this->strCpuThres=signalData.cpuThres;
    this->strBatteryThres=signalData.batteryThres;
    this->strCpuTempThres=signalData.cpuTempThres;
    this->strWifiThres=signalData.wifiThres;
    this->strMobileThres=signalData.mobileThres;
    this->xmlPaths=signalData.xmlPaths;
    this->isDebug=signalData.isDebug;

}

void PerformanceWidget::getMemoryCpuIndex()
{
    if(packageName!="")
    {
        QString cmdStrGetMem="adb -s "+deviceName+" shell top -m 1 -n 1 | grep CPU%";
        QString res=ExeCmd::runCmd(cmdStrGetMem).trimmed();
        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        for(int i=0;i<resList.count();i++)
        {
            if(resList[i]=="CPU%")
                cpuIndex=i;
            else if(resList[i]=="RSS")
                memIndex=i;
        }
    }
}

void PerformanceWidget::getMemoryCpu()
{
    if(deviceName=="")
    {
        if(timerThread->isRunning())
        {
            emit SendMemStopSignal();
        }
        return ;
    }
    flagError=false;

    QString res="";
    QString cmdStrGetMem,cmdStrGetCPU;
    int num;
    QString tmp="";
    QString memRes="";
    QString cpuRes="";
    if(packageName!="")
    {
        cmdStrGetMem="adb -s "+deviceName+" shell top -n 1 | grep "+packageName;
        res=ExeCmd::runCmd(cmdStrGetMem).trimmed();
        qDebug()<<res;
        if(res=="")
        {
            flagGetMem=flagGetMem+1;
            if(flagGetMem==2)
            {
                flagError=true;
                flagGetMem=0;

                QString startApp="adb -s "+deviceName+" shell monkey -p "+packageName+" -c android.intent.category.LAUNCHER 1";
                QString ti=ExeCmd::runCmd(startApp);
                if( ti.contains("monkey aborted")||ti.contains("device not found"))
                    emit SendMemStopSignal();
            }
            return;
        }
        flagGetMem=0;
        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        if(resList.count()>7)
        {
            memRes=resList.at(memIndex);
            cpuRes=resList.at(cpuIndex);
            cpuRes=cpuRes.left(cpuRes.length()-1);
            memRes=memRes.left(memRes.length()-1);
        }
    }
    else
    {
        //获取内存信息
        cmdStrGetMem="adb -s "+deviceName+" shell dumpsys meminfo | grep Used\n";
        res=ExeCmd::runCmd(cmdStrGetMem);
        qDebug()<<res;
        if(res=="")
        {
            flagGetMemTotal++;qDebug()<<"ERROR";
            if(flagGetMemTotal==2)
            {
                flagError=true;
                flagGetMemTotal=0;

                emit SendMemStopSignal();
            }
            return;
        }
        flagGetMemTotal=0;

        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        if(resList.count()>2)
            memRes=resList.at(2);

        //获取CPU信息
        cmdStrGetCPU="adb -s "+deviceName+" shell dumpsys cpuinfo | grep TOTAL\n";
        res=ExeCmd::runCmd(cmdStrGetCPU);
        if(res=="")//如果没有返回结果就继续
        {
            flagGetCPUTotal++;
            if(flagGetCPUTotal==2)
            {
                flagError=true;
                flagGetCPUTotal=0;
                emit SendCpuStopSignal();
            }

            return;
        }
        flagGetCPUTotal=0;

        resList=res.split(" ",QString::SkipEmptyParts);
        if(resList.count()>0)
        {
            cpuRes=resList.at(0);
            cpuRes=cpuRes.left(cpuRes.length()-1);
        }
    }

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    //统计内存信息
    memTime.append(currentTime);
    memNum.append((QString::number(memRes.toDouble()/1024,'f',1)).toDouble());
    memThres.append(strMemThres.toDouble());
    memTimeInt.append( QDateTime::fromString(currentTime,"yyyy-MM-dd hh:mm:ss").toTime_t());

    if(memNum.length()==1)
    {
        memMin=memNum[0];
        memMax=memNum[0];
    }

    if(memNum[memNum.length()-1]>memMax)
        memMax=memNum[memNum.length()-1];
    if(memNum[memNum.length()-1]<memMin)
        memMin=memNum[memNum.length()-1];

    memAve=(memAve*(memNum.count()-1)+memRes.toDouble()/1024)/memNum.count();

    //*****************20170606*****************//
    gMemAve=memAve;
    //*****************20170606*****************//

    emit SendMemPlotSignal(memRes,memMin,memMax);
    emit SendMemWriteEdit(memRes,currentTime);

    //统计CPU信息
    cpuTime.append(currentTime);
    cpuTimeInt.append( QDateTime::fromString(currentTime,"yyyy-MM-dd hh:mm:ss").toTime_t());
    cpuThres.append(strCpuThres.toDouble());
    cpuNum.append(cpuRes.toDouble());

    if(cpuNum.length()==1)
    {
        cpuMin=cpuNum[0];
        cpuMax=cpuNum[0];
    }

    if(cpuNum.length()>1)
    {
        if(cpuMin>cpuNum[cpuNum.length()-1])
            cpuMin=cpuNum[cpuNum.length()-1];
        if(cpuMax<cpuNum[cpuNum.length()-1])
            cpuMax=cpuNum[cpuNum.length()-1];
    }

    cpuAve=(cpuAve*(cpuNum.count()-1)+cpuRes.toDouble())/cpuNum.count();

    //*****************20170606*****************//
    gCpuAve=cpuAve;
    //*****************20170606*****************//

    if(cpuNum.length()>0)
    {
        emit SendCpuPlotSignal(cpuRes,cpuMin,cpuMax);
        emit SendCpuWriteEdit(cpuRes,currentTime);
    }
}

void PerformanceWidget::RecieveMemStopSignal()
{
    if(timerThread->isRunning())
    {
        timerThread->quit();
        timerThread->wait();

        emit SendTrafficStopSignal();

        if(flagError)
            QMessageBox::information(this,tr("提示"),tr("请确认设备是否连接正常"));
        ui->startBtn->setIcon(QIcon(":/start.png"));
        ui->equipBtn->setEnabled(true);
        ui->equipListView->setEnabled(true);
        ui->packageCombo->setEnabled(true);
        ui->packageListView->setEnabled(true);
        isStartFlag=false;
        ui->dataCompareBtn->setEnabled(true);
        ui->dataCpmpareBtn_2->setEnabled(true);

        endTime=QDateTime::currentDateTime();
        //停止，自动导出报告函数
        exportReport();
    }
}

void PerformanceWidget::MemPlotChart(QString numStr, double min,double max)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式

    numStr=QString::number(numStr.toDouble()/1024,'f',1);
    //ui->memTextEdit->append(numStr+"\t"+strTime);

    if( min > strMemThres.toDouble())
        min=strMemThres.toDouble();
    if(max< strMemThres.toDouble())
        max=strMemThres.toDouble();

    ui->memPlot->yAxis->setRange(min*0.9,max*1.25);
    ui->memPlot->xAxis->setRange(memTimeInt[0],memTimeInt[memTimeInt.length()-1]+60);
    ui->memPlot->graph(0)->setData(memTimeInt,memNum);
    ui->memPlot->graph(1)->setData(memTimeInt,memThres);

    double internal=memTimeInt[memTimeInt.length()-1]-memTimeInt[0];
    if(internal>60*1.5)
    {
        int num=(int)(internal/4)/30;
        internal=(num+1)*30;
        ui->memPlot->xAxis->setTickStep(internal); // 一天的秒数

    }
    ui->memPlot->replot();
}

void PerformanceWidget::CpuPlotChart(QString numStr, double min, double max)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    //ui->cpuTextEdit->append(numStr+"%\t"+strTime);
    if( min > strCpuThres.toDouble())
        min=strCpuThres.toDouble();
    if(max< strCpuThres.toDouble())
        max=strCpuThres.toDouble();

    ui->cpuPlot->yAxis->setRange(min*0.9,max*1.25);
    ui->cpuPlot->xAxis->setRange(cpuTimeInt[0],cpuTimeInt[cpuTimeInt.length()-1]+60);
    ui->cpuPlot->graph(0)->setData(cpuTimeInt,cpuNum);
    ui->cpuPlot->graph(1)->setData(cpuTimeInt,cpuThres);

    double internal=cpuTimeInt[cpuTimeInt.length()-1]-cpuTimeInt[0];
    if(internal>60*1.5)
    {
        int num=(int)(internal/4)/30;
        internal=(num+1)*30;
        ui->cpuPlot->xAxis->setTickStep(internal); // 一天的秒数

    }
    ui->cpuPlot->replot();
}

void PerformanceWidget::getBatteryTemp()
{
    if(deviceName=="")
    {
        if(timerThread->isRunning())
        {
            emit SendBatteryStopSignal();
        }
        return ;
    }
    flagError=false;
    QString getBatteryCmd="adb -s "+deviceName+" shell dumpsys battery | grep \"temperature\"";
    QString result=ExeCmd::runCmd(getBatteryCmd).trimmed();

    QString tmp="temperature:";
    if(result.contains(tmp))
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

        result=result.mid(tmp.length());

        batteryTime.append(currentTime);
        batteryTimeInt.append( QDateTime::fromString(currentTime,"yyyy-MM-dd hh:mm:ss").toTime_t());
        batteryThres.append(strBatteryThres.toDouble());
        batteryNum.append(result.toDouble()/10);

        if(batteryNum.length()==1)
        {
            batteryMin=batteryNum[0];
            batteryMax=batteryNum[0];
        }

        if(batteryNum[batteryNum.length()-1]>batteryMax)
            batteryMax=batteryNum[batteryNum.length()-1];
        if(batteryNum[batteryNum.length()-1]<batteryMin)
            batteryMin=batteryNum[batteryNum.length()-1];

        batteryAve=(batteryAve*(batteryNum.count()-1)+result.toDouble()/10)/batteryNum.count();

        //*****************20170606*****************//
        gBatteryAve=batteryAve;
        //*****************20170606*****************//

        emit SendBatteryPlotSignal(result,batteryMin,batteryMax);
        emit SendBatteryWriteEdit(result,currentTime);
        flagErrorNumBattery=0;
    }
    else
    {
        flagErrorNumBattery++;
        qDebug()<<"tERROR没有获取到电池温度信息";
        if(flagErrorNumBattery==2)
        {
            flagError=true;
            flagErrorNumBattery=0;
            emit SendBatteryStopSignal();
        }
        return;
    }
}

void PerformanceWidget::getCPUTemp()
{
    if(deviceName=="")
    {
        if(timerThread->isRunning())
        {
            emit SendCPUTempStopSignal();
        }
        return ;
    }
    flagError=false;
    QString getCPUCmd="adb -s "+deviceName+" shell cat /sys/class/thermal/thermal_zone1/temp";
    QString result=ExeCmd::runCmd(getCPUCmd).trimmed();

    QString tmp="error:";
    if(!result.contains(tmp))
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

        //if(isMeizu)
        //    result=result.left(result.length()-3);

        double resNum=0;bool ok;
        resNum=result.toDouble(&ok);
        if(!ok||resNum<0)
            return;
        if(resNum>1000)
            resNum=resNum/1000;
        else if(resNum>100)
            resNum=resNum/10;

        cpuTempTime.append(currentTime);
        cpuTempTimeInt.append( QDateTime::fromString(currentTime,"yyyy-MM-dd hh:mm:ss").toTime_t());
        cpuTempThres.append(strCpuTempThres.toDouble());
        cpuTempNum.append(resNum);

        if(cpuTempNum.length()==1)
        {
            cpuTempMin=cpuTempNum[0];
            cpuTempMax=cpuTempNum[0];
        }

        if(cpuTempNum.length()>1)
        {
            if(cpuTempNum[cpuTempNum.length()-1]>cpuTempMax)
                cpuTempMax=cpuTempNum[cpuTempNum.length()-1];
            if(cpuTempNum[cpuTempNum.length()-1]<cpuTempMin)
                cpuTempMin=cpuTempNum[cpuTempNum.length()-1];
        }

        cpuTempAve=(cpuTempAve*(cpuTempNum.count()-1)+resNum)/cpuTempNum.count();
        //*****************20170606*****************//
        gCpuTempAve=cpuTempAve;
        //*****************20170606*****************//

        if(cpuTempNum.length()>0)
        {
            emit SendCPUTempPlotSignal(QString::number(resNum),cpuTempMin,cpuTempMax);
            emit SendCpuTempWriteEdit(QString::number(resNum),currentTime);
        }
        flagErrorNumCpu=0;
    }/**/
    else
    {
        flagErrorNumCpu++;
        qDebug()<<"TERROR没有获取到cpu温度信息";
        if(flagErrorNumCpu==2)
        {
            flagError=true;
            flagErrorNumCpu=0;
            emit SendCPUTempStopSignal();
        }

        return;
    }
}

void PerformanceWidget::BatteryPlotChart(QString numStr, double min,double max)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式

    numStr=QString::number(numStr.toDouble()/10,'f',1);

    if( min > strBatteryThres.toDouble())
        min=strBatteryThres.toDouble();
    if(max< strBatteryThres.toDouble())
        max=strBatteryThres.toDouble();

    ui->batteryPlot->yAxis->setRange(min*0.9,max*1.25);
    ui->batteryPlot->graph(0)->setData(batteryTimeInt,batteryNum);
    ui->batteryPlot->graph(1)->setData(batteryTimeInt,batteryThres);
    ui->batteryPlot->xAxis->setRange(batteryTimeInt[0],batteryTimeInt[batteryTimeInt.length()-1]+60);

    double internal=batteryTimeInt[batteryTimeInt.length()-1]-batteryTimeInt[0];
    if(internal>60*1.5)
    {
        int num=(int)(internal/4)/30;
        internal=(num+1)*30;
        ui->batteryPlot->xAxis->setTickStep(internal); // 一天的秒数

    }

    ui->batteryPlot->replot();/**/
}

void PerformanceWidget::CPUTempPlotChart(QString numStr, double min,double max)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式

    numStr=QString::number(numStr.toDouble(),'f',1);

    if( min > strCpuTempThres.toDouble())
        min=strCpuTempThres.toDouble();
    if(max< strCpuTempThres.toDouble())
        max=strCpuTempThres.toDouble();

    ui->cpuTempPlot->yAxis->setRange(min*0.9,max*1.25);
    ui->cpuTempPlot->graph(0)->setData(cpuTempTimeInt,cpuTempNum);
    ui->cpuTempPlot->graph(1)->setData(cpuTempTimeInt,cpuTempThres);
    ui->cpuTempPlot->xAxis->setRange(cpuTempTimeInt[0],cpuTempTimeInt[cpuTempTimeInt.length()-1]+60);
    double internal=cpuTempTimeInt[cpuTempTimeInt.length()-1]-cpuTempTimeInt[0];
    if(internal>60*1.5)
    {
        int num=(int)(internal/4)/30;
        internal=(num+1)*30;
        ui->cpuTempPlot->xAxis->setTickStep(internal); // 一天的秒数

    }
    ui->cpuTempPlot->replot();
}

void PerformanceWidget::RecieveBatteryStopSignal()
{
    if(timerThread->isRunning())
    {
        timerThread->quit();
        timerThread->wait();

        emit SendTrafficStopSignal();

        if(flagError)
            QMessageBox::information(this,tr("提示"),tr("请确认设备是否连接正常"));
        ui->startBtn->setIcon(QIcon(":/start.png"));
        ui->equipBtn->setEnabled(true);
        ui->equipListView->setEnabled(true);
        ui->packageCombo->setEnabled(true);
        ui->packageListView->setEnabled(true);
        isStartFlag=false;

        ui->dataCompareBtn->setEnabled(true);
        ui->dataCpmpareBtn_2->setEnabled(true);

        endTime=QDateTime::currentDateTime();
        //停止，自动导出报告函数
        exportReport();
    }

}

void PerformanceWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget *action=QApplication::widgetAt(QCursor::pos());
    if(action==ui->memPlot)
    {
        double x=ui->memPlot->xAxis->pixelToCoord(e->x());
        double y=ui->memPlot->yAxis->pixelToCoord(e->y());
        QRect rect(0,0,50,50);
        for(int i=1;i<memTimeInt.length();i++)
        {
            if(x>memTimeInt[i-1]&&x<memTimeInt[i])
                if(qAbs(memNum[i]-y)<memMax*0.01)
                {
                    QToolTip::showText(cursor().pos(), memTime[i]+" , "+QString::number(memNum[i]),this,rect,2000);
                }
        }
    }
    if(action==ui->cpuPlot)
    {
        double x=ui->cpuPlot->xAxis->pixelToCoord(e->x());
        double y=ui->cpuPlot->yAxis->pixelToCoord(e->y());
        QRect rect(0,0,50,50);
        for(int i=1;i<cpuTimeInt.length();i++)
        {
            if(x>cpuTimeInt[i-1]&&x<cpuTimeInt[i])
                if(qAbs(cpuNum[i]-y)<cpuMax*0.01)
                {
                    QToolTip::showText(cursor().pos(), cpuTime[i]+" , "+QString::number(cpuNum[i]),this,rect,2000);
                }
        }
    }
    if(action==ui->cpuTempPlot)
    {
        double x=ui->cpuTempPlot->xAxis->pixelToCoord(e->x());
        double y=ui->cpuTempPlot->yAxis->pixelToCoord(e->y());
        QRect rect(0,0,50,50);
        for(int i=1;i<cpuTempTimeInt.length();i++)
        {
            if(x>cpuTempTimeInt[i-1]&&x<cpuTempTimeInt[i])
                if(qAbs(cpuTempNum[i]-y)<cpuTempMax*0.01)
                {
                    QToolTip::showText(cursor().pos(), cpuTempTime[i]+" , "+QString::number(cpuTempNum[i]),this,rect,2000);
                }
        }
    }
    if(action==ui->batteryPlot)
    {
        double x=ui->batteryPlot->xAxis->pixelToCoord(e->x());
        double y=ui->batteryPlot->yAxis->pixelToCoord(e->y());
        QRect rect(0,0,50,50);
        for(int i=1;i<batteryTimeInt.length();i++)
        {
            if(x>batteryTimeInt[i-1]&&x<batteryTimeInt[i])
                if(qAbs(batteryNum[i]-y)<batteryMax*0.01)
                {
                    QToolTip::showText(cursor().pos(), batteryTime[i]+" , "+QString::number(batteryNum[i]),this,rect,2000);
                }
        }
    }

    if(action==ui->wifiPlot)
    {
        double x=ui->wifiPlot->xAxis->pixelToCoord(e->x());
        double y=ui->wifiPlot->yAxis->pixelToCoord(e->y());
        QRect rect(0,0,50,50);
        for(int i=1;i<wifiTimeInt.length();i++)
        {
            if(x>wifiTimeInt[i-1]&&x<wifiTimeInt[i])
                if(qAbs(wifiNum[i]-y)<wifiMax*0.01)
                {
                    QToolTip::showText(cursor().pos(), wifiTime[i]+" , "+QString::number(wifiNum[i]),this,rect,2000);
                }
        }
    }

    if(action==ui->mobilePlot)
    {
        double x=ui->mobilePlot->xAxis->pixelToCoord(e->x());
        double y=ui->mobilePlot->yAxis->pixelToCoord(e->y());
        QRect rect(0,0,50,50);
        for(int i=1;i<mobileTimeInt.length();i++)
        {
            if(x>mobileTimeInt[i-1]&&x<mobileTimeInt[i])
                if(qAbs(mobileNum[i]-y)<mobileMax*0.01)
                {
                    QToolTip::showText(cursor().pos(), mobileTime[i]+" , "+QString::number(mobileNum[i]),this,rect,2000);
                }
        }
    }
}

void PerformanceWidget::closeEvent(QCloseEvent *e)
{
    if(isStartFlag)
    {
        emit SendMemStopSignal();
    }
    if(ui->dataCompareBtn->text()==">")
        dataWidget->close();
}

void PerformanceWidget::on_reportDetailBtn_clicked()
{
    //compareBcak();
    ExeCmd::openFolder(fileNameForReport);
}

void PerformanceWidget::startService()
{
    QString cmdLine;
    cmdLine="adb -s " + deviceName + " shell am startservice com.sohu.trafficservice/.TrafficService";
    ExeCmd::runCmd(cmdLine);
}

QString PerformanceWidget::getUid()
{
    QString uid;
    QString cmdLine;
    cmdLine="cmd /c adb -s " + deviceName + " shell dumpsys package " + packageName + " | findstr userId=";
    uid=ExeCmd::runCmd_getUid(cmdLine);
    return uid;
}

void PerformanceWidget::getTraffic(const QString &uid)
{
    QThreadPool *pool=new QThreadPool();
    //qcmdLine="cmd /c adb -s " + deviceId + " shell cat /proc/net/xt_qtaguid/stats | findstr " + uid;
    //timerId=this->startTimer(dSec*1000);

    QString cmdLine="cmd /c adb -s " + deviceName + " shell cat /proc/net/xt_qtaguid/stats | findstr "+uid;
    TrafficController2::Controller *controllerInstance=new TrafficController2::Controller;

    //qTraffic_Controller_Hash.insert(deviceId,controllerInstance);

    controllerInstance->setAutoDelete(true);
    controllerInstance->setSec(dSec*1000);
    controllerInstance->setDeviceId(deviceName);
    controllerInstance->setCmdLine(cmdLine);

    connect(controllerInstance, SIGNAL(sendResult_wifi1(const QString &,const QString &)), trafficDetail, SLOT(receiveTrafficResult_wifi1(const QString &,const QString & )),Qt::DirectConnection);
    connect(controllerInstance, SIGNAL(sendResult_wifi2(const QString &,const QString &)), trafficDetail, SLOT(receiveTrafficResult_wifi2(const QString &,const QString & )),Qt::DirectConnection);

    connect(controllerInstance, SIGNAL(sendResult_mobile1(const QString &,const QString &)), trafficDetail, SLOT(receiveTrafficResult_mobile1(const QString &,const QString & )),Qt::DirectConnection);
    connect(controllerInstance, SIGNAL(sendResult_mobile2(const QString &,const QString &)), trafficDetail, SLOT(receiveTrafficResult_mobile2(const QString &,const QString & )),Qt::DirectConnection);

    connect(controllerInstance, SIGNAL(sendResult_wifi2(const QString &,const QString &)), this, SLOT(WifiDataHandle(const QString &,const QString & )),Qt::DirectConnection);
    connect(controllerInstance, SIGNAL(sendResult_mobile2(const QString &,const QString &)), this, SLOT(MobileDataHandle(const QString &,const QString & )),Qt::DirectConnection);
    pool->start(controllerInstance);
}

void PerformanceWidget::getNetState()
{
    QString cmdLine;

    cmdLine="adb -s " + deviceName + " logcat -s TrafficService";
    NetStateController::Controller *controllerInstance=new NetStateController::Controller;
    //qNetState_Controller_Hash.insert(deviceId,controllerInstance);

    //connect(controllerInstance,SIGNAL(sendProcFinished(const QString &)),this,SLOT(receiveNetStateProcFinished(const QString &) ) );
    controllerInstance->setDeviceId(deviceName);
    controllerInstance->setCmdLine(cmdLine);
    controllerInstance->startController();
}

void PerformanceWidget::stopService()
{
    QString mCmdLine;
    mCmdLine="adb -s " + deviceName + " shell am force-stop com.sohu.trafficservice";
    //runCmd(mCmdLine);

    AdbShellController::Controller *controller=new AdbShellController::Controller;
    controller->setCmdLine(mCmdLine);
    controller->doController();
}

void PerformanceWidget::stopTraffic()
{
    gIsStopTraffic=true;
}

void PerformanceWidget::WifiDataHandle(const QString &arg_, const QString &arg_all)
{
    QStringList splistResult;
    splistResult=arg_all.split("=");

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    wifiTime.append(currentTime);
    wifiNum.append(splistResult.at(2).toDouble()/1024);//单位是M
    wifiTimeInt.append( QDateTime::fromString(currentTime,"yyyy-MM-dd hh:mm:ss").toTime_t());
    wifiThres.append(strWifiThres.toDouble());

    if(wifiNum.length()==1)
    {
        wifiMin=wifiNum[0];
        wifiMax=wifiNum[0];
    }

    if(wifiNum[wifiNum.length()-1]>wifiMax)
        wifiMax=wifiNum[wifiNum.length()-1];
    if(wifiNum[wifiNum.length()-1]<wifiMin)
        wifiMin=wifiNum[wifiNum.length()-1];

    wifiTrafficstats+=splistResult.at(2).toDouble()/1024;

    //***************20170628********************//
    gPerformanceWifi=wifiTrafficstats;
    //***************20170628********************//

    emit SendWifiPlotSignal(splistResult.at(2),wifiMin,wifiMax);
}

void PerformanceWidget::MobileDataHandle(const QString &arg_, const QString &arg_all)
{
    QStringList splistResult;
    splistResult=arg_all.split("=");

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    mobileTime.append(currentTime);
    mobileNum.append(splistResult.at(2).toDouble()/1024);//单位是M
    mobileTimeInt.append( QDateTime::fromString(currentTime,"yyyy-MM-dd hh:mm:ss").toTime_t());
    mobileThres.append(strMobileThres.toDouble());

    if(mobileNum.length()==1)
    {
        mobileMin=mobileNum[0];
        mobileMax=mobileNum[0];
    }

    if(mobileNum[mobileNum.length()-1]>mobileMax)
        mobileMax=mobileNum[mobileNum.length()-1];
    if(mobileNum[mobileNum.length()-1]<mobileMin)
        mobileMin=mobileNum[mobileNum.length()-1];

    mobileTrafficstats+=splistResult.at(2).toDouble()/1024;

    //***************20170628********************//
    gPerformanceMobile=mobileTrafficstats;
    //***************20170628********************//

    emit SendWifiPlotSignal(splistResult.at(2),mobileMin,mobileMax);
}

void PerformanceWidget::WifiPlotChart(QString numStr, double min,double max)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式

    numStr=QString::number(numStr.toDouble()/1024,'f',1);

    if( min > strWifiThres.toDouble())
        min=strWifiThres.toDouble();
    if(max< strWifiThres.toDouble())
        max=strWifiThres.toDouble();

    ui->wifiPlot->yAxis->setRange(min*0.9,max*1.25);
    ui->wifiPlot->graph(0)->setData(wifiTimeInt,wifiNum);
    ui->wifiPlot->graph(1)->setData(wifiTimeInt,wifiThres);
    ui->wifiPlot->xAxis->setRange(wifiTimeInt[0],wifiTimeInt[wifiTimeInt.length()-1]+60);
    double internal=wifiTimeInt[wifiTimeInt.length()-1]-wifiTimeInt[0];
    if(internal>60*1.5)
    {
        int num=(int)(internal/4)/30;
        internal=(num+1)*30;
        ui->wifiPlot->xAxis->setTickStep(internal); // 一天的秒数

    }
    ui->wifiPlot->replot();/**/
}

void PerformanceWidget::MobilePlotChart(QString numStr, double min,double max)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式

    numStr=QString::number(numStr.toDouble(),'f',1);

    if( min > strMobileThres.toDouble())
        min=strMobileThres.toDouble();
    if(max< strMobileThres.toDouble())
        max=strMobileThres.toDouble();

    ui->mobilePlot->yAxis->setRange(min*0.9,max*1.25);
    ui->mobilePlot->graph(0)->setData(mobileTimeInt,mobileNum);
    ui->mobilePlot->graph(1)->setData(mobileTimeInt,mobileThres);
    ui->mobilePlot->xAxis->setRange(mobileTimeInt[0],mobileTimeInt[mobileTimeInt.length()-1]+60);
    double internal=mobileTimeInt[mobileTimeInt.length()-1]-mobileTimeInt[0];
    if(internal>60*1.5)
    {
        int num=(int)(internal/4)/30;
        internal=(num+1)*30;
        ui->mobilePlot->xAxis->setTickStep(internal); // 一天的秒数

    }
    ui->mobilePlot->replot();/**/
}

void PerformanceWidget::MemPlotDetail()
{
    //compareBcak();//对比对话框收回

    if(compareModal)
    {
        chartDetail->show();
        chartDetail->setFlag("mem");
        chartDetail->plotChart();
    }
    else
    {
        ChartDetail *chartDetail=new ChartDetail;
        chartDetail->setAttribute(Qt::WA_QuitOnClose,false);
        chartDetail->show();
        chartDetail->setFlag("mem");
        chartDetail->startTimer();
        connect(this,SIGNAL(SendThresMax(QString,QString,QString,QString,QString,QString)),chartDetail,SLOT(RecieveThresMax(QString,QString,QString,QString,QString,QString)));
        emit SendThresMax(strMemThres,strCpuThres,strBatteryThres,strCpuTempThres,strWifiThres,strMobileThres);
    }
}

void PerformanceWidget::CpuPlotDetail()
{
    //compareBcak();//对比对话框收回

    if(compareModal)
    {
        chartDetail->show();
        chartDetail->setFlag("cpu");
        chartDetail->plotChart();
    }
    else
    {
        ChartDetail *chartDetail=new ChartDetail;
        chartDetail->setAttribute(Qt::WA_QuitOnClose,false);
        chartDetail->show();
        chartDetail->setFlag("cpu");
        chartDetail->startTimer();
        connect(this,SIGNAL(SendThresMax(QString,QString,QString,QString,QString,QString)),chartDetail,SLOT(RecieveThresMax(QString,QString,QString,QString,QString,QString)));
        emit SendThresMax(strMemThres,strCpuThres,strBatteryThres,strCpuTempThres,strWifiThres,strMobileThres);
    }
}

void PerformanceWidget::BatteryPlotDetail()
{
    //compareBcak();//对比对话框收回

    if(compareModal)
    {
        chartDetail->show();
        chartDetail->setFlag("battery");
        chartDetail->plotChart();
    }
    else
    {
        ChartDetail *chartDetail=new ChartDetail;
        chartDetail->setAttribute(Qt::WA_QuitOnClose,false);
        chartDetail->show();
        chartDetail->setFlag("battery");
        chartDetail->startTimer();
        connect(this,SIGNAL(SendThresMax(QString,QString,QString,QString,QString,QString)),chartDetail,SLOT(RecieveThresMax(QString,QString,QString,QString,QString,QString)));
        emit SendThresMax(strMemThres,strCpuThres,strBatteryThres,strCpuTempThres,strWifiThres,strMobileThres);
    }
}

void PerformanceWidget::CpuTempPlotDetail()
{
    //compareBcak();//对比对话框收回

    if(compareModal)
    {
        chartDetail->show();
        chartDetail->setFlag("cputemp");
        chartDetail->plotChart();
    }
    else
    {
        ChartDetail *chartDetail=new ChartDetail;
        chartDetail->setAttribute(Qt::WA_QuitOnClose,false);
        chartDetail->show();
        chartDetail->setFlag("cputemp");
        chartDetail->startTimer();
        connect(this,SIGNAL(SendThresMax(QString,QString,QString,QString,QString,QString)),chartDetail,SLOT(RecieveThresMax(QString,QString,QString,QString,QString,QString)));
        emit SendThresMax(strMemThres,strCpuThres,strBatteryThres,strCpuTempThres,strWifiThres,strMobileThres);
    }
}

void PerformanceWidget::WifiPlotDetail()
{
    //compareBcak();//对比对话框收回

    if(compareModal)
    {
        chartDetail->show();
        chartDetail->setFlag("wifi");
        chartDetail->plotChart();
    }
    else
    {
        ChartDetail *chartDetail=new ChartDetail;
        chartDetail->setAttribute(Qt::WA_QuitOnClose,false);
        chartDetail->show();
        chartDetail->setFlag("wifi");
        chartDetail->startTimer();
        connect(this,SIGNAL(SendThresMax(QString,QString,QString,QString,QString,QString)),chartDetail,SLOT(RecieveThresMax(QString,QString,QString,QString,QString,QString)));
        emit SendThresMax(strMemThres,strCpuThres,strBatteryThres,strCpuTempThres,strWifiThres,strMobileThres);
    }
}

void PerformanceWidget::MobilePlotDetail()
{
    //compareBcak();//对比对话框收回

    if(compareModal)
    {
        chartDetail->show();
        chartDetail->setFlag("mobile");
        chartDetail->plotChart();
    }
    else
    {
        ChartDetail *chartDetail=new ChartDetail;
        chartDetail->setAttribute(Qt::WA_QuitOnClose,false);
        chartDetail->show();
        chartDetail->setFlag("mobile");
        chartDetail->startTimer();
        connect(this,SIGNAL(SendThresMax(QString,QString,QString,QString,QString,QString)),chartDetail,SLOT(RecieveThresMax(QString,QString,QString,QString,QString,QString)));
        emit SendThresMax(strMemThres,strCpuThres,strBatteryThres,strCpuTempThres,strWifiThres,strMobileThres);
    }
}


void PerformanceWidget::createBaseData()
{
    QString fileNameForPerformance=fileNameForReport+QDir::separator()+"report.txt";
    QFile f(fileNameForPerformance);
    f.open(QIODevice::WriteOnly|QIODevice::Text);

    QString internalTime = QString::number( startTime.daysTo(endTime))+"天"
                    +QString::number( startTime.msecsTo(endTime)/3600000)+"小时"
                    +QString::number(startTime.msecsTo(endTime)%3600000/60000)+"分"
                    +QString::number(startTime.msecsTo(endTime)%60000/1000)+"秒";/**/

    QString cmdLine="adb -s "+deviceName+" shell getprop ro.build.version.release";
    qDebug()<<cmdLine;
    QString systemStr=ExeCmd::runCmd(cmdLine);

    QString tempStr;
    tempStr="手机型号： "+ExeCmd::GetDeviceModel( deviceName) +"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="系统版本： Android "+ systemStr ;
    f.write(tempStr.toStdString().c_str());
    QString str="";
    if(packageName=="")
        str="整体手机";
    else
        str=packageName;
    tempStr="测试项目： "+ str +"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="测量开始时间： "+ startTime.toString("yyyy-MM-dd hh:mm:ss")+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="测量结束时间： "+ endTime.toString("yyyy-MM-dd hh:mm:ss")+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="测量时长： "+ internalTime+"\r\n-----------------------------------------------------"+"\r\n";
    f.write(tempStr.toStdString().c_str());

    tempStr="CPU信息：\r\n最大值： "+ QString::number( cpuMax)+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="最小值： "+ QString::number( cpuMin )+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="平均值： "+ QString::number( cpuAve)+"\r\n"+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="内存信息：\r\n最大值： "+ QString::number( memMax )+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="最小值： "+ QString::number( memMin )+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="平均值： "+ QString::number( memAve )+"\r\n"+"\r\n";
    f.write(tempStr.toStdString().c_str());

    tempStr="电池温度信息：\r\n最大值： "+ QString::number( batteryMax)+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="最小值： "+ QString::number( batteryMin )+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="平均值： "+ QString::number( batteryAve)+"\r\n"+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="CPU温度信息：\r\n最大值： "+ QString::number( cpuTempMax )+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="最小值： "+ QString::number( cpuTempMin )+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="平均值： "+ QString::number( cpuTempAve )+"\r\n"+"\r\n";
    f.write(tempStr.toStdString().c_str());

    tempStr="Wifi流量总值： "+ QString::number( wifiTrafficstats)+"\r\n";
    f.write(tempStr.toStdString().c_str());
    tempStr="Mobile流量总值： "+ QString::number( mobileTrafficstats )+"\r\n";
    f.write(tempStr.toStdString().c_str());

    tempStr="======================================================================\r\n";
    f.write(tempStr.toStdString().c_str());

    f.close();
}

void PerformanceWidget::on_dataCompareBtn_clicked()
{
    if(ui->dataCompareBtn->text()=="<")
    {
        ui->dataCpmpareBtn_2->setFixedWidth(240);
        ui->dataCompareBtn->setText(">");

        dataWidget->show();
        int y1=ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height();
        int y2=ui->mobilePlot->mapToGlobal(QPoint(0,0)).y()+ui->mobilePlot->height();
        dataWidget->setFixedHeight(y2-y1);
        dataWidget->move(ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).x()-160, ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height());
        dataWidget->raise();

        ui->tempLabel->setEnabled(false);
        ui->trafficLabel->setEnabled(false);
        ui->performanceLabel->setEnabled(false);
    }
    else if(ui->dataCompareBtn->text()==">")
    {
        ui->dataCpmpareBtn_2->setFixedWidth(80);
        ui->dataCompareBtn->setText("<");
        dataWidget->close();

        //比对结束时先把1、2两通道的数据清空，但不清空画布
        memNum.clear();
        cpuNum.clear();
        memTime.clear();
        cpuTime.clear();
        memTimeInt.clear();
        cpuTimeInt.clear();
        memThres.clear();
        cpuThres.clear();

        batteryNum.clear();
        cpuTempNum.clear();
        batteryTime.clear();
        cpuTempTime.clear();
        batteryTimeInt.clear();
        cpuTempTimeInt.clear();
        batteryThres.clear();
        cpuTempThres.clear();

        wifiNum.clear();
        mobileNum.clear();
        wifiTime.clear();
        mobileTime.clear();
        wifiTimeInt.clear();
        mobileTimeInt.clear();
        wifiThres.clear();
        mobileThres.clear();
    }
}

void PerformanceWidget::RecieveCompareData(QVector<QVector<QString>> qMemory_data_Vector,QVector<QVector<QString>> qMemory_time_Vector,
                                           QVector<QVector<QString>> qCpu_data_Vector,QVector<QVector<QString>> qCpu_time_Vector,
                                           QVector<QVector<QString>> qCputemp_data_Vector,QVector<QVector<QString>> qCputemp_time_Vector,
                                           QVector<QVector<QString>> qBatterytemp_data_Vector,QVector<QVector<QString>> qBatterytemp_time_Vector,
                                           QVector<QVector<QString>> qWifi_data_Vector,QVector<QVector<QString>> qWifi_time_Vector,
                                           QVector<QVector<QString>> qMobile_data_Vector,QVector<QVector<QString>> qMobile_time_Vector)
{
    compareModal=true;
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
    for(int i=0;i<9;i++)
    {
        ui->memPlot->graph(i)->clearData();
        ui->cpuPlot->graph(i)->clearData();
        ui->batteryPlot->graph(i)->clearData();
        ui->cpuTempPlot->graph(i)->clearData();
        ui->wifiPlot->graph(i)->clearData();
        ui->mobilePlot->graph(i)->clearData();
    }
    ui->memPlot->replot();
    ui->cpuPlot->replot();
    ui->batteryPlot->replot();
    ui->cpuPlot->replot();
    ui->wifiPlot->replot();
    ui->mobilePlot->replot();

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
            ui->memPlot->yAxis->setRange(mem_min*0.9,mem_max*1.25);
            ui->memPlot->xAxis->setRange(Memory_time_int_Vector[0],mem_time_max+60);
            //设置时间轴的间隔显示，大概四五个时间点
            double internalForTime=mem_time_max-Memory_time_int_Vector[0];
            if(internalForTime>60*1.5)
            {
                int num=(int)(internalForTime/4)/30;
                internalForTime=(num+1)*30;
                ui->memPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
            }
            ui->memPlot->graph(i+2)->setData(Memory_time_int_Vector,Memory_data_Vector);
            ui->memPlot->replot();
        }
    }

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
            ui->cpuPlot->yAxis->setRange(cpu_min*0.9,cpu_max*1.25);
            ui->cpuPlot->xAxis->setRange(Cpu_time_int_Vector[0],cpu_time_max+60);
            //设置时间轴的间隔显示，大概四五个时间点
            double internalForTime=cpu_time_max-Cpu_time_int_Vector[0];
            if(internalForTime>60*1.5)
            {
                int num=(int)(internalForTime/4)/30;
                internalForTime=(num+1)*30;
                ui->cpuPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
            }
            ui->cpuPlot->graph(i+2)->setData(Cpu_time_int_Vector,Cpu_data_Vector);
            ui->cpuPlot->replot();
        }
    }

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
            ui->batteryPlot->yAxis->setRange(battery_min*0.9,battery_max*1.25);
            ui->batteryPlot->xAxis->setRange(Batterytemp_time_int_Vector[0],battery_time_max+60);
            //设置时间轴的间隔显示，大概四五个时间点
            double internalForTime=battery_time_max-Batterytemp_time_int_Vector[0];
            if(internalForTime>60*1.5)
            {
                int num=(int)(internalForTime/4)/30;
                internalForTime=(num+1)*30;
                ui->batteryPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
            }
            ui->batteryPlot->graph(i+2)->setData(Batterytemp_time_int_Vector,Batterytemp_data_Vector);
            ui->batteryPlot->replot();
        }
    }

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
            ui->cpuTempPlot->yAxis->setRange(cputemp_min*0.9,cputemp_max*1.25);
            ui->cpuTempPlot->xAxis->setRange(Cputemp_time_int_Vector[0],cputemp_time_max+60);
            //设置时间轴的间隔显示，大概四五个时间点
            double internalForTime=cputemp_time_max-Cputemp_time_int_Vector[0];
            if(internalForTime>60*1.5)
            {
                int num=(int)(internalForTime/4)/30;
                internalForTime=(num+1)*30;
                ui->cpuTempPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
            }
            ui->cpuTempPlot->graph(i+2)->setData(Cputemp_time_int_Vector,Cputemp_data_Vector);
            ui->cpuTempPlot->replot();
        }
    }

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
            internal=QDateTime::fromString(firstOne,"yyyy-MM-dd hh:mm:ss").toTime_t()-QDateTime::fromString(temp0,"yyyy-MM-dd hh:mm:ss").toTime_t();
        }

        for(int j=0;j<qWifi_time_Vector.at(i).count();j++)
        {
            Wifi_time_int_Vector.append(QDateTime::fromString(temp+qWifi_time_Vector.at(i).at(j),"yyyy-MM-dd hh:mm:ss").toTime_t()-internal);
            Wifi_data_Vector.append(qWifi_data_Vector.at(i).at(j).toDouble());
            //计算数据轴的最大最小值
            if(wifi_min>qWifi_data_Vector.at(i).at(j).toDouble())
                wifi_min=qWifi_data_Vector.at(i).at(j).toDouble();
            if(wifi_max<qWifi_data_Vector.at(i).at(j).toDouble())
                wifi_max=qWifi_data_Vector.at(i).at(j).toDouble();
        }
        //设置轴范围，如不设置，不显示图
        if(Wifi_time_int_Vector.length()>0)
        {
            if(wifi_time_max<Wifi_time_int_Vector[Wifi_time_int_Vector.length()-1])
                wifi_time_max=Wifi_time_int_Vector[Wifi_time_int_Vector.length()-1];
            ui->wifiPlot->yAxis->setRange(wifi_min*0.9,wifi_max*1.25);
            ui->wifiPlot->xAxis->setRange(Wifi_time_int_Vector[0],wifi_time_max+60);
            //设置时间轴的间隔显示，大概四五个时间点
            double internalForTime=wifi_time_max-Wifi_time_int_Vector[0];
            if(internalForTime>60*1.5)
            {
                int num=(int)(internalForTime/4)/30;
                internalForTime=(num+1)*30;
                ui->wifiPlot->xAxis->setTickStep(internalForTime); // 一天的秒数
            }
            ui->wifiPlot->graph(i+2)->setData(Wifi_time_int_Vector,Wifi_data_Vector);
            ui->wifiPlot->replot();
        }
    }

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
            ui->mobilePlot->yAxis->setRange(mobile_min*0.9,mobile_max*1.25);
            ui->mobilePlot->xAxis->setRange(Mobile_time_int_Vector[0],mobile_time_max+60);
            //设置时间轴的间隔显示，大概四五个时间点
            double internalForTime=mobile_time_max-Mobile_time_int_Vector[0];
            if(internalForTime>60*1.5)
            {
                int num=(int)(internalForTime/4)/30;
                internalForTime=(num+1)*30;
                ui->mobilePlot->xAxis->setTickStep(internalForTime); // 一天的秒数
            }
            ui->mobilePlot->graph(i+2)->setData(Mobile_time_int_Vector,Mobile_data_Vector);
            ui->mobilePlot->replot();
        }
    }
}

void PerformanceWidget::moveEvent(QMoveEvent *event)
{
    if(ui->dataCompareBtn->text()==">")
    {
        int y1=ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height();
        int y2=ui->mobilePlot->mapToGlobal(QPoint(0,0)).y()+ui->mobilePlot->height();
        dataWidget->setFixedHeight(y2-y1);
        dataWidget->move(ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).x(), ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height());
        dataWidget->raise();
    }
}

void PerformanceWidget::resizeEvent(QResizeEvent *event)
{
    if(ui->dataCompareBtn->text()==">")
    {
        int y1=ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height();
        int y2=ui->mobilePlot->mapToGlobal(QPoint(0,0)).y()+ui->mobilePlot->height();
        dataWidget->setFixedHeight(y2-y1);
        dataWidget->move(ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).x(), ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height());
        dataWidget->raise();
    }
}

void PerformanceWidget::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange)
    {
        return;
    }
    if(this->windowState().operator &(Qt::WindowMinimized))
    {
        if(ui->dataCompareBtn->text()==">")
        {
            ui->dataCpmpareBtn_2->setFixedWidth(80);
            ui->dataCompareBtn->setText("<");
            dataWidget->close();
        }
    }
}

void PerformanceWidget::compareBcak()
{
    if(ui->dataCompareBtn->text()==">")
    {
        if(!dataWidget->hasFocus())
        {
            ui->dataCpmpareBtn_2->setFixedWidth(80);
            ui->dataCompareBtn->setText("<");
            dataWidget->close();
        }
    }
}

void PerformanceWidget::on_dataCpmpareBtn_2_clicked()
{
    if(ui->dataCompareBtn->text()=="<")
    {
        ui->dataCpmpareBtn_2->setFixedWidth(240);
        ui->dataCompareBtn->setText(">");

        dataWidget->show();
        int y1=ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height();
        int y2=ui->mobilePlot->mapToGlobal(QPoint(0,0)).y()+ui->mobilePlot->height();
        dataWidget->setFixedHeight(y2-y1);
        dataWidget->move(ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).x()-160, ui->dataCompareBtn->mapToGlobal(QPoint(0,0)).y()+ui->dataCompareBtn->height());
        dataWidget->raise();

        ui->tempLabel->setEnabled(false);
        ui->trafficLabel->setEnabled(false);
        ui->performanceLabel->setEnabled(false);
    }
    else
    {
        dataWidget->raise();
    }
}

void PerformanceWidget::RecieveWindowDeactive()
{
    compareBcak();
}


void PerformanceWidget::createExcel()
{
    ExcelController::Controller *controller=new ExcelController::Controller;

    controller->setMark("performance");
    controller->setXlsxSaveDir(fileNameForReport);
    controller->setStartTime(qStartTime);
    controller->setDeviceId(deviceName);
    controller->setMemWarningValue(strMemThres);
    controller->setCpuWarningValue(strCpuThres);
    controller->setCpuTempWarningValue(strCpuTempThres);
    controller->setBatteryTempWarningValue(strBatteryThres);
    controller->setWifiWarningValue(strWifiThres);
    controller->setMobileWarningValue(strMobileThres);
    controller->setLunchTime(qLunchTime);
    controller->setFrames(qFrames);

    controller->setPackageName(packageName);
    controller->setStatiList(qStatisticsDataList);

    connect(controller,SIGNAL(sendWorkerFinished()),this,SLOT(receiveWorkerFinished()));
    controller->doController();
}

void PerformanceWidget::receiveWorkerFinished()
{
    QMessageBox::information(NULL,"提示","excel生成完毕");
}


void PerformanceWidget::getXXX()
{
    getFrames();
}


void PerformanceWidget::getFrames()
{
    if(packageName=="")
    {
        qFrames="手机";
    }
    else
    {
        QString cmdLine="adb -s "+deviceName+" shell dumpsys gfxinfo " + packageName;
        QThreadPool *pool=new QThreadPool;


        using FramesController::Controller;
        Controller *controllerInstance=new Controller;

        controllerInstance->setAutoDelete(true);
        controllerInstance->setDeviceId(deviceName);
        controllerInstance->setCmdLine(cmdLine);

        connect(controllerInstance,SIGNAL( sendResult(const QString &,const QString &) ),this,SLOT(receiveFramesResult(const QString &,const QString &) )  ,Qt::DirectConnection  );

        pool->start(controllerInstance);
    }
}

void PerformanceWidget::receiveFramesResult(const QString &arg_deviceId,const QString &arg_text)
{
    qFrames=arg_text;
}


void PerformanceWidget::getLunchTime()
{
    QString cmdLine="cmd /c adb -s " + deviceName + " shell dumpsys activity top | findstr ACTIVITY";

    AdbShellController::Controller *controller=new AdbShellController::Controller;
    controller->setMark("getAll","isActive");
    controller->setCmdLine(cmdLine);
    connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
    controller->doController();
}

void PerformanceWidget::receiveWorkerResult1(const QString&arg_str,const QString&arg_mark)
{
    if(arg_mark=="isActive")
    {
        if(arg_str.contains(packageName))
        {
        }
        else
        {
            QString mFile=gConfigDir + QDir::separator() + "lunchActivity.txt";
            QString mLunchActivity=Helper::getPackageName(mFile);
            QString mCmdLine="adb -s " + deviceName + " shell am start -W -n " + mLunchActivity;
            AdbShellController::Controller *controller=new AdbShellController::Controller;
            controller->setMark("getAll","lunchTime");
            controller->setCmdLine(mCmdLine);
            connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult2(const QString&,const QString&)));
            controller->doController();
        }
    }
}

void PerformanceWidget::receiveWorkerResult2(const QString&arg_str,const QString&arg_mark)
{
    if(arg_mark=="lunchTime")
    {
        qLunchTime=arg_str;
        bool ok;
        int dec=qLunchTime.toInt(&ok, 10);
        float floattime=dec*0.001;
        qLunchTime=QString::number(floattime, 'f', 2);
    }
}

