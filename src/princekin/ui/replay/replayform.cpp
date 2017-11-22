#include "replayform.h"
#include "ui_replayform.h"

ReplayForm::ReplayForm(QWidget *parent) :QMainWindow(parent),ui(new Ui::ReplayForm)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0,7);
    ui->splitter->setStretchFactor(1,3);


    ui->splitter_2->setStretchFactor(0,3);
    ui->splitter_2->setStretchFactor(1,7);

    //createMenuBar();
    createToolBar();
    //createMenuItems();
    //设置设备显示框的属性
    ui->listWidget->setStyleSheet("QListWidget{font-size:12px;background:#ffffff;border:1px #ffffff;}"
                                  "QListWidget::item:selected:active{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                  "QListWidget::item:selected{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                  "QListWidget::item{height:30px;color:#4d4d4d}");
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                       "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->scriptTreeWidget->setStyleSheet("QTreeWidget{background:#ffffff;border:1px #ffffff;}");
    ui->scriptTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                     "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->scriptTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                     "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");

    QTreeWidgetItem *fileItem1 = new QTreeWidgetItem(QStringList() << "脚本文件列表", 1);   //1表示是文件
    ui->scriptTreeWidget->addTopLevelItem(fileItem1);
    ui->scriptTreeWidget->setHeaderHidden(true);
    ui->scriptTreeWidget->setMouseTracking(true);

    connect(ui->scriptTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));

    qDetailSourceXlsx=gConfigDir + QDir::separator() + "report.xlsx";
    qDailySourceXlsx=gConfigDir + QDir::separator() + "daily.xlsx";
    qCrashSourceXlsx=gConfigDir + QDir::separator() + "crash.xlsx";
    showDeviceList();
    mailTimer();
    qRowIndex=5;

}


ReplayForm::~ReplayForm()
{
    delete ui;
}


void ReplayForm::createToolBar()
{
    toolBar=addToolBar("加载脚本库");
    toolBar->setStyleSheet("QToolBar{background:#ffffff;}");

    addScriptButton=new QToolButton(this);
    addScriptButton->setToolButtonStyle((Qt::ToolButtonTextBesideIcon));
    addScriptButton->setToolTip("加载脚本库");
    addScriptButton->setText("加载脚本库");
    addScriptButton->setIcon(QIcon(":/addscript.png"));
    addScriptButton->setFixedWidth(120);
    addScriptButton->setFixedHeight(30);
    addScriptButton->setStyleSheet("QToolButton{background:#ffffff;}");
    toolBar->addWidget(addScriptButton);
    connect(addScriptButton, SIGNAL(clicked(bool)), this, SLOT(loadScriptSlot()));

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spacer->setStyleSheet("QWidget{background:#ffffff;}");
    toolBar->addWidget(spacer);

    settingButton=new QToolButton(this);
    settingButton->setToolTip("设置");
    settingButton->setIcon(QIcon(":/setting.png"));
    settingButton->setFixedWidth(50);
    settingButton->setStyleSheet("QToolButton{background:#ffffff;}");
    connect(settingButton,SIGNAL(clicked()),this,SLOT(settingSlot()));
    toolBar->addWidget(settingButton);

}

void ReplayForm::settingSlot()
{
    if(setupDialog==NULL)
    {
        setupDialog=new ReplaySetup();
        setupDialog->setAttribute(Qt::WA_QuitOnClose,false);   
        connect(setupDialog,SIGNAL(sendData(QVariant)),this,SLOT(recieveData(QVariant)));
        setupDialog->initUI();
    }

    setupDialog->show();
    setupDialog->raise();
}

void ReplayForm::recieveData(QVariant var)
{
    /*
    this->isMemCheck=isMemCheck;
    this->isCpuCheck=isCpuCheck;
    this->isBatteryCheck=isBatteryCheck;
    this->isCpuTempCheck=isCpuTempCheck;
    this->isWifiCheck=isWifiCheck;
    this->isMobileCheck=isMobileCheck;
    */

    signalReplayData_s signalData;
    signalData=var.value<signalReplayData_s>();

    qMemWarningValue=signalData.memThres.trimmed();
    qCpuWarningValue=signalData.cpuThres.trimmed();
    qBatteryWarningValue=signalData.batteryThres.trimmed();
    qCpuTempWarningValue=signalData.cpuTempThres.trimmed();
    qWifiWarningValue=signalData.wifiThres.trimmed();
    qMobileWarningValue=signalData.mobileThres.trimmed();

    xmlPaths=signalData.xmlPaths;
    isDebug=signalData.isDebug;
}

void ReplayForm::createMenuBar()
{
    qMyMenuBar=new QMenuBar(ui->menubar);
}

void ReplayForm::createMenuItems()
{
    qMenuSetting=new QMenu("设置",qMyMenuBar);
    qMyMenuBar->addMenu(qMenuSetting);

    qAction=new QAction("加载脚本库",this);
    connect(qAction, SIGNAL(triggered()), this, SLOT(loadScriptSlot()));
    qMenuSetting->addAction(qAction);

    ui->menubar->setCornerWidget(qMyMenuBar);
}



void ReplayForm::loadScriptSlot()
{
    loadRepository();
}

QString ReplayForm::getWorkspace()
{
    QString strFile=gConfigDir+QDir::separator()+"currentRepository.txt";

    QFile file;
    file.setFileName(strFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        QMessageBox::information(NULL,"提示", "currentRepository 文件异常");
        return "";
    }
    QTextStream inStream(&file);
    QString strPath=inStream.readLine();
    file.close();

    QFileInfo fileInfo(strPath);
    gMyWorkSpace=fileInfo.absolutePath();
    return fileInfo.absolutePath();;

}

void ReplayForm::loadRepository()
{
    createTree();
}

void ReplayForm::createTree()
{
    //QString filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),currentScriptPath,QFileDialog::ShowDirsOnly);
    QString filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),currentScriptPath,QFileDialog::ShowDirsOnly);
    if(filePath=="")
        return;
    qScriptList.clear();
    Helper::loadRepository2(filePath);//赋值gUiautomatorDir2等变量
    filePath=filePath + QDir::separator() + "testcase";
    filePath=QDir::toNativeSeparators(filePath);

    //加入判断testcase文件夹是否存在，若不存在则不是正确的脚本库，所以gmoduleDir2赋值为当前目录，在加载脚本库的时候也显示当前目录
    if(!QDir(filePath).exists())
    {
        QMessageBox::information(0,"提示","所选路径不是脚本库路径！");
        return;
    }

    if(currentScriptPath!=filePath)
    {
        currentScriptPath=filePath;
        ui->scriptTreeWidget->clear();
        QTreeWidgetItem *fileItem = new QTreeWidgetItem(QStringList()<<"script",0);
        fileItem->setIcon(0,QIcon(":/folder.png"));
        fileItem->setCheckState(0,Qt::Unchecked);
        ui->scriptTreeWidget->addTopLevelItem(fileItem);

        QString filePathInner=QDir::toNativeSeparators(filePath + QDir::separator() + "script");
        StaticticsHerper::LoadFiles(filePathInner,NULL,fileItem);

        fileItem = new QTreeWidgetItem(QStringList()<<"suite",0);
        fileItem->setIcon(0,QIcon(":/folder.png"));
        fileItem->setCheckState(0,Qt::Unchecked);
        ui->scriptTreeWidget->addTopLevelItem(fileItem);

        filePathInner=QDir::toNativeSeparators(filePath + QDir::separator() + "suite");
        StaticticsHerper::LoadFiles(filePathInner,NULL,fileItem);

        QTreeWidgetItem *item=ui->scriptTreeWidget->itemAt(0,0);
        ui->scriptTreeWidget->setCurrentItem(item);
    }
    else//如果路径与之前一致，
    {
        QStringList checkedFiles;
        QTreeWidgetItemIterator item(ui->scriptTreeWidget);
        while(*item)
        {
            if((*item)->checkState(0) == Qt::Checked)
            {
                if((*item)->childCount()==0&&(*item)->parent()!=NULL)
                {
                    checkedFiles.append((*item)->text(0)+"="+(*item)->parent()->text(0));
                }
            }
            ++item;
        }
        ui->scriptTreeWidget->clear();
        QTreeWidgetItem *fileItem = new QTreeWidgetItem(QStringList()<<"script",0);
        fileItem->setIcon(0,QIcon(":/folder.png"));
        fileItem->setCheckState(0,Qt::Unchecked);
        ui->scriptTreeWidget->addTopLevelItem(fileItem);

        QString filePathInner=QDir::toNativeSeparators(filePath + QDir::separator() + "script");
        StaticticsHerper::LoadFiles(filePathInner,NULL,fileItem);

        fileItem = new QTreeWidgetItem(QStringList()<<"suite",0);
        fileItem->setIcon(0,QIcon(":/folder.png"));
        fileItem->setCheckState(0,Qt::Unchecked);
        ui->scriptTreeWidget->addTopLevelItem(fileItem);

        filePathInner=QDir::toNativeSeparators(filePath + QDir::separator() + "suite");
        StaticticsHerper::LoadFiles(filePathInner,NULL,fileItem);

        QTreeWidgetItem *item2=ui->scriptTreeWidget->itemAt(0,0);
        ui->scriptTreeWidget->setCurrentItem(item2);

        QTreeWidgetItemIterator item1(ui->scriptTreeWidget);
        while(*item1)
        {
            if((*item1)->childCount()==0&&(*item1)->parent()!=NULL)
            {
                for(int i=0;i<checkedFiles.count();i++)
                {

                    if((*item1)->text(0)+"="+(*item1)->parent()->text(0) == checkedFiles[i])
                    {
                        (*item1)->setCheckState(0,Qt::Checked);
                        break;
                    }
                }
            }
            ++item1;
        }
    }
}

void ReplayForm::on_pushButton_clicked()
{
    ui->listWidget->selectAll();
}

void ReplayForm::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->setTextColor(QColor("black"));
}


void ReplayForm::on_pushButton_4_clicked()
{
    if(isRun)
    {
        emit sendStopReplay();
        isRun=false;
    }
    else
    {
        ui->textEdit->setTextColor(QColor("black"));
        qStatisticsDataList.clear();

        if(qScriptList.count()==0)
        {
            QMessageBox::information(NULL,"提示", "请选择脚本");
            return;
        }

        QList<QListWidgetItem*> selectedDeviceIdItemList;

        selectedDeviceIdItemList=ui->listWidget->selectedItems();
        int count;
        count=selectedDeviceIdItemList.count();
        if(count==0)
        {
            QMessageBox::information(NULL,"信息","请选择设备");
            return;
        }
        if(count>24)
        {
            QMessageBox::information(NULL,"提示","设备个数不能大于24个");
            return;
        }

        QString mStr=gConfigDir + QDir::separator() + "packageName.txt";
        gPackageName=Helper::getPackageName(mStr);
        if(gPackageName.isEmpty())
        {
            QMessageBox::information(NULL,"提示","packageName.txt中应用包名不正确");
            return;
        }
        doWork(selectedDeviceIdItemList);

    }//else
}

void ReplayForm::doWork( QList<QListWidgetItem *> arg_selectedDeviceIdItemList)
{
    int index=qScriptList.indexOf("findfolder");

    if(index!=-1)
    {
        QMessageBox::information(NULL,"提示","脚本中不能有文件夹");
        return;
    }

    //*****************20170505*************************//
    getStatisticsData();
    //*****************20170505*************************//

    //qModuleNameList=PreSet::getModuleNameList(qModel,arg_scriptModelIndexList);
    qModuleNameList=PreSet::_getMoudleName(qScriptList);
    qDeviceIdList=PreSet::getDeviceIdList(arg_selectedDeviceIdItemList);
    qDeviceModelIdNameList=PreSet::getSecondLevelDirNameList(arg_selectedDeviceIdItemList);

    qDevice_TopIndex_Hash=PreSet::getDevice_TopIndex_Hash(qDeviceIdList);
    //qModule_FileCount_Hash=PreSet::getModule_FileCount_Hash(qModel,arg_scriptModelIndexList);
    qModule_FileCount_Hash=PreSet::_getModule_FileCount_Hash(qModuleNameList);
    qModule_FileCount_List=PreSet::getModule_fileCount_List(qModuleNameList,qModule_FileCount_Hash);

    qFirstLevelDirName=qRepoName + "report_" + Helper::getTime2("yyyyMMdd_hhmmss");
    qDetailedFolderName=qRepoName + "report_" + Helper::getTime2("yyyyMMdd_hhmmss");

    gdLoop=PreSet::strToInt1(gLoop);
    gdInterval=PreSet::strToInt2(gInterval);

    preSets();
    createReportSaveDir();

    qDetailedTargetXlsx=qDetailedPath + QDir::separator() + "detail-report.xlsx";
    qDetailedTargetXlsx=QDir::toNativeSeparators(qDetailedTargetXlsx);

    qDailyTargetXlsx=gPackagePath + QDir::separator() + "daily"+gStartTextDate+".xlsx";
    qDailyTargetXlsx=QDir::toNativeSeparators(qDailyTargetXlsx);

    QFile::copy(qDailySourceXlsx,qDailyTargetXlsx);
    qXlsx=new Document(qDailyTargetXlsx);

    createDetailReport(qDetailedTargetXlsx,qDeviceModelIdNameList,qScriptList);

    createWriteResult(qDetailedTargetXlsx);
    writeBaseInfo(qDeviceIdList);

    createCrashReport();

    setDeviceEnv(qDeviceIdList,qModuleNameList);
}



void ReplayForm::createReportSaveDir()
{
    QString strDir;
    QString deviceModelIdName;
    QString deviceModelIdPath;

    gStartTextDate=Helper::getTime2("yyyyMMdd");

    //D:\princekinWorkspace\report\20170927
    gDatePath=gWorkSpace +  QDir::separator() + "report" +  QDir::separator() + gStartTextDate;
    gDatePath=QDir::toNativeSeparators(gDatePath);
    Helper::createPath(gDatePath);

    //D:\princekinWorkspace\report\20170927\com.sohu.qianfan
    gPackagePath=gDatePath + QDir::separator() + gPackageName;
    gPackagePath=QDir::toNativeSeparators(gPackagePath);
    Helper::createPath(gPackagePath);

    //D:\princekinWorkspace\report\20170927\com.sohu.qianfan\sohuvideoreport_20170927_155913
    qDetailedPath=gPackagePath + QDir::separator() + qDetailedFolderName;
    qDetailedPath=QDir::toNativeSeparators(qDetailedPath);
    Helper::createPath(qDetailedPath);
    gDetailedPath=qDetailedPath;

    int count=qDeviceModelIdNameList.count();
    for(int i=0;i<count;i++)
    {
        deviceModelIdName=qDeviceModelIdNameList.at(i);

        deviceModelIdPath=qDetailedPath + QDir::separator() + deviceModelIdName;
        deviceModelIdPath=QDir::toNativeSeparators(deviceModelIdPath);
        Helper::createPath(deviceModelIdPath);

        strDir=deviceModelIdPath + QDir::separator() + "png";
        Helper::createPath(strDir);

        strDir=deviceModelIdPath + QDir::separator() + "logcat";
        Helper::createPath(strDir);

        strDir=deviceModelIdPath + QDir::separator() + "performance";
        Helper::createPath(strDir);

        strDir=deviceModelIdPath + QDir::separator() + "script-log.txt";

        QFile *qfile=new QFile(strDir);
        if(!qfile->open(QIODevice::Append))
        {
            qfile->close();
        }
        else
        {
            qfileHash.insert(deviceModelIdName,qfile);
            QTextStream *outStream=new QTextStream(qfile);
            qfilePointerHash.insert(deviceModelIdName,outStream);
        }
    }//for


    QFile file(gConfigDir+QDir::separator()+gPackageName+"_starttime.txt");
    QTextStream outStream(&file);
    if(!file.open(QIODevice::Append))
    {
        file.close();
    }
    QString stime=Helper::getTime2("yyyy-MM-dd hh:mm:ss");
    outStream<<stime<<endl;
    file.close();
}



void ReplayForm::setDeviceEnv(QStringList arg_deviceIdList,QStringList arg_moduleNameList)
{
    QThreadPool *pool=new QThreadPool;
    InitEnv *initEnvInstance=new InitEnv;
    initEnvInstance->setAutoDelete(true);
    initEnvInstance->setDeviceIdList(qDeviceIdList);
    initEnvInstance->setModuleNameList(qModuleNameList);
    connect(initEnvInstance, SIGNAL(sendFinished()), this, SLOT(receiveInitEnvFinished()));
    pool->start(initEnvInstance);
}

void ReplayForm::receiveInitEnvFinished()
{
    isRun=true;
    startPerformance(qDeviceIdList);
    startRunScript(qDeviceIdList,qDeviceModelIdNameList,qScriptList);
}

void ReplayForm::startRunScript(QStringList arg_deviceIdList,QStringList arg_secondLevelDirNameList,QStringList arg_scriptList)
{
    qNumber=0;
    qNumber2=0;
    qSec=0;
    QString deviceId;
    QString secondLevelDirName;

    QThreadPool *pool=new QThreadPool;
    qSelectedDeviceCount=qDeviceIdList.count();
    for(int i=0;i<qSelectedDeviceCount;i++)
    {
        deviceId=arg_deviceIdList.at(i);
        secondLevelDirName=qDeviceModelIdNameList.at(i);

        using ReplayController::Controller;
        Controller *controllerInstance=new Controller;

        controllerInstance->setAutoDelete(true);
        controllerInstance->setDeviceId(deviceId);
        controllerInstance->setFirstLevelDirName(qFirstLevelDirName);
        controllerInstance->setSecondLevelDirName(secondLevelDirName);
        controllerInstance->setScriptList(qScriptList);

        connect(controllerInstance,SIGNAL(sendRunOneScriptResult(const QString &)),this,SLOT(receiveRunScriptResult(const QString &))   );
        connect(controllerInstance,SIGNAL(sendReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &) ),this,SLOT(receiveReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &))  );
        connect(controllerInstance,SIGNAL(sendCrashResult(const QString &,const QString &,const QString &,const QString &) ),this,SLOT(receiveCrashResult(const QString &,const QString &,const QString &,const QString &))  );

        connect(controllerInstance,SIGNAL(sendErrorResult(const QString &,const QString &) ),this,SLOT(receiveErrorResult(const QString &,const QString &))  );
        connect(controllerInstance,SIGNAL(sendProcFinished(const QString &) ),this,SLOT(receiveProcFinished(const QString &) ) );
        connect(this,SIGNAL(sendStopReplay()),controllerInstance,SLOT(receiveStopReplay())  ,Qt::DirectConnection);

        pool->start(controllerInstance);

        /*
        AdbShellController::Controller *controller=new AdbShellController::Controller;
        controller->setMark("logcat","sendmail");
        controller->setCmdLine(cmdLine);
        //connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
        controller->doController();
        */
    }
}

void ReplayForm::receiveErrorResult(const QString &arg_SecondLevelDirName,const QString &arg_error)
{
    QString receiveTime=Helper::getTime2("yyyyMMdd-hh-mm-ss")+"    ";
    QString strDir=qDetailedPath + QDir::separator() + arg_SecondLevelDirName + QDir::separator()+"script-error-report.txt";
    strDir=QDir::toNativeSeparators(strDir);
    QFile f(strDir);
    f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&f);
    out<<receiveTime+arg_error<<endl;
    f.close();
}


void ReplayForm::receiveProcFinished(const QString &arg_deviceIdFinished)
{
}

void ReplayForm::receiveRunScriptResult(const QString &arg_result)
{
    QString receiveTime=Helper::getTime2("yyyyMMdd-hh-mm-ss")+"    ";
    if(!arg_result.trimmed().isEmpty() && !arg_result.startsWith("--------"))
    {
        qTempSplitResult=arg_result.split(" ",QString::SkipEmptyParts);
        qTempDeviceId=qTempSplitResult.at(0);
        qTempSplitResult=qDeviceModelIdNameList.filter(qTempDeviceId);
        if(qTempSplitResult.count()>=1)
        {
            qTempDeviceModelIdName=qTempSplitResult.at(0);
            *qfilePointerHash.value(qTempDeviceModelIdName)<<receiveTime + arg_result<<endl;
        }
    }



    if(arg_result.contains("princekinfalse") || arg_result.contains("java.lang.RuntimeException") || arg_result.contains("error:" ,Qt::CaseInsensitive)  )
    {
        ui->textEdit->append("<font color = red>" + receiveTime + arg_result + "<\\font>");
    }
    else
    {
        if(!arg_result.trimmed().isEmpty())
        {
            ui->textEdit->append(receiveTime+arg_result);
        }
    }
}


void ReplayForm::receiveReplayResult(const QString &arg_deviceId,const QString &arg_two,QStringList arg_actual, QStringList arg_error,int arg_sec,QStringList arg_moduleCrashList,const QString &arg_startTime)
{

    qDebug()<<arg_deviceId;
    qDebug()<<arg_two;
    qDebug()<<arg_actual;
    qDebug()<<arg_error;
    qDebug()<<arg_moduleCrashList;

    qNumber++;

    qSec=qSec+arg_sec;
    emit sendReplayResultToWrite(arg_deviceId,arg_two,arg_actual,arg_error,qDetailedPath);
    emit sendStopPerformance(arg_deviceId);

    qXlsx->selectSheet("base");
    createDaily(arg_deviceId,arg_actual,arg_moduleCrashList,arg_startTime,arg_error);
    qXlsx->selectSheet("performance");
    createPerformance(arg_deviceId,arg_actual,arg_moduleCrashList);


    QStringList splitResult=qDeviceModelIdNameList.filter(arg_deviceId);
    if(splitResult.count()>=1)
    {
        QString deviceModelIdName=splitResult.at(0);
        qfileHash.value(deviceModelIdName)->close();
        delete qfilePointerHash.value(deviceModelIdName);
        delete qfileHash.value(deviceModelIdName);
        qfilePointerHash.remove(deviceModelIdName);
        qfileHash.remove(deviceModelIdName);
    }







    stopTrafficservice(arg_deviceId);
    stopNetState(arg_deviceId);

    if(arg_error.isEmpty())
    {
    }
    else
    {
        qNumber2++;
    }
    if(qNumber==qSelectedDeviceCount)
    {
        QFile file(gConfigDir+QDir::separator()+gPackageName+"_endtime.txt");
        QTextStream outStream(&file);
        if(!file.open(QIODevice::Append))
        {
            file.close();
        }
        QString stime=Helper::getTime2("yyyy-MM-dd hh:mm:ss");
        outStream<<stime<<endl;
        file.close();


        emit sendStopReplayForStatistics();
        if(qNumber2==0)
        {
            QString s="-------------------------------success-------------------------------";
            ui->textEdit->append("<font color = green>" + s + "<\\font>");
        }
        else
        {
            QString s="-------------------------------fail-------------------------------";
            ui->textEdit->append("<font color = red>" + s + "<\\font>");
        }

        int x=qSec/qSelectedDeviceCount;

        emit sendSec(x);

        qXlsx->selectSheet("base");
        qXlsx->save();
        Helper::writeStr(gConfigDir+QDir::separator()+gPackageName+"_base.txt",QString::number(qRowIndex));
        Helper::writeStr(gConfigDir+QDir::separator()+gPackageName+"_performance.txt",QString::number(qPerRowIndex));


        //ui->pushButton_4->setEnabled(false);
        emit sendSaveXlsx();
        //emit sendClearPerformance();
        delete qWriteResultInstance;
        delete performanceInstance;

        ui->scriptTreeWidget->setEnabled(true);
        ui->listWidget->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_5->setEnabled(true);


        ui->pushButton_4->setIcon(QIcon(":/start.png"));
        settingButton->setEnabled(true);
        addScriptButton->setEnabled(true);

        isRun=false;

        QString filename=gConfigDir + QDir::separator() + "emailsender.txt";
        QStringList list=Helper::getList(filename);

        if(list.size()>=2)
        {
            QString senderName=list.at(0);
            QString senderKey=list.at(1);
            QString cmdLine;

            QString mailContent=gConfigDir + QDir::separator() + "mailContent.txt";
            mailContent=QDir::toNativeSeparators(mailContent);
            QString strDir=gWorkSpace + QDir::separator() + "report" + QDir::separator() + qFirstLevelDirName+QDir::separator()+"report.xlsx";


            QString receiver=gConfigDir + QDir::separator() + "email.txt";
            cmdLine="cmd /c java -jar " + gConfigDir + QDir::separator() + "sendmail.jar" + " " + senderName + " " + senderKey + " " + receiver +  " 测试报告 " + strDir;
            //cmdLine="cmd /c java -jar " + gConfigDir + QDir::separator() + "sendmail.jar" + " " + senderName + " " + senderKey + " " + receiver +  " " + mailContent + " " + strDir;

            //Helper::runCmd(cmdLine);


            /*
            AdbShellController::Controller *controller=new AdbShellController::Controller;
            controller->setMark("getAll","sendmail");
            controller->setCmdLine(cmdLine);
            //connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
            controller->doController();
            */

        }
    }
}

void ReplayForm::createDetailReport(const QString &arg_targetXlsx,QStringList arg_secondLevelDirNameList,QStringList arg_scriptList)
{
    CreateReport report;

    //report.setFirstLevelDirName(qFirstLevelDirName);
    //report.setSecondLevelDirNameList(arg_secondLevelDirNameList);
    report.setModule_FileCount_List(qModule_FileCount_List);
    report.setScriptList(arg_scriptList);
    //report.createReportSaveDir();

    QFile::copy(qDetailSourceXlsx,arg_targetXlsx);
    report.writeExcel(arg_targetXlsx);
}

void ReplayForm::createCrashReport()
{
    if(qCrashreportInstance==NULL)
    {
        qCrashreportInstance=new CreateCrashReport;
        connect(this,SIGNAL(sendCrashResult(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,QStringList) ),qCrashreportInstance,SLOT(receiveCrashResult(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,QStringList))  );
    }

}

void ReplayForm::createWriteResult(const QString &arg_targetXlsx)
{
    qWriteResultInstance=new WriteResult;
    qWriteResultInstance->setTargetXlsx(arg_targetXlsx);
    qWriteResultInstance->setScriptList(qScriptList);
    qWriteResultInstance->setDevice_TopIndex_Hash(qDevice_TopIndex_Hash);
    qWriteResultInstance->setDir(qFirstLevelDirName);
    connect(this,SIGNAL(sendReplayResultToWrite(const QString &,const QString &,QStringList,QStringList,const QString &)),qWriteResultInstance,SLOT(receiveReplayResult(const QString &,const QString &,QStringList,QStringList,const QString &)));
    connect(this,SIGNAL(sendSaveXlsx()),qWriteResultInstance,SLOT(receiveSaveXlsx()));

    connect(this,SIGNAL(sendBaseInfoToWrite(QString)),qWriteResultInstance,SLOT(receiveBaseInfoResult(QString)));
    connect(this,SIGNAL(sendIcon(QString)),qWriteResultInstance,SLOT(receiveIcon(QString)));
    connect(this,SIGNAL(sendBaseInfoToWrite(QStringList)),qWriteResultInstance,SLOT(receiveBaseInfoResult(QStringList)));
    connect(this,SIGNAL(sendSec(int)),qWriteResultInstance,SLOT(receiveSec(int)));

}

void ReplayForm::writeBaseInfo(QStringList arg_deviceIdList)
{
    QString strFile;
    QString strTemp;
    QString cmdLine;
    QString strFlag;
    QString fileSize;
    QString iconresPath;

    QString appName;
    QString apkBaseName;
    QString appVersion;

    QStringList strList;
    QStringList valueList;
    QStringList splitResult;
    QHash<QString,QString> hashBrad;


    //lenovo=联想
    strTemp=gConfigDir + QDir::separator() + "brand.txt";
    hashBrad=Helper::getBrand(strTemp);

    //sohu.apk
    strFile=gConfigDir + QDir::separator() + "install-apk-name.txt";
    strList=Helper::getList(strFile);
    if(!strList.isEmpty())
    {
        QString apkName=strList.at(0);
        splitResult=apkName.split(".");
        apkBaseName=splitResult.at(0);//sohuvideo

        strFlag="application:";
        strFile=gApkDir + QDir::separator() + apkName;
        cmdLine="cmd /c aapt dump badging " + strFile + " | findstr application:";
        valueList=ExeCmd::runCmd_getAppName(cmdLine,strFlag);
        if(valueList.size()>=4)
        {
            appName=valueList.at(1);
            iconresPath=valueList.at(3);
        }

        strFlag="versionName=";
        cmdLine="cmd /c aapt dump badging " + strFile + " | findstr versionName=";
        appVersion=ExeCmd::runCmd_getVersionName(cmdLine,strFlag);
        qAppVersion=appVersion;

        QFileInfo info(strFile);
        qint64 m=info.size();
        int x=m/1024/1024;

        fileSize=QString::number(x);

        getIcon(strFile,apkBaseName,iconresPath);
    }

    QDateTime currentTime=QDateTime::currentDateTime();
    QString time=currentTime.toString("yyyy/MM/dd");

    int count=arg_deviceIdList.size();
    QString s1=appName + "=" + appVersion + "=" + fileSize + "=" + QString::number(count) + "=" + "android" + "=" + time;
    qAppInfo=appName + "=" + appVersion + "=" + fileSize + "=" + "android" + "=" + time;

    qAppName=appName;
    emit sendBaseInfoToWrite(s1);

    QString brand;

    QString chineseBrand;
    QString model;
    QString version;
    QString wmSize;
    QString sss;
    QStringList infoList;
    for(QString deviceId:arg_deviceIdList)
    {
        cmdLine="adb -s " + deviceId + " shell getprop ro.product.brand";
        brand=ExeCmd::runCmd_getProp(cmdLine);
        chineseBrand=hashBrad.value(brand);//中国

        cmdLine="adb -s " + deviceId + " shell getprop ro.product.model";
        model=ExeCmd::runCmd_getModel(cmdLine);//Lenovo_K920

        cmdLine="adb -s " + deviceId + " shell getprop ro.build.version.release";
        version=ExeCmd::runCmd_getProp(cmdLine);//"4.4.2"

        cmdLine="adb -s " + deviceId + " shell wm size";
        wmSize=ExeCmd::runCmd_getWmsize(cmdLine);

        sss=deviceId + "=" + chineseBrand + "=" + model + "=" + version + "=" + wmSize;

        infoList.append(sss);
    }

    qMobileInfoList=infoList;
    emit sendBaseInfoToWrite(infoList);
}


void ReplayForm::getIcon(const QString &arg_apkPath,const QString &arg_apkBaseName,const QString &arg_res)
{
    int count;
    QString cmdLine;
    QString iconName;
    QString iconPath;
    QString apkPngDir;
    QStringList iconList;
    QStringList splitResult;


    splitResult=arg_res.split("/");

    int len=splitResult.size();
    for(int i=0;i<len-1;i++)
    {
        apkPngDir=apkPngDir+splitResult.at(i)+"\\";
    }
    apkPngDir=apkPngDir+splitResult.at(len-1);


    iconPath=gApkDir+QDir::separator() + arg_apkBaseName + QDir::separator() + arg_res;
    iconPath=QDir::toNativeSeparators(iconPath);

    gIconPath="";
    QFile f(iconPath);
    if(f.exists())
    {
        gIconPath=iconPath;
        emit sendIcon(iconPath);
    }
    else
    {
        QString ss=gApkDir + QDir::separator() + arg_apkBaseName;
        Helper::createPath(ss);

        cmdLine="WinRAR x -ibck -o+ " + arg_apkPath + " " + apkPngDir + " " + ss;
        qDebug()<<cmdLine;

        ExeCmd::runCmd(cmdLine);
        gIconPath=iconPath;
        emit sendIcon(iconPath);
    }

}






void ReplayForm::receiveIconFinished(const QString &icon)
{

}

void ReplayForm::startPerformance(QStringList arg_deviceIdList)
{
    performanceInstance=new Performance;
    connect(performanceInstance,SIGNAL(sendHtmlFinished(const QString&)),this,SLOT(receiveHtmlFinished(const QString &)),Qt::DirectConnection);
    connect(this, SIGNAL(sendStopPerformance(const QString &)), performanceInstance, SLOT(receiveStopPerformance(const QString &))  );
    connect(this, SIGNAL(sendClearPerformance()), performanceInstance, SLOT(receiveClearPerformance()) );

    performanceInstance->setMemWarningValue(qMemWarningValue);
    performanceInstance->setCpuWarningValue(qCpuWarningValue);
    performanceInstance->setBatteryTempWarningValue(qBatteryWarningValue);
    performanceInstance->setCpuTempWarningValue(qCpuTempWarningValue);
    performanceInstance->setWifiWarningValue(qWifiWarningValue);
    performanceInstance->setMobileWarningValue(qMobileWarningValue);
    performanceInstance->setDeviceIdList(arg_deviceIdList);
    performanceInstance->setWriteResult(qWriteResultInstance);
    performanceInstance->setFirstLevelDirName(qFirstLevelDirName);
    performanceInstance->setSecondLevelDirNameList(qDeviceModelIdNameList);

    performanceInstance->startWatcher();
}


void ReplayForm::preSets()
{
    gNetStateHash.clear();

    ui->scriptTreeWidget->setEnabled(false);
    ui->listWidget->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_5->setEnabled(false);



    settingButton->setEnabled(false);
    addScriptButton->setEnabled(false);

    ui->pushButton_4->setIcon(QIcon(":/stop.png"));

    ui->textEdit->clear();

    QString filepath;
    QString s1;
    bool isOK;
    QFile file;

    filepath=gConfigDir+QDir::separator()+gPackageName+"_base.txt";
    file.setFileName(filepath);
    if(file.exists())
    {
        s1=Helper::getFirstLine(filepath);
        qRowIndex=s1.toInt(&isOK,10);
    }
    else
    {
        qRowIndex=5;
    }

    filepath=gConfigDir+QDir::separator()+gPackageName+"_performance.txt";
    file.setFileName(filepath);
    if(file.exists())
    {
        s1=Helper::getFirstLine(filepath);
        qPerRowIndex=s1.toInt(&isOK,10);
    }
    else
    {
        qPerRowIndex=3;
    }
}
void ReplayForm::closeEvent(QCloseEvent *event)
{
    if(testTimer->isActive())
    {
        testTimer->stop();
    }
}

void ReplayForm::showDeviceList()
{
    testTimer=new QTimer(this);
    connect(testTimer, SIGNAL(timeout()), this, SLOT(getDeviceSlot()) );
    testTimer->start(2000);
}

void ReplayForm::mailTimer()
{
    qMailtimer=new QTimer(this);
    connect(qMailtimer, SIGNAL(timeout()), this, SLOT(mailTimerSlot()) );
    qMailtimer->start(1000*60);
}

void ReplayForm::getDeviceSlot()
{
    QString cmdLine;
    QString strModelName;

    QStringList deviceList;
    QStringList device_model_List;

    deviceList=ExeCmd::runCmd_getDevice("adb devices");


    if(deviceList.isEmpty())
    {
        ui->listWidget->clear();
        qDevice_list.clear();
    }
    else
    {
        if(qDevice_list==deviceList)
        {
        }
        else
        {
            foreach(QString deviceId,deviceList)
            {
                cmdLine="adb -s " + deviceId + " shell getprop ro.product.model";
                strModelName=ExeCmd::runCmd_getModel(cmdLine);
                if(!strModelName.isEmpty())
                {
                    device_model_List.append(strModelName + "+" + deviceId);
                }
            }
            ui->listWidget->clear();
            ui->listWidget->addItems(device_model_List);
            ui->listWidget->item(0)->setSelected(true);
            qDevice_list=deviceList;
        }
    }
}


void ReplayForm::stopTrafficservice(const QString &arg_deviceId)
{
    QString cmdLine;
    cmdLine="adb -s " + arg_deviceId + " shell am force-stop com.sohu.trafficservice";
    runexe(cmdLine,"stopTrafficservice");
}

void ReplayForm::stopNetState(const QString &arg_deviceId)
{
    QString cmdLine;
    QStringList strList;

    cmdLine="adb -s " + arg_deviceId + " shell ps logcat";
    strList=ExeCmd::runCmd_getPid(cmdLine);

    foreach(QString pid,strList)
    {
        cmdLine="adb -s " + arg_deviceId + " shell kill " + pid;
        runexe(cmdLine,"killPid");
    }
}


//*****************20170505*************************//
void ReplayForm::createUrl()
{
    Document xlsxDoc(qDetailedTargetXlsx);
    xlsxDoc.selectSheet("url");

    int rowIndex=2;
    int len;
    int y;
    QStringList splitResult1;
    QStringList splitResult2;
    QString fileName;
    QString state;
    QString tempStr;
    QString mark;

    foreach (QString var, qStatisticsDataList)
    {
        splitResult1=var.split("&");
        len=splitResult1.count();
        if(len>=2)
        {
            fileName=splitResult1.at(0);
            xlsxDoc.write("A"+QString::number(rowIndex),fileName);
            state=splitResult1.at(1);
            xlsxDoc.write("B"+QString::number(rowIndex),state);

            y=rowIndex;
            for(int i=2;i<len;i++)
            {
                tempStr=splitResult1.at(i);
                splitResult2=tempStr.split("=");
                if(splitResult2.count()==2)
                {
                    xlsxDoc.write("C"+QString::number(y),splitResult2.at(0));
                    xlsxDoc.write("D"+QString::number(y),splitResult2.at(1));
                    xlsxDoc.write("E"+QString::number(y),"?");
                }
                if(splitResult2.count()==3)
                {
                    xlsxDoc.write("C"+QString::number(y),splitResult2.at(0));
                    xlsxDoc.write("D"+QString::number(y),splitResult2.at(1));
                    xlsxDoc.write("E"+QString::number(y),splitResult2.at(2));
                }


                y++;
            }
        }
        else
        {
        }
        if(len==2)
        {
            y++;
        }
        rowIndex=y;
    }
    xlsxDoc.save();

    Document xlsxDoc2(qDetailedTargetXlsx);
    xlsxDoc.selectSheet("总体报告");
    xlsxDoc2.save();
}


void ReplayForm::getStatisticsData()
{
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
        connect(this,SIGNAL(sendStopReplayForStatistics()),worker,SLOT(stopStatistics()));

        connect(worker,SIGNAL(sendStatisticsData(QStringList)),this,SLOT(ReadStatisticsData(QStringList)));

        worker->moveToThread(workerThread);

        workerThread->start();
    }


}
void ReplayForm::ReadStatisticsData(QStringList statistics)
{
    qStatisticsDataList=statistics;
    if(gReplayNumber!=0)
    {
        createUrl();
    }
}

void ReplayForm::on_pushButton_5_clicked()
{
    ExeCmd::openFolder(qDetailedPath);
}

void ReplayForm::receiveHtmlFinished(const QString &arg_text)
{
    //QMessageBox::information(this,"提示","excel生成完毕");
}

void ReplayForm::runexe(const QString &arg_cmdLine,const QString &arg_mark)
{
    AdbShellController::Controller *controller=new AdbShellController::Controller;
    controller->setMark("getAll",arg_mark);
    controller->setCmdLine(arg_cmdLine);
    //connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
    controller->doController();
}

void ReplayForm::receiveCrashResult(const QString &arg_deviceId, const QString &arg_secondLevelDirName, const QString &arg_moduleBaseName, const QString &arg_scriptBaseName)
{
    emit sendCrashResult(arg_deviceId,qFirstLevelDirName,arg_secondLevelDirName,arg_moduleBaseName,arg_scriptBaseName,qAppInfo,qMobileInfoList);
}
void ReplayForm::createDaily(const QString &arg_deviceId, QStringList arg_actual,QStringList arg_moduleCrashList,const QString &arg_startTime,QStringList arg_error)
{
    QString moduleName;
    QString endTime;
    QStringList moduleList;
    QStringList deviceModuleList;
    QStringList splitResult;
    QString str;
    endTime=Helper::getTime2("yyyy-MM-dd hh:mm:ss");
    for(QString var:arg_actual)
    {
        splitResult=var.split("=");
        moduleName=splitResult.at(0);
        if(moduleList.indexOf(moduleName)==-1)
        {
            moduleList.append(moduleName);
        }

        qDebug()<<"moduleName-------------------";
        qDebug()<<moduleName;

        str=arg_deviceId+"="+moduleName;
        if(deviceModuleList.indexOf(str)==-1)
        {
            deviceModuleList.append(str);
        }
    }

    //qAppVersion;
    //sss=deviceId + "=" + chineseBrand + "=" + model + "=" + version + "=" + wmSize;
    //arg_moduleCrashList=qDeviceId+"="+qModuleBaseName;

    QString deviceBrand;
    QString systemVersion;
    QString model;
    for(QString var:qMobileInfoList)
    {
        str=arg_deviceId+"=";
        if(var.startsWith(str))
        {
            splitResult=var.split("=");
            deviceBrand=splitResult.at(1)+"_"+splitResult.at(2);
            model=splitResult.at(2);
            systemVersion=splitResult.at(3);
        }
    }

    QString cell;
    if(qRowIndex==5)
    {
        Format format=getNormalFormat();

        QImage fitimgpic=getImage();
        qXlsx->insertImage(0,0,fitimgpic);

        /*
        QString index1="1";
        QString index2="2";
        cell="A" + index1 + ":" + "A" + index2;
        qXlsx->mergeCells(cell, format);
        */

        cell="C1";
        qXlsx->write(cell,qAppName);
        cell="E1";
        qXlsx->write(cell,"Android");

        cell="G1";
        qXlsx->write(cell,gStartTextDate);

    }

    for(QString var:moduleList)
    {
        cell="A"+QString::number(qRowIndex);
        qXlsx->write(cell,qAppVersion);

        cell="B"+QString::number(qRowIndex);
        qXlsx->write(cell,deviceBrand);

        cell="C"+QString::number(qRowIndex);
        qXlsx->write(cell,systemVersion);

        cell="D"+QString::number(qRowIndex);
        qXlsx->write(cell,var);

        QString s1;
        str=arg_deviceId+"="+var;
        if(arg_moduleCrashList.indexOf(str)==-1)
        {
            s1="No";
            cell="E"+QString::number(qRowIndex);
            qXlsx->write(cell,"No");
        }
        else
        {
            s1="Yes";
            cell="E"+QString::number(qRowIndex);
            qXlsx->write(cell,"Yes");
        }

        s1="Pass";
        str="="+arg_deviceId;
        for(QString errorLine:arg_error)
        {
            if(errorLine.endsWith(str))
            {
                s1="Fail";
            }
        }



        cell="F"+QString::number(qRowIndex);
        qXlsx->write(cell,s1);

        //QString logcatPath=qDetailedPath+QDir::separator()+model+"-"+arg_deviceId+QDir::separator()+"logcat";
        QString logcatPath=qDetailedPath+QDir::separator()+model+"-"+arg_deviceId;
        Format hyperlinkFormat;
        hyperlinkFormat=getHyperlinkFormat();
        QUrl url=QUrl::fromLocalFile(logcatPath);
        qXlsx->currentWorksheet()->writeHyperlink(qRowIndex,7,url,hyperlinkFormat,"具体详情","");


        cell="H"+QString::number(qRowIndex);
        qXlsx->write(cell,arg_startTime);

        cell="I"+QString::number(qRowIndex);
        qXlsx->write(cell,endTime);

        QFile file(gConfigDir+QDir::separator()+gPackageName+"_rowindex.txt");
        QTextStream outStream(&file);
        if(!file.open(QIODevice::Append))
        {
            file.close();
        }
        outStream<<deviceBrand + "=" + s1<<endl;
        file.close();


        qRowIndex++;
    }
}


void ReplayForm::createPerformance(const QString &arg_deviceId, QStringList arg_actual,QStringList arg_moduleCrashList)
{
    QString moduleName;
    QStringList moduleList;
    QStringList deviceModuleList;
    QStringList splitResult;
    QString str;

    QString deviceBrand;
    QString systemVersion;
    QString model;
    for(QString var:qMobileInfoList)
    {
        str=arg_deviceId+"=";
        if(var.startsWith(str))
        {
            splitResult=var.split("=");
            deviceBrand=splitResult.at(1)+"_"+splitResult.at(2);
            model=splitResult.at(2);
            systemVersion=splitResult.at(3);
        }
    }

    Format format=getNormalFormat();
    QString perValue;
    QString aveValue;
    QString cell;

    cell="A"+QString::number(qPerRowIndex);
    qXlsx->write(cell,deviceBrand,format);


    perValue=getMinMaxMemory(arg_deviceId,gMin_Mem_Hash);
    cell="B"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    perValue=getMinMaxMemory(arg_deviceId,gMax_Mem_Hash);
    cell="C"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    aveValue=getMemoryAveValue(arg_deviceId,gMemValue_list);
    cell="D"+QString::number(qPerRowIndex);
    qXlsx->write(cell,aveValue,format);


    perValue=getMinMaxCpu(arg_deviceId,gMin_Cpu_Hash);
    cell="E"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    perValue=getMinMaxCpu(arg_deviceId,gMax_Cpu_Hash);
    cell="F"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    aveValue=getCpuAveValue(arg_deviceId,gCpuValue_list);
    cell="G"+QString::number(qPerRowIndex);
    qXlsx->write(cell,aveValue,format);

    perValue=getMinMaxBatterytemp(arg_deviceId,gMin_Batterytemp_Hash);
    cell="H"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    perValue=getMinMaxBatterytemp(arg_deviceId,gMax_Batterytemp_Hash);
    cell="I"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    aveValue=getBatterytempAveValue(arg_deviceId,gBatteryTempValue_list);
    cell="J"+QString::number(qPerRowIndex);
    qXlsx->write(cell,aveValue,format);

    perValue=getMinMaxCputemp(arg_deviceId,gMin_Cputemp_Hash);
    cell="K"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    perValue=getMinMaxCputemp(arg_deviceId,gMax_Cputemp_Hash);
    cell="L"+QString::number(qPerRowIndex);
    qXlsx->write(cell,perValue,format);

    aveValue=getCputempAveValue(arg_deviceId,gCpuTempValue_list);
    cell="M"+QString::number(qPerRowIndex);
    qXlsx->write(cell,aveValue,format);

    QString wifimobileValue=getWifiMobile(arg_deviceId);
    splitResult=wifimobileValue.split("=");
    QString wifiValue=splitResult.at(0);
    QString mobileValue=splitResult.at(1);

    cell="N"+QString::number(qPerRowIndex);
    qXlsx->write(cell,wifiValue,format);

    cell="O"+QString::number(qPerRowIndex);
    qXlsx->write(cell,mobileValue,format);

    qPerRowIndex++;

}

Format ReplayForm::getHyperlinkFormat()
{
    QColor color(220,230,241,255);
    Format format;
    format.setFontColor(Qt::red);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setPatternBackgroundColor(color);
    format.setBorderStyle(Format::BorderThin);
    format.setFontUnderline(Format::FontUnderlineSingle);
    format.setFontName("Verdana");
    return format;
}
Format ReplayForm::getNormalFormat()
{
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);
    return format;
}

QImage ReplayForm::getImage()
{
    QImage icon(gIconPath);

    QImage fitimgpic;
    if(icon.width()>200)
    {
        fitimgpic=icon.scaled(icon.width()/2,icon.height()/2, Qt::KeepAspectRatioByExpanding);
    }
    else
    {
        fitimgpic=icon.scaled(icon.width(),icon.height(), Qt::KeepAspectRatioByExpanding);
    }
    return fitimgpic;
}


QString ReplayForm::getMinMaxMemory(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    double doubleValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    doubleValue=intValue/1024;
    strValue=QString::number(doubleValue,'f',2);
    return strValue;
}


QString ReplayForm::getMinMaxCpu(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    strValue=QString::number(intValue);
    return strValue;
}


QString ReplayForm::getMinMaxBatterytemp(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    strValue=QString::number(intValue*0.1);
    return strValue;
}

QString ReplayForm::getMinMaxCputemp(const QString &arg_deviceId,QHash<QString,int> arg_minmaxHash)
{
    int intValue;
    QString strValue;

    intValue=arg_minmaxHash.value(arg_deviceId);
    strValue=QString::number(intValue);
    return strValue;
}


QString ReplayForm::getMemoryAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue/1024;
        d=d/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString ReplayForm::getCpuAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString ReplayForm::getBatterytempAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue*0.1;
        d=d/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString ReplayForm::getCputempAveValue(const QString &arg_deviceId,QStringList Value_list)
{
    double d=0;
    double doubleValue=0;
    QString strValue;

    QString tempStr;
    QStringList splitResult;
    QStringList tempList=Value_list.filter(arg_deviceId);
    foreach(QString var,tempList)
    {
        splitResult=var.split("->");
        tempStr=splitResult.at(1);
        d=tempStr.toDouble();
        doubleValue=doubleValue+d;
    }

    int count=tempList.count();
    if(count>0)
    {
        d=doubleValue/count;
        strValue=QString::number(d,'f',2);
    }
    return strValue;
}

QString ReplayForm::getWifiMobile(const QString &arg_deviceId)
{
    QString lrxWifi;
    QString lrxMobile;
    QString tempStr;
    QString trafficValue;
    QStringList splitResult;

    if(gData_Traffic_Hash.isEmpty())
    {
        lrxWifi="";
        lrxMobile="";
    }
    else
    {
        tempStr=arg_deviceId + "=" + "currentStateWifi";
        if(gData_Traffic_Hash.keys().contains(tempStr))
        {
            trafficValue=gData_Traffic_Hash.value(tempStr);
            splitResult=trafficValue.split("=");
            lrxWifi=splitResult.at(0);
            double d=lrxWifi.toDouble();
            d=d/1024;
            lrxWifi=QString::number(d,'f',2);
        }
        else
        {
            lrxWifi="";
        }


        tempStr=arg_deviceId + "=" + "currentStateMobile";
        if(gData_Traffic_Hash.keys().contains(tempStr))
        {
            trafficValue=gData_Traffic_Hash.value(tempStr);
            splitResult=trafficValue.split("=");
            lrxMobile=splitResult.at(2);

            double d=lrxMobile.toDouble();
            d=d/1024;
            lrxMobile=QString::number(d,'f',2);
        }
        else
        {
            lrxMobile="";
        }
    }

    return lrxWifi+"="+lrxMobile;
}

void ReplayForm::mailTimerSlot()
{
    QString currentTime;
    QString settime;
    QDateTime datetime=QDateTime::currentDateTime();
    currentTime=datetime.toString("hh:mm");

    settime=Helper::getFirstLine(gConfigDir+QDir::separator()+"time.txt");
    if(settime.isEmpty())
    {
        settime="09:00";
    }

    if(currentTime==settime)
    {
        if(isRun)
        {
            emit sendStopReplay();
            isRun=false;
        }

        tt=new QTimer(this);
        connect(tt, SIGNAL(timeout()), this, SLOT(sendMailSlot()) );
        tt->start(1000*30);
    }//900
}

void ReplayForm::sendMailSlot()
{
    QString dailyXlsx;
    QStringList tempList;

    QFile file;
    gDatePath=QDir::toNativeSeparators(gDatePath);
    QDir dir(gDatePath);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList list=dir.entryList();
    for(QString var:list)
    {
        dailyXlsx=gDatePath + QDir::separator() + var + QDir::separator() + "daily" + gStartTextDate + ".xlsx";
        dailyXlsx=QDir::toNativeSeparators(dailyXlsx);
        tempList.append(dailyXlsx);

        int allTest=0;
        int errorTest=0;

        QString strLine;
        QStringList deviceList;
        QStringList crashList;
        QStringList splitResult;

        file.setFileName(gConfigDir+QDir::separator()+var+"_rowindex.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream inStream(&file);
        while(!inStream.atEnd())
        {
            strLine=inStream.readLine();
            splitResult=strLine.split("=");
            if(deviceList.indexOf(splitResult.at(0))==-1)
            {
                deviceList.append(splitResult.at(0));
            }
            if(strLine.contains("Fail"))
            {
                if(crashList.indexOf(strLine)==-1)
                {
                    crashList.append(strLine);
                }
                errorTest++;
            }
            allTest++;
        }
        file.close();

        double per;
        QString cell;
        QString appName;
        QString strPer;

        int allDevice=deviceList.count();
        int crashDevice=crashList.count();
        int goodDevice=allDevice-crashDevice;

        Document qXlsx2(dailyXlsx);
        qXlsx2.selectSheet("base");
        appName=qXlsx2.cellAt(1,3)->value().toString();


        per=(double)goodDevice/allDevice*100;
        strPer=QString::number(per,'f',2);

        cell="C2";
        qXlsx2.write(cell,QString::number(goodDevice)+"/"+QString::number(allDevice));

        int goodTest=allTest-errorTest;
        per=(double)goodTest/allTest*100;
        strPer=QString::number(per,'f',2);

        qDebug()<<goodTest;
        qDebug()<<allTest;
        qDebug()<<per;
        qDebug()<<strPer;


        cell="E2";
        qXlsx2.write(cell,QString::number(goodTest)+"/"+QString::number(allTest));


        appName=qXlsx2.cellAt(1,3)->value().toString();

        QStringList startTimeList;
        file.setFileName(gConfigDir+QDir::separator()+var+"_starttime.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream inStream3(&file);
        while(!inStream3.atEnd())
        {
            strLine=inStream3.readLine();
            if(!strLine.trimmed().isEmpty())
            {
                startTimeList.append(strLine.trimmed());
            }
        }
        file.close();


        QStringList endTimeList;
        file.setFileName(gConfigDir+QDir::separator()+var+"_endtime.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream inStream4(&file);
        while(!inStream4.atEnd())
        {
            strLine=inStream4.readLine();
            if(!strLine.trimmed().isEmpty())
            {
                endTimeList.append(strLine.trimmed());
            }
        }
        file.close();


        qint64 t=0;
        qint64 t2=0;
        QString startT;
        QString endT;

        int n1=startTimeList.count();
        int n2=endTimeList.count();
        if(n1==n2)
        {
            for(int i=0;i<n1;i++)
            {
                startT=startTimeList.at(i);
                endT=endTimeList.at(i);
                QDateTime d1=QDateTime::fromString(startT,"yyyy-MM-dd hh:mm:ss");
                QDateTime d2=QDateTime::fromString(endT,"yyyy-MM-dd hh:mm:ss");
                t=d1.msecsTo(d2)/1000;
                t2=t2+t;
            }
            int hours=t2/3600;
            int re=t2%3600;
            int minutes=re/60;
            int seconds=re%60;
            QString times=QString::number(hours)+"时"+QString::number(minutes)+"分"+QString::number(seconds)+"秒";
            cell="G2";
            qXlsx2.write(cell,times);
        }

        qXlsx2.save();


        file.setFileName(gConfigDir+QDir::separator() + var+"_mailContent_daily.txt");
        QTextStream outStream(&file);
        if(!file.open(QIODevice::WriteOnly))
        {
            file.close();
        }

        QString tempStr;

        tempStr="To All:";
        outStream<<tempStr<<endl;

        tempStr="APP名称: "+appName;
        outStream<<tempStr<<endl;
        qDebug()<<tempStr;

        tempStr="测试平台: android";
        outStream<<tempStr<<endl;

        tempStr="测试设备: " + deviceList.join(",");
        outStream<<tempStr<<endl;


        tempStr="测试设备通过数/总数: "+QString::number(goodDevice)+"/"+QString::number(allDevice);
        outStream<<tempStr<<endl;

        tempStr="测试通过率: "+QString::number(goodTest)+"/"+QString::number(allTest);
        outStream<<tempStr<<endl;

        tempStr="测试日期: "+gStartTextDate;
        outStream<<tempStr<<endl;

        tempStr="------------------------------------------------------";
        outStream<<tempStr<<endl;

        tempStr="Brs";
        outStream<<tempStr<<endl;

        tempStr="小王子团队";
        outStream<<tempStr<<endl;

        tempStr="Tel: 何畅022-65303756 OR 周朝彬010-56602399";
        outStream<<tempStr<<endl;

        tempStr="Email: changhe@sohu-inc.com OR chaobinzhou@sohu-inc.com";
        outStream<<tempStr<<endl;

        tempStr="QQ: 周朝彬1787072341";
        outStream<<tempStr<<endl;
        file.close();

        QString filename=gConfigDir + QDir::separator() + "emailsender.txt";
        tempList=Helper::getList(filename);
        if(tempList.size()>=2)
        {
            QString senderName=tempList.at(0);
            QString senderKey=tempList.at(1);
            QString cmdLine;

            QString subject=appName + "自动化测试-日报-" + gStartTextDate;
            QString mailContentFile=gConfigDir + QDir::separator() + var +"_mailContent_daily.txt";
            mailContentFile=QDir::toNativeSeparators(mailContentFile);

            QString receiver=gConfigDir + QDir::separator() + "email.txt";

            cmdLine="cmd /c java -jar " + gConfigDir + QDir::separator() + "sendmail2.jar" + " " + senderName + " " + senderKey + " " + receiver +  " " + subject + " " + mailContentFile + " " + dailyXlsx;
            cmdLine=cmdLine.trimmed();
            //Helper::runCmd(cmdLine);

            AdbShellController::Controller *controller=new AdbShellController::Controller;
            controller->setMark("getAll","sendmail");
            controller->setCmdLine(cmdLine);
            //connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
            controller->doController();
        }
        QFile file(gConfigDir+QDir::separator()+var+"_rowindex.txt");
        file.remove();

        file.setFileName(gConfigDir+QDir::separator()+var+"_starttime.txt");
        file.remove();

        file.setFileName(gConfigDir+QDir::separator()+var+"_endtime.txt");
        file.remove();

        Helper::writeStr(gConfigDir+QDir::separator()+var+"_base.txt",QString::number(5));
        Helper::writeStr(gConfigDir+QDir::separator()+var+"_performance.txt",QString::number(3));
    }//for

    tt->stop();

    restartscript();
}

void ReplayForm::treeItemChanged(QTreeWidgetItem *item, int column)
{
    QString itemText = item->text(0);
    if (item->checkState(0) == Qt::Checked)
    {
        int cnt = item->childCount();
        if (cnt >0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Checked);
            }
        }
        else
        {
            StaticticsHerper::updateParentItem(item);
        }
    }
    else if (item->checkState(0) == Qt::Unchecked)
    {
        int cnt = item->childCount();
        if (cnt > 0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
        else
        {
            StaticticsHerper::updateParentItem(item);
        }
    }
}

void ReplayForm::on_scriptTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->childCount()==0&&item->parent()!=NULL)
    {
        if(item->checkState(0) == Qt::Checked)
        {
            if(item!=NULL)
            {
                QString rootText=StaticticsHerper::getTreeRootText(item);
                if(rootText!="suite")//不是测试套文件，正常添加
                {
                    QString tempStr=item->parent()->text(0)+"="+item->text(0).left(item->text(0).length()-4);
                    qScriptList.append(tempStr);
                }
                else//测试套文件，需要解析再添加
                {
                    QString filePath=currentScriptPath+QDir::separator()+StaticticsHerper::getBasePath(item);
                    QStringList scriptList = EditScriptFileWidget::readSuiteFile(filePath);
                    qScriptList.append(scriptList);
                }
            }
        }

        else
        {
            if(StaticticsHerper::getTreeRootText(item)!="suite")//不是测试套文件，正常删除
            {
                QString tempStr=item->parent()->text(0)+"="+item->text(0).left(item->text(0).length()-4);
                qScriptList.removeAt( qScriptList.indexOf(tempStr) );
            }
            else
            {
                QString filePath=currentScriptPath+QDir::separator()+StaticticsHerper::getBasePath(item);
                QStringList scriptList = EditScriptFileWidget::readSuiteFile(filePath);
                for(int i=0;i<scriptList.count();i++)
                {
                    qScriptList.removeAt( qScriptList.indexOf(scriptList[i]) );
                }
            }
        }
    }
}

void ReplayForm::on_scriptTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item!=NULL)
    {
        QString strFile=currentScriptPath+QDir::separator()+ StaticticsHerper::getBasePath(item);//!!!!!!!!!

        strFile=QDir::toNativeSeparators(strFile);

        qDebug()<<strFile;
        QFile file(strFile);
        if(!file.open(QIODevice::ReadOnly))
        {
            file.close();
            return;
        }
        QTextStream inStream(&file);
        QTextCodec *tc=QTextCodec::codecForName("GBK");
        inStream.setCodec(tc);
        QString ss=inStream.readAll();


        if(formPointer.isNull())
        {
            formInstance=new Form();
            formPointer=formInstance;
            formInstance->setAttribute(Qt::WA_QuitOnClose,false);
            formInstance->setWindowFlags(Qt::Window);
            formInstance->setScriptText(ss);
            formPointer->show();
            formPointer->raise();
        }
        else
        {
            formInstance->setScriptText(ss);
            formPointer->show();
            formPointer->raise();
        }
    }
}


void ReplayForm::restartscript()
{
    qStatisticsDataList.clear();
    getStatisticsData();
    qFirstLevelDirName=qRepoName + "report_" + Helper::getTime2("yyyyMMdd_hhmmss");
    qDetailedFolderName=qRepoName + "report_" + Helper::getTime2("yyyyMMdd_hhmmss");

    gdLoop=PreSet::strToInt1(gLoop);
    gdInterval=PreSet::strToInt2(gInterval);

    preSets();
    createReportSaveDir();

    qDetailedTargetXlsx=qDetailedPath + QDir::separator() + "detail-report.xlsx";
    qDetailedTargetXlsx=QDir::toNativeSeparators(qDetailedTargetXlsx);

    qDailyTargetXlsx=gPackagePath + QDir::separator() + "daily"+gStartTextDate+".xlsx";
    qDailyTargetXlsx=QDir::toNativeSeparators(qDailyTargetXlsx);

    QFile::copy(qDailySourceXlsx,qDailyTargetXlsx);
    qXlsx=new Document(qDailyTargetXlsx);

    createDetailReport(qDetailedTargetXlsx,qDeviceModelIdNameList,qScriptList);

    createWriteResult(qDetailedTargetXlsx);
    writeBaseInfo(qDeviceIdList);

    createCrashReport();

    isRun=true;
    startPerformance(qDeviceIdList);
    startRunScript(qDeviceIdList,qDeviceModelIdNameList,qScriptList);
}
