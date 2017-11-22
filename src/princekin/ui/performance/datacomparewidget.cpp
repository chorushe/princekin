#include "datacomparewidget.h"
#include "ui_datacomparewidget.h"

DataCompareWidget::DataCompareWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataCompareWidget)
{
    ui->setupUi(this);
//    this->setWindowFlags(Qt::FramelessWindowHint| Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::FramelessWindowHint);

    installEventFilter(this);
}

DataCompareWidget::~DataCompareWidget()
{
    delete ui;
}

bool DataCompareWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(QEvent::WindowDeactivate==event->type())
    {
        qDebug()<<"hide";
        emit WindowDeactiveSignal();
       // hide();
    }
    else
        return QWidget::eventFilter(watched,event);
}

void DataCompareWidget::on_chooseFileBtn_clicked()
{
    if(ListWidget::num>=7)
    {
        this->removeEventFilter(this);
        QMessageBox::information(this,"","最多选则七个比对文件！");
        this->installEventFilter(this);
        return;
    }
    gExcelFilePath.clear();

    this->removeEventFilter(this);//弹出选择文件对话框的时候，该窗体的焦点失去，会导致对比窗口缩回，所以在弹框之前先去掉时间过滤器，等弹窗之后再安装过滤器
    QString filePath=QFileDialog::getOpenFileName(this, tr("选择文件"),gWorkSpace,tr("xlsxfile(*.xlsx);;Allfile(*.*)"));
    if(filePath.trimmed()=="")
        return;
    this->installEventFilter(this);
    QFileInfo fileinfo;
    fileinfo = QFileInfo(filePath);
    //文件名
    QString fileName = fileinfo.fileName();
    ListWidget *widget=new ListWidget("performance");
    widget->SetLabelStr(fileName,filePath);
    ui->verticalLayout_2->insertWidget(0,widget);
}

void DataCompareWidget::on_compareBtn_clicked()
{
    gExcelFilePath.clear();
    for(int i=0;i<ui->verticalLayout_2->count()-2;i++)
    {
        ListWidget *p=(ListWidget* )ui->verticalLayout_2->itemAt(i)->widget();
        QString excelStr=p->GetLabelStr();
        if(excelStr!="")
            gExcelFilePath.append(excelStr);
        //QMessageBox::information(this,"",p->GetLabelStr());
    }
    for(int i=0;i<gExcelFilePath.count();i++)
        qDebug()<<gExcelFilePath.at(i);
    if(gExcelFilePath.count()==0)
    {
        this->removeEventFilter(this);//弹出选择文件对话框的时候，该窗体的焦点失去，会导致对比窗口缩回，所以在弹框之前先去掉时间过滤器，等弹窗之后再安装过滤器
        QMessageBox::information(this,"提示","请选择Excel文件");
        this->installEventFilter(this);
    }

    //*****************20170315*****************//
    getExcelData();
    //*****************20170315*****************//

    emit SendCompareData(qMemory_data_Vector,qMemory_time_Vector,qCpu_data_Vector,qCpu_time_Vector,qCputemp_data_Vector,qCputemp_time_Vector,qBatterytemp_data_Vector,qBatterytemp_time_Vector,qWifi_data_Vector,qWifi_time_Vector,qMobile_data_Vector,qMobile_time_Vector);
}

void DataCompareWidget::on_clearBtn_clicked()
{
    for(int i=0;i<ui->verticalLayout_2->count()-2;i++)
    {
        ListWidget *p=(ListWidget* )ui->verticalLayout_2->itemAt(i)->widget();
        if(p->filePath!="")
            p->Close();
    }
}

//*****************20170315*****************//
void DataCompareWidget::getExcelData()
{
    QString filePath;
    QVector<QString> dataVector;

   /* qMemory_time_Map.clear();
    qCpu_time_Map.clear();
    qCputemp_time_Map.clear();
    qBatterytemp_time_Map.clear();
    qWifi_time_Map.clear();
    qMobile_time_Map.clear();


    qMemory_warning_Map.clear();
    qCpu_warning_Map.clear();
    qCputemp_warning_Map.clear();
    qBatterytemp_warning_Map.clear();
    qWifi_warning_Map.clear();
    qMobile_warning_Map.clear();

    qMemory_data_Map.clear();
    qCpu_data_Map.clear();
    qCputemp_data_Map.clear();
    qBatterytemp_data_Map.clear();
    
    qBatterytemp_data_Map.clear();
    qWifi_data_Map.clear();
    qMobile_data_Map.clear();*/

    qMemory_time_Vector.clear();
    qCpu_time_Vector.clear();
    qCputemp_time_Vector.clear();
    qBatterytemp_time_Vector.clear();

    qMemory_data_Vector.clear();
    qCpu_data_Vector.clear();
    qCputemp_data_Vector.clear();
    qBatterytemp_data_Vector.clear();

    for(int i=0;i<gExcelFilePath.count();i++)
    {
        filePath=gExcelFilePath.at(i);
        ReadSource::readData(filePath);

        //**********************************************//
        dataVector=ReadSource::get_memory_time();
        qMemory_time_Vector.append(dataVector);

        dataVector=ReadSource::get_cpu_time();
        qCpu_time_Vector.append(dataVector);

        dataVector=ReadSource::get_cputemp_time();
        qCputemp_time_Vector.append(dataVector);

        dataVector=ReadSource::get_batterytemp_time();
        qBatterytemp_time_Vector.append(dataVector);

        dataVector=ReadSource::get_wifi_time();
        qWifi_time_Vector.append(dataVector);

        dataVector=ReadSource::get_mobile_time();
        qMobile_time_Vector.append(dataVector);

        //**********************************************//
        dataVector=ReadSource::get_memory_data();
        qMemory_data_Vector.append(dataVector);

        dataVector=ReadSource::get_cpu_data();
        qCpu_data_Vector.append(dataVector);

        dataVector=ReadSource::get_cputemp_data();
        qCputemp_data_Vector.append(dataVector);

        dataVector=ReadSource::get_batterytemp_data();
        qBatterytemp_data_Vector.append(dataVector);

        dataVector=ReadSource::get_wifi_data();
        qWifi_data_Vector.append(dataVector);

        dataVector=ReadSource::get_mobile_data();
        qMobile_data_Vector.append(dataVector);
    }
}
//*****************20170315*****************//
