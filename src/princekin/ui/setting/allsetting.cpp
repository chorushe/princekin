#include "allsetting.h"
#include "ui_allsetting.h"
#include <QMessageBox>
#include <QDebug>

AllSetting::AllSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllSetting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

AllSetting::~AllSetting()
{
    delete ui;
}

void AllSetting::dataInit()
{
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    QString TempPath=gWorkSpace.left(gWorkSpace.indexOf("PrinceWorkSpace"));
    ui->lineEdit->setText(gNewWorkSpaceDir);//20170713
    isMemCheck=gisMemCheck;
    isCpuCheck=gisCpuCheck;
    isBatteryCheck=gisBatteryCheck;
    isCpuTempCheck=gisCpuTempCheck;
    isWifiCheck=gisWifiCheck;
    isMobileCheck=gisMobileCheck;
    ui->memCBtn->setChecked(isMemCheck);
    ui->cpuCBtn->setChecked(isCpuCheck);
    ui->batteryCBtn->setChecked(isBatteryCheck);
    ui->cpuTempCBtn->setChecked(isCpuTempCheck);
    ui->wifiCBtn->setChecked(isWifiCheck);
    ui->mobileCBtn->setChecked(isMobileCheck);

    ui->memLineEdit->setText(gMemThres);
    ui->cpuLineEdit->setText(gCpuThres);
    ui->batteryLineEdit->setText(gBatteryThres);
    ui->cpuTempLineEdit->setText(gCpuTempThres);
    ui->wifiLineEdit->setText(gWifiThres);
    ui->mobileLineEdit->setText(gMobileThres);

    qDebug()<<"gAndroidJarDir"<<gAndroidJarDir;
    ui->AndroidLlineEdit->setText(gAndroidJarDir);
}

void AllSetting::on_pushButton_clicked()
{
    QString s= QFileDialog::getExistingDirectory(this,"选择路径");
    if(s.contains(" "))
    {
        QMessageBox::information(this,"提示","选择的路径中不能包含空格，请重新选择！");
        s="";
    }
    ui->lineEdit->setText(s);
}

void AllSetting::on_memCBtn_clicked()
{
    isMemCheck=ui->memCBtn->isChecked();
}

void AllSetting::on_cpuCBtn_clicked()
{
    isCpuCheck=ui->cpuCBtn->isChecked();
}

void AllSetting::on_batteryCBtn_clicked()
{
    isBatteryCheck=ui->batteryCBtn->isChecked();
}

void AllSetting::on_cpuTempCBtn_clicked()
{
    isCpuTempCheck=ui->cpuTempCBtn->isChecked();
}

void AllSetting::closeEvent(QCloseEvent *e)
{
    if(ui->emailLineEdit->text()=="")
    {
        QMessageBox::StandardButton bt;
        bt = QMessageBox::question(this, tr("提示"),
                                       QString(tr("收件邮箱地址为空，是否继续？")),
                                       QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(bt==QMessageBox::No)
            e->ignore();
        else if(bt==QMessageBox::Yes)
        {
            //把邮箱地址写进mail.txt
            QString emailAdd=ui->emailLineEdit->text();
            WriteEmailToFile(emailAdd);

            emit SendData(ui->lineEdit->text(),ui->AndroidLlineEdit->text(),isMemCheck,isCpuCheck,isBatteryCheck,isCpuTempCheck,isWifiCheck,isMobileCheck, ui->memLineEdit->text(),ui->cpuLineEdit->text(),ui->batteryLineEdit->text(),ui->cpuTempLineEdit->text(),ui->wifiLineEdit->text(),ui->mobileLineEdit->text());
            e->accept();//程序退出，不创建也不新建
        }
    }
    else
    {
        //把邮箱地址写进mail.txt
        QString emailAdd=ui->emailLineEdit->text();
        WriteEmailToFile(emailAdd);

        emit SendData(ui->lineEdit->text(),ui->AndroidLlineEdit->text(),isMemCheck,isCpuCheck,isBatteryCheck,isCpuTempCheck,isWifiCheck,isMobileCheck, ui->memLineEdit->text(),ui->cpuLineEdit->text(),ui->batteryLineEdit->text(),ui->cpuTempLineEdit->text(),ui->wifiLineEdit->text(),ui->mobileLineEdit->text());
        e->accept();//程序退出，不创建也不新建
    }


}

void AllSetting::on_wifiCBtn_clicked()
{
    isWifiCheck=ui->wifiCBtn->isChecked();
}

void AllSetting::on_mobileCBtn_clicked()
{
    isMobileCheck=ui->mobileCBtn->isChecked();
}

void AllSetting::on_androidBtn_clicked()
{
    QString s= QFileDialog::getExistingDirectory(this,"选择路径");
    ui->AndroidLlineEdit->setText(s);
}

void AllSetting::WriteEmailToFile(QString emailAdds)
{
    QStringList emailList=emailAdds.split(";");
    QString s=gConfigDir+QDir::separator()+"email.txt";
    QFile f(s);
    f.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&f);
    for(int i=0;i<emailList.length();i++)
    {
        out<<emailList.at(i)<<endl;
    }
    f.close();
}

void AllSetting::ReadEmailAdd()
{
    //读入收件邮箱地址
    QString s=gConfigDir+QDir::separator()+"email.txt";
    QFile f(s);
    f.open(QIODevice::ReadOnly | QIODevice::Text);

    QString emailAdds="";
    QTextStream out(&f);
    while(!out.atEnd())
    {
        QString email=out.readLine();
        if(email!="")
            emailAdds+=email+";";
    }
    f.close();
    ui->emailLineEdit->setText(emailAdds);
}
