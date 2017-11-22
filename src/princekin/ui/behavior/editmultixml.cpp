#include "editmultixml.h"
#include "ui_editmultixml.h"


EditMultiXML::EditMultiXML(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditMultiXML)
{
    ui->setupUi(this);

    setWindowTitle("批量修改XML文件");
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    ui->editBtn->setStyleSheet("QPushButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                    "QPushButton::hover{background-color:#c1904d;color:#ffffff}");
    ui->detailBtn->setStyleSheet("QPushButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                    "QPushButton::hover{background-color:#c1904d;color:#ffffff}");
    ui->clearBtn->setStyleSheet("QPushButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                "QPushButton::hover{background-color:#c1904d;color:#ffffff}");
    ui->pushButton->setStyleSheet("QPushButton{background:#ffffff;border: 0px;border-radius:3px;}"
                                  "QPushButton::hover{background-color:#c1904d;color:#ffffff}");
   /* ui->scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                       "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->scrollArea->horizontalScrollBar()->setVisible(false);*/

    ui->xmlTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                          "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->xmlTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                                "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");

    ui->xmlTreeWidget->setHeaderHidden(true);
    connect(ui->xmlTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));

    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(4);//要在下边设置表头的前边，表头才能显示出来

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "参数名"<<"参数值"<<"描述"<<"验证规则");
    ui->tableWidget->verticalHeader()->setVisible(false); // 隐藏水平header
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//最后一列自适应宽度
    ui->tableWidget->setColumnWidth(0,70);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,120);

    ui->tableWidget->setItem(0,0,new QTableWidgetItem(""));//前两栏的也new个实例
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(""));//这里先把描述这一栏new一个实例，否则以后取值的时候会报错

    QComboBox *comBox = new QComboBox();
    comBox->addItem("不验证");
    comBox->addItem("验证非空");
    comBox->addItem("需要验证");
    comBox->addItem("唯一识别值");
    comBox->setStyleSheet("QComboBox QAbstractItemView::item{height:25px;}"
                          "QComboBox {border: 1px solid #e4e4e4;  border-radius: 3px;padding: 1px 2px 1px 2px;min-width: 9em;}"
                          "QComboBox::drop-down{ width: 40px; border-left-style: none;border-top-right-radius: 3px;  border-bottom-right-radius: 3px;}"
                          "QComboBox::down-arrow{image:url(:/arrow.png);}");
    comBox->setView(new QListView());

    ui->tableWidget->setCellWidget(0,3,comBox);
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(tableWidget_itemChanged(QTableWidgetItem*)));

}

EditMultiXML::~EditMultiXML()
{
    delete ui;
}

void EditMultiXML::on_editBtn_clicked()
{
    if(xmlFilePaths.count()==0)
    {
        QMessageBox::information(this,"提示","请选择XML文件");
        return;
    }

    QStringList editDataList = getEditDatas();

    if(editDataList.count()!=0)
    {
        QString resStr="";
        QString errorStr="";
        for(int i=0;i<xmlFilePaths.count();i++)
        {
            QString res = ParseXml::EditMultiXml(xmlFilePaths[i],editDataList);
            if(res.trimmed()!=""&&res.trimmed()!="EEROR")
            {
                QFileInfo f(xmlFilePaths[i]);
                resStr+=f.fileName()+"文件中未找到 \""+res+"\" 字段\n";
            }
            if(res=="ERROR")
            {
                errorStr=res;
            }
        }
        if(resStr==""&&errorStr=="")
            QMessageBox::information(this,"提示","修改成功！\n");
        else if(resStr!="")
        {
            QMessageBox::information(this,"提示",resStr);
        }

    }
}

QStringList EditMultiXML:: getEditDatas()
{
    QStringList lists;
    unique=false;
    //lists.append("cv=6.7.0=版本号=2");
    //lists.append("mtype=3=设备类型=1");
    if(ui->tableWidget->rowCount()<=1)
    {
        QMessageBox::information(this,"提示","请输入要修改的字段数据");
        return lists;
    }
    for(int i=0;i<ui->tableWidget->rowCount()-1;i++)
    {
        QWidget * widget=ui->tableWidget->cellWidget(i,3);//获得widget
        QComboBox *combox=(QComboBox*)widget;//强制转化为QComboBox
        int index=combox->currentIndex();
        if(index==3)
        {
            if(unique==false)
                unique=true;
            else
            {
                QMessageBox::information(this,"提示","唯一识别值只能有一个,请重新选择");
                lists.clear();
                return lists;
            }
        }
        lists.append(ui->tableWidget->item(i,0)->text()+"="+ui->tableWidget->item(i,1)->text()+"="+ui->tableWidget->item(i,2)->text()+"="+QString::number(index));
    }
    return lists;
}

void EditMultiXML::tableWidget_itemChanged(QTableWidgetItem *item)
{
    int count=ui->tableWidget->rowCount();
    if(ui->tableWidget->item(count-1,0)->text()!="")
    {
        disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(tableWidget_itemChanged(QTableWidgetItem*)));
        ui->tableWidget->setRowCount(count+1);
        ui->tableWidget->setItem(count,0,new QTableWidgetItem(""));
        ui->tableWidget->setItem(count,1,new QTableWidgetItem(""));
        ui->tableWidget->setItem(count,2,new QTableWidgetItem(""));
        QComboBox *comBox = new QComboBox();
        comBox->addItem("不验证");
        comBox->addItem("验证非空");
        comBox->addItem("需要验证");
        comBox->addItem("唯一识别值");
        comBox->setStyleSheet("QComboBox QAbstractItemView::item{height:25px;}"
                              "QComboBox {border: 1px solid #e4e4e4;  border-radius: 3px;padding: 1px 2px 1px 2px;min-width: 9em;}"
                              "QComboBox::drop-down{ width: 40px; border-left-style: none;border-top-right-radius: 3px;  border-bottom-right-radius: 3px;}"
                              "QComboBox::down-arrow{image:url(:/arrow.png);}");
        comBox->setView(new QListView());

        ui->tableWidget->setCellWidget(count,3,comBox);
        connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(tableWidget_itemChanged(QTableWidgetItem*)));
    }
    else if(item->column()==0 && ui->tableWidget->item(item->row(),0)->text()=="" && ui->tableWidget->rowCount()>1)
        ui->tableWidget->removeRow(item->row());
}

void EditMultiXML::on_clearBtn_clicked()
{
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(tableWidget_itemChanged(QTableWidgetItem*)));
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(""));//前两栏的也new个实例
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(""));//这里先把描述这一栏new一个实例，否则以后取值的时候会报错

    QComboBox *comBox = new QComboBox();
    comBox->addItem("不验证");
    comBox->addItem("验证非空");
    comBox->addItem("需要验证");
    comBox->addItem("唯一识别值");
    comBox->setStyleSheet("QComboBox QAbstractItemView::item{height:25px;}"
                          "QComboBox {border: 1px solid #e4e4e4;  border-radius: 3px;padding: 1px 2px 1px 2px;min-width: 9em;}"
                          "QComboBox::drop-down{ width: 40px; border-left-style: none;border-top-right-radius: 3px;  border-bottom-right-radius: 3px;}"
                          "QComboBox::down-arrow{image:url(:/arrow.png);}");
    comBox->setView(new QListView());

    ui->tableWidget->setCellWidget(0,3,comBox);
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(tableWidget_itemChanged(QTableWidgetItem*)));
}

void EditMultiXML::on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->checkState(0) == Qt::Checked)
    {
        if(item->childCount()==0)
        {
            QString xmlFilePath=filePath+"/"+ StaticticsHerper::getBasePath(item);//!!!!!!!!!
            qDebug()<<xmlFilePath;
            xmlFilePaths.append(xmlFilePath);
        }
    }
    else
    {
        if(item->childCount()==0)
        {
            for(int i=0;i<xmlFilePaths.length();i++)
            {
                if(xmlFilePaths[i]==filePath+"/"+ StaticticsHerper::getBasePath(item))
                {
                   xmlFilePaths.removeAt(i);
                   break;
                }
            }

        }
    }
}

void EditMultiXML::on_pushButton_clicked()
{
    xmlFilePaths.clear();

    filePath=QFileDialog::getExistingDirectory(this, tr("选择文件夹"),gWorkSpace,QFileDialog::ShowDirsOnly);
    if(filePath=="")
        return;
    ui->xmlTreeWidget->clear();

    QTreeWidgetItem *fileItem = new QTreeWidgetItem();
    StaticticsHerper::LoadFiles(filePath,ui->xmlTreeWidget,fileItem);
    QTreeWidgetItem *item=ui->xmlTreeWidget->itemAt(0,0);
    ui->xmlTreeWidget->setCurrentItem(item);
}

void EditMultiXML::treeItemChanged(QTreeWidgetItem *item, int column)
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

void EditMultiXML::on_detailBtn_clicked()
{
    ExeCmd::openFolder(filePath);
}
