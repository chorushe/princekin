#include "performancesetup.h"
#include "ui_performancesetup.h"
#include <QProcess>
#include <QMessageBox>
#include <QToolTip>

PerformanceSetup::PerformanceSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PerformanceSetup)
{
    ui->setupUi(this);
    this->setFixedWidth(250);
    ui->statCBox->setChecked(false);

    ui->xmlTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                          "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->xmlTreeWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                            "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");

    ui->xmlTreeWidget->setHeaderHidden(true);

    connect(ui->xmlTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
    signalData_s signalData;
    QVariant DataVar;
    DataVar.setValue(signalData);
    qRegisterMetaType<QVariant>("QVariant"); //写在构造函数里

    //赋初值
    time=2000;
    workspace=gWorkSpace;
    //ui->pathLineEdit->setText(workspace);

    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    //ui->xmlLineEdit->setFocusPolicy(Qt::NoFocus);//编辑框设置为不可编辑状态

    QStringList tempList=ParseXml::readConfigXmlForIP_Port(gConfigDir + QDir::separator() + "config.xml");
    ui->IPLineEdit->setText(tempList[0]);
    ui->portLineEdit->setText(tempList[1]);
}

PerformanceSetup::~PerformanceSetup()
{
    delete ui;
}

void PerformanceSetup::closeEvent(QCloseEvent *event)
{
    ParseXml::saveConfigXmlIP_Port(ui->IPLineEdit->text(),ui->portLineEdit->text());
    signalData_s signalData;
    signalData.time=time;
    signalData.memThres=ui->memLineEdit->text();
    signalData.cpuThres=ui->cpuLineEdit->text();
    signalData.batteryThres=ui->batteryLineEdit->text();
    signalData.cpuTempThres=ui->cpuTempLineEdit->text();
    signalData.wifiThres=ui->wifiLineEdit->text();
    signalData.mobileThres=ui->mobileLineEdit->text();
    signalData.xmlPaths=xmlFilePaths;
    signalData.isDebug=isDebug;
    QVariant var;
    var.setValue(signalData);
    emit SendData(var);
}

void PerformanceSetup::on_timeLineEdit_textChanged(const QString &arg1)
{
    if(ui->timeLineEdit->text()=="")
        time=2000;
    else
    {
        QString pattern("^(0|[1-9][0-9]*)$");
        QRegExp rx(pattern);
        bool match = rx.exactMatch(ui->timeLineEdit->text());
        if(match)
            time=ui->timeLineEdit->text().toInt()*1000;
        else
        {
            QMessageBox::information(this,"提示","显示间隔输入错误，请重新输入！");
            time=2000;
        }
    }
}

void PerformanceSetup::on_batteryLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void PerformanceSetup::on_cpuTempLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void PerformanceSetup::on_wifiLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void PerformanceSetup::on_mobileLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void PerformanceSetup::on_memLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void PerformanceSetup::on_cpuLineEdit_textChanged(const QString &arg1)
{
    QToolTip::showText(cursor().pos(),"此次修改只在当前模块运行周期内生效");
}

void PerformanceSetup::recieveSettingSig(QString memThres,QString cpuThres, QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres)
{
    ui->memLineEdit->setText(memThres);
    ui->cpuLineEdit->setText(cpuThres);
    ui->batteryLineEdit->setText(batteryThres);
    ui->cpuTempLineEdit->setText(cpuTempThres);
    ui->wifiLineEdit->setText(wifiThres);
    ui->mobileLineEdit->setText(mobileThres);
    this->memThres=batteryThres;
    this->cpuThres=cpuTempThres;
    this->batteryThres=batteryThres;
    this->cpuTempThres=cpuTempThres;
    this->wifiThres=wifiThres;
    this->mobileThres=mobileThres;
}


/*void PerformanceSetup::on_xmlbrowseBtn_clicked()
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
}*/

void PerformanceSetup::on_debugRBtn_clicked()
{
    isDebug=true;
}

void PerformanceSetup::on_releaseRBtn_clicked()
{
    isDebug=false;
}

void PerformanceSetup::on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->checkState(0) == Qt::Checked)
    {
        if(item->childCount()==0)
        {
            QString xmlFilePath=QDir::toNativeSeparators( filePath+"/"+StaticticsHerper::getBasePath(item));//!!!!!!!!!
            qDebug()<<xmlFilePath;
            xmlFilePaths.append(xmlFilePath);
            gPerformanceNumber++;
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
                    gPerformanceNumber--;
                    break;
                }
            }

        }
    }
}

void PerformanceSetup::on_selectXmlBtn_clicked()
{
   // ui->xmlTreeWidget->header()->setMinimumSectionSize(400);
    xmlFilePaths.clear();
    gPerformanceNumber=0;

    filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),gWorkSpace,QFileDialog::ShowDirsOnly);
    if(filePath=="")
        return;
    ui->xmlTreeWidget->clear();

    QTreeWidgetItem *fileItem = new QTreeWidgetItem();
    StaticticsHerper::LoadFiles(filePath,ui->xmlTreeWidget,fileItem);
    QTreeWidgetItem *item=ui->xmlTreeWidget->itemAt(0,0);
    ui->xmlTreeWidget->setCurrentItem(item);
}

void PerformanceSetup::treeItemChanged(QTreeWidgetItem *item, int column)
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


void PerformanceSetup::on_statCBox_clicked()
{
    gPerformanceNumber=0;
    if(ui->statCBox->isChecked())
    {
        this->setFixedWidth(642);
    }
    else
    {
        this->setFixedWidth(250);
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
