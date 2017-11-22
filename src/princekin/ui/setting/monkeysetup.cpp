#include "monkeysetup.h"
#include <QFileDialog>
#include <QDebug>
#include <QToolTip>
#include <QScrollBar>
#include <QHeaderView>

#define LINETEXTWIDTH 60

MonkeySetUp::MonkeySetUp(QWidget *parent) : QWidget(parent)
{
    signalMonkeyData_s signalData;
    QVariant DataVar;
    DataVar.setValue(signalData);
    qRegisterMetaType<QVariant>("QVariant"); //写在构造函数里

    widgetInit();

    isMemCheck=false;
    isCpuCheck=false;
    isBatteryCheck=false;
    isCpuTempCheck=false;
    isWifiCheck=false;
    isMobileCheck=false;

    QStringList tempList=ParseXml::readConfigXmlForIP_Port(gConfigDir + QDir::separator() + "config.xml");
    ipLineEdit->setText(tempList[0]);
    portLineEdit->setText(tempList[1]);

    xmlTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                      "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    xmlTreeWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                        "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");

    xmlTreeWidget->setHeaderHidden(true);


    connect(levelOneRBtn,SIGNAL(clicked(bool)),this,SLOT(level1Changed()));
    connect(levelTwoRBtn,SIGNAL(clicked(bool)),this,SLOT(level2Changed()));
    connect(levelThreeRBtn,SIGNAL(clicked(bool)),this,SLOT(level3Changed()));
    connect(levelZeroRBtn,SIGNAL(clicked(bool)),this,SLOT(level0Changed()));
    connect(countCBtn,SIGNAL(clicked(bool)),this,SLOT(countChanged()));
    connect(countLineEdit,SIGNAL(textChanged(QString)),this,SLOT(countTextChanged()));
    connect(seedCBtn,SIGNAL(clicked(bool)),this,SLOT(seedChanged()));
    connect(seedLineEdit,SIGNAL(textChanged(QString)),this,SLOT(seedTextChanged()));
    connect(throttleCBtn,SIGNAL(clicked(bool)),this,SLOT(throttleChanged()));
    connect(throttleLineEdit,SIGNAL(textChanged(QString)),this,SLOT(throttleTextChanged()));
    connect(randomCBtn,SIGNAL(clicked(bool)),this,SLOT(randomChanged()));

    //事件百分比
    connect(touchCBtn,SIGNAL(clicked(bool)),this,SLOT(touchChanged()));
    connect(touchLineEdit,SIGNAL(textChanged(QString)),this,SLOT(touchTextChanged()));
    connect(motionCBtn,SIGNAL(clicked(bool)),this,SLOT(motionChanged()));
    connect(motionLineEdit,SIGNAL(textChanged(QString)),this,SLOT(motionTextChanged()));
    connect(trackBallCBtn,SIGNAL(clicked(bool)),this,SLOT(trackballChanged()));
    connect(trackBallLineEdit,SIGNAL(textChanged(QString)),this,SLOT(trackballTextChanged()));
    connect(syskeysCBtn,SIGNAL(clicked(bool)),this,SLOT(syskeysChanged()));
    connect(syskeysLineEdit,SIGNAL(textChanged(QString)),this,SLOT(syskeysTextChanged()));
    connect(navCBtn,SIGNAL(clicked(bool)),this,SLOT(navChanged()));
    connect(navLineEdit,SIGNAL(textChanged(QString)),this,SLOT(navTextChanged()));
    connect(majorNavCBtn,SIGNAL(clicked(bool)),this,SLOT(majornavChanged()));
    connect(majorNavLineEdit,SIGNAL(textChanged(QString)),this,SLOT(majornavTextChanged()));

    connect(appSwitchCBtn,SIGNAL(clicked(bool)),this,SLOT(appswitchChanged()));
    connect(appSwitchLineEdit,SIGNAL(textChanged(QString)),this,SLOT(appswitchTextChanged()));
    connect(flipCBtn,SIGNAL(clicked(bool)),this,SLOT(flipChanged()));
    connect(flipLineEdit,SIGNAL(textChanged(QString)),this,SLOT(flipTextChanged()));
    connect(pinchZoomCBtn,SIGNAL(clicked(bool)),this,SLOT(pinchzoomChanged()));
    connect(pinchZoomLineEdit,SIGNAL(textChanged(QString)),this,SLOT(pinchzoomTextChanged()));
    connect(anyEventCBtn,SIGNAL(clicked(bool)),this,SLOT(anyeventChanged()));
    connect(anyEventLineEdit,SIGNAL(textChanged(QString)),this,SLOT(anyeventTextChanged()));

    //类名
    connect(className,SIGNAL(textChanged()),this,SLOT(classNameChanged()));

    //调试
    connect(iCrashesCBtn,SIGNAL(clicked(bool)),this,SLOT(iCrashChanged()));
    connect(iTimeoutCBtn,SIGNAL(clicked(bool)),this,SLOT(iTimeoutChanged()));
    connect(isecurityCBtn,SIGNAL(clicked(bool)),this,SLOT(iSecurityChanged()));
    connect(dbgNoEventCBtn,SIGNAL(clicked(bool)),this,SLOT(dbgNoEventChanged()));
    connect(waitDBGNavCBtn,SIGNAL(clicked(bool)),this,SLOT(waitDngChanged()));
    connect(hprofCBtn,SIGNAL(clicked(bool)),this,SLOT(hprofChanged()));
    connect(monitorCBtn,SIGNAL(clicked(bool)),this,SLOT(monitorChanged()));
    connect(killProcessCBtn,SIGNAL(clicked(bool)),this,SLOT(killProcessChanged()));

    //命令
    connect(cmdTextEdit,SIGNAL(textChanged()),this,SLOT(cmdMonkeyChanged()));

    //参数测量
    connect(memCBtn,SIGNAL(clicked(bool)),this,SLOT(memCBtnCheck()));
    connect(cpuCBtn,SIGNAL(clicked(bool)),this,SLOT(cpuCBtnCheck()));
    connect(batteryCBtn,SIGNAL(clicked(bool)),this,SLOT(batteryCBtnCheck()));
    connect(cpuTempCBtn,SIGNAL(clicked(bool)),this,SLOT(cpuTempCBtnCheck()));
    connect(wifiCBtn,SIGNAL(clicked(bool)),this,SLOT(wifiCBtnCheck()));
    connect(mobileCBtn,SIGNAL(clicked(bool)),this,SLOT(mobileCBtnCheck()));

    connect(memLineEdit,SIGNAL(textChanged(QString)),this,SLOT(TextChanged()));
    connect(cpuLineEdit,SIGNAL(textChanged(QString)),this,SLOT(TextChanged()));
    connect(batteryLineEdit,SIGNAL(textChanged(QString)),this,SLOT(TextChanged()));
    connect(cpuTempLineEdit,SIGNAL(textChanged(QString)),this,SLOT(TextChanged()));

    connect(logKeyWordName,SIGNAL(textChanged(QString)),this,SLOT(logTextChanged()));

    //统计点
    connect(debugRBtn,SIGNAL(clicked(bool)),this,SLOT(debugRBtnClick()));
    connect(releaseRBtn,SIGNAL(clicked(bool)),this,SLOT(releaseRBtnClick()));
    connect(selectXmlBtn,SIGNAL(clicked(bool)),this,SLOT(selectXmlBtnClick()));
    connect(xmlTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
    connect(xmlTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_xmlTreeWidget_itemChanged(QTreeWidgetItem*,int)));
    connect(statCBox,SIGNAL(clicked(bool)),this,SLOT(statClick()));
}

void MonkeySetUp::statClick()
{
    gMonkeyNumber=0;
    if(statCBox->isChecked())
    {
        statisticsGroup->setVisible(true);
        this->setFixedWidth(960);
    }
    else
    {
        statisticsGroup->setVisible(false);
        this->setFixedWidth(700);
        //取消埋点统计测试，把xmltree选中的勾都去掉
        QTreeWidgetItemIterator item(xmlTreeWidget);
        while(*item)
        {
            if((*item)->checkState(0) == Qt::Checked)
            {
                if((*item)->childCount()==0)
                {
                    (*item)->setCheckState(0,Qt::Unchecked);
                }
            }
            ++item;
        }
    }
}

void MonkeySetUp::TextChanged()
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void MonkeySetUp::logTextChanged()
{
    QToolTip::showText(cursor().pos(),"日志关键字用分号(;)分开");
}

void MonkeySetUp::closeEvent(QCloseEvent *event)
{
    ParseXml::saveConfigXmlIP_Port(ipLineEdit->text(),portLineEdit->text());

    /*if(pathLineEdit->text()=="")
    {
        QMessageBox::information(this,"提示","工作路径为空，默认为当前运行目录!");
        QString s=QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
        pathLineEdit->setText(s);
    }*/
    signalMonkeyData_s signalData;
    signalData.data=data;
    signalData.logKeyWords=logKeyWordName->text();
    signalData.isMemCheck=isMemCheck;
    signalData.isCpuCheck=isCpuCheck;
    signalData.isBatteryCheck=isBatteryCheck;
    signalData.isCpuTempCheck=isCpuTempCheck;
    signalData.isWifiCheck=isWifiCheck;
    signalData.isMobileCheck=isMobileCheck;
    signalData.memThres=memLineEdit->text();
    signalData.cpuThres=cpuLineEdit->text();
    signalData.batteryThres=batteryLineEdit->text();
    signalData.cpuTempThres=cpuTempLineEdit->text();
    signalData.wifiThres=wifiLineEdit->text();
    signalData.mobileThres=mobileLineEdit->text();
    signalData.xmlPaths=xmlFilePaths;
    signalData.isDebug=isDebug;

    QVariant var;
    var.setValue(signalData);
    emit SendData(var);
}

void MonkeySetUp::level0Changed()
{
    data.logLevel=0;
}
void MonkeySetUp::level1Changed()
{
    data.logLevel=1;
}
void MonkeySetUp::level2Changed()
{
    data.logLevel=2;
}
void MonkeySetUp::level3Changed()
{
    data.logLevel=3;
}

void MonkeySetUp::countChanged()
{
    if(countCBtn->isChecked())
    {
        data.count=true;
    }
    else data.count=false;
}
void MonkeySetUp::countTextChanged()
{
    if(countLineEdit->text().trimmed()=="")
    {
        data.countNum="0";
        return;
    }
    QString pattern("^(0|[1-9][0-9]*)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(countLineEdit->text().trimmed());
    if(match)
        data.countNum=countLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","运行次数输入有误，请重新输入！");
        countLineEdit->setText("");
    }

}

void MonkeySetUp::seedChanged()
{
    if(seedCBtn->isChecked())
        data.seed=true;
    else data.seed=false;
}
void MonkeySetUp::seedTextChanged()
{
    if(seedLineEdit->text().trimmed()=="")
    {
        data.seedNum="0";
        return;
    }
    QString pattern("^([0-9]*)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(seedLineEdit->text().trimmed());
    if(match)
        data.seedNum=seedLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","seed输入有误，请重新输入！");
        seedLineEdit->setText("");
    }
}

void MonkeySetUp::throttleChanged()
{
    if(throttleCBtn->isChecked())
    {
        data.throttle=true;
        randomCBtn->setEnabled(true);
    }
    else
    {
        randomCBtn->setEnabled(false);
        data.throttle=false;
    }
}
void MonkeySetUp::throttleTextChanged()
{
    if(throttleLineEdit->text().trimmed()=="")
    {
        data.throttleNum="0";
        return;
    }
    QString pattern("^(0|[1-9][0-9]*)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(throttleLineEdit->text().trimmed());
    if(match)
        data.throttleNum=throttleLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","throttle输入有误，请重新输入！");
        throttleLineEdit->setText("");
    }
}
void MonkeySetUp::randomChanged()
{
    if(randomCBtn->isChecked())
        data.random=true;
    else
        data.random=false;
}

void MonkeySetUp::touchChanged()
{
    if(touchCBtn->isChecked())
        data.touch=true;
    else
        data.touch=false;
}
void MonkeySetUp::touchTextChanged()
{
    if(touchLineEdit->text().trimmed()=="")
    {
        data.touchNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(touchLineEdit->text().trimmed());
    if(match)
        data.touchNum=touchLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","touch百分比输入有误，请重新输入！");
        touchLineEdit->setText("");
    }
}

void MonkeySetUp::motionChanged()
{
    if(motionCBtn->isChecked())
        data.motion=true;
    else
        data.motion=false;
}
void MonkeySetUp::motionTextChanged()
{
    if(motionLineEdit->text().trimmed()=="")
    {
        data.motionNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(motionLineEdit->text().trimmed());
    if(match)
        data.motionNum=motionLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","motion百分比输入有误，请重新输入！");
        motionLineEdit->setText("");
    }
}

void MonkeySetUp::trackballChanged()
{
    if(trackBallCBtn->isChecked())
        data.trackBall=true;
    else
        data.trackBall=false;
}
void MonkeySetUp::trackballTextChanged()
{
    if(trackBallLineEdit->text().trimmed()=="")
    {
        data.trackBalNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(trackBallLineEdit->text().trimmed());
    if(match)
        data.trackBalNum=trackBallLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","rtackball百分比输入有误，请重新输入！");
        trackBallLineEdit->setText("");
    }
}

void MonkeySetUp::syskeysChanged()
{
    if(syskeysCBtn->isChecked())
        data.syskeys=true;
    else
        data.syskeys=false;
}
void MonkeySetUp::syskeysTextChanged()
{
    if(syskeysLineEdit->text().trimmed()=="")
    {
        data.syskeysNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(syskeysLineEdit->text().trimmed());
    if(match)
        data.syskeysNum=syskeysLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","syskeys百分比输入有误，请重新输入！");
        syskeysLineEdit->setText("");
    }
}

void MonkeySetUp::navChanged()
{
    if(navCBtn->isChecked())
        data.nav=true;
    else
        data.nav=false;
}
void MonkeySetUp::navTextChanged()
{
    if(navLineEdit->text().trimmed()=="")
    {
        data.navNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(navLineEdit->text().trimmed());
    if(match)
        data.navNum=navLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","nav百分比输入有误，请重新输入！");
        navLineEdit->setText("");
    }
}

void MonkeySetUp::majornavChanged()
{
    if(majorNavCBtn->isChecked())
        data.majorNav=true;
    else
        data.majorNav=false;
}
void MonkeySetUp::majornavTextChanged()
{
    if(majorNavLineEdit->text().trimmed()=="")
    {
        data.majorNavNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(majorNavLineEdit->text().trimmed());
    if(match)
        data.majorNavNum=majorNavLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","majornav百分比输入有误，请重新输入！");
        majorNavLineEdit->setText("");
    }
}

void MonkeySetUp::appswitchChanged()
{
    if(appSwitchCBtn->isChecked())
        data.appSwitch=true;
    else
        data.appSwitch=false;
}
void MonkeySetUp::appswitchTextChanged()
{
    if(appSwitchLineEdit->text().trimmed()=="")
    {
        data.appSwitchNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(appSwitchLineEdit->text().trimmed());
    if(match)
        data.appSwitchNum=appSwitchLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","appswitch百分比输入有误，请重新输入！");
        appSwitchLineEdit->setText("");
    }
}

void MonkeySetUp::flipChanged()
{
    if(flipCBtn->isChecked())
        data.flip=true;
    else
        data.flip=false;
}
void MonkeySetUp::flipTextChanged()
{
    if(flipLineEdit->text().trimmed()=="")
    {
        data.flipNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(flipLineEdit->text().trimmed());
    if(match)
        data.flipNum=flipLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","flip百分比输入有误，请重新输入！");
        flipLineEdit->setText("");
    }
}

void MonkeySetUp::pinchzoomChanged()
{
    if(pinchZoomCBtn->isChecked())
        data.pinchZoom=true;
    else
        data.pinchZoom=false;
}
void MonkeySetUp::pinchzoomTextChanged()
{
    if(pinchZoomLineEdit->text().trimmed()=="")
    {
        data.pinchZoomNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(pinchZoomLineEdit->text().trimmed());
    if(match)
        data.pinchZoomNum=pinchZoomLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","pinchzoom百分比输入有误，请重新输入！");
        pinchZoomLineEdit->setText("");
    }
}

void MonkeySetUp::anyeventChanged()
{
    if(anyEventCBtn->isChecked())
        data.anyevent=true;
    else
        data.anyevent=false;
}
void MonkeySetUp::anyeventTextChanged()
{
    if(anyEventLineEdit->text().trimmed()=="")
    {
        data.anyeventNum="0";
        return;
    }
    QString pattern("^(100|0|[1-9][0-9]?)$");
    QRegExp rx(pattern);
    bool match = rx.exactMatch(anyEventLineEdit->text().trimmed());
    if(match)
        data.anyeventNum=anyEventLineEdit->text().trimmed();
    else
    {
        QMessageBox::information(this,"提示","anyevent百分比输入有误，请重新输入！");
        anyEventLineEdit->setText("");
    }
}

void MonkeySetUp::widgetInit()
{
    setWindowTitle("设置");
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    logLevelGroup=new QGroupBox(tr("日志输出级别"));
    levelZeroRBtn=new QRadioButton(tr("level0"));
    levelOneRBtn=new QRadioButton(tr("level1"));
    levelOneRBtn->setChecked(true);
    levelTwoRBtn=new QRadioButton(tr("level2"));
    levelThreeRBtn=new QRadioButton(tr("level3"));
    logLevelLayout=new QHBoxLayout;

    logLevelLayout->addWidget(levelZeroRBtn);
    logLevelLayout->addWidget(levelOneRBtn);
    logLevelLayout->addWidget(levelTwoRBtn);
    logLevelLayout->addWidget(levelThreeRBtn);
    logLevelLayout->setContentsMargins(30,10,10,10);
    logLevelGroup->setLayout(logLevelLayout);

    basicEventGroup=new QGroupBox(tr("基本事件"));
    countCBtn=new QCheckBox(tr("count"));
    countCBtn->setChecked(true);
    seedCBtn=new QCheckBox(tr("seed"));
    seedCBtn->setChecked(true);
    throttleCBtn=new QCheckBox(tr("throttle"));
    throttleCBtn->setChecked(true);
    randomCBtn=new QCheckBox(tr("random"));
    countLineEdit=new QLineEdit;
    countLineEdit->setFixedWidth(LINETEXTWIDTH);
    countLineEdit->setText("100");
    seedLineEdit=new QLineEdit;
    seedLineEdit->setFixedWidth(LINETEXTWIDTH);
    //seed加随机码
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int n = qrand()%9999;
    seedLineEdit->setText(QString::number(n));
    data.seedNum=QString::number(n);//直接设置的不会去调用textchanged，只有这一个涉及到手动改动

    throttleLineEdit=new QLineEdit;
    throttleLineEdit->setFixedWidth(LINETEXTWIDTH);
    throttleLineEdit->setText("100");
    basicEventLayout=new QGridLayout;


    basicEventLayout->addWidget(countCBtn,0,0);
    basicEventLayout->addWidget(countLineEdit,0,1,Qt::AlignLeft);
    basicEventLayout->addWidget(seedCBtn,0,2);
    basicEventLayout->addWidget(seedLineEdit,0,3,Qt::AlignLeft);
    basicEventLayout->addWidget(throttleCBtn,1,0);
    basicEventLayout->addWidget(throttleLineEdit,1,1,Qt::AlignLeft);
    basicEventLayout->addWidget(randomCBtn,1,2);
    basicEventLayout->setContentsMargins(30,10,10,10);
    basicEventGroup->setLayout(basicEventLayout);

    eventPercGroup=new QGroupBox(tr("事件百分比"));
    touchCBtn=new QCheckBox(tr("touch"));
    motionCBtn=new QCheckBox(tr("motion"));
    trackBallCBtn=new QCheckBox(tr("trackball"));
    syskeysCBtn=new QCheckBox(tr("syskeys"));
    navCBtn=new QCheckBox(tr("nav"));
    majorNavCBtn=new QCheckBox(tr("majornav"));
    appSwitchCBtn=new QCheckBox(tr("appswitch"));
    flipCBtn=new QCheckBox(tr("flip"));
    pinchZoomCBtn=new QCheckBox(tr("pinchzoom"));
    anyEventCBtn=new QCheckBox(tr("anyevent"));

    touchLineEdit=new QLineEdit;
    touchLineEdit->setFixedWidth(LINETEXTWIDTH);
    motionLineEdit=new QLineEdit;
    motionLineEdit->setFixedWidth(LINETEXTWIDTH);
    trackBallLineEdit=new QLineEdit;
    trackBallLineEdit->setFixedWidth(LINETEXTWIDTH);
    syskeysLineEdit=new QLineEdit;
    syskeysLineEdit->setFixedWidth(LINETEXTWIDTH);
    navLineEdit=new QLineEdit;
    navLineEdit->setFixedWidth(LINETEXTWIDTH);
    majorNavLineEdit=new QLineEdit;
    majorNavLineEdit->setFixedWidth(LINETEXTWIDTH);
    appSwitchLineEdit=new QLineEdit;
    appSwitchLineEdit->setFixedWidth(LINETEXTWIDTH);
    flipLineEdit=new QLineEdit;
    flipLineEdit->setFixedWidth(LINETEXTWIDTH);
    pinchZoomLineEdit=new QLineEdit;
    pinchZoomLineEdit->setFixedWidth(LINETEXTWIDTH);
    anyEventLineEdit=new QLineEdit;
    anyEventLineEdit->setFixedWidth(LINETEXTWIDTH);

    eventPercLayout=new QGridLayout;
    eventPercLayout->addWidget(touchCBtn,0,0);
    eventPercLayout->addWidget(touchLineEdit,0,1,Qt::AlignLeft);
    eventPercLayout->addWidget(motionCBtn,0,2);
    eventPercLayout->addWidget(motionLineEdit,0,3,Qt::AlignLeft);
    eventPercLayout->addWidget(trackBallCBtn,1,0);
    eventPercLayout->addWidget(trackBallLineEdit,1,1,Qt::AlignLeft);
    eventPercLayout->addWidget(syskeysCBtn,1,2);
    eventPercLayout->addWidget(syskeysLineEdit,1,3,Qt::AlignLeft);
    eventPercLayout->addWidget(navCBtn,2,0);
    eventPercLayout->addWidget(navLineEdit,2,1,Qt::AlignLeft);
    eventPercLayout->addWidget(majorNavCBtn,2,2);
    eventPercLayout->addWidget(majorNavLineEdit,2,3,Qt::AlignLeft);
    eventPercLayout->addWidget(appSwitchCBtn,3,0);
    eventPercLayout->addWidget(appSwitchLineEdit,3,1,Qt::AlignLeft);
    eventPercLayout->addWidget(flipCBtn,3,2);
    eventPercLayout->addWidget(flipLineEdit,3,3,Qt::AlignLeft);
    eventPercLayout->addWidget(pinchZoomCBtn,4,0);
    eventPercLayout->addWidget(pinchZoomLineEdit,4,1,Qt::AlignLeft);
    eventPercLayout->addWidget(anyEventCBtn,4,2);
    eventPercLayout->addWidget(anyEventLineEdit,4,3,Qt::AlignLeft);
    eventPercLayout->setContentsMargins(30,10,10,10);
    eventPercGroup->setLayout(eventPercLayout);

    logKeyWordGroup=new QGroupBox(tr("日志关键字过滤"));
    logKeyWordName=new QLineEdit;
    logKeyWordName->setText("ANR;CRASH;Exception");

    logKeyWordLayout=new QHBoxLayout;
    logKeyWordLayout->addWidget(logKeyWordName);
    logKeyWordGroup->setLayout(logKeyWordLayout);

    classGroup=new QGroupBox(tr("类名约束"));
    className=new QTextEdit;
    className->setFixedHeight(40);
    classLayout=new QHBoxLayout;
    classLayout->addWidget(className);
    classGroup->setLayout(classLayout);

    debugGroup=new QGroupBox(tr("调试"));
    iCrashesCBtn=new QCheckBox(tr("ignore-crashes"));
    iCrashesCBtn->setChecked(true);
    iTimeoutCBtn=new QCheckBox(tr("ignore-timeouts"));
    iTimeoutCBtn->setChecked(true);
    isecurityCBtn=new QCheckBox(tr("ignore-security-exceptions"));
    isecurityCBtn->setChecked(true);
    dbgNoEventCBtn=new QCheckBox(tr("dbg-no-events"));
    killProcessCBtn=new QCheckBox(tr("kill-process-after-error"));
    monitorCBtn=new QCheckBox(tr("monitor-native-crashes"));
    monitorCBtn->setChecked(true);
    waitDBGNavCBtn=new QCheckBox(tr("wait-dbg"));
    hprofCBtn=new QCheckBox(tr("hprof"));
    debugLayout=new QVBoxLayout;

    debugLayout->addWidget(iCrashesCBtn);
    debugLayout->addWidget(iTimeoutCBtn);
    debugLayout->addWidget(isecurityCBtn);
    debugLayout->addWidget(dbgNoEventCBtn);
    debugLayout->addWidget(killProcessCBtn);
    debugLayout->addWidget(monitorCBtn);
    debugLayout->addWidget(waitDBGNavCBtn);
    debugLayout->addWidget(hprofCBtn);
    debugLayout->setContentsMargins(30,10,10,10);
    debugGroup->setLayout(debugLayout);

    cmdGroup=new QGroupBox(tr("命令"));
    cmdTextEdit=new QTextEdit;
    cmdTextEdit->setFixedHeight(40);
    cmdLayout=new QHBoxLayout;
    cmdLayout->addWidget(cmdTextEdit);
    cmdGroup->setLayout(cmdLayout);

    cmdTextEdit->setToolTip("命令框若有内容，其他设置不起作用");

    //参数测量
    memCBtn=new QCheckBox("内存");
    cpuCBtn=new QCheckBox("CPU");
    batteryCBtn=new QCheckBox("电池温度");
    cpuTempCBtn=new QCheckBox("CPU温度");
    wifiCBtn=new QCheckBox("Wifi流量");
    mobileCBtn=new QCheckBox("Mobile流量");
    memLabel=new QLabel("警戒值(M):");
    cpuLabel=new QLabel("警戒值(%):");
    batteryLabel=new QLabel("警戒值(℃):");
    cpuTempLabel=new QLabel("警戒值(℃):");
    wifiLabel=new QLabel("警戒值(M):");
    mobileLabel=new QLabel("警戒值(M):");
    memLineEdit=new QLineEdit;
    cpuLineEdit=new QLineEdit;
    batteryLineEdit=new QLineEdit;
    cpuTempLineEdit=new QLineEdit;
    wifiLineEdit=new QLineEdit;
    mobileLineEdit=new QLineEdit;
    memLineEdit->setFixedWidth(LINETEXTWIDTH);
    cpuLineEdit->setFixedWidth(LINETEXTWIDTH);
    batteryLineEdit->setFixedWidth(LINETEXTWIDTH);
    cpuTempLineEdit->setFixedWidth(LINETEXTWIDTH);
    wifiLineEdit->setFixedWidth(LINETEXTWIDTH);
    mobileLineEdit->setFixedWidth(LINETEXTWIDTH);

    measureLayout=new QGridLayout;
    measureLayout->addWidget(memCBtn,0,0);
    measureLayout->addWidget(memLabel,0,1);
    measureLayout->addWidget(memLineEdit,0,2);
    measureLayout->addWidget(cpuCBtn,1,0);
    measureLayout->addWidget(cpuLabel,1,1);
    measureLayout->addWidget(cpuLineEdit,1,2);
    measureLayout->addWidget(batteryCBtn,2,0);
    measureLayout->addWidget(batteryLabel,2,1);
    measureLayout->addWidget(batteryLineEdit,2,2);
    measureLayout->addWidget(cpuTempCBtn,3,0);
    measureLayout->addWidget(cpuTempLabel,3,1);
    measureLayout->addWidget(cpuTempLineEdit,3,2);
    measureLayout->addWidget(wifiCBtn,4,0);
    measureLayout->addWidget(wifiLabel,4,1);
    measureLayout->addWidget(wifiLineEdit,4,2);
    measureLayout->addWidget(mobileCBtn,5,0);
    measureLayout->addWidget(mobileLabel,5,1);
    measureLayout->addWidget(mobileLineEdit,5,2);
    measureLayout->setContentsMargins(30,10,30,10);
    measureGroup=new QGroupBox("参数测量选项");
    measureGroup->setLayout(measureLayout);

    appVersionLabel=new QLabel("APP版本：");
    debugRBtn=new QRadioButton("debug");
    debugRBtn->setChecked(true);
    releaseRBtn=new QRadioButton("release");
    selectXmlBtn=new QPushButton("点击选择XML文件");
    ipLable=new QLabel("IP:");
    portLabel=new QLabel("Port:");
    ipLineEdit=new QLineEdit;
    portLineEdit=new QLineEdit;
    portLineEdit->setFixedWidth(LINETEXTWIDTH);
    ipPortLayout=new QHBoxLayout;
    versionLayout=new QHBoxLayout;
    versionLayout->addWidget(appVersionLabel);
    versionLayout->addWidget(debugRBtn);
    versionLayout->addWidget(releaseRBtn);
    ipPortLayout->addWidget(ipLable);
    ipPortLayout->addWidget(ipLineEdit);
    ipPortLayout->addWidget(portLabel);
    ipPortLayout->addWidget(portLineEdit);
    xmlTreeWidget=new QTreeWidget;

    statisticsLayout=new QVBoxLayout;
    statisticsLayout->addLayout(versionLayout);
    statisticsLayout->addLayout(ipPortLayout);
    statisticsLayout->addWidget(selectXmlBtn);
    statisticsLayout->addWidget(xmlTreeWidget);

    statisticsGroup=new QGroupBox("统计点");
    statisticsGroup->setLayout(statisticsLayout);

    leftLayout=new QVBoxLayout;
    leftLayout->addWidget(logLevelGroup,1);
    leftLayout->addWidget(basicEventGroup,1);
    leftLayout->addWidget(eventPercGroup,3);
    leftLayout->addWidget(classGroup,1);
    leftLayout->addWidget(logKeyWordGroup,1);
   // leftLayout->addWidget(statisticsGroup,6);

    statCBox=new QCheckBox("统计点测试");
    QHBoxLayout *statLayout=new QHBoxLayout;
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    statLayout->addWidget(spacer);
    statLayout->addWidget(statCBox);
    statLayout->setContentsMargins(0,0,30,0);
    rightLayout=new QVBoxLayout;
    rightLayout->addWidget(debugGroup,8);
    rightLayout->addWidget(measureGroup,2);
    rightLayout->addLayout(statLayout,1);

    QVBoxLayout *contentLayout=new QVBoxLayout;
    QHBoxLayout *topLayout=new QHBoxLayout;
    topLayout->addLayout(leftLayout,5);
    topLayout->addLayout(rightLayout,3);
    contentLayout->addLayout(topLayout,12);
    contentLayout->addWidget(cmdGroup,1);

    mainlayout=new QHBoxLayout(this);
    mainlayout->addLayout(contentLayout,8);
    mainlayout->addWidget(statisticsGroup,3);

    statisticsGroup->setVisible(false);
    setFixedSize(700,540);
}

void MonkeySetUp::classNameChanged()
{
    QString cName=className->toPlainText();
    if(cName=="")
        data.className="";
    else
    {
        cName.replace("\n"," -c ");
        data.className=cName;
    }

}

void MonkeySetUp::iCrashChanged()
{
    if(iCrashesCBtn->isChecked())
        data.iCrashes=true;
    else
        data.iCrashes=false;
}
void MonkeySetUp::iTimeoutChanged()
{
    if(iTimeoutCBtn->isChecked())
        data.iTimeouts=true;
    else
        data.iTimeouts=false;
}
void MonkeySetUp::iSecurityChanged()
{
    if(isecurityCBtn->isChecked())
        data.iSecurity=true;
    else
        data.iSecurity=false;
}

void MonkeySetUp::dbgNoEventChanged()
{
    if(dbgNoEventCBtn->isChecked())
        data.dbgNoEvent=true;
    else
        data.dbgNoEvent=false;
}
void MonkeySetUp::killProcessChanged()
{
    if(killProcessCBtn->isChecked())
        data.killProcess=true;
    else
        data.killProcess=false;
}
void MonkeySetUp::monitorChanged()
{
    if(monitorCBtn->isChecked())
        data.monitorNative=true;
    else
        data.monitorNative=false;
}
void MonkeySetUp::waitDngChanged()
{
    if(waitDBGNavCBtn->isChecked())
        data.waitDbg=true;
    else
        data.waitDbg=false;
}
void MonkeySetUp::hprofChanged()
{
    if(hprofCBtn->isChecked())
        data.hprof=true;
    else
        data.hprof=false;
}

void MonkeySetUp::cmdMonkeyChanged()
{
    data.cmdMonkeyStr=cmdTextEdit->toPlainText();

    if(cmdTextEdit->toPlainText().trimmed()!="")
    {
        logLevelGroup->setEnabled(false);
        basicEventGroup->setEnabled(false);
        eventPercGroup->setEnabled(false);
        classGroup->setEnabled(false);
        debugGroup->setEnabled(false);
    }
    else
    {
        logLevelGroup->setEnabled(true);
        basicEventGroup->setEnabled(true);
        eventPercGroup->setEnabled(true);
        classGroup->setEnabled(true);
        debugGroup->setEnabled(true);
    }

}

/*void MonkeySetUp::filePathChanged()
{
    data.filePath=pathLineEdit->text();
}

void MonkeySetUp::browseBtnClick()
{
    QString s= QFileDialog::getExistingDirectory(this,"选择路径",pathLineEdit->text());
    pathLineEdit->setText(s);
}*/

void MonkeySetUp::memCBtnCheck()
{
    isMemCheck=memCBtn->isChecked();
}

void MonkeySetUp::cpuCBtnCheck()
{
    isCpuCheck=cpuCBtn->isChecked();
}

void MonkeySetUp::batteryCBtnCheck()
{
    isBatteryCheck=batteryCBtn->isChecked();
}

void MonkeySetUp::cpuTempCBtnCheck()
{
    isCpuTempCheck=cpuTempCBtn->isChecked();
}

void MonkeySetUp::wifiCBtnCheck()
{
    isWifiCheck=wifiCBtn->isChecked();
}

void MonkeySetUp::mobileCBtnCheck()
{
    isMobileCheck=mobileCBtn->isChecked();
}

void MonkeySetUp::stateChanged(bool state)
{
    batteryCBtn->setEnabled(!state);
    cpuTempCBtn->setEnabled(!state);
    memCBtn->setEnabled(!state);
    cpuCBtn->setEnabled(!state);
    wifiCBtn->setEnabled(!state);
    mobileCBtn->setEnabled(!state);

    batteryLineEdit->setEnabled(!state);
    cpuTempLineEdit->setEnabled(!state);
    memLineEdit->setEnabled(!state);
    cpuLineEdit->setEnabled(!state);
    wifiLineEdit->setEnabled(!state);
    mobileLineEdit->setEnabled(!state);

    memLabel->setEnabled(!state);
    cpuLabel->setEnabled(!state);
    batteryLabel->setEnabled(!state);
    cpuTempLabel->setEnabled(!state);
    wifiLabel->setEnabled(!state);
    mobileLabel->setEnabled(!state);
}

void MonkeySetUp::recieveSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck,bool isMobileCheck,
                                    QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres)
{
    memCBtn->setChecked(isMemCheck);
    cpuCBtn->setChecked(isCpuCheck);
    batteryCBtn->setChecked(isBatteryCheck);
    cpuTempCBtn->setChecked(isCpuTempCheck);
    wifiCBtn->setChecked(isWifiCheck);
    mobileCBtn->setChecked(isMobileCheck);

  //  pathLineEdit->setText(filePath);

    this->isMemCheck=isMemCheck;
    this->isCpuCheck=isCpuCheck;
    this->isBatteryCheck=isBatteryCheck;
    this->isCpuTempCheck=isCpuTempCheck;
    this->isWifiCheck=isWifiCheck;
    this->isMobileCheck=isMobileCheck;

    memLineEdit->setText(memThres);
    cpuLineEdit->setText(cpuThres);
    batteryLineEdit->setText(batteryThres);
    cpuTempLineEdit->setText(cpuTempThres);
    wifiLineEdit->setText(wifiThres);
    mobileLineEdit->setText(mobileThres);

}

void MonkeySetUp::debugRBtnClick()
{
    isDebug=true;
}

void MonkeySetUp::releaseRBtnClick()
{
    isDebug=false;
}

void MonkeySetUp::selectXmlBtnClick()
{
   // xmlTreeWidget->header()->setMinimumSectionSize(400);
    xmlFilePaths.clear();
    gMonkeyNumber=0;

    filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),gWorkSpace,QFileDialog::ShowDirsOnly);
    if(filePath=="")
        return;
    xmlTreeWidget->clear();

    QTreeWidgetItem *fileItem = new QTreeWidgetItem();
    StaticticsHerper::LoadFiles(filePath,xmlTreeWidget,fileItem);
    QTreeWidgetItem *item=xmlTreeWidget->itemAt(0,0);
    xmlTreeWidget->setCurrentItem(item);
}

void MonkeySetUp::treeItemChanged(QTreeWidgetItem *item, int column)
{
    QString itemText = item->text(0);
    if (item->checkState(0) == Qt::Checked)
    {
        QTreeWidgetItem *parent = item->parent();
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

void MonkeySetUp::on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->checkState(0) == Qt::Checked)
    {
        if(item->childCount()==0)
        {
            QString xmlFilePath=QDir::toNativeSeparators( filePath+"/"+StaticticsHerper::getBasePath(item));//!!!!!!!!!
            qDebug()<<xmlFilePath;
            xmlFilePaths.append(xmlFilePath);
            gMonkeyNumber++;
        }
    }
    else
    {
        if(item->childCount()==0)
        {
            for(int i=0;i<xmlFilePaths.length();i++)
            {
                if(xmlFilePaths[i]==QDir::toNativeSeparators( filePath+"/"+ StaticticsHerper::getBasePath(item)))
                {
                   xmlFilePaths.removeAt(i);
                   gMonkeyNumber--;
                   break;
                }
            }

        }
    }
}

MonkeySetUp::~MonkeySetUp()
{
    //delete logLevelGroup;
    delete levelZeroRBtn;
    delete levelOneRBtn;
    delete levelTwoRBtn;
    delete levelThreeRBtn;
    delete logLevelLayout;
    delete countCBtn;
    delete seedCBtn;
    delete throttleCBtn;
    delete randomCBtn;
    delete countLineEdit;
    delete seedLineEdit;
    delete throttleLineEdit;
    delete basicEventLayout;

    delete touchCBtn;
    delete motionCBtn;
    delete trackBallCBtn;
    delete syskeysCBtn;
    delete navCBtn;
    delete majorNavCBtn;
    delete appSwitchCBtn;
    delete flipCBtn;
    delete pinchZoomCBtn;
    delete anyEventCBtn;
    delete touchLineEdit;
    delete motionLineEdit;
    delete trackBallLineEdit;
    delete syskeysLineEdit;
    delete navLineEdit;
    delete majorNavLineEdit;
    delete appSwitchLineEdit;
    delete flipLineEdit;
    delete pinchZoomLineEdit;
    delete anyEventLineEdit;
    delete eventPercLayout;
    delete className;
    delete classLayout;
    delete iCrashesCBtn;
    delete iTimeoutCBtn;
    delete isecurityCBtn;
    delete dbgNoEventCBtn;
    delete killProcessCBtn;
    delete monitorCBtn;
    delete waitDBGNavCBtn;
    delete hprofCBtn;
    delete debugLayout;
    //delete pathLineEdit;
    //delete browseBtn;
   // delete pathLayout;
    delete cmdTextEdit;
    delete cmdLayout;
    delete leftLayout;
    delete rightLayout;

    delete memCBtn;
    delete cpuCBtn;
    delete batteryCBtn;
    delete cpuTempCBtn;
    delete measureLayout;
}
