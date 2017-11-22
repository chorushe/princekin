#include "interfacesetup.h"
#include "ui_interfacesetup.h"

InterfaceSetup::InterfaceSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterfaceSetup)
{
    ui->setupUi(this);

    setWindowTitle("设置");
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    isMemCheck=false;
    isCpuCheck=false;
    isBatteryCheck=false;
    isCpuTempCheck=false;
    isWifiCheck=false;
    isMobileCheck=false;

    ui->memLineEdit->setText(gMemThres);
    ui->cpuLineEdit->setText(gCpuThres);
    ui->batteryLineEdit->setText(gBatteryThres);
    ui->cpuTempLineEdit->setText(gCpuTempThres);
    ui->wifiLineEdit->setText(gWifiThres);
    ui->mobileLineEdit->setText(gMobileThres);

    QStringList tempList=ParseXml::readConfigXmlForIP_Port(gConfigDir + QDir::separator() + "config.xml");
    ui->IPLineEdit->setText(tempList[0]);
    ui->portLineEdit->setText(tempList[1]);
}

InterfaceSetup::~InterfaceSetup()
{
    delete ui;
}

void InterfaceSetup::closeEvent(QCloseEvent *event)
{
    ParseXml::saveConfigXmlIP_Port(ui->IPLineEdit->text(),ui->portLineEdit->text());

    signalInterfaceData_s signalData;

    signalData.isMemCheck=ui->memCBtn->isChecked();
    signalData.isCpuCheck=ui->cpuCBtn->isChecked();
    signalData.isBatteryCheck=ui->batteryCBtn->isChecked();
    signalData.isCpuTempCheck=ui->cpuTempCBtn->isChecked();
    signalData.isWifiCheck=ui->wifiCBtn->isChecked();
    signalData.isMobileCheck=ui->mobileCBtn->isChecked();
    signalData.memThres=ui->memLineEdit->text();
    signalData.cpuThres=ui->cpuLineEdit->text();
    signalData.batteryThres=ui->batteryLineEdit->text();
    signalData.cpuTempThres=ui->cpuTempLineEdit->text();
    signalData.wifiThres=ui->wifiLineEdit->text();
    signalData.mobileThres=ui->mobileLineEdit->text();
    signalData.isAllTest=ui->allTestCBtn->isChecked();
    signalData.method=ui->methodComboBox->currentText();
    signalData.filterStr=ui->filterLineEdit->text();

    QVariant var;
    var.setValue(signalData);
    emit SendData(var);
}

void InterfaceSetup::stateChanged(bool state)
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

void InterfaceSetup::recieveSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
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

    //ui->pathLineEdit->setText(gWorkSpace);
}

void InterfaceSetup::on_allTestCBtn_clicked()
{
    if(ui->allTestCBtn->isChecked())
    {
        ui->methodComboBox->setEnabled(false);
        ui->filterLineEdit->setEnabled(false);
    }
    else
    {
        ui->methodComboBox->setEnabled(true);
        ui->filterLineEdit->setEnabled(true);
    }
}
