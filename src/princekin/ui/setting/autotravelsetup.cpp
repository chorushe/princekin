#include "autotravelsetup.h"
#include "ui_autotravelsetup.h"

AutoTravelSetup::AutoTravelSetup(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoTravelSetup)
{
    ui->setupUi(this);
    this->setFixedWidth(410);
    ui->statCBox->setChecked(false);

    signalAutotravelData_s signalAutotravelData;
    QVariant var;
    var.setValue(signalAutotravelData);
    qRegisterMetaType<QVariant>("QVariant");

    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    QStringList tempList=ParseXml::readConfigXmlForIP_Port(gConfigDir + QDir::separator() + "config.xml");
    ui->IPLineEdit->setText(tempList[0]);
    ui->portLineEdit->setText(tempList[1]);

    ui->xmlTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                          "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");

    ui->xmlTreeWidget->setHeaderHidden(true);
    ui->xmlTreeWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                            "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");

    connect(ui->xmlTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));

}

AutoTravelSetup::~AutoTravelSetup()
{
    delete ui;
}

/*void AutoTravelSetup::on_browseBtn_clicked()
{
    QString s= QFileDialog::getExistingDirectory(this,"选择路径",ui->pathLineEdit->text());
    ui->pathLineEdit->setText(s);
}*/

void AutoTravelSetup::closeEvent(QCloseEvent *)
{
   /* if(ui->pathLineEdit->text()=="")
    {
        QMessageBox::information(this,"提示","工作路径为空，默认为当前运行目录!");
        QString s=QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
        ui->pathLineEdit->setText(s);
    }*/

    ParseXml::saveConfigXmlIP_Port(ui->IPLineEdit->text(),ui->portLineEdit->text());

    signalAutotravelData_s signalAutotravelData;
    signalAutotravelData.logKeyWord=ui->logKeyWordLineEdit->text();
   // signalAutotravelData.filePath=ui->pathLineEdit->text();
    signalAutotravelData.isMemCheck=ui->memCBtn->isChecked();
    signalAutotravelData.isCpuCheck=ui->cpuCBtn->isChecked();
    signalAutotravelData.isBatteryCheck=ui->batteryCBtn->isChecked();
    signalAutotravelData.isCpuTempCheck=ui->cpuTempCBtn->isChecked();
    signalAutotravelData.isWifiCheck=ui->wifiCBtn->isChecked();
    signalAutotravelData.isMobileCheck=ui->mobileCBtn->isChecked();
    signalAutotravelData.memThres=ui->memLineEdit->text();
    signalAutotravelData.cpuThres=ui->cpuLineEdit->text();
    signalAutotravelData.batteryThres=ui->batteryLineEdit->text();
    signalAutotravelData.cpuTempThres=ui->cpuTempLineEdit->text();
    signalAutotravelData.wifiThres=ui->wifiLineEdit->text();
    signalAutotravelData.mobileThres=ui->mobileLineEdit->text();
    signalAutotravelData.xmlPaths=xmlFilePaths;
    signalAutotravelData.isDebug=isDebug;
    signalAutotravelData.clickInternalTime=ui->timeLineEdit->text().trimmed();
    QVariant var;
    var.setValue(signalAutotravelData);

    emit SendData(var);
}

void AutoTravelSetup::recieveSettingSig( bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck, bool isMobileCheck, QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres)
{
  //  ui->pathLineEdit->setText(workSpace);

    ui->memCBtn->setChecked(isMemCheck);
    ui->cpuCBtn->setChecked(isCpuCheck);
    ui->batteryCBtn->setChecked(isBatteryCheck);
    ui->cpuTempCBtn->setChecked(isCpuTempCheck);
    ui->wifiCBtn->setChecked(isWifiCheck);
    ui->mobileCBtn->setChecked(isMobileCheck);


    ui->memLineEdit->setText(memThres);
    ui->cpuLineEdit->setText(cpuThres);
    ui->batteryLineEdit->setText(batteryThres);
    ui->cpuTempLineEdit->setText(cpuTempThres);
    ui->wifiLineEdit->setText(wifiThres);
    ui->mobileLineEdit->setText(mobileThres);
}

void AutoTravelSetup::stateChanged(bool state)
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

void AutoTravelSetup::on_memLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void AutoTravelSetup::on_cpuLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void AutoTravelSetup::on_batteryLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void AutoTravelSetup::on_cpuTempLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void AutoTravelSetup::on_wifiLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void AutoTravelSetup::on_mobileLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void AutoTravelSetup::on_logKeyWordLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"日志关键字用分号（;）分开");
}

void AutoTravelSetup::on_debugRBtn_clicked()
{
    isDebug=true;
}

void AutoTravelSetup::on_releaseRBtn_clicked()
{
    isDebug=false;
}

void AutoTravelSetup::on_selectXmlBtn_clicked()
{
    xmlFilePaths.clear();
    gTravelNumber=0;
   // ui->xmlTreeWidget->header()->setMinimumSectionSize(400);

    filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),gWorkSpace,QFileDialog::ShowDirsOnly);
    if(filePath=="")
        return;
    ui->xmlTreeWidget->clear();

    QTreeWidgetItem *fileItem = new QTreeWidgetItem();
    StaticticsHerper::LoadFiles(filePath,ui->xmlTreeWidget,fileItem);
    QTreeWidgetItem *item=ui->xmlTreeWidget->itemAt(0,0);
    ui->xmlTreeWidget->setCurrentItem(item);
}

void AutoTravelSetup::treeItemChanged(QTreeWidgetItem *item, int column)
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

void AutoTravelSetup::on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->checkState(0) == Qt::Checked)
    {
        if(item->childCount()==0)
        {
            QString xmlFilePath=QDir::toNativeSeparators( filePath+"/"+StaticticsHerper::getBasePath(item));//!!!!!!!!!
            qDebug()<<xmlFilePath;
            xmlFilePaths.append(xmlFilePath);
            gTravelNumber++;
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
                    gTravelNumber--;
                    break;
                }
            }

        }
    }
}

void AutoTravelSetup::on_statCBox_clicked()
{
    gTravelNumber=0;
    if(ui->statCBox->isChecked())
    {
        this->setFixedWidth(787);
    }
    else
    {
        this->setFixedWidth(410);
        //取消埋点统计测试，把xmltree选中的勾都去掉
        QTreeWidgetItemIterator item(ui->xmlTreeWidget);
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
