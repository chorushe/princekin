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
    ui->proxyRBtn->setChecked(true);
    ui->configBtn->setEnabled(false);
    filePath=gWorkSpace;
    isMemCheck=false;
    isCpuCheck=false;
    isBatteryCheck=false;
    isCpuTempCheck=false;
    isWifiCheck=false;
    isMobileCheck=false;

    isServerPattern=false;

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

void BehaviourSetup::on_proxyRBtn_clicked()
{
    if(ui->proxyRBtn->isChecked())
    {
        isServerPattern=false;
        ui->configBtn->setEnabled(false);
        ui->versionGroup->setEnabled(true);
        QToolTip::showText(cursor().pos(),"代理器模式可根据安装的实际情况选则APP版本");
    }
}

void BehaviourSetup::on_serverRBtn_clicked()
{
    if(ui->serverRBtn->isChecked())
    {
        isServerPattern=true;
        ui->configBtn->setEnabled(true);
        ui->versionGroup->setEnabled(false);
        QToolTip::showText(cursor().pos(),"服务器模式只能测试debug版本的APP包");
    }
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

    //读出来behaviorconfig里的文件，写入behavior_cmd.bat文件
    parseIP_PortToBat(gConfigDir + QDir::separator() + "behaviorconfig.xml");

    emit SendData(ui->xmlRootLineEdit->text(), isMemCheck,isCpuCheck,isBatteryCheck,isCpuTempCheck,isWifiCheck,isMobileCheck,appUid,isServerPattern,isDebugVersion,ui->memLineEdit->text(),ui->cpuLineEdit->text(),ui->batteryLineEdit->text(),ui->cpuTempLineEdit->text(),ui->wifiLineEdit->text(),ui->mobileLineEdit->text());
    event->accept();

}

void BehaviourSetup::parseIP_PortToBat(QString fileName)
{
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(!file.exists())
        {
            QMessageBox::information(NULL, QString("提示"), QString(fileName+" 文件不存在!"));
            return;
        }
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::information(NULL, QString("提示"), QString(fileName+"打开错误!"));
            return ;
        }
        QDomDocument document;
        QString error;
        int row = 0, column = 0;
        if(!document.setContent(&file, false, &error, &row, &column))
        {
            QMessageBox::information(NULL, QString("提示"), QString("解析文件错误，行： ") + QString::number(row, 10) + QString(" ,列： ") + QString::number(column, 10));
            return ;
        }
        if(document.isNull())
        {
            QMessageBox::information(NULL, QString("提示"), QString("文件为空!"));
            return ;
        }
        QDomElement root = document.documentElement();
        QDomNode node = root.firstChild();

        QString plinkDir=gConfigDir+QDir::separator()+"plink.exe";
        QString serverName;
        QString serverPassword;
        QString serverIP;
        QString serverPort;
        while(!node.isNull())
        {
            if(node.toElement().tagName()=="serverName")
                serverName=node.toElement().text();
            if(node.toElement().tagName()=="serverPassword")
                serverPassword=node.toElement().text();
            if(node.toElement().tagName()=="serverIP")
                serverIP=node.toElement().text();
            if(node.toElement().tagName()=="serverPort")
                serverPort=node.toElement().text();

            if(node.toElement().tagName()=="appUid")
                appUid=node.toElement().text();

            node = node.nextSibling();//读取兄弟节点

        }

        //写入behavior_cmd.bat里
        QFile file1(gConfigDir + QDir::separator() + "behavior_cmd.bat");
        if(!file1.exists())
        {
            //   QMessageBox::warning(this,"警告",file1.fileName()+"文件不存在",QMessageBox::Yes);
        }
        if(!file1.open(QIODevice::WriteOnly  | QIODevice::Text))//如果文件不存在会自动创建
        {
            QMessageBox::warning(this,"警告",file1.fileName()+"打开失败",QMessageBox::Yes);
        }
        QTextStream in(&file1);
        QString str;
        if(ui->serverRBtn->isChecked())
        {
            str="\""+plinkDir+"\" -ssh -l "+serverName+" -pw "+serverPassword+" -P "+serverPort+" "+serverIP;
            in << "echo y | "+str+" \"exit\"\n";
            in << str;
            //qDebug()<<plinkDir+" -ssh -l "+serverName+" -pw "+serverPassword+" -P "+serverPort+" "+serverIP;
        }
        else
        {
            //QString IP=getIPAddress(runCmd("ipconfig"));
            QString IP=ui->IPLineEdit->text();
            QString Port=ui->portLineEdit->text();
            str="mitmdump -b "+IP+" -p "+Port;
            //str="mitmdump -b 10.2.8.94 -p 7788";
            in<<str;
        }
        file.close();
    }
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

    ui->serverRBtn->setEnabled(!state);
    ui->proxyRBtn->setEnabled(!state);
}

void BehaviourSetup::on_configBtn_clicked()
{
    QFile file(gConfigDir + QDir::separator() + "behaviorconfig.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(NULL, QString("提示"), gConfigDir + QDir::separator() + "behaviorconfig.xml 不存在！" );
        return ;
    }
    XmlEditDialog *configXmlDialog=new XmlEditDialog(0,gConfigDir +QDir::separator() + "behaviorconfig.xml");
    //configXmlDialog->setWindowModality(Qt::ApplicationModal);
    configXmlDialog->setAttribute(Qt::WA_QuitOnClose,false);
    configXmlDialog->showNormal();
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

    //isCpuTempCheck=ui->cpuTempCBtn->isChecked();
}

void BehaviourSetup::on_browseBtn_clicked()
{
    QString s= QFileDialog::getExistingDirectory(this,"选择路径");
    ui->xmlRootLineEdit->setText(s);
}

void BehaviourSetup::setSeverBtnEnabled(bool flag)//脚本模式设置服务器模式不可选
{
    ui->serverRBtn->setEnabled(flag);
    if(!flag)
        ui->proxyRBtn->setChecked(true);
}
