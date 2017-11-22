#include "getlogcatwidget.h"
#include "ui_getlogcatwidget.h"

GetLogcatWidget::GetLogcatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GetLogcatWidget)
{
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    setWindowTitle("抓取logcat");

    ui->startBtn->setStyleSheet("QPushButton{background:#c1904d;border: 0px;border-radius:3px;color:#ffffff}");
    ui->clearBtn->setStyleSheet("QPushButton{background:#c1904d;border: 0px;border-radius:3px;color:#ffffff}");
    ui->checkLogBtn->setStyleSheet("QPushButton{background:#c1904d;border: 0px;border-radius:3px;color:#ffffff}");
    ui->equipBtn->setStyleSheet("QPushButton{background:#c1904d;border: 0px;border-radius:3px;color:#ffffff}");
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                     "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->equipListView->setStyleSheet("QListView{font-size:12px;background:#ffffff;border:1px #ffffff;}"
                                     "QListView::item:selected:active{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                     "QListView::item:selected{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                     "QListView::item{height:30px;color:#4d4d4d}");


    ui->splitter->setStretchFactor(0,3);
    ui->splitter->setStretchFactor(1,7);

    equipModel = new QStringListModel(equipList);

    keyqWords.append(" ANR ");
    keyqWords.append("FATAL");

}

void GetLogcatWidget::RecieveDevicesSigal(QStringList devicesList)
{
    if(numForDeviceSignal<3)
        numForDeviceSignal++;
    QString currentDevice = ui->equipListView->currentIndex().data().toString();
    equipList.clear();
    equipList=devicesList;
    equipModel->setStringList(equipList);
    ui->equipListView->setModel(equipModel);

    int row=equipList.indexOf(currentDevice);
    if(row==-1)
    {
        deviceName="";
    }

    QModelIndex index=equipModel->index(row);
    if(equipList.contains(currentDevice))
    {
        ui->equipListView->setCurrentIndex(index);
    }
    if(numForDeviceSignal==1)
    {
        index=equipModel->index(0);
        ui->equipListView->setCurrentIndex(index);
        on_equipListView_clicked(index);
    }
}

GetLogcatWidget::~GetLogcatWidget()
{
    delete ui;
}

void GetLogcatWidget::on_startBtn_clicked()
{
    if(!isStartFlag)
    {
        errorFileLogcat=NULL;
        errorFlag=false;
        isStartFlag=true;
        ui->startBtn->setText("停止");
        qStartTime=Helper::getTime2("yyyyMMdd_hhmmss");
        fileNameForReport=gWorkSpace + QDir::separator() + "report"+QDir::separator()+ "Logcat_" + deviceName + "_" + qStartTime;
        Helper::createPath(fileNameForReport);
        QString fileName=fileNameForReport + QDir::separator()+"logcat_"+qStartTime+".txt";
        fileLogcat=new QFile(fileName);
        if ( !fileLogcat->exists())
            fileLogcat->open( QIODevice::WriteOnly );
        QString cmdStrLogcat="adb -s "+deviceName+" shell logcat -c\n";
        ExeCmd::runCmd(cmdStrLogcat);
        cmdStrLogcat="adb -s "+deviceName+" shell logcat -v time -b main -b system\n";
        qDebug()<<cmdStrLogcat;

        getLogcatWorker *worker=new getLogcatWorker;
        worker->cmdStr=cmdStrLogcat;
        QThread *workerThread=new QThread(this);
        connect(workerThread,SIGNAL(started()),worker,SLOT(startLogcat()));
        connect(worker,SIGNAL(logcatStop()),workerThread,SLOT(quit()));
        connect(workerThread,SIGNAL(finished()),worker,SLOT(deleteLater()));
        connect(worker,SIGNAL(logcatStop()),this,SLOT(RecieveStopSignal()));

        connect(worker,SIGNAL(readOutputFromWorker(QString)),this,SLOT(RecieveLogcatOutput(QString)));
        connect(this,SIGNAL(sendStopSignal()),worker,SLOT(recieveStopLogcatSignal()));

        worker->moveToThread(workerThread);
        workerThread->start();
    }
    else
    {
        emit sendStopSignal();
    }
}

void GetLogcatWidget::RecieveStopSignal()
{
    isStartFlag=false;
    ui->startBtn->setText("开始");
    ExeCmd::StopLogcat(deviceName);
    if(fileLogcat!=NULL)
    {
        fileLogcat->close();
    }
    if(errorFileLogcat!=NULL)
    {
        errorFileLogcat->close();
    }
    if(errorFlag)
        ui->textEdit->append("<font color=red>----------有异常出现---------</font>");
    else
        ui->textEdit->append("<font color=green>----------未见异常---------</font>");
}

void GetLogcatWidget::RecieveLogcatOutput(QString tempStr)
{
    if(fileLogcat->isOpen())
        fileLogcat->write(tempStr.toStdString().c_str());
    bool error=false;
    for(int i=0;i<keyqWords.length();i++)
    {
        error=error||tempStr.contains(keyqWords[i],Qt::CaseInsensitive);
    }
    errorFlag=errorFlag||error;
    if(error)
    {
        ui->textEdit->append("<font color=red>"+tempStr+"</font>");
        if(errorFileLogcat==NULL)
        {
            errorFileLogcat=new QFile(fileNameForReport + QDir::separator()+"errorlog_"+qStartTime+".txt");
            errorFileLogcat->open( QIODevice::WriteOnly | QIODevice::Text );
        }
        if(errorFileLogcat->isOpen())
        {
            errorFileLogcat->write(tempStr.toStdString().c_str());
            errorFileLogcat->write("\r\n\r\n");
        }
    }
    else
        ui->textEdit->append(tempStr);
}

void GetLogcatWidget::on_clearBtn_clicked()
{
    ui->textEdit->clear();
}

void GetLogcatWidget::on_checkLogBtn_clicked()
{
    ExeCmd::openFolder(fileNameForReport);
}

void GetLogcatWidget::on_equipListView_clicked(const QModelIndex &index)
{
    if((!ui->equipListView->currentIndex().data().toString().contains(deviceName))||deviceName=="")//如果没有换设备，不对当前界面操作
    {
        QModelIndex indexEquip=ui->equipListView->currentIndex();
        deviceName=ExeCmd::getDeviceSN(indexEquip.data().toString());
    }
}

void GetLogcatWidget::closeEvent(QCloseEvent *e)
{
    emit sendStopSignal();
}

void GetLogcatWidget::on_keyWordLineEdit_textChanged(const QString &arg1)
{
    QString keyWordsStr=ui->keyWordLineEdit->text();
    while(keyWordsStr.trimmed().endsWith(";"))
    {
        qDebug()<<keyWordsStr;
        keyWordsStr=keyWordsStr.left(keyWordsStr.length()-1);
    }
    keyqWords=keyWordsStr.split(";");
    qDebug()<<keyqWords.count();
}
