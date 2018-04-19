#include "behavioursetup.h"
#include "ui_behavioursetup.h"

BehaviourSetup::BehaviourSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BehaviourSetup)
{
    ui->setupUi(this);

    setWindowTitle("设置");
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    ui->debugRBtn->setChecked(true);
    filePath=gWorkSpace;
    isMemCheck=false;
    isCpuCheck=false;
    isBatteryCheck=false;
    isCpuTempCheck=false;
    isWifiCheck=false;
    isMobileCheck=false;

    isDebugVersion=true;

    ui->memLineEdit->setText(gMemThres);
    ui->cpuLineEdit->setText(gCpuThres);
    ui->batteryLineEdit->setText(gBatteryThres);
    ui->cpuTempLineEdit->setText(gCpuTempThres);
    ui->wifiLineEdit->setText(gWifiThres);
    ui->mobileLineEdit->setText(gMobileThres);

    QStringList tempList=ParseXml::readConfigXmlForIP_Port(gConfigDir + QDir::separator() + "config.xml");
    ui->IPLineEdit->setText(tempList[0]);
    ui->portLineEdit->setText(tempList[1]);
    QString xmlRootPath=gWorkSpace+QDir::separator()+"StatisticalXML";
    if(QDir(xmlRootPath).exists())
        ui->xmlRootLineEdit->setText(xmlRootPath);
    else
        ui->xmlRootLineEdit->setText(gWorkSpace);

    ui->RBtnAndroid->setChecked(true);
}

BehaviourSetup::~BehaviourSetup()
{
    delete ui;
}

void BehaviourSetup::on_memCBtn_clicked()
{
    isMemCheck=ui->memCBtn->isChecked();
}

void BehaviourSetup::on_cpuCBtn_clicked()
{
    isCpuCheck=ui->cpuCBtn->isChecked();
}

void BehaviourSetup::on_batteryCBtn_clicked()
{
    isBatteryCheck=ui->batteryCBtn->isChecked();
}

void BehaviourSetup::on_cpuTempCBtn_clicked()
{
    isCpuTempCheck=ui->cpuTempCBtn->isChecked();
}

void BehaviourSetup::on_wifiCBtn_clicked()
{
    isWifiCheck=ui->wifiCBtn->isChecked();
}

void BehaviourSetup::on_mobileCBtn_clicked()
{
    isMobileCheck=ui->mobileCBtn->isChecked();
}

void BehaviourSetup::on_debugRBtn_clicked()
{
    isDebugVersion=ui->debugRBtn->isChecked();
}

void BehaviourSetup::on_releaseRBtn_clicked()
{
    isDebugVersion=!(ui->releaseRBtn->isChecked());
}

void BehaviourSetup::on_memLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void BehaviourSetup::on_cpuLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void BehaviourSetup::on_batteryLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}


void BehaviourSetup::on_cpuTempLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void BehaviourSetup::on_wifiLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void BehaviourSetup::on_mobileLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void BehaviourSetup::closeEvent(QCloseEvent *event)
{
    ParseXml::saveConfigXmlIP_Port(ui->IPLineEdit->text(),ui->portLineEdit->text());

    //写入behavior_cmd.bat文件
    parseIP_PortToBat();

    signalBehaviorData_s signalData;

    signalData.xmlRootPath=ui->xmlRootLineEdit->text();
    signalData.isMemCheck=isMemCheck;
    signalData.isCpuCheck=isCpuCheck;
    signalData.isBatteryCheck=isBatteryCheck;
    signalData.isCpuTempCheck=isCpuTempCheck;
    signalData.isWifiCheck=isWifiCheck;
    signalData.isMobileCheck=isMobileCheck;
    signalData.isDebugVersion=isDebugVersion;
    signalData.memThres=ui->memLineEdit->text();
    signalData.cpuThres=ui->cpuLineEdit->text();
    signalData.batteryThres=ui->batteryLineEdit->text();
    signalData.cpuTempThres=ui->cpuTempLineEdit->text();
    signalData.wifiThres=ui->wifiLineEdit->text();
    signalData.mobileThres=ui->mobileLineEdit->text();
    signalData.mobileSystem=mobileSystem;
    QVariant var;
    var.setValue(signalData);
    emit SendData(var);

    event->accept();
}

void BehaviourSetup::parseIP_PortToBat()
{
    //写入behavior_cmd.bat里
    QFile file1(gConfigDir + QDir::separator() + "behavior_cmd.bat");
    if(!file1.open(QIODevice::WriteOnly  | QIODevice::Text))//如果文件不存在会自动创建
    {
        QMessageBox::warning(this,"警告",file1.fileName()+"打开失败",QMessageBox::Yes);
    }
    QTextStream in(&file1);
    QString str;
    QString IP=ui->IPLineEdit->text();
    QString Port=ui->portLineEdit->text();
    //str="mitmdump -p "+Port;//新安装的mitmproxy换了新命令，不能用-b，所以为了统一，把设置IP的项去掉了，默认都是测本机
    str="mitmdump --listen-host "+IP+" -p "+Port;
    in<<str;
    file1.close();

}

void BehaviourSetup::stateChanged(bool state)
{
    ui->batteryCBtn->setEnabled(!state);
    ui->cpuTempCBtn->setEnabled(!state);
    ui->memCBtn->setEnabled(!state);
    ui->cpuCBtn->setEnabled(!state);
    ui->wifiCBtn->setEnabled(!state);
    ui->mobileCBtn->setEnabled(!state);

    ui->batteryLineEdit->setEnabled(!state);
    ui->cpuTempLineEdit->setEnabled(!state);
    ui->memLineEdit->setEnabled(!state);
    ui->cpuLineEdit->setEnabled(!state);
    ui->wifiLineEdit->setEnabled(!state);
    ui->mobileLineEdit->setEnabled(!state);

    ui->memLabel->setEnabled(!state);
    ui->cpuLabel->setEnabled(!state);
    ui->batteryLabel->setEnabled(!state);
    ui->cpuTempLabel->setEnabled(!state);
    ui->wifiLabel->setEnabled(!state);
    ui->mobileLabel->setEnabled(!state);
}

void BehaviourSetup::recieveSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
                                       QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres)
{
    ui->memCBtn->setChecked(isMemCheck);
    ui->cpuCBtn->setChecked(isCpuCheck);
    ui->batteryCBtn->setChecked(isBatteryCheck);
    ui->cpuTempCBtn->setChecked(isCpuTempCheck);
    ui->wifiCBtn->setChecked(isWifiCheck);
    ui->mobileCBtn->setChecked(isMobileCheck);

    this->isMemCheck=isMemCheck;
    this->isCpuCheck=isCpuCheck;
    this->isBatteryCheck=isBatteryCheck;
    this->isCpuTempCheck=isCpuTempCheck;
    this->isWifiCheck=isWifiCheck;
    this->isMobileCheck=isMobileCheck;

    ui->memLineEdit->setText(memThres);
    ui->cpuLineEdit->setText(cpuThres);
    ui->batteryLineEdit->setText(batteryThres);
    ui->cpuTempLineEdit->setText(cpuTempThres);
    ui->wifiLineEdit->setText(wifiThres);
    ui->mobileLineEdit->setText(mobileThres);

    ui->wifiCBtn->setEnabled(gWifiNet);
    ui->wifiLabel->setEnabled(gWifiNet);
    ui->wifiLineEdit->setEnabled(gWifiNet);
    ui->mobileCBtn->setEnabled(!gWifiNet);
    ui->mobileLabel->setEnabled(!gWifiNet);
    ui->mobileLineEdit->setEnabled(!gWifiNet);

}

void BehaviourSetup::on_browseBtn_clicked()
{
    QString s= QFileDialog::getExistingDirectory(this,"选择路径");
    ui->xmlRootLineEdit->setText(s);
}


void BehaviourSetup::on_RBtnAndroid_clicked()
{
    mobileSystem="android";
    ui->groupBox_3->setEnabled(true);
}

void BehaviourSetup::on_RBtnIos_clicked()
{
    mobileSystem="ios";
    ui->groupBox_3->setEnabled(false);//ios时，性能数据都没办法测
}

void BehaviourSetup::setMobilePaternAndroid(bool flag)
{
    ui->RBtnAndroid->setChecked(flag);
    ui->RBtnIos->setEnabled(!flag);
    if(flag)
        on_RBtnAndroid_clicked();
}
