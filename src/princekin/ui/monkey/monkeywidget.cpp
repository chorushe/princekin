#include "monkeywidget.h"
#include "ui_monkeywidget.h"
#include <QScrollBar>

QString countStr=" 100";
QString logLevelStr=" -v";
QString seedStr="";
QString throttleStr=" --throttle 100";
QString touchStr="";
QString motionStr="";
QString trackBallStr="";
QString syskeysStr="";
QString navStr="";
QString majorNavStr="";
QString appswitchStr="";
QString flipStr="";
QString pinchZoomStr="";
QString anyeventStr="";

QString iCrashesStr="";
QString iTimeoutsStr="";
QString iSecurityStr="";
QString dbgNoEventStr="";
QString killProcessStr="";
QString monitorStr="";
QString waitDbgStr="";
QString hprofStr="";
QString classStr="";

QString filePath="";
QString filePathDate="";
QString filePathDevice="";
QString monkeyFilePath="";

QString cmdAdvStr="";


MonkeyWidget::MonkeyWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonkeyWidget)
{
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));//写入logcat的汉字是乱码，加这一条可改变编码方式
    createToolBar();
    setWindowTitle("稳定性测试");
    isMemOverThres=false;
    isCpuOverThres=false;
    isBatteryOverThres=false;
    isCpuTempOverThres=false;
    isWifiOverThres=false;
    isMobileOverThres=false;

    ui->splitter->setStretchFactor(0,4);
    ui->splitter->setStretchFactor(1,6);
    ui->splitter_3->setStretchFactor(0,3);
    ui->splitter_3->setStretchFactor(1,7);

    setupDialog=new MonkeySetUp;
    connect(setupDialog,SIGNAL(SendData(QVariant)),this,SLOT(RecieveData(QVariant)));

    pLogcat=new QProcess(this);

    connect(settingButton,SIGNAL(clicked(bool)),this,SLOT(ShowSetupWindow()));
    connect(pLogcat,SIGNAL(readyReadStandardOutput()),this,SLOT(ReadOutputLogcat()));

    connect(this,SIGNAL(sendStateChange(bool)),setupDialog,SLOT(stateChanged(bool)));
    connect(this,SIGNAL(sendSettingSig(bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)),setupDialog,SLOT(recieveSettingSig(bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)));

    proLogcat=new QProcess(this);
    connect(proLogcat,SIGNAL(readyReadStandardOutput()),this,SLOT(RecieveLogcatOutput()));

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
    ui->logTextEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                        "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->packageListView->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;width: 10px;}"
                                                              "QScrollBar::handle:horizontal {background: grey;border: 3px solid grey;border-radius:5px;}");
    //on_equipBtn_clicked();
    packageModel = new QStringListModel(packageList);
    equipModel = new QStringListModel(equipList);

    logKeyWordList.append("ANR");
    logKeyWordList.append("CRASH");
    logKeyWordList.append("Exception");
}

MonkeyWidget::~MonkeyWidget()
{
    delete ui;
}

void MonkeyWidget::createToolBar()
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

void MonkeyWidget::showInit()
{
    emit sendSettingSig(gisMemCheck,gisCpuCheck,gisBatteryCheck,gisCpuTempCheck,gisWifiCheck,gisMobileCheck,gMemThres,gCpuThres,gBatteryThres,gCpuTempThres,gWifiThres,gMobileThres);
    //ShowEquip();
    isMemCheck=gisMemCheck;
    isCpuCheck=gisCpuCheck;
    isBatteryCheck=gisBatteryCheck;
    isCpuTempCheck=gisCpuTempCheck;
    isWifiCheck=gisWifiCheck;
    isMobileCheck=gisMobileCheck;
    memThres=gMemThres;
    cpuThres=gCpuThres;
    batteryThres=gBatteryThres;
    cpuTempThres=gCpuTempThres;
    wifiThres=gWifiThres;
    mobileThres=gMobileThres;
    filePath=gWorkSpace;
}

void MonkeyWidget::on_equipBtn_clicked()//ShowEquip
{
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
    //ui->packageCombo->setItemText(0,"");//在没关设备的情况下给包名的第一个清空

    QModelIndex indexEquip=ui->equipListView->model()->index(0,0);
    ui->equipListView->setCurrentIndex(indexEquip);

    deviceName=ExeCmd::getDeviceSN(indexEquip.data().toString());
    if(deviceName!="")
    {
        if(ui->packageCombo->count()==3)
            ui->packageCombo->insertItem(0,"");
        ui->packageCombo->setCurrentIndex(0);
        ui->packageCombo->setItemText(0,indexEquip.data().toString());
    }
    //else
    //    ui->packageCombo->setCurrentIndex(0);
}

void MonkeyWidget::on_packageCombo_currentIndexChanged(int index)//ShowPackage
{
    if(numForPackageIndexChange<3)
        numForPackageIndexChange++;
    if(deviceName=="")
    {
        if(numForPackageIndexChange!=1)
            QMessageBox::information(this,tr("提示"),tr("请选择一个设备"));
        return ;
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
    packageModel->setStringList (packageList);

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
    packageName="";
}

void MonkeyWidget::on_equipListView_clicked(const QModelIndex &index)//equipSelect
{
    QModelIndex indexEquip=ui->equipListView->currentIndex();
    deviceName=ExeCmd::getDeviceSN(indexEquip.data().toString());
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

void MonkeyWidget::on_packageListView_clicked(const QModelIndex &index)//packageSelect
{
    QModelIndex indexPackage=ui->packageListView->currentIndex();
    packageName=indexPackage.data().toString();
}


//*****************20170315*****************//
void MonkeyWidget::on_startBtn_clicked()//StartMonkey
{
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

        isMemOverThres=false;
        isCpuOverThres=false;
        isWifiOverThres=false;
        isMobileOverThres=false;
        isBatteryOverThres=false;
        isCpuTempOverThres=false;

        CreateRootFiles();
        CreateMonkeyFiles();

        //*****************20170505*****************//
        qStatisticsDataList.clear();
        //*****************20170505*****************//


        if(cmdAdvStr!="")//判断自定义monkey命令中是否有内容
        {
            cmdStr=cmdAdvStr;
            QString temp="adb -s ";
            int num=cmdAdvStr.indexOf(temp);
            if(num!=-1)//查看命令中是否有设备名，如果有，提取出来，后面好在根目录下建立文件夹
            {
                QString amdAdvStrMid=cmdAdvStr.mid(num+temp.length()).trimmed();
                temp=" ";
                num=amdAdvStrMid.indexOf(temp);
                deviceName=amdAdvStrMid.left(num);
            }
            else//如果没有就在设备列表里取第一个设备，当然这样的命令会出错
            {
                QProcess p(0);
                p.start("cmd");
                p.waitForStarted();
                p.write("adb devices\n");
                p.closeWriteChannel();
                p.waitForFinished();
                QString result=QString::fromLocal8Bit(p.readAllStandardOutput());

                //去掉前面开头的
                QString tmp="List of devices attached";
                int num=result.indexOf(tmp)+tmp.length();
                result=result.mid(num).trimmed();

                tmp="\tdevice";
                num=result.indexOf(tmp);
                deviceName=result.left(num);
            }

            ui->startBtn->setIcon(QIcon(":/stop.png"));
            isStartFlag=true;
            emit sendStateChange(true);
            //deviceName="";//使用完暂时的devicename要把他清除，否则会对后续操作有影响
        }
        else//判断自定义monkey命令中没有内容
        {
            if(deviceName=="")
            {
                QMessageBox::information(this,"提示","请选择一个设备");
                return;
            }

            if(packageName.trimmed()=="")
            {
                qLunchTime="手机";
                cmdStr="adb -s "+deviceName+" shell monkey"+logLevelStr;
            }
            else
            {
                if(packageName!=qOldPackageName)
                {
                    qLunchTime="";
                    qFrames="";
                    cmdStr="adb -s "+deviceName+" shell monkey"+logLevelStr+" -p " + packageName.trimmed() ;

                }
                else
                {
                    //*****************20170505*****************//
                    getLunchTime();
                    //*****************20170505*****************//
                    cmdStr="adb -s "+deviceName+" shell monkey"+logLevelStr+" -p " + packageName.trimmed() ;

                }


            }

            cmdStr += classStr;
            cmdStr += seedStr + touchStr + motionStr + trackBallStr + syskeysStr + navStr ;
            cmdStr += majorNavStr + appswitchStr + flipStr + pinchZoomStr + anyeventStr + throttleStr ;
            cmdStr += iCrashesStr + iTimeoutsStr + iSecurityStr + dbgNoEventStr + hprofStr + killProcessStr + monitorStr + waitDbgStr + countStr + "\n";

            ui->startBtn->setIcon(QIcon(":/stop.png"));
            isStartFlag=true;
            emit sendStateChange(true);
        }

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
            connect(this,SIGNAL(monkeyStop()),worker,SLOT(stopStatistics()));

            connect(worker,SIGNAL(sendStatisticsData(QStringList)),this,SLOT(ReadStatisticsData(QStringList)));

            worker->moveToThread(workerThread);

            workerThread->start();
        }

        qDebug()<<cmdStr;

        MonkeyWorker *worker=new MonkeyWorker;
        worker->cmdStr=cmdStr;
        QThread *workerThread=new QThread(this);
        connect(workerThread,SIGNAL(started()),worker,SLOT(startMonkey()));
        connect(worker,SIGNAL(monkeyStop()),workerThread,SLOT(quit()));
        connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));

        connect(worker,SIGNAL(readOutputFromWorker(QString)),this,SLOT(ReadOutput(QString)));
        connect(worker,SIGNAL(readErrorFromWorker(QString)),this,SLOT(ReadError(QString)));
        connect(worker,SIGNAL(monkeyStop()),this,SLOT(monkeyNotRunning()));

        worker->moveToThread(workerThread);
        workerThread->start();

        if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
        {
            assist=new AssistMeasure();
            assist->deviceModel=ui->equipListView->currentIndex().data().toString();
            assist->packageName=this->packageName;
            assist->isCpuTempCheck=this->isCpuTempCheck;
            assist->isBatteryCheck=this->isBatteryCheck;
            assist->isMemCheck=this->isMemCheck;
            assist->isCpuCheck=this->isCpuCheck;
            assist->isWifiCheck=this->isWifiCheck;
            assist->isMobileCheck=this->isMobileCheck;
            assist->memThres=this->memThres;
            assist->cpuThres=this->cpuThres;
            assist->batteryThres=this->batteryThres;
            assist->cpuTempThres=this->cpuTempThres;
            assist->wifiThres=this->wifiThres;
            assist->mobileThres=this->mobileThres;
            connect(assist,SIGNAL(sendMemOverThresSignal(bool,QString)),this,SLOT(RecieveMemOverThresSignal(bool,QString)));
            connect(assist,SIGNAL(sendCpuOverThresSignal(bool,QString)),this,SLOT(RecieveCpuOverThresSignal(bool,QString)));
            connect(assist,SIGNAL(sendBatteryOverThresSignal(bool,QString)),this,SLOT(RecieveBatteryOverThresSignal(bool,QString)));
            connect(assist,SIGNAL(sendCpuTempOverThresSignal(bool,QString)),this,SLOT(RecieveCpuTempOverThresSignal(bool,QString)));
            connect(assist,SIGNAL(sendWifiOverThresSignal(bool,QString)),this,SLOT(RecieveWifiOverThresSignal(bool,QString)));
            connect(assist,SIGNAL(sendMobileOverThresSignal(bool,QString)),this,SLOT(RecieveMobileOverThresSignal(bool,QString)));
            assist->StartMeasure();
        }

        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd_hh-mm-ss");
        //建立logcat文件,有监测需求时才建立文件
        if(isMemCheck||isCpuCheck||isBatteryCheck||isCpuTempCheck||isWifiCheck||isMobileCheck)
        {
            fLogcat=new QFile( filePathDevice+"/bugreport/MonkeyErrorReport"+current_date+".txt" );
            if ( !fLogcat->exists())
                fLogcat->open( QIODevice::WriteOnly );
        }


        catchLogcat();

        ui->equipBtn->setEnabled(false);
        ui->equipListView->setEnabled(false);
        ui->packageCombo->setEnabled(false);
        ui->packageListView->setEnabled(false);

        //*****************20170505*************************//
        getXXX();
        //*****************20170505*************************//
    }
    else
    {
        killMonkey();
    }
}

void MonkeyWidget::ReadStatisticsData(QStringList statistics)
{
    //*****************20170606*****************//
    qStatisticsDataList=statistics;
    if(gMonkeyNumber!=0)
    {
        if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
        {
            createExcel();
        }
        else
        {
            createBaseExcel();
        }
    }

    //*****************20170606*****************//
}

//*****************20170315*****************//


void MonkeyWidget::on_clearBtn_clicked()//ClearText
{
    ui->logTextEdit->clear();
}

void MonkeyWidget::catchLogcat()
{
    CreateLogcatFiles();
    ExeCmd::runCmd("adb -s "+deviceName+" shell logcat -c");
    cmdStr="adb -s "+deviceName+" shell logcat -v time -b main -b system\n";
    if(pLogcat->state())
        pLogcat->write(cmdStr.toLatin1());
    else
        pLogcat->start("cmd", QStringList()<<"/c"<< cmdStr );
}


void MonkeyWidget::exportBugreport()//BugReportControl
{
    CreateBugReportFiles();

    cmdStr="adb -s "+deviceName+" bugreport\n";

    MonkeyWorker *worker1=new MonkeyWorker;
    worker1->cmdStr=cmdStr;
    QThread *workerThread1=new QThread(this);
    connect(workerThread1,SIGNAL(started()),worker1,SLOT(startMonkey()));
    connect(worker1,SIGNAL(monkeyStop()),workerThread1,SLOT(quit()));
    connect(workerThread1,SIGNAL(finished()),worker1,SLOT(deleteLater()));
    connect(worker1,SIGNAL(readOutputFromWorker(QString)),this,SLOT(ReadOutputBugreport(QString)));
    connect(worker1,SIGNAL(monkeyStop()),this,SLOT(BugReportNotRunning()));

    worker1->moveToThread(workerThread1);
    workerThread1->start();

    cmdStr="adb -s "+deviceName+" shell dumpsys\n";

    MonkeyWorker *worker=new MonkeyWorker;
    worker->cmdStr=cmdStr;
    QThread *workerThread=new QThread(this);
    connect(workerThread,SIGNAL(started()),worker,SLOT(startMonkey()));
    connect(worker,SIGNAL(monkeyStop()),workerThread,SLOT(quit()));
    connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));
    connect(worker,SIGNAL(readOutputFromWorker(QString)),this,SLOT(ReadOutputDumpsys(QString)));
    connect(worker,SIGNAL(monkeyStop()),this,SLOT(DumpsysNotRunning()));

    worker->moveToThread(workerThread);
    workerThread->start();

    if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
    {
        if(assist!=NULL)
        {
            if(gMonkeyNumber==0)
            {
                createExcel();
            }

        }
    }
    else
    {
        if(gMonkeyNumber==0)
        {
            createBaseExcel();
        }
    }
}


void MonkeyWidget::RecieveDevicesSigal(QStringList devicesList)
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
        //ui->packageCombo->setCurrentIndex(0);
        packageModel->setStringList( QStringList());//清空包名列表
        ui->packageListView->setModel(packageModel);
        deviceName="";
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

void MonkeyWidget::ShowSetupWindow()
{
    //setupDialog->setWindowModality(Qt::ApplicationModal);
    setupDialog->setAttribute(Qt::WA_QuitOnClose,false);
    setupDialog->show();
    setupDialog->raise();
}

void MonkeyWidget::RecieveData(QVariant var)
{
    signalMonkeyData_s signalData;
    signalData=var.value<signalMonkeyData_s>();

    logKeyWordList.clear();
    logKeyWordList=signalData.logKeyWords.split(";");
    if(signalData.logKeyWords=="")
        logKeyWordList.clear();
    if(signalData.data.cmdMonkeyStr!="")
    {
        cmdAdvStr=signalData.data.cmdMonkeyStr;
        return;
    }
    cmdAdvStr="";
    switch(signalData.data.logLevel)
    {
    case 0:
        logLevelStr="";
        break;
    case 1:
        logLevelStr=" -v";
        break;
    case 2:
        logLevelStr=" -v -v";
        break;
    case 3:
        logLevelStr=" -v -v -v";
        break;
    default:
        logLevelStr=" -v";
        break;
    }
    if(signalData.data.count)
        countStr=" "+signalData.data.countNum;
    else countStr="";
    if(signalData.data.throttle)
    {
        throttleStr=" --throttle "+signalData.data.throttleNum;
        if(signalData.data.random)
            throttleStr+=" --randomize-throttle";
    }
    else throttleStr="";
    if(signalData.data.seed)
        seedStr=" -s "+signalData.data.seedNum;
    else seedStr="";

    //事件百分比
    if(signalData.data.touch)
    {
        touchStr=" --pct-touch "+signalData.data.touchNum;
    }
    else touchStr="";
    if(signalData.data.motion)
    {
        motionStr=" --pct-motion "+signalData.data.motionNum;
    }
    else motionStr="";
    if(signalData.data.trackBall)
    {
        trackBallStr=" --pct-trackball "+signalData.data.trackBalNum;
    }
    else trackBallStr="";
    if(signalData.data.syskeys)
    {
        syskeysStr=" --pct-syskeys "+signalData.data.syskeysNum;
    }
    else syskeysStr="";
    if(signalData.data.nav)
    {
        navStr=" --pct-nav "+signalData.data.navNum;
    }
    else navStr="";
    if(signalData.data.majorNav)
    {
        majorNavStr=" --pct-majornav "+signalData.data.majorNavNum;
    }
    else majorNavStr="";
    if(signalData.data.appSwitch)
    {
        appswitchStr=" --pct-appswitch "+signalData.data.appSwitchNum;
    }
    else appswitchStr="";
    if(signalData.data.flip)
    {
        flipStr=" --pct-flip "+signalData.data.flipNum;
    }
    else flipStr="";
    if(signalData.data.pinchZoom)
    {
        pinchZoomStr=" --pct-pinchzoom "+signalData.data.pinchZoomNum;
    }
    else pinchZoomStr="";
    if(signalData.data.anyevent)
    {
        anyeventStr=" --pct-anyevent "+signalData.data.anyeventNum;
    }
    else anyeventStr="";
    if(signalData.data.className!="")
        classStr=" -c "+signalData.data.className;
    else classStr="";

    if(signalData.data.iCrashes)
    {
        iCrashesStr=" --ignore-crashes";
    }
    else iCrashesStr="";
    if(signalData.data.iSecurity)
    {
        iSecurityStr=" --ignore-security-exceptions";
    }
    else iSecurityStr="";
    if(signalData.data.iTimeouts)
    {
        iTimeoutsStr=" --ignore-timeouts";
    }
    else iTimeoutsStr="";

    if(signalData.data.dbgNoEvent)
        dbgNoEventStr=" --dbg-no-events";
    else
        dbgNoEventStr="";
    if(signalData.data.killProcess)
        killProcessStr=" --kill-process-after-error";
    else
        killProcessStr="";
    if(signalData.data.monitorNative)
        monitorStr=" --monitor-native-crashes";
    else
        monitorStr="";
    if(signalData.data.waitDbg)
        waitDbgStr=" --wait-dbg";
    else
        waitDbgStr="";
    if(signalData.data.hprof)
        hprofStr=" --hprof";
    else
        hprofStr="";


    this->isMemCheck=signalData.isMemCheck;
    this->isCpuCheck=signalData.isCpuCheck;
    this->isBatteryCheck=signalData.isBatteryCheck;
    this->isCpuTempCheck=signalData.isCpuTempCheck;
    this->isWifiCheck=signalData.isWifiCheck;
    this->isMobileCheck=signalData.isMobileCheck;

    this->memThres=signalData.memThres;
    this->cpuThres=signalData.cpuThres;
    this->batteryThres=signalData.batteryThres;
    this->cpuTempThres=signalData.cpuTempThres;
    this->wifiThres=signalData.wifiThres;
    this->mobileThres=signalData.mobileThres;

    this->xmlPaths=signalData.xmlPaths;
    this->isDebug=signalData.isDebug;

}

void MonkeyWidget::RecieveLogcatOutput()
{
    QString tempStr=QString::fromUtf8(proLogcat->readAllStandardOutput());
    if(fLogcat->isOpen())
        fLogcat->write(tempStr.toStdString().c_str());
}

void MonkeyWidget::RecieveMemOverThresSignal(bool flag,QString deviceName)
{
    isMemOverThres=flag;
}

void MonkeyWidget::RecieveCpuOverThresSignal(bool flag,QString deviceName)
{
    isCpuOverThres=flag;
}

void MonkeyWidget::RecieveWifiOverThresSignal(bool flag,QString deviceName)
{
    isWifiOverThres=flag;
}

void MonkeyWidget::RecieveMobileOverThresSignal(bool flag,QString deviceName)
{
    isMobileOverThres=flag;
}

void MonkeyWidget::RecieveBatteryOverThresSignal(bool flag,QString deviceName)
{
    isBatteryOverThres=flag;
}

void MonkeyWidget::RecieveCpuTempOverThresSignal(bool flag,QString deviceName)
{
    isCpuTempOverThres=flag;

    if(isBatteryOverThres||isCpuTempOverThres||isMemOverThres||isCpuOverThres||isWifiOverThres||isMobileOverThres)
    {
        qDebug()<<"超过阀值了";
        QString cmdStr="adb -s "+deviceName+" shell logcat -v time -b main -b system\n";
        if(proLogcat->state())
            proLogcat->write(cmdStr.toLatin1());
        else
            proLogcat->start("cmd", QStringList()<<"/c"<< cmdStr );
    }
    else
    {
        qDebug()<<"停止信号";
        ExeCmd::StopLogcat(deviceName);
    }
}


//*****************20170315*****************//
void MonkeyWidget::CreateRootFiles()
{
    qStartTime=Helper::getTime2("yyyyMMdd_hhmmss");
    filePathDevice=gWorkSpace+QDir::separator() + "report" + QDir::separator()+ "Monkey_" + deviceName + "_" + qStartTime;
    Helper::createPath(filePathDevice);
}

void MonkeyWidget::CreateMonkeyFiles()
{
    //建立monkey文件夹
    QString sDir=filePathDevice + QDir::separator() + "monkey";
    QDir *tempDir = new QDir;
    bool exist=tempDir->exists(sDir);
    if(!exist)
    {
        tempDir->mkpath(sDir);
    }

    //建立monkeylog文件
    QString current_date;
    monkeyFilePath=sDir + QDir::separator() + "MonkeyLog.txt";

    fileMonkeyLog=new QFile( monkeyFilePath );
    if ( !fileMonkeyLog->exists())
        fileMonkeyLog->open( QIODevice::WriteOnly );
    //建立monkeyreport文件
    monkeyFilePath=sDir + QDir::separator() + "MonkeyReport.txt";

    fileMonkeyReport=new QFile( monkeyFilePath );
    if ( !fileMonkeyReport->exists())
        fileMonkeyReport->open( QIODevice::WriteOnly );
    timeStart = QDateTime::currentDateTime();
    current_date = timeStart.toString("yyyy-MM-dd hh:mm:ss");
    QString writeData="monkey开始时间: "+current_date+"\r\n";
    fileMonkeyReport->write(writeData.toStdString().c_str());

    delete tempDir;
}
//*****************20170315*****************//

//*****************20170315*****************//
void MonkeyWidget::CreateLogcatFiles()
{
    //建立logcat文件夹
    QString sDir=filePathDevice + QDir::separator() + "logcat";
    QDir *tempDir = new QDir;
    bool exist=tempDir->exists(sDir);
    if(!exist)
    {
        tempDir->mkpath(sDir);
    }
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString logcatFilePath=sDir + QDir::separator() +"logcat_"+current_date_time.toString("yyyy-MM-dd_hh-mm-ss")+".txt";
    fileLogcat=new QFile( logcatFilePath );
    if ( !fileLogcat->exists())
        fileLogcat->open( QIODevice::WriteOnly );

    delete tempDir;
}
//*****************20170315*****************//

//*****************20170315*****************//
void MonkeyWidget::CreateBugReportFiles()
{
    //建立bugreport文件夹
    QString sDir=filePathDevice + QDir::separator() + "bugreport";
    QDir *tempDir = new QDir;
    bool exist=tempDir->exists(sDir);
    if(!exist)
    {
        tempDir->mkpath(sDir);
    }

    //建立bugreport/anr文件夹
    QString bugReportAnrPath=sDir + QDir::separator() + "anr";
    exist=tempDir->exists(bugReportAnrPath);
    if(!exist)
    {
        tempDir->mkpath(bugReportAnrPath);
    }
    QString bugReportAnrPath_1=sDir + QDir::separator() + "anr_1";
    exist=tempDir->exists(bugReportAnrPath_1);
    if(!exist)
    {
        tempDir->mkpath(bugReportAnrPath_1);
    }

    cmdStr="adb -s "+deviceName+" pull /data/anr/ "+ bugReportAnrPath +"/\n";
    MonkeyWorker *worker1=new MonkeyWorker;
    worker1->cmdStr=cmdStr;
    QThread *workerThread1=new QThread(this);
    connect(workerThread1,SIGNAL(started()),worker1,SLOT(startMonkey()));
    connect(worker1,SIGNAL(monkeyStop()),workerThread1,SLOT(quit()));
    connect(workerThread1,SIGNAL(finished()),worker1,SLOT(deleteLater()));

    worker1->moveToThread(workerThread1);
    workerThread1->start();

    cmdStr="adb -s "+deviceName+" pull /mnt/sdcard/anr/ "+ bugReportAnrPath_1 +"/\n";

    MonkeyWorker *worker=new MonkeyWorker;
    worker->cmdStr=cmdStr;
    QThread *workerThread=new QThread(this);
    connect(workerThread,SIGNAL(started()),worker,SLOT(startMonkey()));
    connect(worker,SIGNAL(monkeyStop()),workerThread,SLOT(quit()));
    connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));

    worker->moveToThread(workerThread);
    workerThread->start();


    //建立bugreport文件
    fileBugReport=new QFile( sDir + QDir::separator() + "bugreport.log" );
    if ( !fileBugReport->exists())
        fileBugReport->open( QIODevice::WriteOnly );

    //建立bugdumpsys文件
    fileDumpsys=new QFile( sDir + QDir::separator() + "dumpsys.txt" );
    if ( !fileDumpsys->exists())
        fileDumpsys->open( QIODevice::WriteOnly );

    reportFilePath=sDir + QDir::separator() +"report.txt";

    delete tempDir;
}
//*****************20170315*****************//


void MonkeyWidget::ReadOutput(QString str)
{
    recieveStr=str;
    bool errorFlag=false;
    for(int i=0;i<logKeyWordList.length();i++)
    {
        if(recieveStr.contains(logKeyWordList.at(i),Qt::CaseInsensitive))
        {
            ui->logTextEdit->append("<font color=red>"+recieveStr+"</font>");
            ui->logTextEdit->append("\r\n");
            if(logKeyWordList.at(i).contains("ANR",Qt::CaseInsensitive))
                ANRErrorStr+=recieveStr+"\r\n";
            else if(logKeyWordList.at(i).contains("CRASH",Qt::CaseInsensitive))
                CrashErrorStr+=recieveStr+"\r\n";
            else if(logKeyWordList.at(i).contains("Exception",Qt::CaseInsensitive))
                ExceptionErrorStr+=recieveStr+"\r\n";
            else
                OtherErrorStr+=recieveStr+"\r\n";
            errorFlag=true;
            break;
        }
    }
    if(!errorFlag)
    {
        if((isBatteryOverThres||isCpuTempOverThres||isMemOverThres||isCpuOverThres||isWifiOverThres||isMobileOverThres))
        {
            ui->logTextEdit->append("<font color=orange>"+recieveStr+"</font>");
            ui->logTextEdit->append("\r\n");
        }
        else
            ui->logTextEdit->append(recieveStr);
    }
    fileMonkeyLog->write(recieveStr.toStdString().c_str());
}

void MonkeyWidget::ReadError(QString recieveStr)
{
    ui->logTextEdit->append("<font color=red>"+recieveStr+"</font>");
    ui->logTextEdit->append("\r\n");
}

void MonkeyWidget::ReadOutputLogcat()
{
    QString tempStr=QString::fromUtf8(pLogcat->readAllStandardOutput());
    fileLogcat->write(tempStr.toStdString().c_str());
}

void MonkeyWidget::ReadOutputBugreport(QString tempStr)
{
    fileBugReport->write(tempStr.toStdString().c_str());
}

void MonkeyWidget::ReadOutputDumpsys(QString tempStr)
{
    fileDumpsys->write(tempStr.toStdString().c_str());
}

void MonkeyWidget::monkeyNotRunning()
{
    emit monkeyStop();

    fileMonkeyLog->close();
    timeEnd = QDateTime::currentDateTime();
    QString current_date = timeEnd.toString("yyyy-MM-dd hh:mm:ss");
    QString writeData="monkey结束时间: "+current_date+"\r\n";
    fileMonkeyReport->write(writeData.toStdString().c_str());
    QString timeSub = QString::number( timeStart.daysTo(timeEnd))+"天"
            +QString::number( timeStart.msecsTo(timeEnd)/3600000)+"小时"
            +QString::number(timeStart.msecsTo(timeEnd)%3600000/60000)+"分"
            +QString::number(timeStart.msecsTo(timeEnd)%60000/1000)+"秒";
    writeData="monkey执行时长: "+ timeSub + "\r\n---------------------------------------------------------------------------\r\n";

    //*****************20170315*****************//
    qMonkeyTime=timeSub;
    //*****************20170315*****************//

    fileMonkeyReport->write(writeData.toStdString().c_str());

    writeData="测试结果：\r\n";
    if(ANRErrorStr==""&&CrashErrorStr==""&&ExceptionErrorStr==""&&OtherErrorStr=="")
        ;
    else
    {
        writeData+="结果存在 ";
        if(ANRErrorStr!="")
            writeData+="ANR, ";
        if(CrashErrorStr!="")
            writeData+="Crash, ";
        if(ExceptionErrorStr!="")
            writeData+="异常, ";
        if(OtherErrorStr!="")
            writeData+="其他错误， ";
        writeData+="详情如下： \r\n";
    }

    fileMonkeyReport->write(writeData.toStdString().c_str());
    ui->logTextEdit->append(writeData);

    if(ANRErrorStr!="")
    {
        writeData="ANR： \r\n"+ANRErrorStr;
        fileMonkeyReport->write(writeData.toStdString().c_str());
        ui->logTextEdit->append(writeData);
    }
    if(CrashErrorStr!="")
    {
        writeData="Crash： \r\n"+CrashErrorStr;
        fileMonkeyReport->write(writeData.toStdString().c_str());
        ui->logTextEdit->append(writeData);
    }
    if(ExceptionErrorStr!="")
    {
        writeData="Exception： \r\n"+ExceptionErrorStr;
        fileMonkeyReport->write(writeData.toStdString().c_str());
        ui->logTextEdit->append(writeData);
    }
    if(OtherErrorStr!="")
    {
        writeData="Other： \r\n"+OtherErrorStr;
        fileMonkeyReport->write(writeData.toStdString().c_str());
        ui->logTextEdit->append(writeData);
    }
    if(recieveStr.contains("Monkey finished"))
    {
        writeData="Monkey 运行成功！\r\n";
        fileMonkeyReport->write(writeData.toStdString().c_str());
        ui->logTextEdit->append(writeData);
    }
    fileMonkeyReport->close();

    if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
    {
        if(assist!=NULL)
        {
            disconnect(assist,SIGNAL(sendMemOverThresSignal(bool,QString)),this,SLOT(RecieveMemOverThresSignal(bool,QString)));
            disconnect(assist,SIGNAL(sendCpuOverThresSignal(bool,QString)),this,SLOT(RecieveCpuOverThresSignal(bool,QString)));
            disconnect(assist,SIGNAL(sendBatteryOverThresSignal(bool,QString)),this,SLOT(RecieveBatteryOverThresSignal(bool,QString)));
            disconnect(assist,SIGNAL(sendCpuTempOverThresSignal(bool,QString)),this,SLOT(RecieveCpuTempOverThresSignal(bool,QString)));
            disconnect(assist,SIGNAL(sendWifiOverThresSignal(bool,QString)),this,SLOT(RecieveWifiOverThresSignal(bool,QString)));
            disconnect(assist,SIGNAL(sendMobileOverThresSignal(bool,QString)),this,SLOT(RecieveMobileOverThresSignal(bool,QString)));
            assist->StopMeasure();

        }
    }

    ExeCmd::StopLogcat(deviceName);
    sleep(1000);
    fileLogcat->close();
    if(fLogcat!=NULL)
    {
        fLogcat->close();
        if(fLogcat->size()==0)
            QFile::remove(fLogcat->fileName());
    }

    exportBugreport();

    setEnabledTrue();
}

void MonkeyWidget::BugReportNotRunning()
{
        fileBugReport->close();
        if(fileBugReport->size()==0)
        {
            fileBugReport->remove();
        }
}

void MonkeyWidget::DumpsysNotRunning()
{
    fileDumpsys->close();
    if(fileDumpsys->size()==0)
        fileDumpsys->remove();
}

void MonkeyWidget::sleep(unsigned int msec)
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void MonkeyWidget::closeEvent(QCloseEvent *e)
{
    if(isStartFlag)
    {
        killMonkey();
    }
}

void MonkeyWidget::setEnabledTrue()
{
    ui->startBtn->setIcon(QIcon(":/start.png"));
    isStartFlag=false;
    emit sendStateChange(false);
    ui->equipBtn->setEnabled(true);
    ui->equipListView->setEnabled(true);
    ui->packageCombo->setEnabled(true);
    ui->packageListView->setEnabled(true);
}

void MonkeyWidget::killMonkey()
{
    emit monkeyStop();

    ui->logTextEdit->append("正在终止进程......");

    QString cmdMomkey="adb -s "+deviceName+ " shell ps | grep monkey";
    QString result=ExeCmd::runCmd(cmdMomkey).trimmed();
    if(result.contains("monkey"))
    {
        QString pid="";
        QStringList mList=result.split(" ");
        for(int i=1;i<mList.count();i++)
        {
            if(!mList.at(i).isEmpty())
            {
                pid=mList.at(i);
                break;
            }
        }
        cmdMomkey="adb -s "+deviceName+" shell kill "+pid;
        ExeCmd::runCmd(cmdMomkey);
        sleep(2000);
        ui->logTextEdit->append("\nmonkey进程结束！");
    }
    else
    {
        ui->logTextEdit->append("\n未启动任何关于monkey的进程！");
    }
}

void MonkeyWidget::on_detailReportBtn_clicked()
{
    ExeCmd::openFolder(filePathDevice);
}



void MonkeyWidget::createExcel()
{
    ExcelController::Controller *controller=new ExcelController::Controller;

    controller->setMark("monkey");
    controller->setXlsxSaveDir(filePathDevice);
    controller->setStartTime(qStartTime);
    controller->setDeviceId(deviceName);
    controller->setLunchTime(qLunchTime);
    controller->setFrames(qFrames);

    controller->setMemWarningValue(memThres);
    controller->setCpuWarningValue(cpuThres);
    controller->setCpuTempWarningValue(cpuTempThres);
    controller->setBatteryTempWarningValue(batteryThres);
    controller->setWifiWarningValue(wifiThres);
    controller->setMobileWarningValue(mobileThres);

    controller->setMonkeyTime(qMonkeyTime);
    controller->setPackageName(packageName);
    controller->setStatiList(qStatisticsDataList);
    controller->setMeasureVector(assist->reportInfo);

    connect(controller,SIGNAL(sendWorkerFinished()),this,SLOT(receiveWorkerFinished()));
    controller->doController();
}

void MonkeyWidget::createBaseExcel()
{
    ExcelController::Controller *controller=new ExcelController::Controller;
    controller->setMark("base-monkey");
    controller->setXlsxSaveDir(filePathDevice);
    controller->setStartTime(qStartTime);
    controller->setDeviceId(deviceName);
    controller->setLunchTime(qLunchTime);
    controller->setFrames(qFrames);

    controller->setMemWarningValue(memThres);
    controller->setCpuWarningValue(cpuThres);
    controller->setCpuTempWarningValue(cpuTempThres);
    controller->setBatteryTempWarningValue(batteryThres);
    controller->setWifiWarningValue(wifiThres);
    controller->setMobileWarningValue(mobileThres);

    controller->setMonkeyTime(qMonkeyTime);
    controller->setPackageName(packageName);
    controller->setStatiList(qStatisticsDataList);
}

void MonkeyWidget::receiveWorkerFinished()
{
    QMessageBox::information(NULL,"提示","excel生成完毕");
}


void MonkeyWidget::getXXX()
{
    getFrames();
}


void MonkeyWidget::getFrames()
{
    if(packageName=="")
    {
        qFrames="手机";
    }
    else
    {
        QString cmdLine="adb -s " + deviceName + " shell dumpsys gfxinfo " + packageName;
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

void MonkeyWidget::receiveFramesResult(const QString &arg_deviceId,const QString &arg_text)
{
    qFrames=arg_text;
}

void MonkeyWidget::getLunchTime()
{
    QString cmdLine="cmd /c adb -s " + deviceName + " shell dumpsys activity top | findstr ACTIVITY";

    AdbShellController::Controller *controller=new AdbShellController::Controller;
    controller->setMark("getAll","isActive");
    controller->setCmdLine(cmdLine);
    connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
    controller->doController();
}

void MonkeyWidget::receiveWorkerResult1(const QString&arg_str,const QString&arg_mark)
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

void MonkeyWidget::receiveWorkerResult2(const QString&arg_str,const QString&arg_mark)
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
