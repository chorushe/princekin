#include "behaviourwidget.h"
#include "ui_behaviourwidget.h"
#include <QScrollBar>

BehaviourWidget::BehaviourWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BehaviourWidget)
{
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    setWindowTitle("埋点统计");
    ui->scriptTextEdit->setReadOnly(true);

    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(1,3);
    ui->splitter->setStretchFactor(2,1);
    ui->splitter->setStretchFactor(3,1);
    ui->splitter_2->setStretchFactor(0,5);
    ui->splitter_2->setStretchFactor(1,5);
    ui->splitter_3->setStretchFactor(0,3);
    ui->splitter_3->setStretchFactor(1,7);

    ui->verticalLayout->setStretch(0,4);
    ui->verticalLayout->setStretch(1,3);

    QTreeWidgetItem *fileItem = new QTreeWidgetItem(QStringList() << "XML文件列表", 1);   //1表示是文件
    ui->xmlTreeWidget->addTopLevelItem(fileItem);
    ui->xmlTreeWidget->setHeaderHidden(true);
    ui->xmlTreeWidget->setMouseTracking(true);

    ui->urlListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);//设置可选中多行
    // urlListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);//不是一条一条的滚动。。
    ui->urlListWidget->scrollToBottom();

    urlList.append("URL列表");
    QListWidgetItem *item=new QListWidgetItem(ui->urlListWidget);
    QLabel *tmpLabel=new QLabel;
    tmpLabel->setText("<font color=gray>URL列表</font>");
    ui->urlListWidget->setItemWidget(item,tmpLabel);

    equipModel = new QStringListModel(equipList);
    ui->statisticsListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);

    QListWidgetItem *item1=new QListWidgetItem(ui->statisticsListWidget);
    QLabel *tmpLabel1=new QLabel;
    tmpLabel1->setText("<font color=gray>统计点列表</font>");
    ui->statisticsListWidget->setItemWidget(item1,tmpLabel1);

    createToolBar();

    urlToXmlDialog=new UrlToXml();
    editMultiXmlDialog=new EditMultiXML();
    setupDialog=new BehaviourSetup();

    revelantHead="";//初始化
    xmlCount=0;
    focusFlag=0;

    currentPath=gWorkSpace;//初始化加载xml文件的路径也为工作目录

    this->isServerPattern=false;
    this->isDebugVersion=true;

    ui->xmlTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                          "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->urlListWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                          "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->urlListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                            "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");
    ui->statisticsListWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                                 "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->statisticsListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                                   "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");
    ui->xmlTreeWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                            "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");
    ui->equipListView->setStyleSheet("QListView{font-size:12px;background:#ffffff;border:1px #ffffff;}"
                                     "QListView::item:selected:active{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                     "QListView::item:selected{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                     "QListView::item{height:30px;color:#4d4d4d}");

    ui->urlListWidget->setStyleSheet("QListWidget{font-size:12px;background:#ffffff;border:1px #ffffff;}");

    ui->statisticsListWidget->setStyleSheet("QListWidget{font-size:12px;background:#ffffff;border:1px #ffffff;}");

    ui->scriptTreeWidget->setStyleSheet("QTreeWidget{background:#ffffff;border:1px #ffffff;}");
    ui->scriptTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                             "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->scriptTextEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                           "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    QTreeWidgetItem *fileItem1 = new QTreeWidgetItem(QStringList() << "脚本文件列表", 1);   //1表示是文件
    ui->scriptTreeWidget->addTopLevelItem(fileItem1);
    ui->scriptTreeWidget->setHeaderHidden(true);
    ui->scriptTreeWidget->setMouseTracking(true);

    ui->urlListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);//设置可选中多行
    ui->scriptTextEdit->append("脚本日志");

    connect(setupDialog,SIGNAL(SendData(QString, bool,bool,bool,bool,bool,bool,QString,bool,bool,QString,QString,QString,QString,QString,QString)),this,SLOT(RecieveData(QString, bool,bool,bool,bool,bool,bool,QString,bool,bool,QString,QString,QString,QString,QString,QString)));
    connect(settingButton,SIGNAL(clicked(bool)),this,SLOT(SetupBtnClicked()));
    connect(this,SIGNAL(sendStateChange(bool)),setupDialog,SLOT(stateChanged(bool)));
    connect(this,SIGNAL(sendSettingSig(bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)),setupDialog,SLOT(recieveSettingSig(bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)));

    connect(createXmlButton,SIGNAL(clicked(bool)),this,SLOT(CreateXmlBtnClicked()));
    connect(addXmlButton,SIGNAL(clicked(bool)),this,SLOT(AddXmlBtnClicked()));
    connect(editXmlButton,SIGNAL(clicked(bool)),this,SLOT(EditXmlBtnClicked()));
    connect(addScriptButton,SIGNAL(clicked(bool)),this,SLOT(AddScriptBtnClicked()));

    pLogcat=new QProcess(this);
    connect(pLogcat,SIGNAL(readyReadStandardOutput()),this,SLOT(RecieveLogcatOutput()));

    //给两个显示控件添加事件过滤器，判断是否处于焦点
    ui->urlListWidget->installEventFilter(this);
    ui->statisticsListWidget->installEventFilter(this);
    ui->urlListWidget->viewport()->installEventFilter(this);
    ui->statisticsListWidget->viewport()->installEventFilter(this);

    connect(ui->xmlTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
    connect(ui->scriptTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));

    ui->xmlTreeWidget->setVisible(false);//一开始的脚本模式，去掉xml文件列表的显示
    toolBar3->setVisible(false);
    //初始化脚本模式xml文件的根路径
    xmlRootPath = gWorkSpace + QDir::separator() + "StatisticalXML";
}

BehaviourWidget::~BehaviourWidget()
{
    delete ui;
}

void BehaviourWidget::createToolBar()
{
    toolBar2=addToolBar("脚本");
    toolBar2->setStyleSheet("QToolBar{background:#ffffff;}");

    addScriptButton=new QToolButton(this);
    addScriptButton->setToolTip("加载脚本文件");
    addScriptButton->setText("加载脚本文件");
    addScriptButton->setFixedWidth(130);
    addScriptButton->setFixedHeight(35);
    addScriptButton->setStyleSheet("QToolButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                   "QToolButton::hover{background-color:#c1904d;color:#ffffff}");
    toolBar2->addWidget(addScriptButton);

    toolBar3=addToolBar("xml");
    toolBar3->setStyleSheet("QToolBar{background:#ffffff;}");

    addXmlButton=new QToolButton(this);
    addXmlButton->setToolTip("加载XML文件");
    addXmlButton->setText("加载XML文件");
    addXmlButton->setFixedWidth(130);
    addXmlButton->setFixedHeight(35);
    addXmlButton->setStyleSheet("QToolButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                "QToolButton::hover{background-color:#c1904d;color:#ffffff}");
    toolBar3->addWidget(addXmlButton);

    toolBar1=addToolBar("xml");
    toolBar1->setStyleSheet("QToolBar{background:#ffffff;}");

    createXmlButton=new QToolButton(this);
    createXmlButton->setToolTip("XML文件生成");
    createXmlButton->setText("XML文件生成");
    createXmlButton->setFixedWidth(130);
    createXmlButton->setFixedHeight(35);
    createXmlButton->setStyleSheet("QToolButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                   "QToolButton::hover{background-color:#c1904d;color:#ffffff}");
    toolBar1->addWidget(createXmlButton);

    editXmlButton=new QToolButton(this);
    editXmlButton->setToolTip("批量修改XML");
    editXmlButton->setText("批量修改XML");
    editXmlButton->setFixedWidth(130);
    editXmlButton->setFixedHeight(35);
    editXmlButton->setStyleSheet("QToolButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                 "QToolButton::hover{background-color:#c1904d;color:#ffffff}");
    toolBar1->addWidget(editXmlButton);

    toolBar=addToolBar("setting");
    toolBar->setStyleSheet("QToolBar{background:#ffffff;}");
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spacer->setStyleSheet("QWidget{background:#ffffff;}");
    toolBar->addWidget(spacer);

    scriptLabel=new QLabel("脚本模式 ");
    toolBar->addWidget(scriptLabel);

    scriptSC=new SwitchControl;
    scriptSC->setToggle(true);
    scriptSC->setCheckedColor(QColor("#c1904d"));
    scriptSC->setFixedSize(40,20);
    toolBar->addWidget(scriptSC);
    connect(scriptSC,SIGNAL(toggled(bool)),this,SLOT(ScriptSwitch(bool)));

    settingButton=new QToolButton(this);
    settingButton->setToolTip("设置");
    settingButton->setIcon(QIcon(":/setting.png"));
    settingButton->setFixedWidth(50);
    settingButton->setStyleSheet("QToolButton{background:#ffffff;}");
    toolBar->addWidget(settingButton);
}

void BehaviourWidget::ScriptSwitch(bool state)
{
    if(state)
    {
        scriptLabel->setText("脚本模式 ");
        ui->xmlTreeWidget->setVisible(false);
        ui->scriptTreeWidget->setVisible(true);
        ui->scriptTextEdit->setVisible(true);
        toolBar2->setVisible(true);
        toolBar3->setVisible(false);
        scriptPatternFlag=true;
    }
    else
    {
        scriptLabel->setText("手动模式 ");
        ui->xmlTreeWidget->setVisible(true);
        ui->scriptTreeWidget->setVisible(false);
        ui->scriptTextEdit->setVisible(false);
        toolBar2->setVisible(false);
        toolBar3->setVisible(true);
        scriptPatternFlag=false;
    }
}

void BehaviourWidget::showInit()
{
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

    emit sendSettingSig(isMemCheck,isCpuCheck,isBatteryCheck,isCpuTempCheck,isWifiCheck,isMobileCheck,memThres,cpuThres,batteryThres,cpuTempThres,wifiThres,mobileThres);
}

void BehaviourWidget::on_xmlTreeWidget_itemClicked(QTreeWidgetItem *item, int column)//ReadXML()
{
    if(ui->xmlTreeWidget->currentItem()->text(0)=="XML文件列表")//第一次点方框时没有当前选中项，这里会失败
    {
        // SetupBtnClicked();//加载按钮做到菜单栏去了，所以点击XML文件列表不做处理,但因为会失败，所以这里还是if出来了
    }
    else
    {
        //点击xml文件，定位右侧URL位置
        QString uniqueTemp="";
        QMap<QString,QList<DataClass> >::iterator it;
        for(it=allXmlData.begin();it!=allXmlData.end();++it)
        {
            if(it.key()== StaticticsHerper::getBasePath(ui->xmlTreeWidget->currentItem()) )
            {
                uniqueTemp=it.value()[0].UniqueIdenti;
                break;
            }
        }

        for(int i=0;i<urlList.count();i++)
        {
            if(urlList[i].contains(uniqueTemp)&&uniqueTemp!="")
            {
                ui->urlListWidget->setCurrentRow(i);
                QString url=urlList[i];

                ParseURL(url,true);
                break;
            }
        }

    }
}

//点击checkbox时，这里会响应，readxml也会响应，但点xml文件时，这里不响应，只有readxml响应
void BehaviourWidget::on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column)//ItemStateChanged(QTreeWidgetItem*,int)
{
    if(item->checkState(0) == Qt::Checked)
    {
        if(item->childCount()==0)
        {
            QLabel *tmplabel=new QLabel;
            tmplabel->setText("<font color=orange>...</font>");
            ui->xmlTreeWidget->setItemWidget(item,1,tmplabel);
            QString xmlFilePath=currentPath+"/"+ StaticticsHerper::getBasePath(item);//!!!!!!!!!
            QList<DataClass> tempData = GetXML(xmlFilePath);
            if(tempData.isEmpty())
            {
                item->setCheckState(0,Qt::Unchecked);
                return;
            }
            allXmlData.insert(StaticticsHerper::getBasePath(item),tempData);
            xmlCount++;

        }
    }
    else
    {
        if(item->childCount()==0)
        {
            //清除没有被选中的第二行标识
            QLabel *tmplabel=new QLabel;
            tmplabel->setText("");
            ui->xmlTreeWidget->setItemWidget(item,1,tmplabel);
            xmlCount--;
            //删除allXMLData数据
            QMap<QString,QList<DataClass> >::iterator it;
            it=allXmlData.find(StaticticsHerper::getBasePath(item) );
            allXmlData.erase(it);
        }
    }

    int width=ui->xmlTreeWidget->width();

    ui->xmlTreeWidget->setColumnCount(2); //设置列数
    ui->xmlTreeWidget->setColumnWidth(0,width*0.88);
    ui->xmlTreeWidget->setColumnWidth(1,width*0.08);
}

void BehaviourWidget::on_urlListWidget_itemPressed(QListWidgetItem *item)//urlListClicked()
{
    if(ui->urlListWidget->currentItem()->text()=="URL列表")
        return;
    int row=ui->urlListWidget->currentRow();
    QString url=urlList[row];

    if(url=="")
        ui->statisticsListWidget->clear();
    else
    {
        if(!scriptPatternFlag)
            ParseURL(url,true);
        else
            ParseURLForCheck(url,allXmlDataForCheck.at(row),true);
    }
}

void BehaviourWidget::on_xmlTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)//editXml()
{
    if(ui->xmlTreeWidget->currentItem()->childCount()==0)
    {
        QString xmlFilePath=currentPath+QDir::separator()+ StaticticsHerper::getBasePath(ui->xmlTreeWidget->currentItem());//!!!!!!!!!!
        XmlEditDialog *editXMLDialog=new XmlEditDialog(0,xmlFilePath);
        editXMLDialog->show();
    }
}

void BehaviourWidget::on_statisticsListWidget_itemClicked(QListWidgetItem *item)//statisticsListClicked()
{
    QString statisticsStr=ui->statisticsListWidget->currentItem()->text();
    if(statisticsStr=="统计点列表")
        return;
    QString tmp="----";
    int num=statisticsStr.indexOf(tmp);
    statisticsStr=statisticsStr.left(num);
    statisticsStr=statisticsStr.replace(" : ","=");

    if(statisticsStr.contains("×")||statisticsStr.contains("√"))
    {
        statisticsStr=statisticsStr.mid(1).trimmed();
        qDebug()<<statisticsStr;
    }
    if(statisticsStr.contains(" / "))
    {
        statisticsStr = statisticsStr.left( statisticsStr.indexOf(" / ") );
        qDebug()<<statisticsStr;
    }

    QString url=urlList[ui->urlListWidget->currentRow()];
    QString urlBefore=url.left(url.indexOf(statisticsStr));
    urlBefore=urlBefore.replace("&","& ");
    QString urlAfter=url.mid(url.indexOf(statisticsStr)+statisticsStr.length());
    urlAfter=urlAfter.replace("&","& ");
    statisticsStr=statisticsStr.replace("&","& ");

    QListWidgetItem *item1=ui->urlListWidget->currentItem();
    QLabel *tmpLabel=new QLabel;
    tmpLabel->setWordWrap(true);
    tmpLabel->setText(urlBefore+"<font color=red>"+statisticsStr+"</font>"+urlAfter);
    ui->urlListWidget->setItemWidget(item1,tmpLabel);
}

void BehaviourWidget::exportReport()
{
    if(gWorkSpace=="")
        gWorkSpace=gNewDisk + QDir::separator() + "princekinWorkspace";
    QString deviceStr=ExeCmd::GetDeviceModel(deviceName.replace(" ",""))+"-"+ deviceName.replace(" ","");//去掉空格


    fileNameForReport=gWorkSpace+QDir::separator() + "report" + QDir::separator()+ "Behaviour_" + deviceName + "_" + qStartTime;
    Helper::createPath(fileNameForReport);

    QString urlFileName=qUrlTxtPath;
    QString statisticsFileName=qStatisticsTxtPath;


    if ((!urlFileName.isNull())&&(!statisticsFileName.isNull()))
    {
        QFileInfo fi=QFileInfo(urlFileName);
        QString filePath=fi.absolutePath();

        //fileName是文件名
        QFile f1(urlFileName);
        f1.open(QIODevice::WriteOnly|QIODevice::Text);//这里创建文件，以只写方式打开
        if(!urlList.isEmpty())
        {
            for(int i=0;i<urlList.count();i++)
            {
                if(!urlErrorList.isEmpty())
                {
                    if(urlErrorList[i]==false)
                    {
                        f1.write("/***ERROR***/");
                        f1.write("\r\n");
                    }
                }
                f1.write(urlList[i].toStdString().c_str());
                f1.write("\r\n\r\n");
            }
        }
        f1.close();

        QFile f2(statisticsFileName);
        f2.open(QIODevice::WriteOnly|QIODevice::Text);//这里创建文件，以只写方式打开
        for(int i=0;i<ui->statisticsListWidget->count();i++)
        {
            f2.write(ui->statisticsListWidget->item(i)->text().toStdString().c_str());
            f2.write("\r\n");
        }

        f2.close();
        urlErrorNum=0;
        for(int i=0;i<urlErrorList.count();i++)
        {
            if(!urlErrorList[i])
                urlErrorNum++;
        }
        urlTotalNum=urlErrorList.count();
        qDebug()<<"urlTotalNum"<<urlTotalNum<<"urlerrornum"<<urlErrorNum;

        if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
        {
            if(assist!=NULL)
            {
                createExcel();
            }
        }
        else
        {
            createBaseExcel();
        }
    }
}

void BehaviourWidget::on_clearBtn_clicked()
{
    ui->urlListWidget->clear();
    urlList.clear();
    urlErrorList.clear();
    ui->statisticsListWidget->clear();
    ui->scriptTextEdit->clear();
    allXmlData.clear();
    xmlData.clear();
    xmlMatch.clear();
    revelantHead="";

    urlTimeList.clear();
    numForUrl=0;
    singleXmlData.clear();
    urlListFour.clear();
    allXmlDataForCheck.clear();//装载所有要检测的xml信息，为了点击之前的url时对其进行解析
    statRes.clear();
    qScriptList.clear();

    lineNumberList.clear();

    //清空是把之前的检测结果恢复初始状态
    QTreeWidgetItemIterator item(ui->xmlTreeWidget);
    while(*item)
    {
        if((*item)->checkState(0) == Qt::Checked)
        {
            if((*item)->childCount()==0)
            {
                //设置被选中的文件是...状态
                QLabel *tmplabel=new QLabel;
                tmplabel->setText("<font color=orange>...</font>");
                ui->xmlTreeWidget->setItemWidget((*item),1,tmplabel);

                QString xmlFilePath=currentPath+"/"+ StaticticsHerper::getBasePath((*item));//!!!!!!!!!
                QList<DataClass> tempData = GetXML(xmlFilePath);
                if(!tempData.isEmpty())
                    allXmlData.insert(StaticticsHerper::getBasePath((*item)),tempData);
            }
        }
        else
        {
            //清除没有被选中的第二行标识
            QLabel *tmplabel=new QLabel;
            tmplabel->setText("");
            ui->xmlTreeWidget->setItemWidget((*item),1,tmplabel);
        }
        ++item;
    }

    QTreeWidgetItemIterator item1(ui->scriptTreeWidget);
    while(*item1)
    {
        if((*item1)->checkState(0) == Qt::Checked)
        {
            if((*item1)->childCount()==0&&(*item)->parent()!=NULL)
            {
                //设置被选中的文件是...状态
                QLabel *tmplabel=new QLabel;
                tmplabel->setText("<font color=orange>...</font>");
                ui->scriptTreeWidget->setItemWidget((*item1),1,tmplabel);

                QString tempStr=(*item1)->parent()->text(0)+"="+(*item1)->text(0).left((*item1)->text(0).length()-4);
                qScriptList.append(tempStr);
            }
        }
        else
        {
            //清除没有被选中的第二行标识
            QLabel *tmplabel=new QLabel;
            tmplabel->setText("");
            ui->scriptTreeWidget->setItemWidget((*item1),1,tmplabel);
        }
        ++item1;
    }
}

void BehaviourWidget::on_startBtn_clicked()
{
    if(deviceName=="")
    {
        QMessageBox::information(this,"提示","请选择一个设备");
        return;
    }
    if(revelantHead==""&&!scriptPatternFlag)
    {
        QMessageBox::information(this,"提示","请选择一个正确的xml文件");
        return;
    }
    if(scriptPatternFlag)//脚本模式，运行脚本
    {
        if(qScriptList.count()==0)
        {
            QMessageBox::information(this,"提示","请选择一个脚本文件，或选择手动模式");
            return;
        }
    }

    QString mFile=gConfigDir + QDir::separator() + "packageName.txt";
    QFileInfo info(mFile);
    if(!info.exists())
    {
        QMessageBox::information(this,"提示","请在工具箱进行apk信息提取");
        return;
    }

    gOldPackageName=Helper::getFirstLine(gConfigDir + QDir::separator() + "packageName.txt");


    if(isStartFlag)//已经开始的状态，点击停止
    {
        if(scriptPatternFlag)
            //添加停止脚本逻辑,脚本停止之后会自动调用stopRun（）
            emit sendStopReplay();
        else
            stopRun();
    }
    else
    {
        if((ui->urlListWidget->count()==1)&&(urlList[0]=="URL列表"))
        {
            ui->urlListWidget->clear();
            urlList.clear();
            urlErrorList.clear();
            ui->statisticsListWidget->clear();
        }

        //*****************20170505*************************//
        //getLunchTime();
        qStatisticsDataList.clear();
        //*****************20170505*************************//

        if(isServerPattern)//服务器模式
        {
            QMap<QString,QString>::iterator it;
            for(it=xmlMatch.begin();it!=xmlMatch.end();++it)
            {
                ServerControl *tmp=new ServerControl;
                QFile file(gConfigDir+QDir::separator()+"serverCmd.sh");
                if(!file.exists())
                {
                    QMessageBox::warning(this,"警告",gConfigDir+QDir::separator()+"serverCmd.sh 文件不存在");
                    return;
                }
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream in(&file);
                    tmp->cdCmd= in.readLine()+it.value()+"\n";
                    qDebug()<<tmp->cdCmd;
                    tmp->getCmd=in.readLine()+UID+ "\n";
                    qDebug()<<tmp->getCmd;
                    server.append( tmp);
                }
                else
                {
                    QMessageBox::warning(this,"警告",gConfigDir+QDir::separator()+"serverCmd.sh 文件打开失败");
                    return;
                }
            }
            for(int i=0;i<server.count();i++)
            {
                connect(server[i],SIGNAL(SendReadOutput(QString)),this,SLOT(readFromServerClass(QString)));
                server[i]->StartControl();
            }
        }
        else//代理模式
        {
            QString cmdStr=gConfigDir + QDir::separator() +"behavior_cmd.bat\n";
            qDebug()<<cmdStr;

            BehaviorWorker *worker=new BehaviorWorker;
            worker->cmdStr=cmdStr;
            QThread *workerThread=new QThread(this);
            connect(workerThread,SIGNAL(started()),worker,SLOT(startStat()));
            connect(worker,SIGNAL(statStop()),workerThread,SLOT(quit()));
            connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));
            connect(worker,SIGNAL(statStop()),this,SLOT(RecieveStopSignal()));

            connect(worker,SIGNAL(readOutputFromWorker(QString)),this,SLOT(ReadStandardOutput(QString)));
            connect(worker,SIGNAL(readErrorFromWorker(QString)),this,SLOT(ReadStandardError(QString)));
            connect(this,SIGNAL(sendStopStatSignal()),worker,SLOT(recieveStopStatSignal()));

            worker->moveToThread(workerThread);
            workerThread->start();
        }
        if(scriptPatternFlag)//脚本模式，运行脚本
        {
            startRunScript();
        }

        ui->startBtn->setIcon(QIcon(":/stop.png"));
        isStartFlag=true;
        emit sendStateChange(true);//开始之后，设置中的内存温度等选项失效

        if(isCpuTempCheck||isBatteryCheck||isMemCheck||isCpuCheck||isWifiCheck||isMobileCheck)
        {
            assist=new AssistMeasure();
            assist->deviceModel=ExeCmd::GetDeviceModel( deviceName )+" + "+deviceName;

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

        if(gWorkSpace=="")
            gWorkSpace=gNewDisk + QDir::separator() + "princekinWorkspace";

        qStartTime=Helper::getTime2("yyyyMMdd_hhmmss");
        fileNameForReport=gWorkSpace+QDir::separator() + "report" + QDir::separator()+ "Behaviour_" + deviceName + "_" + qStartTime;
        Helper::createPath(fileNameForReport);

        qUrlTxtPath=fileNameForReport + QDir::separator() + "url_" + qStartTime +".txt";
        qStatisticsTxtPath=fileNameForReport + QDir::separator() + "statistics_"+  qStartTime +".txt";
        qReportTxtPath=fileNameForReport + QDir::separator() + "report_"+  qStartTime +".txt";
        qErrorReportPath=fileNameForReport + QDir::separator() + "BehaviorErrorReport_"+  qStartTime +".txt";
        //*****************20170505*****************//

        QString errorFileName=qErrorReportPath;

        //建立logcat文件
        if(isMemCheck||isCpuCheck||isBatteryCheck||isCpuTempCheck||isWifiCheck||isMobileCheck)
        {
            fileLogcat=new QFile(errorFileName);
            if ( !fileLogcat->exists())
                fileLogcat->open( QIODevice::WriteOnly );
        }

        getXXX();
    }
}

void BehaviourWidget::RecieveStopSignal()
{
    if(isStartFlag)//已经开始的状态，点击停止
    {
        if(scriptPatternFlag)
            //添加停止脚本逻辑,脚本停止之后会自动调用stopRun（）
            emit sendStopReplay();
        else
            stopRun();
    }
}

void BehaviourWidget::startRunScript()
{
    statResForScript.clear();
    tempFlagForStat=true;

    QString secondLevelDirName;
    secondLevelDirName=ui->equipListView->currentIndex().data().toString().replace("+","-").replace(" ","");
    QThreadPool *pool=new QThreadPool;
    ReplayForBehaviorController *controllerInstance=new ReplayForBehaviorController;

    controllerInstance->setAutoDelete(true);
    controllerInstance->setDeviceId(deviceName);
    controllerInstance->setFirstLevelDirName(fileNameForReport);
    controllerInstance->setSecondLevelDirName(secondLevelDirName);
    controllerInstance->setScriptList(qScriptList);

    connect(controllerInstance,SIGNAL(sendRunOneScriptResult(const QString &)),this,SLOT(receiveRunScriptResult(const QString &))   );//接收到的每一条信息
    connect(controllerInstance,SIGNAL(sendReplayResult(const QString &,const QString &,QMap<QString,bool> ,int) ),this,SLOT(receiveReplayResult(const QString &,const QString &,QMap<QString,bool> ,int))  );//所有脚本跑完后结束信息
    connect(controllerInstance,SIGNAL(sendErrorResult(const QString &,const QString &) ),this,SLOT(receiveErrorResult(const QString &,const QString &))  );
    connect(this,SIGNAL(sendStopReplay()),controllerInstance,SLOT(receiveStopReplay())  ,Qt::DirectConnection);
    connect(controllerInstance,SIGNAL(sendOneScriptFinish(const QString &,bool)),this,SLOT(recieveOneScriptFinish(const QString &,bool)));

    pool->start(controllerInstance);
}

void BehaviourWidget::receiveReplayResult(const QString &arg_deviceId,const QString &arg_two,QMap<QString,bool> arg_resmap,int arg_sec)
{
    gmap.clear();
    bool errorFlag=true;
    QMap<QString, bool>::const_iterator i;
    for (i = arg_resmap.constBegin(); i != arg_resmap.constEnd(); ++i)
    {
        qDebug()<<i.key()<<" : "<<i.value();
        bool b;
        if(statResForScript.contains(i.key()))
        {
            b=i.value()&&statResForScript[i.key()];
            qDebug()<<statResForScript[i.key()];
        }
        else
            b=i.value();
        qDebug()<<b;
        if(!b)
            errorFlag=false;
        gmap.insert(i.key(),b);
    }

    if(errorFlag)
    {
        QString s="-----------------------success-----------------------";
        ui->scriptTextEdit->append("<font color = green>" + s + "</font>");
    }
    else
    {
        QString s="-----------------------fail----------------------";
        ui->scriptTextEdit->append("<font color = red>" + s + "</font>");
    }

    if(arg_sec==0)
    {
        qLunchTime="";
    }
    else
    {
        float floattime=arg_sec*0.001;
        qLunchTime=QString::number(floattime, 'f', 2);
    }

    stopRun();
}

void BehaviourWidget::receiveErrorResult(const QString &arg_SecondLevelDirName,const QString &arg_error)
{
    //QString strDir=fileNameForReport+QDir::separator()+arg_SecondLevelDirName+QDir::separator()+"script-report.txt";20170726
    QString strDir=fileNameForReport+QDir::separator()+"script-report.txt";

    strDir=QDir::toNativeSeparators(strDir);
    QFile f(strDir);
    f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&f);
    out<<arg_error<<endl;
    f.close();
}

void BehaviourWidget::RecieveLogcatOutput()
{
    QString tempStr=QString::fromUtf8(pLogcat->readAllStandardOutput());
    if(fileLogcat->isOpen())
        fileLogcat->write(tempStr.toStdString().c_str());
}

void BehaviourWidget::RecieveMemOverThresSignal(bool flag,QString deviceName)
{
    isMemOverThres=flag;
}

void BehaviourWidget::RecieveCpuOverThresSignal(bool flag,QString deviceName)
{
    isCpuOverThres=flag;
}

void BehaviourWidget::RecieveWifiOverThresSignal(bool flag,QString deviceName)
{
    isWifiOverThres=flag;
}

void BehaviourWidget::RecieveMobileOverThresSignal(bool flag,QString deviceName)
{
    isMobileOverThres=flag;
}

void BehaviourWidget::RecieveBatteryOverThresSignal(bool flag,QString deviceName)
{
    isBatteryOverThres=flag;
}

void BehaviourWidget::RecieveCpuTempOverThresSignal(bool flag,QString deviceName)
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

void BehaviourWidget::readFromServerClass(QString str)
{
    qDebug()<<str;
    QString tmp;int num;
    QMap<QString,QList<DataClass> >::iterator it;

    for(it=allXmlData.begin();it!=allXmlData.end();++it)
    {
        if(str.contains(it.value()[0].UniqueIdenti)&&str.contains(it.value()[0].revelantHead))//unique是全局检测的检测标准，但单个检测时需要revelant
        {
            tmp=it.value()[0].revelantHead;
            num=str.indexOf(tmp);
            str=str.mid(num);
            tmp="HTTP/1.0";//结尾的标志，但是和对手机直接抓包的不一样，那是1.1这是1.0
            num=str.indexOf(tmp);
            if(num!=-1)
            {
                num+=tmp.length();
                QString url=UrlDecode( str.left(num));
                ShowURL(url,it.value()[0].UniqueIdenti,it.key());
                str=str.mid(num);
            }
            break;
        }

    }
}

void BehaviourWidget::ReadStandardOutput(QString res)
{
    res=UrlDecode(res);
    if(!scriptPatternFlag)//手动模式
    {
        QMap<QString,QList<DataClass> >::iterator it;
        for(it=allXmlData.begin();it!=allXmlData.end();++it)
        {
            if(res.contains(it.value()[0].UniqueIdenti)&&res.contains(it.value()[0].revelantHead))//unique是全局检测的检测标准，但单个检测时需要revelant
            {
                ShowURL(res,it.value()[0].UniqueIdenti,it.key());
                break;
            }
        }
    }
    else//脚本模式
    {
        if(res.contains("http"))
        {
            numForUrl++;
            urlTimeStruct stru;
            stru.time=QDateTime::currentDateTime().toMSecsSinceEpoch();
            stru.url=res;
            urlTimeList.append(stru);
        }
    }

}

void BehaviourWidget::ShowURL(QString url,QString unique,QString key)
{
    //url=UrlDecode(url);
    if(url.contains(unique))
    {
        urlList<<url;//所有的url都放在一个qstring list里保存，对字符串操作时都对原始的字符串操作，而显示出来的字符串是有空格或其他变化的，包括用label显示后无法拿到label的text

        url.replace("&","& ");//setwordwrap属性需要针对字符串中有空格等分隔符才行，所以这里加了空格作为分隔，这样再用label显示时便可以达到自动换行的效果
        QLabel *tmpLabel=new QLabel;
        tmpLabel->setText(url);
        tmpLabel->setWordWrap(true);

        QListWidgetItem *item=new QListWidgetItem(ui->urlListWidget);
        item->setSizeHint(QSize(ui->urlListWidget->width()-30,ui->urlListWidget->height()));
        ui->urlListWidget->setItemWidget(item,tmpLabel);

        //鼠标回到最新一条更新结果时，会更新统计点显示，滚动条也会随之移动，否则统计点只显示当前选中的url，滚动条也不动！！！
        bool isLatest ;
        if(ui->urlListWidget->currentRow()==(ui->urlListWidget->count()-2))//-2是因为此时已经添加了最新的一条，而当前值还在上一条
            isLatest=true;
        else
            isLatest=false;
        if(isLatest)
        {
            ui->urlListWidget->setCurrentRow(ui->urlListWidget->count()-1);
            ui->urlListWidget->scrollToBottom();
        }
        isSuccess=true;
        QString errorStr = ParseURL(urlList[urlList.count()-1],isLatest);//如果当前选中项是最后一项，解析最新的url,如果选中项是以前的，则不更新

        if(isSuccess)
            statRes.append(key+"&√");
        else
            statRes.append(key+"&×"+errorStr);
        qDebug()<<key;
        qDebug()<<statRes[statRes.length()-1];

        urlErrorList<<isSuccess;//虽然不是最新的不会显示，但是解析时还是能判断isSuccess字段的

        QTreeWidgetItemIterator treeitem(ui->xmlTreeWidget);
        while(*treeitem)
        {
            if(StaticticsHerper::getBasePath((*treeitem))==key)
            {
                //设置未成功的文件是X状态
                QLabel *tmplabel=new QLabel;
                if(isSuccess==false)//判断检测是否成功，不成功加X,成功加对号
                {
                    tmplabel->setText("<font color=red>X</font>");
                    ui->xmlTreeWidget->setItemWidget((*treeitem),1,tmplabel);
                    break;
                }
                else
                {
                    tmplabel->setText("<font color=green>√</font>");
                    ui->xmlTreeWidget->setItemWidget((*treeitem),1,tmplabel);
                    break;
                }
            }
            ++treeitem;
        }

    }
}

QString BehaviourWidget::ParseURL(QString url,bool isLatest)//点击url列表时槽函数，解析url变成统计点
{
    //isLatest，如果当前url不是最新一条，就是false，其他都是true,比如点击某一条URL解析时
    //url="10.7.36.41:53926: GET http://stat.m.tv.sohu.com/mobiledata_test/mcc/mc.gif?uid=c9e25b6c5931d72ac0a8f77ac6c611d1&tkey=472c80786bc2941025c46ad1f115965004572956&vid=&tvid=&mtype=6&mfov=NX531J&memo=&passport=4BEC8B9477DA5AAB14B235D4F08F1E3E@qq.sohu.com&ltype=&cv=6.3.0&mos=2&mosv=6.0.1&pro=1&mfo=nubia&webtype=WiFi&time=1493972334292&channelid=93&sim=0&playlistid=&catecode=&preid=&newuser=0&enterid=0&startid=1493972327094&loc=&mnc=-2&build=02232253&url=2001&type=1&value=&androidId=b6dc20841c665c08&imei=869465023061802&timer=12f900b6b0421aca1d62e0b2044db36c3b963ce9&xid=RpLaFhOVejLuADC+vM6FprUPsAd6dRRNTRNyxBr27vfdw8pdXlu0hZOZpEd4Rz/cdR33Rh5cIH96XYIxg9k6p23QBpdo0gnlO3Q0I33BXfV80eTrGj8VDvrD1b8KKV24kCrDhljbKPsoujmFSk96D58wTDrQuM4o0XfxAMHyY2E=";
    //statistics.clear();
    QString errorStr;
    QString tmp="";
    bool isFind=false;//下面的for循环是找
    QMap<QString,QList<DataClass> >::iterator it;
    for(it=allXmlData.begin();it!=allXmlData.end();++it)
    {
        if(url.contains(it.value()[0].UniqueIdenti))
        {
            isFind=true;
            tmp=it.value()[0].revelantHead;
            url=url.mid(tmp.length());//把get前缀去掉
            tmp="HTTP/1.1";
            url=url.left(url.length()-tmp.length());//这里如果是服务器抓包可能是HTTP/1.0,但是长度是一致的
            tmp="";
            int num;
            QString tagName="";
            QString value="";
            if(isLatest)
            {
                ui->statisticsListWidget->clear();
            }
            while(url!="")
            {
                tmp="=";
                num=url.indexOf(tmp);
                tagName=url.left(num);
                url=url.mid(num+1);
                if(tagName=="memo")//memo这个统计点中可能会带有&字符，这截取的内容就会不完整，考虑到该字段由{}括着，所以选取}作为结尾判断
                    tmp="}";
                else
                    tmp="&";
                num=url.indexOf(tmp);
                if(num==-1)
                {
                    if(tagName=="memo")//有的memo是空
                    {
                        tmp="&";
                        num=url.indexOf(tmp);
                        value=url.left(num);
                        url=url.mid(num+1);
                    }
                    else
                    {
                        value=url;
                        url="";
                    }
                }
                else
                {
                    if(tagName=="memo")//以}结尾的，需要把}放到value值中，位置往后扩一位
                    {
                        value=url.left(num+1);
                        url=url.mid(num+2);
                    }
                    else
                    {
                        value=url.left(num);
                        url=url.mid(num+1);
                    }
                }
                //把每个字段和从xml中读出来的数据对比
                //多个字段的onlyCheck是Y，2.7版本修改，但此处的逻辑无需改变
                int flag=0;//xml文件和解析出来的url字段对比，正确或错误的标志
                for(int i=1;i<it.value().length();i++)//list<dataclass>的第一个值是存revelanthead头的，所以遍历的时候从1开始
                {
                    if(tagName==it.value()[i].tagName)//每一个字段在for循环中遇到tagName相同之后，就不会再有相同的了，会跳出for循环了
                    {
                        flag=1;
                        if(it.value()[i].onlyCheck=="Y")
                        {
                            bool mflag=false;//onlycheck是Y时，可能有多个值，一个对不上时，要判断下其他值是不是都对不上
                            while(i<it.value().length()&&tagName==it.value()[i].tagName)
                            {
                                if(it.value()[i].value==value)
                                {
                                    mflag=true;
                                    if(isLatest)
                                    {
                                        ui->statisticsListWidget->addItem("√    "+tagName+" : "+value+"----"+it.value()[i].des);
                                    }
                                    flag=2;
                                    break;//如果一样就break,不用考虑顺序问题，都不一样就打叉
                                }
                                i++;
                            }

                            if(!mflag)
                            {
                                isSuccess=false;
                                if(isLatest)
                                {
                                    QString label="×    "+tagName+" : "+value+" / "+it.value()[i-1].value+"----"+it.value()[i-1].des;
                                    QListWidgetItem *item=new QListWidgetItem(ui->statisticsListWidget);
                                    item->setTextColor(Qt::red);
                                    item->setText(label);
                                    ui->statisticsListWidget->addItem(item);
                                    errorStr+="&"+tagName+"="+value+"="+it.value()[i-1].value;
                                }
                                flag=2;
                            }
                            break;
                        }
                        else if(it.value()[i].onlyCheck=="N")
                        {
                            if(value==it.value()[i].value||it.value()[i].value=="unknown"||it.value()[i].value=="hotvrs获取")//
                            {
                                if(isLatest)
                                {
                                    ui->statisticsListWidget->addItem("√    "+tagName+" : "+value+"----"+it.value()[i].des);
                                }
                                flag=2;
                                break;
                            }
                            if(it.value()[i].value=="unnull")//非空字段
                            {
                                if(value!="")
                                {
                                    if(isLatest)
                                    {
                                        ui->statisticsListWidget->addItem("√    "+tagName+" : "+value+"----"+it.value()[i].des);
                                    }
                                    flag=2;
                                    break;
                                }
                                else
                                {
                                    isSuccess=false;
                                    if(isLatest)
                                    {
                                        QString label="×    "+tagName+" : "+value+" / unnull----"+it.value()[i].des;
                                        QListWidgetItem *item=new QListWidgetItem(ui->statisticsListWidget);
                                        item->setTextColor(Qt::red);
                                        item->setText(label);
                                        ui->statisticsListWidget->addItem(item);
                                        errorStr+="&"+tagName+"="+value+"=unnull";
                                    }
                                    flag=2;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(flag==1)//有这个名，但值对不上，url很多值都对不上，但也是错了。。。
                {
                    isSuccess=false;
                    if(isLatest)
                    {
                        QString label="×    "+tagName+" : "+value+"----";
                        QListWidgetItem *item=new QListWidgetItem(ui->statisticsListWidget);
                        item->setTextColor(Qt::red);
                        item->setText(label);
                        ui->statisticsListWidget->addItem(item);
                        errorStr+="&"+tagName+"="+value;
                    }
                }
            }
            break;
        }
    }
    if(!isFind)
    {
        ui->statisticsListWidget->clear();
        qDebug()<<" isfind=true 没从allxmaldata中找到匹配项";
    }
    qDebug()<<errorStr;
    return errorStr;
    //*****************20170505*************************//
    //************20170828 hechang 统计点结果方式已换**************/
    /* QString sss="";
    for(int i=0;i<statistics.count();i++)
    {
        sss=sss + statistics[i] + "->";
    }
    qStatisticsDataList.append(sss);*/
    //*****************20170505*************************//
}

void BehaviourWidget::ReadStandardError(QString result)
{
    qDebug()<<"errorresGBK: "<<result;

    ui->urlListWidget->addItem(result);
    ui->urlListWidget->setCurrentRow(ui->urlListWidget->count()-1);
    urlList<<result;
    urlErrorList<<true;
    ui->statisticsListWidget->clear();
}

QString BehaviourWidget::UrlDecode(QString str)
{
    if(str.isEmpty())//两种方法都可以对url解码
    {
        return "";
    }
    QByteArray byteArrayGB2312 = str.toUtf8();
    QByteArray byteArrayPercentEncoded = QByteArray::fromPercentEncoding(byteArrayGB2312);
    return QString(byteArrayPercentEncoded);
}

QList<DataClass> BehaviourWidget::GetXML(QString fileName)//打开XML文件，判断路径或文件格式是否正确
{
    QList<DataClass> tempData;
    if(!fileName.isEmpty())
    {
        //return NULL;
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::information(NULL, QString("提示"), QString("打开错误!"));
            return tempData;
        }
        QDomDocument document;
        QString error;
        int row = 0, column = 0;
        if(!document.setContent(&file, false, &error, &row, &column))
        {
            QMessageBox::information(NULL, QString("提示"), QString("解析文件错误，行： ") + QString::number(row, 10) + QString(" ,列： ") + QString::number(column, 10));
            return tempData;
        }
        if(document.isNull())
        {
            QMessageBox::information(NULL, QString("提示"), QString("文件为空"));
            return tempData;
        }
        QDomElement root = document.documentElement();
        QDomNode node = root.firstChild();

        if((!isServerPattern)&&isDebugVersion)
            revelantHead=node.toElement().attributeNode("behaviour_debug").value();
        else if((!isServerPattern)&&(!isDebugVersion))
            revelantHead=node.toElement().attributeNode("behaviour_release").value();
        else if(isServerPattern)
            revelantHead=node.toElement().attributeNode("behaviour").value();

        xmlData.clear();
        DataClass tempData;
        tempData.revelantHead=revelantHead;
        unique=node.toElement().attributeNode("unique").value();
        if(unique!="")
        {
            // tempData.UniqueIdenti="&"+unique+"&";//为什么要加上两个前后缀的呀？？？记不起来了
            tempData.UniqueIdenti=unique;//加上前后缀后，如果把第一个字段和最后一个字段设为unique会出问题，把它去掉，20170608
        }
        xmlData<<tempData;

        if(isServerPattern)
        {
            QString behaviourPattern=iniContent->value("xmlMatch/"+revelantHead).toString();
            xmlMatch.insert(revelantHead,behaviourPattern);
        }

        ParseXML(root);

        return xmlData;
    }
}

void BehaviourWidget::ParseXML(const QDomElement &element)//解析xml文件，把节点分离出来
{
    QDomNode node = element.firstChild();
    while(!node.isNull())
    {
        if(node.toElement().tagName()!="data")
        {
            DataClass tempData;
            tempData.tagName=node.toElement().tagName();
            if(node.toElement().hasAttribute("des"))
            {
                tempData.onlyCheck = node.toElement().attributeNode("onlyCheck").value();
            }
            if(node.toElement().hasAttribute("des"))
            {
                tempData.des = node.toElement().attributeNode("des").value();
            }
            tempData.value = node.toElement().text();
            xmlData<<tempData;
        }
        if(node.toElement().tagName()=="data")
        {
            ParseXML(node.toElement());
        }
        node = node.nextSibling();//读取兄弟节点
    }
}

void BehaviourWidget::SetupBtnClicked()
{
    QString iniName=gConfigDir + QDir::separator() + "match.ini";
    QFileInfo fi(iniName);
    if(!fi.exists())
    {
        QMessageBox::warning(this,"警告", gConfigDir + QDir::separator()+"match.ini 不存在");
    }

    QString tempIniPath=gConfigDir + QDir::separator() + "match.ini";
    iniContent=new QSettings(tempIniPath,QSettings::IniFormat);//加载ini文件

    //setupDialog->setWindowModality(Qt::ApplicationModal);
    setupDialog->setAttribute(Qt::WA_QuitOnClose,false);
    setupDialog->setSeverBtnEnabled(!scriptPatternFlag);//脚本模式设置服务器模式不可选
    setupDialog->show();
    setupDialog->raise();
    setupDialog->move ((QApplication::desktop()->width() - setupDialog->width())/2,(QApplication::desktop()->height() - setupDialog->height())/2);
}

void BehaviourWidget::CreateXmlBtnClicked()
{
    urlToXmlDialog->setAttribute(Qt::WA_QuitOnClose,false);
    urlToXmlDialog->show();
    urlToXmlDialog->raise();
    urlToXmlDialog->move ((QApplication::desktop()->width() - urlToXmlDialog->width())/2,(QApplication::desktop()->height() - urlToXmlDialog->height())/2);
}

void BehaviourWidget::AddXmlBtnClicked()
{
    QString filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),currentPath,QFileDialog::ShowDirsOnly);
    if(filePath=="")
        return;
    if(currentPath!=filePath)
    {
        currentPath=filePath;
        ui->xmlTreeWidget->clear();

        QTreeWidgetItem *fileItem = new QTreeWidgetItem();
        StaticticsHerper::LoadFiles(filePath,ui->xmlTreeWidget,fileItem);
        QTreeWidgetItem *item=ui->xmlTreeWidget->itemAt(0,0);
        ui->xmlTreeWidget->setCurrentItem(item);

        //重新加载xml文件时所有东西清空。。。
        ui->statisticsListWidget->clear();
        urlList.clear();
        urlErrorList.clear();
        ui->urlListWidget->clear();
        revelantHead="";
        allXmlData.clear();
        xmlData.clear();
        xmlMatch.clear();

        QListWidgetItem *item1=new QListWidgetItem(ui->urlListWidget);
        QLabel *tmpLabel=new QLabel;
        tmpLabel->setText("<font color=gray>URL列表</font>");
        ui->urlListWidget->setItemWidget(item1,tmpLabel);
        urlList.append("URL列表");
        QListWidgetItem *item2=new QListWidgetItem(ui->statisticsListWidget);
        QLabel *tmpLabel2=new QLabel;
        tmpLabel2->setText("<font color=gray>统计点列表</font>");
        ui->statisticsListWidget->setItemWidget(item2,tmpLabel2);
        if(isStartFlag)
        {
            stopRun();
        }
    }
    else//如果路径与之前一致，
    {
        QStringList checkedFiles;
        QTreeWidgetItemIterator item(ui->xmlTreeWidget);
        while(*item)
        {
            if((*item)->checkState(0) == Qt::Checked)
            {
                if((*item)->childCount()==0)
                {
                    checkedFiles.append((*item)->text(0));
                }
            }
            ++item;
        }
        ui->xmlTreeWidget->clear();
        QTreeWidgetItem *fileItem = new QTreeWidgetItem();
        StaticticsHerper::LoadFiles(filePath,ui->xmlTreeWidget,fileItem);
        QTreeWidgetItem *itemm=ui->xmlTreeWidget->itemAt(0,0);
        ui->xmlTreeWidget->setCurrentItem(itemm);

        QTreeWidgetItemIterator item1(ui->xmlTreeWidget);
        while(*item1)
        {
            if((*item1)->childCount()==0)
            {
                for(int i=0;i<checkedFiles.count();i++)
                {
                    if((*item1)->text(0) == checkedFiles[i])
                    {
                        (*item1)->setCheckState(0,Qt::Checked);
                        //设置被选中的文件是...状态
                        QLabel *tmplabel=new QLabel;
                        tmplabel->setText("<font color=orange>...</font>");
                        ui->xmlTreeWidget->setItemWidget((*item1),1,tmplabel);
                        break;
                    }
                }
            }
            ++item1;
        }
    }
}

void BehaviourWidget::EditXmlBtnClicked()
{
    editMultiXmlDialog->setAttribute(Qt::WA_QuitOnClose,false);
    editMultiXmlDialog->show();
    editMultiXmlDialog->raise();
    editMultiXmlDialog->move ((QApplication::desktop()->width() - editMultiXmlDialog->width())/2,(QApplication::desktop()->height() - editMultiXmlDialog->height())/2);
}

void BehaviourWidget::RecieveData(QString xmlRootPath, bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
                                  QString UID , bool isServerPattern, bool isDebugVersion,
                                  QString memThres,QString cpuThres,QString batteryThres,QString cpuTempThres,QString wifiThres,QString mobileThres)
{
    this->xmlRootPath=xmlRootPath;
    this->isMemCheck=isMemCheck;
    this->isCpuCheck=isCpuCheck;
    this->isBatteryCheck=isBatteryCheck;
    this->isCpuTempCheck=isCpuTempCheck;
    this->isWifiCheck=isWifiCheck;
    this->isMobileCheck=isMobileCheck;

    this->UID=UID;
    this->isServerPattern=isServerPattern;
    this->isDebugVersion=isDebugVersion;

    this->memThres=memThres;
    this->cpuThres=cpuThres;
    this->batteryThres=batteryThres;
    this->cpuTempThres=cpuTempThres;
    this->wifiThres=wifiThres;
    this->mobileThres=mobileThres;

}

bool BehaviourWidget::eventFilter(QObject *watched, QEvent *event)//事件过滤器，检测鼠标当前焦点在哪个控件上
{
    if(watched==ui->urlListWidget)
    {
        if(event->type()==QEvent::FocusIn)
        {
            focusFlag=0;
        }
    }
    if(watched==ui->statisticsListWidget)
    {
        if(event->type()==QEvent::FocusIn)
        {
            focusFlag=1;
        }
    }

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *event1 = static_cast<QKeyEvent*>(event);
        if (event1->key() == Qt::Key_Return && (event1->modifiers() & Qt::ControlModifier))
        {
            //sendMessage();
            return true;
        }
    }

    return QWidget::eventFilter(watched,event);
}

void BehaviourWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == ( Qt::ControlModifier) && e->key() == Qt::Key_S)
    {
        exportReport();
    }

    if ( e->modifiers()==Qt::ControlModifier && e->key() == Qt::Key_A)
    {
        if(focusFlag==1)
        {
            ui->statisticsListWidget->selectAll();
        }
        else if(focusFlag==0)
        {
            ui->urlListWidget->selectAll();
        }
    }
    if (e->modifiers() == ( Qt::ControlModifier) && e->key() == Qt::Key_C)//复制可以选择性复制，但导出是全部导出
    {
        QString copyStr="";
        if(focusFlag==0)
        {
            for(int i=0;i<ui->urlListWidget->count();i++)
            {
                if(ui->urlListWidget->item(i)->isSelected())
                {
                    copyStr+=urlList[i]+"\r\n";//urllistwidget放的是label，还得用urllist取数据
                }
            }
            QClipboard *board=QApplication::clipboard();
            board->setText(copyStr);
        }
        else if(focusFlag==1)
        {
            for(int i=0;i<ui->statisticsListWidget->count();i++)
            {
                if(ui->statisticsListWidget->item(i)->isSelected())
                {
                    copyStr+=ui->statisticsListWidget->item(i)->text()+"\r\n";
                }
            }
            QClipboard *board=QApplication::clipboard();
            board->setText(copyStr);
        }
    }
}

void BehaviourWidget::treeItemChanged(QTreeWidgetItem *item, int column)
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

void BehaviourWidget::closeEvent(QCloseEvent *e)
{
    if(isStartFlag)//已经开始的状态，点击停止
    {
        if(scriptPatternFlag)
            //添加停止脚本逻辑,脚本停止之后会自动调用stopRun（）
            emit sendStopReplay();
        else
            stopRun();
    }
}

void BehaviourWidget::stopRun()
{

    isStartFlag=false;
    emit sendStopStatSignal();
    ui->startBtn->setIcon(QIcon(":/start.png"));

    ExeCmd::runCmd("taskkill /im mitmdump.exe /f");

    emit sendStateChange(false);

    if(server.count()!=0)
    {
        for(int i=0;i<server.count();i++)
        {
            server[i]->StopControl();
        }
    }
    server.clear();

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
    ExeCmd::StopLogcat(deviceName);//停止后停止logcat
    if(fileLogcat!=NULL)
    {
        fileLogcat->close();
        if(fileLogcat->size()==0)
            QFile::remove(fileLogcat->fileName());
    }

    exportReport();
}

void BehaviourWidget::on_reportDetailBtn_clicked()
{
    ExeCmd::openFolder(fileNameForReport);
}

void BehaviourWidget::RecieveDevicesSigal(QStringList devicesList)
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




void BehaviourWidget::on_equipListView_clicked(const QModelIndex &index)
{
    QModelIndex indexEquip=ui->equipListView->currentIndex();
    deviceName=ExeCmd::getDeviceSN(indexEquip.data().toString());
}

void BehaviourWidget::on_equipBtn_clicked()
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

    QModelIndex indexEquip=ui->equipListView->model()->index(0,0);
    ui->equipListView->setCurrentIndex(indexEquip);

    deviceName=ExeCmd::getDeviceSN(indexEquip.data().toString());
}



void BehaviourWidget::on_xmlTreeWidget_itemEntered(QTreeWidgetItem *item, int column)
{
    QToolTip::showText(cursor().pos(),item->text(0));
}


void BehaviourWidget::AddScriptBtnClicked()
{
    QString filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),currentScriptPath,QFileDialog::ShowDirsOnly);
    if(filePath=="")
        return;
    qScriptList.clear();
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

        QTreeWidgetItem *itemm=ui->scriptTreeWidget->itemAt(0,0);
        ui->scriptTreeWidget->setCurrentItem(itemm);

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

void BehaviourWidget::on_scriptTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->childCount()==0&&item->parent()!=NULL)
    {
        if(item->checkState(0) == Qt::Checked)
        {

            QLabel *tmplabel=new QLabel;
            tmplabel->setText("<font color=orange>...</font>");
            ui->scriptTreeWidget->setItemWidget(item,1,tmplabel);

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

            //清除没有被选中的第二行标识
            QLabel *tmplabel=new QLabel;
            tmplabel->setText("");
            ui->xmlTreeWidget->setItemWidget(item,1,tmplabel);

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
    for(int i=0;i<qScriptList.count();i++)
        qDebug()<<qScriptList.at(i);

    int width=ui->xmlTreeWidget->width();
    ui->scriptTreeWidget->setColumnCount(2); //设置列数
    ui->scriptTreeWidget->setColumnWidth(0,width*0.88);
    ui->scriptTreeWidget->setColumnWidth(1,width*0.08);
}

void BehaviourWidget::receiveRunScriptResult(const QString &arg_result)
{
    if(ui->scriptTextEdit->toPlainText().contains("脚本日志"))
        ui->scriptTextEdit->clear();

    if(arg_result.contains("princekinfalse") || arg_result.contains("java.lang.RuntimeException") || arg_result.contains("error:" ,Qt::CaseInsensitive)  )
    {
        ui->scriptTextEdit->append("<font color = red>" + arg_result + "</font>");
        ui->scriptTextEdit->append("\r\n");
    }
    else
    {
        ui->scriptTextEdit->append("<font color = black>" + arg_result + "</font>");
        ui->scriptTextEdit->append("\r\n");
    }
    if(arg_result.contains("checkStatPoint"))//脚本中检测统计点命令
    {
        //  ui->scriptTextEdit->append("~~~"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
        GetFourSecData();//获取前4秒的数据

        QStringList xmlList=arg_result.split(" ",QString::SkipEmptyParts);
        xmlList=xmlList[xmlList.length()-1].split("=");
        qDebug()<<xmlList[1];
        for(int i=1;i<xmlList.length();i++)
        {
            QString allPath;
            if(xmlList[i].startsWith("\\")||xmlList[i].startsWith("/"))
            {
                allPath=QDir::toNativeSeparators( xmlRootPath+xmlList[i] );
            }
            else
            {
                allPath=QDir::toNativeSeparators( xmlRootPath+QDir::separator()+xmlList[i] );
            }
            allPath=allPath.trimmed();
            qDebug()<<"allpath"<<allPath.trimmed();
            QFile f(allPath.trimmed());
            if(f.exists())
            {
                qDebug()<<"exist";
                singleXmlData=GetXML(allPath);
                allXmlDataForCheck.append(singleXmlData);
                bool isFind = FindMatchUrl(xmlList[i].trimmed());
            }
            else
            {
                qDebug()<<"noexist";
                ui->scriptTextEdit->append("<span style=\"background:#e4e4e4;color:red;\">" +allPath + "  文件不存在</span>");
                ui->scriptTextEdit->append("\r\n");
            }
        }
        urlTimeList.clear();//完成一句checkStatPoint校验可清空数据下次校验不会用到这些数据
        //ui->scriptTextEdit->append("~~~"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    }
}

void BehaviourWidget::GetFourSecData()
{
    urlListFour.clear();
    int m=urlTimeList.length();
    if(m>0)
    {
        int endTime=urlTimeList[m-1].time;
        for(int i=m-1;i>=0;--i)
        {
            if(endTime-urlTimeList[i].time<4000)
            {
                urlListFour.append(urlTimeList[i].url);
            }
            else
            {
                break;
            }
        }

        //qDebug()<<"----"<<urlListFour.length();
    }
}

bool BehaviourWidget::FindMatchUrl(QString xmlFile)
{
    bool isFind=false;
    QString errorStr="";
    for(int p=0;p<urlListFour.length();p++)
    {
        // qDebug()<<"----"<<urlListFour[p];
        if(urlListFour[p].contains(unique)&&urlListFour[p].contains(revelantHead))//unique是全局检测的检测标准，但单个检测时需要revelant
        {
            QString url=urlListFour[p];
            urlList<<url;//所有的url都放在一个qstring list里保存，对字符串操作时都对原始的字符串操作，而显示出来的字符串是有空格或其他变化的，包括用label显示后无法拿到label的text

            url.replace("&","& ");//setwordwrap属性需要针对字符串中有空格等分隔符才行，所以这里加了空格作为分隔，这样再用label显示时便可以达到自动换行的效果

            QLabel *tmpLabel=new QLabel;
            tmpLabel->setText(url);
            tmpLabel->setWordWrap(true);
            QListWidgetItem *item=new QListWidgetItem(ui->urlListWidget);
            item->setSizeHint(QSize(ui->urlListWidget->width()-30,ui->urlListWidget->height()));
            ui->urlListWidget->setItemWidget(item,tmpLabel);
            //判断是否是最低端数据，如果是，滚动条跟着动
            bool isLatest ;
            if(ui->urlListWidget->currentRow()==(ui->urlListWidget->count()-2))//-2是因为此时已经添加了最新的一条，而当前值还在上一条
                isLatest=true;
            else
                isLatest=false;
            if(isLatest)
            {
                ui->urlListWidget->setCurrentRow(ui->urlListWidget->count()-1);
                ui->urlListWidget->scrollToBottom();
            }
            errorStr = ParseURLForCheck(urlListFour[p],singleXmlData,isLatest);
            isFind=true;
            if(errorStr=="")
            {
                urlErrorList<<true;
            }
            else
            {
                urlErrorList<<false;
                tempFlagForStat=false;
            }
            break;
        }
    }

    if(!isFind)
    {
        urlList<<"";
        QLabel *tmpLabel=new QLabel;
        tmpLabel->setText("");
        tmpLabel->setWordWrap(true);
        QListWidgetItem *item=new QListWidgetItem(ui->urlListWidget);
        item->setSizeHint(QSize(ui->urlListWidget->width()-30,ui->urlListWidget->height()));
        ui->urlListWidget->setItemWidget(item,tmpLabel);
        ui->statisticsListWidget->clear();

        bool isLatest ;
        if(ui->urlListWidget->currentRow()==(ui->urlListWidget->count()-2))//-2是因为此时已经添加了最新的一条，而当前值还在上一条
            isLatest=true;
        else
            isLatest=false;
        if(isLatest)
        {
            ui->urlListWidget->setCurrentRow(ui->urlListWidget->count()-1);
            ui->urlListWidget->scrollToBottom();
        }
    }

    QString tempstr="";
    if(errorStr=="")
    {
        if(isFind)
        {
            ui->scriptTextEdit->append("<font color=green>checkStatpoint "+ xmlFile +" true</font>");
            ui->scriptTextEdit->append("\r\n");
            tempstr=xmlFile+"&√";

            int currentrow=ui->scriptTextEdit->document()->lineCount();
            lineNumberList.append(currentrow);
        }
        else
        {
            ui->scriptTextEdit->append("<span style=\"background:#e4e4e4;color:red;\">" +xmlFile + " notfound</span>");
            ui->scriptTextEdit->append("\r\n");
            tempstr=xmlFile+"&×";
            tempFlagForStat=false;//找不到xml文件时也要赋值false
            int currentrow=ui->scriptTextEdit->document()->lineCount();
            lineNumberList.append(currentrow);
        }
    }
    else
    {
        ui->scriptTextEdit->append("<font color=red>checkStatpoint "+ xmlFile +" false</font>");
        ui->scriptTextEdit->append("\r\n");
        tempstr=xmlFile+"&×"+errorStr;
        isFind=false;
        tempFlagForStat=false;
        int currentrow=ui->scriptTextEdit->document()->lineCount();
        lineNumberList.append(currentrow);
    }
    statRes.append(tempstr);
    return isFind;
}

void BehaviourWidget::recieveOneScriptFinish(const QString &arg_scriptline,bool flag)
{
    statResForScript.insert(arg_scriptline,tempFlagForStat);//一个脚本执行结束，list中记录该时间段内的统计点统计情况，并把标志位清零

    //清空是把之前的检测结果恢复初始状态
    QTreeWidgetItemIterator item(ui->scriptTreeWidget);
    while(*item)
    {
        if((*item)->childCount()==0&&(*item)->parent()!=NULL)
        {
            QString tempStr=(*item)->parent()->text(0)+"="+(*item)->text(0).left((*item)->text(0).length()-4);
            if(tempStr==arg_scriptline)
            {
                QLabel *tmplabel=new QLabel;
                if(flag&&tempFlagForStat)
                {
                    tmplabel->setText("<font color=green>√</font>");
                }
                else
                {
                    tmplabel->setText("<font color=red>×</font>");
                }
                ui->scriptTreeWidget->setItemWidget((*item),1,tmplabel);
                break;
            }
        }
        ++item;
    }

    tempFlagForStat=true;
}

QString BehaviourWidget::ParseURLForCheck(QString url, QList<DataClass> xmlData, bool isLatest)//点击url列表时槽函数，解析url变成统计点
{
    //isLatest，如果当前url不是最新一条，就是false，其他都是true,比如点击某一条URL解析时
    //url="10.7.36.41:53926: GET http://stat.m.tv.sohu.com/mobiledata_test/mcc/mc.gif?uid=c9e25b6c5931d72ac0a8f77ac6c611d1&tkey=472c80786bc2941025c46ad1f115965004572956&vid=&tvid=&mtype=6&mfov=NX531J&memo=&passport=4BEC8B9477DA5AAB14B235D4F08F1E3E@qq.sohu.com&ltype=&cv=6.3.0&mos=2&mosv=6.0.1&pro=1&mfo=nubia&webtype=WiFi&time=1493972334292&channelid=93&sim=0&playlistid=&catecode=&preid=&newuser=0&enterid=0&startid=1493972327094&loc=&mnc=-2&build=02232253&url=2001&type=1&value=&androidId=b6dc20841c665c08&imei=869465023061802&timer=12f900b6b0421aca1d62e0b2044db36c3b963ce9&xid=RpLaFhOVejLuADC+vM6FprUPsAd6dRRNTRNyxBr27vfdw8pdXlu0hZOZpEd4Rz/cdR33Rh5cIH96XYIxg9k6p23QBpdo0gnlO3Q0I33BXfV80eTrGj8VDvrD1b8KKV24kCrDhljbKPsoujmFSk96D58wTDrQuM4o0XfxAMHyY2E=";
    if(url=="")
    {
        ui->statisticsListWidget->clear();
        return "";
    }

    QString tmp="";
    QString errorStr="";
    if(url.contains(xmlData[0].UniqueIdenti))
    {
        tmp=xmlData[0].revelantHead;
        url=url.mid(tmp.length());//把get前缀去掉
        tmp="HTTP/1.1";
        url=url.left(url.length()-tmp.length());//这里如果是服务器抓包可能是HTTP/1.0,但是长度是一致的
        tmp="";
        int num;
        QString tagName="";
        QString value="";
        if(isLatest)
        {
            ui->statisticsListWidget->clear();
        }
        while(url!="")
        {
            tmp="=";
            num=url.indexOf(tmp);
            tagName=url.left(num);
            url=url.mid(num+1);
            if(tagName=="memo")//memo这个统计点中可能会带有&字符，这截取的内容就会不完整，考虑到该字段由{}括着，所以选取}作为结尾判断
                tmp="}";
            else
                tmp="&";
            num=url.indexOf(tmp);
            if(num==-1)
            {
                if(tagName=="memo")//有的memo是空
                {
                    tmp="&";
                    num=url.indexOf(tmp);
                    value=url.left(num);
                    url=url.mid(num+1);
                }
                else
                {
                    value=url;
                    url="";
                }
            }
            else
            {
                if(tagName=="memo")//以}结尾的，需要把}放到value值中，位置往后扩一位
                {
                    value=url.left(num+1);
                    url=url.mid(num+2);
                }
                else
                {
                    value=url.left(num);
                    url=url.mid(num+1);
                }
            }
            //把每个字段和从xml中读出来的数据对比
            //多个字段的onlyCheck是Y，2.7版本修改，但此处的逻辑无需改变
            int flag=0;//xml文件和解析出来的url字段对比，正确或错误的标志
            for(int i=1;i<xmlData.length();i++)//list<dataclass>的第一个值是存revelanthead头的，所以遍历的时候从1开始
            {
                if(tagName==xmlData[i].tagName)//每一个字段在for循环中遇到tagName相同之后，就不会再有相同的了，会跳出for循环了
                {
                    flag=1;
                    if(xmlData[i].onlyCheck=="Y")
                    {
                        bool mflag=false;//onlycheck是Y时，可能有多个值，一个对不上时，要判断下其他值是不是都对不上
                        while(tagName==xmlData[i].tagName)
                        {
                            if(xmlData[i].value==value)
                            {
                                mflag=true;
                                if(isLatest)
                                {
                                    ui->statisticsListWidget->addItem("√    "+tagName+" : "+value+"----"+xmlData[i].des);
                                }
                                flag=2;
                                break;//如果一样就break,不用考虑顺序问题，都不一样就打叉
                            }
                            i++;
                        }

                        if(!mflag)
                        {
                            isSuccess=false;
                            if(isLatest)
                            {
                                QString label="×    "+tagName+" : "+value+" / "+xmlData[i-1].value+"----"+xmlData[i-1].des;
                                QListWidgetItem *item=new QListWidgetItem(ui->statisticsListWidget);
                                item->setTextColor(Qt::red);
                                item->setText(label);
                                ui->statisticsListWidget->addItem(item);
                            }
                            QString tempStr=tagName+"="+value+"="+xmlData[i-1].value;
                            errorStr+="&"+tempStr;
                            flag=2;
                        }
                        break;
                    }
                    else if(xmlData[i].onlyCheck=="N")
                    {
                        if(value==xmlData[i].value||xmlData[i].value=="unknown"||xmlData[i].value=="hotvrs获取")//
                        {
                            if(isLatest)
                            {
                                ui->statisticsListWidget->addItem("√    "+tagName+" : "+value+"----"+xmlData[i].des);
                            }
                            flag=2;
                            break;
                        }
                        if(xmlData[i].value=="unnull")//非空字段
                        {
                            if(value!="")
                            {
                                if(isLatest)
                                {
                                    ui->statisticsListWidget->addItem("√    "+tagName+" : "+value+"----"+xmlData[i].des);
                                }
                                flag=2;
                                break;
                            }
                            else
                            {
                                isSuccess=false;
                                if(isLatest)
                                {
                                    QString label="×    "+tagName+" : "+value+" / unnull----"+xmlData[i].des;
                                    QListWidgetItem *item=new QListWidgetItem(ui->statisticsListWidget);
                                    item->setTextColor(Qt::red);
                                    item->setText(label);
                                    ui->statisticsListWidget->addItem(item);
                                }
                                QString tempStr=tagName+"="+value+"="+xmlData[i-1].value;
                                errorStr+="&"+tempStr;
                                flag=2;
                                break;
                            }
                        }
                    }
                }
            }
            if(flag==1)//有这个名，但值对不上，url很多值都对不上，但也是错了。。。
            {
                isSuccess=false;
                if(isLatest)
                {
                    QString label="×    "+tagName+" : "+value+"----";
                    QListWidgetItem *item=new QListWidgetItem(ui->statisticsListWidget);
                    item->setTextColor(Qt::red);
                    item->setText(label);
                    ui->statisticsListWidget->addItem(item);
                }
                QString tempStr=tagName+"="+value+"=";
                errorStr+="&"+tempStr;
            }
        }
    }
    return errorStr;
}

void BehaviourWidget::on_scriptTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
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




void BehaviourWidget::createExcel()
{
    qDebug()<<"statRes--------------";
    qDebug()<<statRes;

    ExcelController::Controller *controller=new ExcelController::Controller;

    controller->setMark("behaviour");
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

    controller->setUrlTotalNum(urlTotalNum);
    controller->setUrlErrorNum(urlErrorNum);
    controller->setUrlSuccessNum(urlTotalNum-urlErrorNum);

    controller->setPackageName(qPackageName);
    controller->setStatiList(statRes);
    controller->setMeasureVector(assist->reportInfo);

    connect(controller,SIGNAL(sendWorkerFinished()),this,SLOT(receiveWorkerFinished()));
    controller->doController();
}

void BehaviourWidget::createBaseExcel()
{
    ExcelController::Controller *controller=new ExcelController::Controller;
    controller->setMark("base-behaviour");
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

    controller->setUrlTotalNum(urlTotalNum);
    controller->setUrlErrorNum(urlErrorNum);
    controller->setUrlSuccessNum(urlTotalNum-urlErrorNum);

    controller->setPackageName(qPackageName);
    controller->setStatiList(statRes);

    connect(controller,SIGNAL(sendWorkerFinished()),this,SLOT(receiveWorkerFinished()));
    controller->doController();
}

void BehaviourWidget::receiveWorkerFinished()
{
    QMessageBox::information(NULL,"提示","excel生成完毕");
}

void BehaviourWidget::getXXX()
{
    getFrames();
}


void BehaviourWidget::getFrames()
{
    QString cmdLine="adb -s " + deviceName + " shell dumpsys gfxinfo " + qPackageName;
    QThreadPool *pool=new QThreadPool;


    using FramesController::Controller;
    Controller *controllerInstance=new Controller;

    controllerInstance->setAutoDelete(true);
    controllerInstance->setDeviceId(deviceName);
    controllerInstance->setCmdLine(cmdLine);

    connect(controllerInstance,SIGNAL( sendResult(const QString &,const QString &) ),this,SLOT(receiveFramesResult(const QString &,const QString &) )  ,Qt::DirectConnection  );

    pool->start(controllerInstance);
}

void BehaviourWidget::receiveFramesResult(const QString &arg_deviceId,const QString &arg_text)
{
    qFrames=arg_text;
}

void BehaviourWidget::getLunchTime()
{
    QString cmdLine="cmd /c adb -s " + deviceName + " shell dumpsys activity top | findstr ACTIVITY";

    AdbShellController::Controller *controller=new AdbShellController::Controller;
    controller->setMark("getAll","isActive");
    controller->setCmdLine(cmdLine);
    connect(controller,SIGNAL(sendWorkerResult(const QString&,const QString&)),this,SLOT(receiveWorkerResult1(const QString&,const QString&)));
    controller->doController();
}

void BehaviourWidget::receiveWorkerResult1(const QString&arg_str,const QString&arg_mark)
{
    if(arg_mark=="isActive")
    {
        if(arg_str.contains(qPackageName))
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

void BehaviourWidget::receiveWorkerResult2(const QString&arg_str,const QString&arg_mark)
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

void BehaviourWidget::on_scriptTextEdit_cursorPositionChanged()
{
    //if(!isStartFlag)
    {
        QTextCursor tc = ui->scriptTextEdit->textCursor();
        tc.select(QTextCursor::LineUnderCursor);
        int currentrow = tc.blockNumber()+3;//list中加入行号时就大2行，所以这里获取到的真正行数加2行才能和list中一致
        for(int i=0;i<lineNumberList.count();i++)
        {
            if(currentrow==lineNumberList[i])
            {
                ui->urlListWidget->setCurrentRow(i);
                ParseURLForCheck(urlList[i],allXmlDataForCheck[i],true);
                break;
            }
        }
    }
}
