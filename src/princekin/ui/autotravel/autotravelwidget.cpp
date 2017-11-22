#include "autotravelwidget.h"
#include "ui_autotravelwidget.h"

AutoTravelWidget::AutoTravelWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoTravelWidget)
{
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    createToolBar();
    setWindowTitle("遍历测试");
    qDebug()<<"d";
    ui->splitter->setStretchFactor(0,4);
    ui->splitter->setStretchFactor(1,6);
    ui->splitter_3->setStretchFactor(0,3);
    ui->splitter_3->setStretchFactor(1,7);

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

    setupDialog=new AutoTravelSetup();
    connect(this,SIGNAL(sendStateChange(bool)),setupDialog,SLOT(stateChanged(bool)));
    connect(settingButton,SIGNAL(clicked(bool)),this,SLOT(ShowSetupWindow()));
    connect(setupDialog,SIGNAL(SendData(QVariant)),this,SLOT(RecieveData(QVariant)));
    connect(this,SIGNAL(sendSettingSig(bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)),setupDialog,SLOT(recieveSettingSig(bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)));

    pLogcat=new QProcess(this);
    connect(pLogcat,SIGNAL(readyReadStandardOutput()),this,SLOT(RecieveLogcatOutput()));
    //CreatReportFolder();

    equipModel = new QStringListModel(equipList);
    packageModel = new QStringListModel(packageList);

    logKeyWordList.append(" ANR ");
    logKeyWordList.append("FATAL");
    //on_equipBtn_clicked();

    idBlackList=ParseXml::readConfigXmlForBlackList(gConfigDir + QDir::separator() + "config.xml");
    qDebug()<<"idblacklist length"<<idBlackList.count();
    for(int i=0;i<idBlackList.count();i++)
        idBlackString+=idBlackList.at(i)+"=";
    if(idBlackString.length()>1)
        idBlackString=idBlackString.left(idBlackString.length()-1);

    gTravelNumber=0;
}

AutoTravelWidget::~AutoTravelWidget()
{
    delete ui;
}

void AutoTravelWidget::createToolBar()
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

void AutoTravelWidget::showInit()
{
    emit sendSettingSig(gisMemCheck,gisCpuCheck,gisBatteryCheck,gisMobileCheck,gisWifiCheck,gisMobileCheck,gMemThres,gCpuThres,gBatteryThres,gCpuTempThres,gWifiThres,gMobileThres);
    isCpuTempCheck=gisCpuTempCheck;
    isBatteryCheck=gisBatteryCheck;
    isWifiCheck=gisWifiCheck;
    isMobileCheck=gisMobileCheck;
    isMemCheck=gisMemCheck;
    isCpuCheck=gisCpuCheck;

    batteryThres=gBatteryThres;
    cpuTempThres=gCpuTempThres;
    wifiThres=gWifiThres;
    mobileThres=gMobileThres;
    memThres=gMemThres;
    cpuThres=gCpuThres;

}

void AutoTravelWidget::ShowSetupWindow()
{
    //setupDialog->setWindowModality(Qt::ApplicationModal);
    setupDialog->setAttribute(Qt::WA_QuitOnClose,false);
    setupDialog->setWindowState(Qt::WindowActive);
    setupDialog->show();
    setupDialog->raise();
    setupDialog->move ((QApplication::desktop()->width() - setupDialog->width())/2,(QApplication::desktop()->height() - setupDialog->height())/2);
}

void AutoTravelWidget::RecieveDevicesSigal(QStringList devicesList)
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
        //ui->packageCombo->setItemText(0,"");
        //ui->packageCombo->setCurrentIndex(0);
        packageModel->setStringList( QStringList());
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
        if(deviceName!="")
            on_packageCombo_currentIndexChanged(0);
    }
}

void AutoTravelWidget::on_equipBtn_clicked()
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
    ui->packageCombo->setCurrentIndex(0);
    on_packageCombo_currentIndexChanged(0);
   // ui->packageCombo->setItemText(0,indexEquip.data().toString());
}

void AutoTravelWidget::on_packageCombo_currentIndexChanged(int index)
{
    if(deviceName=="")
    {
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
    /*case 0:
        flag=0;
        break;*/
    case 1:
        p.start(cmdStrAllPackages);
        flag=1;
        break;
    case 0:
        p.start(cmdStrThirdPackages);
        flag=1;
        break;
    case 2:
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

void AutoTravelWidget::on_equipListView_clicked(const QModelIndex &index)
{
    QModelIndex indexEquip=ui->equipListView->currentIndex();
    deviceName=ExeCmd::getDeviceSN(indexEquip.data().toString());
    //ui->packageCombo->setCurrentIndex(0);
   // ui->packageCombo->setItemText(0,indexEquip.data().toString());
    if(deviceName!="")
        on_packageCombo_currentIndexChanged(ui->packageCombo->currentIndex());
}

void AutoTravelWidget::on_packageListView_clicked(const QModelIndex &index)
{
    QModelIndex indexPackage=ui->packageListView->currentIndex();
    packageName=indexPackage.data().toString();
}

void AutoTravelWidget::on_startBtn_clicked()
{
    if(!isStartFlag)
    {
        if(deviceName=="")
        {
            QMessageBox::information(this,tr("提示"),tr("请选择一个设备"));
            return;
        }
        if(packageName=="")
        {
            QMessageBox::information(this,tr("提示"),tr("请选择一个应用"));
            return;
        }

        //*****************20170717*****************//
        qOldPackageName=Helper::getFirstLine(gConfigDir + QDir::separator() + "packageName.txt");
        gOldPackageName=qOldPackageName;
        bool b=Helper::isPackageName1(packageName);
        if(!b)
        {
            return;
        }
        qStatisticsDataList.clear();
        qTravelStartTime="";
        //*****************20170717*****************//

        //获取系统版本
        QString cmdLine="adb -s "+deviceName+" shell getprop ro.build.version.release";
        qDebug()<<cmdLine;
        systemStr=ExeCmd::runCmd(cmdLine);
        CreateReportFile();//创建日志文件


        isLogEnd=false;
        emit sendStateChange(true);
        ui->equipBtn->setEnabled(false);
        ui->equipListView->setEnabled(false);
        ui->packageCombo->setEnabled(false);
        ui->packageListView->setEnabled(false);
        settingButton->setEnabled(false);

        if(xmlPaths.count()>0)
        {
            QStringList tempList=ParseXml::readConfigXmlForIP_Port(gConfigDir + QDir::separator() + "config.xml");
            IP=tempList[0];
            Port=tempList[1];
            AssistBehaviorWorker *worker=new AssistBehaviorWorker;
            worker->xmlPaths=this->xmlPaths;
            worker->isDebugVersion=this->isDebug;
            worker->IP=this->IP;
            worker->Port=this->Port;
            QThread *workerThread=new QThread(this);
            connect(workerThread,SIGNAL(started()),worker,SLOT(startStatistics()));
            connect(worker,SIGNAL(sendStopSignal()),workerThread,SLOT(quit()));
            connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));
            connect(this,SIGNAL(travelStop()),worker,SLOT(stopStatistics()));

            connect(worker,SIGNAL(sendStatisticsData(QStringList)),this,SLOT(ReadStatisticsData(QStringList)));

            worker->moveToThread(workerThread);

            workerThread->start();
        }//统计开始应该在启动app之前

        QString killApp="adb -s "+deviceName+" shell am force-stop "+packageName;
        ExeCmd::runCmd(killApp);



        //*****************20170717*****************//
        if(packageName==qOldPackageName)
        {
            getLunchTime();
        }
        else
        {
            qLunchTime="";
            qFrames="";
            //如果选择的app不是预定的，那么不会自动启动APP，而如果之前已经启动，会在之前又杀死，所以在这里特殊启动一次 hechang 20170727
            QString startApp="adb -s "+deviceName+" shell monkey -p "+packageName+" -c android.intent.category.LAUNCHER 1";
            QString ti=ExeCmd::runCmd(startApp);
            if( ti.contains("monkey aborted")||ti.contains("device not found"))
            {
                qDebug()<<"启动应用失败，请重试！";
                ui->logTextEdit->append("启动应用失败，请重试！");
                //启动失败后return，但此时统计工作已经开始了，此处需发送停止统计信号
                emit travelStop();
                return;
            }
        }
        //*****************20170717*****************//
        ui->startBtn->setIcon(QIcon(":/stop.png"));
        isStartFlag=true;

        QString cmdStr="adb -s "+ deviceName +" push "+gConfigDir+QDir::separator()+"TravelTest.jar /data/local/tmp/\n";

        qDebug()<<cmdStr;
        ExeCmd::runCmd(cmdStr);
        if(idBlackString=="")
            idBlackString="NULL";
        cmdStr="adb -s "+ deviceName +" shell uiautomator runtest TravelTest.jar -e blackName-resouceId "+idBlackString+" -e time "+clickInternalTime+" --nohup -c com.sohu.princekin.autotravel.TravelClass#testTravel\n";


        TravelWorker *worker=new TravelWorker;
        worker->cmdStr=cmdStr;
        QThread *workerThread=new QThread(this);
        connect(workerThread,SIGNAL(started()),worker,SLOT(startTravel()));
        connect(worker,SIGNAL(travelStop()),workerThread,SLOT(quit()));
        connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));

        connect(worker,SIGNAL(readOutputFromWorker(QString)),this,SLOT(ReadOutput(QString)));
        connect(worker,SIGNAL(readErrorFromWorker(QString)),this,SLOT(ReadError(QString)));
        connect(worker,SIGNAL(travelStop()),this,SLOT(travelerNotRunning()));

        worker->moveToThread(workerThread);
        workerThread->start();

        QString cmdStrLogcat="adb -s "+deviceName+" shell logcat -c\n";
        ExeCmd::runCmd(cmdStrLogcat);
        cmdStrLogcat="adb -s "+deviceName+" shell logcat -v time -b main -b system\n";
        qDebug()<<cmdStrLogcat;
        if(pLogcat->state())
            pLogcat->write(cmdStrLogcat.toLatin1());
        else
            pLogcat->start("cmd", QStringList()<<"/c"<< cmdStrLogcat );



        if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
        {
            assist=new AssistMeasure();
            assist->deviceModel=ExeCmd::GetDeviceModel( deviceName )+" + "+deviceName;
            assist->packageName=packageName;

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

            assist->StartMeasure();
        }


        QString fileName=fileNameForReport+QDir::separator()+"log"+ QDir::separator()+"AutotravelErrorReport.txt";
        //建立logcat文件
     //   if(isMemCheck||isCpuCheck||isBatteryCheck||isCpuTempCheck||isWifiCheck||isMobileCheck)//是否测内存等都要抓logcat，因为有关键字的过滤，一直需要logcat
        {
            fileLogcat=new QFile(fileName);
            if ( !fileLogcat->exists())
                fileLogcat->open( QIODevice::WriteOnly );
        }

        getXXX();
    }
    else
    {
        StopUiautomatorByHand();

        //stopAutoTravel();

    }
}

void AutoTravelWidget::ReadStatisticsData(QStringList statistics)
{
    //*****************20170606*****************//
    qStatisticsDataList=statistics;
    if(gTravelNumber!=0)
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

QString AutoTravelWidget::getpsuid(const QString &arg_cmdline)
{
    QString mPid="";
    QProcess mProc;
    mProc.setReadChannelMode(QProcess::MergedChannels);
    mProc.start(arg_cmdline);
    mProc.waitForStarted();
    while(mProc.waitForFinished()==false)
    {
    }

    QString mStr;
    QStringList mSplitResult;


    while(mProc.canReadLine())
    {
        mStr=mProc.readLine();
        if(mStr.contains("uiautomator"))
        {
            mSplitResult=mStr.split(" ");
        }
    }
    mProc.close();

    int mLen=mSplitResult.count();
    for(int i=1;i<mLen;i++)
    {
        mStr=mSplitResult.at(i);
        mStr=mStr.trimmed();

        if(!mStr.isEmpty())
        {
            mPid=mStr;
            break;
        }
    }

    return mPid;
}

void AutoTravelWidget::StopUiautomatorByHand()
{
    QString tempFpath=QCoreApplication::applicationDirPath()+QDir::separator()+"stop.txt";

    QString pushPath="/data/local/tmp/local/stop.txt";
    QFile tempF(tempFpath);
    tempF.open(QIODevice::WriteOnly|QIODevice::Text);
    tempF.close();
    QString cmd = "adb -s "+deviceName+" push "+tempFpath+" "+pushPath;
    ExeCmd::runCmd(cmd);
}

void AutoTravelWidget::on_clearBtn_clicked()
{
    ui->logTextEdit->clear();
}

void AutoTravelWidget::on_reportDetailBtn_clicked()
{
    QString path;
    if(fileNameForReport=="")
    {
        if(gWorkSpace=="")
            gWorkSpace=gNewDisk + QDir::separator() + "princekinWorkspace";
        path=gWorkSpace;
    }
    else
        path=fileNameForReport;
    qDebug()<<path;
    path.replace("/","\\");//将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
    QProcess::startDetached("explorer "+path);//打开上面获取的目录
}

void AutoTravelWidget::ReadOutput(QString recieveStr)
{
    //QString recieveStr=QString::fromLocal8Bit(p->readAllStandardOutput());
    qDebug()<<recieveStr;
    if(recieveStr.trimmed()=="")
        return;
    if(recieveStr.contains("开始时间"))
    {
        isLogEnd=true;
    }
    if(recieveStr.contains("startClickFlag"))
        clickStartEndFlag=true;
    if(recieveStr.contains("endClickFlag"))
        clickStartEndFlag=false;

    if(recieveStr.contains("endClickFlag"))
    {
        recieveStr=recieveStr.replace("endClickFlag","").trimmed();
    }
    if(recieveStr.contains("startClickFlag"))
    {
        recieveStr=recieveStr.replace("startClickFlag","").trimmed();
    }
    if(recieveStr.trimmed()=="")
        return;
    if(!(recieveStr.contains("INSTRUMENT")||recieveStr.contains("OK (1 test)")))
    {
        QStringList lineStr=recieveStr.split("=$");
        for(int p=0;p<lineStr.count();p++)
        {
            if(logcatErrorFlag)
            {
                ui->logTextEdit->append("<font color=red>"+lineStr[p]+"</font>");
            }
            else
            {
                ui->logTextEdit->append("<font color=black>"+lineStr[p]+"</font>");
            }
            if(isLogEnd)
            {
                reportFile->write((lineStr[p]+"\r\n").toStdString().c_str());
            }
            else
            {
                logFile->write((lineStr[p]+"\r\n").toStdString().c_str());
            }
            if(recieveStr.contains("点击异常  "))
            {
                errorStr+=lineStr[p]+"\r\n";
            }
        }
        if(recieveStr.contains("takeScreen")&&logcatErrorFlag)
        {
            QString pngName = recieveStr.mid(recieveStr.indexOf("takeScreen ")+11).trimmed();
            qDebug()<<"pngName"<<pngName;
            errorLogcatPngList.append(pngName);
        }
    }

    //*****************20170315*****************//
    if(recieveStr.contains("遍历开始时间："))
    {
        //=$遍历开始时间：  5;
        QStringList splitResult;
        splitResult=recieveStr.split("遍历开始时间：");
        qTravelStartTime=splitResult.at(1).trimmed();
        if(qTravelStartTime.contains("=$"))
            qTravelStartTime=qTravelStartTime.left(qTravelStartTime.indexOf("=$"));

    }
    if(recieveStr.contains("遍历结束时间："))
    {
        //=$遍历结束时间：  5;
        QStringList splitResult;
        splitResult=recieveStr.split("遍历结束时间：");
        qTravelEndTime=splitResult.at(1).trimmed();
        if(qTravelEndTime.contains("=$"))
            qTravelEndTime=qTravelEndTime.left(qTravelEndTime.indexOf("=$"));
    }
    if(recieveStr.contains("遍历时长："))
    {
        //=$遍历时长：  3天4小时22分55秒=$-----------------------------------------------------;
        QString tempStr;
        QStringList splitResult;
        splitResult=recieveStr.split("时长：");
        tempStr=splitResult.at(1).trimmed();

        splitResult=tempStr.split("=");
        qTravelTime=splitResult.at(0).trimmed();
    }
    if(recieveStr.contains("检测到的控件总数："))
    {
        //=$检测到的控件总数： 11  点击到的控件个数：  22   点击异常控件个数：  33;
        QString tempStr;
        QStringList splitResult;
        splitResult=recieveStr.split("控件总数：");
        tempStr=splitResult.at(1).trimmed();
        QRegExp regExp;
        QStringList tempList;
        regExp.setPattern("(\\d+)");
        int pos=0;
        while((pos=regExp.indexIn(tempStr, pos)) != -1)
        {
            tempList << regExp.cap(1);
            pos += regExp.matchedLength();
        }
        qCheckNumber=tempList.at(0);
        qClickNumber=tempList.at(1);
        qErrorNumber=tempList.at(2);
    }
    if(recieveStr.contains("遍历过的Activity数量："))
    {
        QStringList splitResult;
        splitResult=recieveStr.split("数量：");
        qTravelActivityNumber=splitResult.at(1).trimmed();
        if(qTravelActivityNumber.contains("=$"))
            qTravelActivityNumber=qTravelActivityNumber.left(qTravelActivityNumber.indexOf("=$"));
    }
    //*****************20170315*****************//
}

void AutoTravelWidget::ReadError(QString recieveStr)
{
    //QString recieveStr=QString::fromLocal8Bit(p->readAllStandardError());
    ui->logTextEdit->append("<font color=orange>"+recieveStr.trimmed()+"</font>");
    ui->logTextEdit->append("\r\n");
    qDebug()<<recieveStr;
}

void AutoTravelWidget::travelerNotRunning()
{
    if(isStartFlag)
        stopAutoTravel();
}

//*****************20170505*****************//
void AutoTravelWidget::CreateReportFile()
{
    if(gWorkSpace=="")
    {
        gWorkSpace=QCoreApplication::applicationDirPath();
    }
    QString deviceStr=ExeCmd::GetDeviceModel(deviceName.replace(" ",""))+"-"+ deviceName.replace(" ","");//去掉空格
    if(deviceName==""||deviceStr.contains("error"))
    {
        fileNameForReport=gWorkSpace;
    }
    else
    {
        fileNameForReport=gWorkSpace+QDir::separator()+deviceStr;
    }

    qStartTime=Helper::getTime2("yyyyMMdd_hhmmss");

    fileNameForReport=gWorkSpace + QDir::separator() + "report" + QDir::separator() +"Autotravel_" + deviceName + "_" + qStartTime;
    Helper::createPath(fileNameForReport);

    QString logPath=fileNameForReport + QDir::separator()+"log";
    Helper::createPath(logPath);


    QDateTime current_date_time = QDateTime::currentDateTime();

    logFilePath=logPath + QDir::separator() + "autotravel_log_" + current_date_time.toString("yyyy-MM-dd_hh-mm-ss")+".txt";

    reportFilePath=logPath + QDir::separator() + "autotravel_report_" + current_date_time.toString("yyyy-MM-dd_hh-mm-ss")+".txt";

   // logcatFilePath=logPath + QDir::separator() + "report_" + current_date_time.toString("yyyy-MM-dd_hh-mm-ss")+".txt";

    logFile=new QFile(logFilePath);
    logFile->open(QIODevice::WriteOnly|QIODevice::Text);
    reportFile=new QFile(reportFilePath);
    reportFile->open(QIODevice::WriteOnly|QIODevice::Text);

    QString tempstr="手机型号： "+ui->equipListView->currentIndex().data().toString()+"\r\n";
    reportFile->write(tempstr.toStdString().c_str());
    tempstr="系统版本： Android "+systemStr;
    reportFile->write(tempstr.toStdString().c_str());
    tempstr="测试项目： "+ui->packageListView->currentIndex().data().toString()+"\r\n";
    reportFile->write(tempstr.toStdString().c_str());
}
//*****************20170505*****************//




void AutoTravelWidget::RecieveData(QVariant var)
{
    signalAutotravelData_s signalAutotravelData;
    signalAutotravelData=var.value<signalAutotravelData_s>();

    logKeyWordList.clear();
    logKeyWordList=signalAutotravelData.logKeyWord.split(";");


    this->isMemCheck=signalAutotravelData.isMemCheck;
    this->isCpuCheck=signalAutotravelData.isCpuCheck;
    this->isBatteryCheck=signalAutotravelData.isBatteryCheck;
    this->isCpuTempCheck=signalAutotravelData.isCpuTempCheck;
    this->isWifiCheck=signalAutotravelData.isWifiCheck;
    this->isMobileCheck=signalAutotravelData.isMobileCheck;

    this->memThres=signalAutotravelData.memThres;
    this->cpuThres=signalAutotravelData.cpuThres;
    this->batteryThres=signalAutotravelData.batteryThres;
    this->cpuTempThres=signalAutotravelData.cpuTempThres;
    this->wifiThres=signalAutotravelData.wifiThres;
    this->mobileThres=signalAutotravelData.mobileThres;

    this->xmlPaths=signalAutotravelData.xmlPaths;
    this->isDebug=signalAutotravelData.isDebug;
    this->clickInternalTime=signalAutotravelData.clickInternalTime;

}

void AutoTravelWidget::closeEvent(QCloseEvent *e)
{
    if(isStartFlag)
    {
        StopUiautomatorByHand();

       // stopAutoTravel();
    }
}

void AutoTravelWidget::stopAutoTravel()
{
    emit travelStop();

    isStartFlag=false;
    ui->startBtn->setIcon(QIcon(":/start.png"));

    if(errorStr.length()!=9)
        reportFile->write(errorStr.toStdString().c_str());
    logFile->close();
    reportFile->close();

    if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
    {
        if(assist!=NULL)
        {      
            assist->StopMeasure();

            if(gTravelNumber==0)
            {
                createExcel();
            }
        }
    }
    else
    {
        if(gTravelNumber==0)
        {
            createBaseExcel();
        }
    }
    ExeCmd::StopLogcat(deviceName);
    if(fileLogcat!=NULL)
    {
        fileLogcat->close();
        if(fileLogcat->size()==0)
            QFile::remove(fileLogcat->fileName());
    }
    QString cmdStr="adb -s "+deviceName+" pull /data/local/tmp/local/png "+fileNameForReport+QDir::separator()+"png";
    //runCmd(cmdStr);
    emit sendStateChange(false);
    ui->equipBtn->setEnabled(true);
    ui->equipListView->setEnabled(true);
    ui->packageCombo->setEnabled(true);
    ui->packageListView->setEnabled(true);
    settingButton->setEnabled(true);

    CmdWorker *worker=new CmdWorker;
    worker->arg_cmdLine=cmdStr;
    QThread *workerThread=new QThread(this);
    connect(workerThread,SIGNAL(started()),worker,SLOT(pullPicture()));
    connect(worker,SIGNAL(pullPictureStop()),workerThread,SLOT(quit()));
    connect(worker,SIGNAL(pullPictureStop()),this,SLOT(copyErrorPicture()));
    connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));
    worker->moveToThread(workerThread);
    workerThread->start();

    cmdStr="adb -s "+deviceName+" pull /data/local/tmp/local/errorpng "+fileNameForReport+QDir::separator()+"errorpng";

    CmdWorker *workerError=new CmdWorker;
    workerError->arg_cmdLine=cmdStr;
    QThread *workerThreadError=new QThread(this);
    connect(workerThreadError,SIGNAL(started()),workerError,SLOT(pullPicture()));
    connect(workerError,SIGNAL(pullPictureStop()),workerThreadError,SLOT(quit()));
    connect(workerThreadError,SIGNAL(finished()),workerError,SLOT(deleteLater()));
    workerError->moveToThread(workerThreadError);
    workerThreadError->start();

    //删除本地及手机里stop文件
    QString tempFpath=QCoreApplication::applicationDirPath()+QDir::separator()+"stop.txt";
    QFile tempF(tempFpath);
    if(tempF.exists())
        tempF.remove();

    QString cmd="adb -s "+deviceName+" shell rm /data/local/tmp/local/stop.txt";
    ExeCmd::runCmd(cmd);
}



void AutoTravelWidget::RecieveLogcatOutput()
{
    if(!clickStartEndFlag)
        logcatErrorFlag=false;
    QString tempStr=QString::fromUtf8(pLogcat->readAllStandardOutput());
    bool error=false;
    for(int i=0;i<logKeyWordList.length();i++)
    {
        error=error||tempStr.contains(logKeyWordList[i],Qt::CaseInsensitive);
    }
    if(error)
    {
        if(clickStartEndFlag)
            logcatErrorFlag|=true;
        qDebug()<<"===================================true";
    }
    else
    {
        if(clickStartEndFlag)
            logcatErrorFlag|=false;
    }
    if(fileLogcat->isOpen())
        fileLogcat->write(tempStr.toStdString().c_str());

}

void AutoTravelWidget::RecieveMemOverThresSignal(bool flag,QString deviceName)
{
    isMemOverThres=flag;
}

void AutoTravelWidget::RecieveCpuOverThresSignal(bool flag,QString deviceName)
{
    isCpuOverThres=flag;
}

void AutoTravelWidget::RecieveWifiOverThresSignal(bool flag,QString deviceName)
{
    isWifiOverThres=flag;
}

void AutoTravelWidget::RecieveMobileOverThresSignal(bool flag,QString deviceName)
{
    isMobileOverThres=flag;
}

void AutoTravelWidget::RecieveBatteryOverThresSignal(bool flag,QString deviceName)
{
    isBatteryOverThres=flag;
}

void AutoTravelWidget::RecieveCpuTempOverThresSignal(bool flag,QString deviceName)
{
    isCpuTempOverThres=flag;
    if(isBatteryOverThres||isCpuTempOverThres||isMemOverThres||isCpuOverThres||isWifiOverThres||isMobileOverThres)
    {
        qDebug()<<"超过阀值了";

        QString cmdStr="adb -s "+deviceName+" shell logcat -v time -b main -b system\n";
        qDebug()<<cmdStr;
        if(pLogcat->state())
            pLogcat->write(cmdStr.toLatin1());
        else
            pLogcat->start("cmd", QStringList()<<"/c"<< cmdStr );
    }
    else
    {
        qDebug()<<"停止信号";
        ExeCmd::StopLogcat(deviceName);
    }
}

bool AutoTravelWidget::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

void AutoTravelWidget::copyErrorPicture()
{
    for(int i=0;i<errorLogcatPngList.count();i++)
    {
        copyFileToPath(fileNameForReport+QDir::separator()+"png\\"+errorLogcatPngList[i],fileNameForReport+QDir::separator()+"errorpng\\"+errorLogcatPngList[i],true);
    }
}

void AutoTravelWidget::createExcel()
{
    ExcelController::Controller *controller=new ExcelController::Controller;

    controller->setMark("travel");
    controller->setXlsxSaveDir(fileNameForReport);
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

    controller->setTravelStartTime(qTravelStartTime);
    controller->setTravelEndTime(qTravelEndTime);
    controller->setTravelTime(qTravelTime);
    controller->setCheckNumber(qCheckNumber);
    controller->setClickNumber(qClickNumber);
    controller->setErrorNumber(qErrorNumber);
    controller->setTravelActivityNumber(qTravelActivityNumber);

    controller->setPackageName(packageName);
    controller->setStatiList(qStatisticsDataList);
    controller->setMeasureVector(assist->reportInfo);

    connect(controller,SIGNAL(sendWorkerFinished()),this,SLOT(receiveWorkerFinished()));
    controller->doController();
}

void AutoTravelWidget::createBaseExcel()
{
    ExcelController::Controller *controller=new ExcelController::Controller;

    controller->setMark("base-travel");
    controller->setXlsxSaveDir(fileNameForReport);
    controller->setStartTime(qStartTime);
    controller->setDeviceId(deviceName);
    controller->setLunchTime(qLunchTime);
    controller->setFrames(qFrames);

    controller->setTravelStartTime(qTravelStartTime);
    controller->setTravelEndTime(qTravelEndTime);
    controller->setTravelTime(qTravelTime);
    controller->setCheckNumber(qCheckNumber);
    controller->setClickNumber(qClickNumber);
    controller->setErrorNumber(qErrorNumber);
    controller->setTravelActivityNumber(qTravelActivityNumber);

    controller->setPackageName(packageName);
    controller->setStatiList(qStatisticsDataList);

    connect(controller,SIGNAL(sendWorkerFinished()),this,SLOT(receiveWorkerFinished()));
    controller->doController();
}

void AutoTravelWidget::receiveWorkerFinished()
{
    QMessageBox::information(NULL,"提示","excel生成完毕");
}

void AutoTravelWidget::getXXX()
{
    getFrames();
}

void AutoTravelWidget::getFrames()
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

void AutoTravelWidget::receiveFramesResult(const QString &arg_deviceId,const QString &arg_text)
{
    qFrames=arg_text;
}

void AutoTravelWidget::getLunchTime()
{
    QString cmdLine="cmd /c adb -s " + deviceName + " shell dumpsys activity top | findstr ACTIVITY";

    AdbShellController::Controller *controller=new AdbShellController::Controller;
    controller->setMark("getAll","isActive");
    controller->setCmdLine(cmdLine);
    connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
    controller->doController();
}

void AutoTravelWidget::receiveWorkerResult1(const QString&arg_str,const QString&arg_mark)
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

void AutoTravelWidget::receiveWorkerResult2(const QString&arg_str,const QString&arg_mark)
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
