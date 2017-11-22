#include "apkinfoform.h"
#include "ui_apkinfoform.h"

ApkInfoForm::ApkInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApkInfoForm)
{
    ui->setupUi(this);

    showList();
}

void ApkInfoForm::showList()
{
    QString apkDir;
    QStringList filter;
    QStringList fileList;

    apkDir=gConfigDir + QDir::separator() + "apk";
    filter<<"*.apk";

    QDir dir(apkDir);
    fileList=dir.entryList(filter,QDir::Files);
    ui->listWidget->addItems(fileList);
}

ApkInfoForm::~ApkInfoForm()
{
    delete ui;
}

void ApkInfoForm::on_pushButton_clicked()
{
    bool isS=isSelected();
    if(isS)
    {
        QString apkName;
        QString filePath;
        QString packageName;
        QString activityName;
        QString cmdLine;

        apkName=ui->listWidget->selectedItems().at(0)->text();
        filePath=gConfigDir + QDir::separator() + "apk" + QDir::separator() + apkName;

        cmdLine="cmd /c aapt dump badging " + filePath + " | findstr package:";
        packageName=ExeCmd::runCmd_getPackageActivity(cmdLine,"package:");

        cmdLine="cmd /c aapt dump badging " + filePath + " | findstr launchable-activity:";
        activityName=ExeCmd::runCmd_getPackageActivity(cmdLine,"launchable-activity:");

        filePath=gConfigDir + QDir::separator() + "install-apk-name.txt";
        Helper::writeStr(filePath,apkName);

        filePath=gConfigDir + QDir::separator() + "packageName.txt";
        Helper::writeStr(filePath,packageName);

        filePath=gConfigDir + QDir::separator() + "lunchActivity.txt";
        Helper::writeStr(filePath,packageName + "/" + activityName);

        ui->lineEdit->setText(packageName + "/" + activityName);
        QMessageBox::information(NULL,"提示", "成功");
    }
}


bool ApkInfoForm::isSelected()
{
    bool isS=true;
    if(ui->listWidget->selectedItems().count()==0)
    {
        QMessageBox::information(NULL,"信息","请选择一个apk文件");
        isS=false;
    }
    return isS;
}

void ApkInfoForm::refreshApk()
{
    ui->listWidget->clear();
    showList();
}

void ApkInfoForm::on_pushButton_2_clicked()
{
    QStringList list=QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    QString str;
    if(list.count()==0)
    {
        str="c:\\";
    }
    else
    {
        str=list.at(0);
    }
    QString directory=QFileDialog::getOpenFileName(this,("选择"),str);
    if(directory.isEmpty())
    {
    }
    else
    {
        int len;
        QString apkName;
        QString baseName;
        QFileInfo info(directory);

        apkName=info.fileName();
        len=apkName.length();
        baseName=apkName.left(len-4);
        baseName=baseName.replace(".","_");
        apkName=baseName+".apk";

        QString apkdir=gConfigDir + QDir::separator() + "apk" + QDir::separator() + apkName;
        info.setFile(apkdir);
        if(info.exists())
        {
            QFile file(apkdir);
            file.remove();
        }
        int n=ui->listWidget->findItems(apkName,Qt::MatchExactly).count();
        if(n==0)
        {
            ui->listWidget->addItem(apkName);
        }
        QFile::copy(directory, gConfigDir + QDir::separator() + "apk" + QDir::separator() + apkName);
    }
}

void ApkInfoForm::on_pushButton_3_clicked()
{
    bool isS=isSelected();
    if(isS)
    {
        QString apkName;
        QString filePath;

        apkName=ui->listWidget->selectedItems().at(0)->text();
        filePath=gConfigDir + QDir::separator() + "apk" + QDir::separator() + apkName;
        QFile file(filePath);
        file.remove();
        refreshApk();
    }
}
