#include "replaysetup.h"
#include "ui_replaysetup.h"
#include "globalvar.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QToolTip>

ReplaySetup::ReplaySetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReplaySetup)
{
    ui->setupUi(this);

    this->setFixedWidth(400);
    ui->statCBox->setChecked(false);

    signalReplayData_s signalReplayData;
    QVariant var;
    var.setValue(signalReplayData);
    qRegisterMetaType<QVariant>("QVariant");

    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);

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

void ReplaySetup::initUI()
{
    ui->memCBtn->setChecked(gisMemCheck);
    ui->cpuCBtn->setChecked(gisCpuCheck);
    ui->batteryCBtn->setChecked(gisBatteryCheck);
    ui->cpuTempCBtn->setChecked(gisCpuTempCheck);
    ui->wifiCBtn->setChecked(gisWifiCheck);
    ui->mobileCBtn->setChecked(gisMobileCheck);

    ui->memLineEdit->setText(gMemThres);
    ui->cpuLineEdit->setText(gCpuThres);
    ui->batteryLineEdit->setText(gBatteryThres);
    ui->cpuTempLineEdit->setText(gCpuTempThres);
    ui->wifiLineEdit->setText(gWifiThres);
    ui->mobileLineEdit->setText(gMobileThres);

}



ReplaySetup::~ReplaySetup()
{
    delete ui;
}

void ReplaySetup::on_timesLineEdit_textChanged(const QString &arg1)
{
    textChange();
}

void ReplaySetup::on_intervalLineEdit_textChanged(const QString &arg1)
{
    textChange();
}

void ReplaySetup::textChange()
{
    gLoop=ui->timesLineEdit->text();
    gInterval=ui->intervalLineEdit->text();
}

void ReplaySetup::closeEvent(QCloseEvent *event)
{
    ParseXml::saveConfigXmlIP_Port(ui->IPLineEdit->text(),ui->portLineEdit->text());

    signalReplayData_s signalReplayData;

    signalReplayData.isMemCheck=ui->memCBtn->isChecked();
    signalReplayData.isCpuCheck=ui->cpuCBtn->isChecked();
    signalReplayData.isBatteryCheck=ui->batteryCBtn->isChecked();
    signalReplayData.isCpuTempCheck=ui->cpuTempCBtn->isChecked();
    signalReplayData.isWifiCheck=ui->wifiCBtn->isChecked();
    signalReplayData.isMobileCheck=ui->mobileCBtn->isChecked();
    signalReplayData.memThres=ui->memLineEdit->text();
    signalReplayData.cpuThres=ui->cpuLineEdit->text();
    signalReplayData.batteryThres=ui->batteryLineEdit->text();
    signalReplayData.cpuTempThres=ui->cpuTempLineEdit->text();
    signalReplayData.wifiThres=ui->wifiLineEdit->text();
    signalReplayData.mobileThres=ui->mobileLineEdit->text();
    signalReplayData.xmlPaths=xmlFilePaths;
    signalReplayData.isDebug=isDebug;
    QVariant var;
    var.setValue(signalReplayData);



    if(ui->timesLineEdit->text()==""||ui->intervalLineEdit->text()=="")
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("提示"),
                                       QString(tr("请填写运行次数及运行间隔！")),
                                       QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);

        if (button == QMessageBox::Yes)
        {
            event->ignore();  //忽略退出信号，程序继续运行，填写名称或路径
        }
        else if (button == QMessageBox::No)
        {
            emit sendData(var);
            event->accept();//程序退出，不创建也不新建
        }
    }
    else
    {
        emit sendData(var);
        event->accept();
    }
}

void ReplaySetup::on_memLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void ReplaySetup::on_cpuLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void ReplaySetup::on_batteryLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void ReplaySetup::on_cpuTempLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void ReplaySetup::on_lineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"邮箱地址用分号隔开");
}

void ReplaySetup::on_wifiLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void ReplaySetup::on_mobileLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void ReplaySetup::on_debugRBtn_clicked()
{
    isDebug=true;
}

void ReplaySetup::on_releaseRBtn_clicked()
{
    isDebug=false;
}

/*void ReplaySetup::on_xmlbrowseBtn_clicked()
{
    xmlPaths=QFileDialog::getOpenFileNames(this,tr("选择xml文件"),gWorkSpace,tr("xml files(*.xml)"));
    QString xmlNames="";
    for(int i=0;i<xmlPaths.length();i++)
    {
        qDebug()<<xmlPaths[i];
        QFileInfo fileinfo(xmlPaths[i]);
        xmlNames+=fileinfo.fileName()+";";
    }
    ui->xmlLineEdit->setText(xmlNames);
}
*/
void ReplaySetup::on_selectXmlBtn_clicked()
{
    xmlFilePaths.clear();
    gReplayNumber=0;
   // ui->xmlTreeWidget->header()->setMinimumSectionSize(400);

    filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),gWorkSpace,QFileDialog::ShowDirsOnly);
    ui->xmlTreeWidget->clear();
    if(filePath=="")
        return;

    QTreeWidgetItem *fileItem = new QTreeWidgetItem();
    StaticticsHerper::LoadFiles(filePath,ui->xmlTreeWidget,fileItem);
    QTreeWidgetItem *item=ui->xmlTreeWidget->itemAt(0,0);
    ui->xmlTreeWidget->setCurrentItem(item);
}

void ReplaySetup::on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->checkState(0) == Qt::Checked)
    {
        if(item->childCount()==0)
        {
            QString xmlFilePath=QDir::toNativeSeparators( filePath+"/"+StaticticsHerper::getBasePath(item));//!!!!!!!!!
            qDebug()<<xmlFilePath;
            xmlFilePaths.append(xmlFilePath);
            gReplayNumber++;
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
                   gReplayNumber--;
                   break;
                }
            }

        }
    }
}

void ReplaySetup::treeItemChanged(QTreeWidgetItem *item, int column)
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

void ReplaySetup::on_statCBox_clicked()
{
    gReplayNumber=0;
    if(ui->statCBox->isChecked())
    {
        this->setFixedWidth(784);
    }
    else
    {
        this->setFixedWidth(400);
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
