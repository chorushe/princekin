#include "princekin.h"
#include "ui_princekin.h"



Princekin::Princekin(QWidget *parent) :QMainWindow(parent),ui(new Ui::Princekin)
{
    ui->setupUi(this);
    this->setMouseTracking(true);

    centralWidget()->setMouseTracking(true);
    ui->widget->setMouseTracking(true);
    ui->widget_2->setMouseTracking(true);
    ui->widget_3->setMouseTracking(true);

    palWhite.setColor(QPalette::ButtonText, QColor(255,255,255));
    palBlack.setColor(QPalette::ButtonText, QColor(0,0,0));

    ui->pushButton_2->setPalette(palWhite);

    //*****************************************************************//

    settingDialog=new AllSetting;
    settingDialog->setAttribute(Qt::WA_QuitOnClose,false);
    settingDialog->setWindowFlags(Qt::Window);

    connect(settingDialog,SIGNAL(SendData(QString,QString, bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)),this,SLOT(RecieveData(QString,QString,bool,bool,bool,bool,bool,bool,QString,QString,QString,QString,QString,QString)));

    //***********20170713***************//
    QString ss=gConfigDir+QDir::separator()+"generalconfig.xml";
    ss=QDir::toNativeSeparators(ss);
    readConfigFile(ss);
    //***********20170713***************//

    qDebug()<<"workspace: "<<gWorkSpace;

    //*****************20170606*****************//
    QString ss1=gNewConfigDir + QDir::separator() + "js";
    QString ss2=gWorkSpace + QDir::separator() + "js";
    Helper::copyModify(ss1,ss2,false);
    //*****************20170606*****************//

    timer = new QTimer;
    timerThread=new QThread;
    //timer->setSingleShot(true);
    timer->start(3000);
    timer->moveToThread(timerThread);
    connect( timer, SIGNAL(timeout()),this, SLOT(TimerSlot()),Qt::DirectConnection );
    timerThread->start();
}

void Princekin::TimerSlot()
{
    QStringList mDevicesResult;
    mDevicesResult=ExeCmd::getDevices();
    if(mDevicesResult.length()==0)
    {
        isDeviceNull++;
    }
    else
        isDeviceNull=0;

    if(!((mDevicesResult.length()==1&&mDevicesResult.at(0)=="")||isDeviceNull==1))//如果在获取设备时，如果有error，会把list清空,同时加个标志项（区别于真正为空的情况），这时先不发送给所有模块hechang
        emit SendDevicesSignal(mDevicesResult);
}

Princekin::~Princekin()
{
    delete performanceDialog;
    delete monkeyDialog;
    delete behaviourDialog;
    delete ui;
    if(timerThread!=NULL)
        timerThread->quit();
}

void Princekin::mouseMoveEvent(QMouseEvent *e)
{
    if(isPress)
    {
        QPoint move_pos = e->globalPos();
        this->move(move_pos - move_point);
    }


    const int widget_x=ui->widget->pos().x();
    const int widget_y=ui->widget->pos().y();

    const int widget3_x=ui->widget_3->pos().x();
    const int widget3_y=ui->widget_3->pos().y();

    const int w=ui->pushButton_10->width();
    const int h=ui->pushButton_10->height();

    const int btnW=ui->pushButton->width();
    const int btnH=ui->pushButton->height();


    const int minBtn_x=ui->pushButton_11->pos().x();
    const int minBtn_y=ui->pushButton_11->pos().y();

    const int minBtn_minposx=widget3_x+minBtn_x;
    const int minBtn_minposy=widget3_y+minBtn_y;

    const int minBtn_maxposx=minBtn_minposx+w;
    const int minBtn_maxposy=minBtn_minposy+h;

    const int maxBtn_x=ui->pushButton_10->pos().x();
    const int maxBtn_y=ui->pushButton_10->pos().y();

    const int maxBtn_minposx=widget3_x+maxBtn_x;
    const int maxBtn_minposy=widget3_y+maxBtn_y;

    const int maxBtn_maxposx=maxBtn_minposx+w;
    const int maxBtn_maxposy=maxBtn_minposy+h;

    const int closeBtn_x=ui->pushButton_3->pos().x();
    const int closeBtn_y=ui->pushButton_3->pos().y();

    const int closeBtn_minposx=widget3_x+closeBtn_x;
    const int closeBtn_minposy=widget3_y+closeBtn_y;

    const int closeBtn_maxposx=closeBtn_minposx+w;
    const int closeBtn_maxposy=closeBtn_minposy+h;

    //
    const int btn1_x=ui->pushButton->pos().x();
    const int btn1_y=ui->pushButton->pos().y();

    const int btn1_minposx=widget_x+btn1_x;
    const int btn1_minposy=widget_y+btn1_y;

    const int btn1_maxposx=btn1_minposx+btnW;
    const int btn1_maxposy=btn1_minposy+btnH;

    //////
    const int btn2_x=ui->pushButton_2->pos().x();
    const int btn2_y=ui->pushButton_2->pos().y();

    const int btn2_minposx=widget_x+btn2_x;
    const int btn2_minposy=widget_y+btn2_y;

    const int btn2_maxposx=btn2_minposx+btnW;
    const int btn2_maxposy=btn2_minposy+btnH;
    //////
    //////
    const int btn3_x=ui->pushButton_4->pos().x();
    const int btn3_y=ui->pushButton_4->pos().y();

    const int btn3_minposx=widget_x+btn3_x;
    const int btn3_minposy=widget_y+btn3_y;

    const int btn3_maxposx=btn3_minposx+btnW;
    const int btn3_maxposy=btn3_minposy+btnH;
    ////////////
    const int btn4_x=ui->pushButton_5->pos().x();
    const int btn4_y=ui->pushButton_5->pos().y();

    const int btn4_minposx=widget_x+btn4_x;
    const int btn4_minposy=widget_y+btn4_y;

    const int btn4_maxposx=btn4_minposx+btnW;
    const int btn4_maxposy=btn4_minposy+btnH;
    ////////////
    const int btn5_x=ui->pushButton_6->pos().x();
    const int btn5_y=ui->pushButton_6->pos().y();

    const int btn5_minposx=widget_x+btn5_x;
    const int btn5_minposy=widget_y+btn5_y;

    const int btn5_maxposx=btn5_minposx+btnW;
    const int btn5_maxposy=btn5_minposy+btnH;
    ////////////
    const int btn6_x=ui->pushButton_7->pos().x();
    const int btn6_y=ui->pushButton_7->pos().y();

    const int btn6_minposx=widget_x+btn6_x;
    const int btn6_minposy=widget_y+btn6_y;

    const int btn6_maxposx=btn6_minposx+btnW;
    const int btn6_maxposy=btn6_minposy+btnH;
    ////////////
    const int btn7_x=ui->pushButton_8->pos().x();
    const int btn7_y=ui->pushButton_8->pos().y();

    const int btn7_minposx=widget_x+btn7_x;
    const int btn7_minposy=widget_y+btn7_y;

    const int btn7_maxposx=btn7_minposx+btnW;
    const int btn7_maxposy=btn7_minposy+btnH;
    ////////////
    const int btn8_x=ui->pushButton_9->pos().x();
    const int btn8_y=ui->pushButton_9->pos().y();

    const int btn8_minposx=widget_x+btn8_x;
    const int btn8_minposy=widget_y+btn8_y;

    const int btn8_maxposx=btn8_minposx+btnW;
    const int btn8_maxposy=btn8_minposy+btnH;
    //////


    QPoint temp = e->pos();

    if ( temp.x()>closeBtn_minposx && temp.x()<closeBtn_maxposx && temp.y()>closeBtn_minposy && temp.y()<closeBtn_maxposy )
    {
        ui->pushButton_3->setIcon(QIcon(":/close-hightlight.png"));
    }
    else
    {
        ui->pushButton_3->setIcon(QIcon(":/close-normal.png"));
    }


    if ( temp.x()>maxBtn_minposx && temp.x()<maxBtn_maxposx && temp.y()>maxBtn_minposy && temp.y()<maxBtn_maxposy )
    {
        ui->pushButton_10->setIcon(QIcon(":/narrow-highlight.png"));
    }
    else
    {
        ui->pushButton_10->setIcon(QIcon(":/narrow-normal.png"));
    }

    if ( temp.x()>minBtn_minposx && temp.x()<minBtn_maxposx && temp.y()>minBtn_minposy && temp.y()<minBtn_maxposy )
    {
        ui->pushButton_11->setIcon(QIcon(":/setup_hover.png"));
    }
    else
    {
        ui->pushButton_11->setIcon(QIcon(":/setup.png"));
    }


    if ( temp.x()>btn1_minposx && temp.x()<btn1_maxposx && temp.y()>btn1_minposy && temp.y()<btn1_maxposy )
    {
        ui->pushButton->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton->setPalette(palWhite);
    }
    else
    {
        ui->pushButton->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton->setPalette(palBlack);
    }

    if ( temp.x()>btn2_minposx && temp.x()<btn2_maxposx && temp.y()>btn2_minposy && temp.y()<btn2_maxposy )
    {
        ui->pushButton_2->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_2->setPalette(palWhite);
    }
    else
    {
        ui->pushButton_2->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton_2->setPalette(palBlack);
    }

    if ( temp.x()>btn3_minposx && temp.x()<btn3_maxposx && temp.y()>btn3_minposy && temp.y()<btn3_maxposy )
    {
        ui->pushButton_4->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_4->setPalette(palWhite);
    }
    else
    {
        ui->pushButton_4->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton_4->setPalette(palBlack);
    }
    if ( temp.x()>btn4_minposx && temp.x()<btn4_maxposx && temp.y()>btn4_minposy && temp.y()<btn4_maxposy )
    {
        ui->pushButton_5->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_5->setPalette(palWhite);
    }
    else
    {
        ui->pushButton_5->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton_5->setPalette(palBlack);
    }
    if ( temp.x()>btn5_minposx && temp.x()<btn5_maxposx && temp.y()>btn5_minposy && temp.y()<btn5_maxposy )
    {
        ui->pushButton_6->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_6->setPalette(palWhite);
    }
    else
    {
        ui->pushButton_6->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton_6->setPalette(palBlack);
    }


    if ( temp.x()>btn6_minposx && temp.x()<btn6_maxposx && temp.y()>btn6_minposy && temp.y()<btn6_maxposy )
    {
        ui->pushButton_7->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_7->setPalette(palWhite);
    }
    else
    {
        ui->pushButton_7->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton_7->setPalette(palBlack);
    }
    if ( temp.x()>btn7_minposx && temp.x()<btn7_maxposx && temp.y()>btn7_minposy && temp.y()<btn7_maxposy )
    {
        ui->pushButton_8->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_8->setPalette(palWhite);
    }
    else
    {
        ui->pushButton_8->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton_8->setPalette(palBlack);
    }

    if ( temp.x()>btn8_minposx && temp.x()<btn8_maxposx && temp.y()>btn8_minposy && temp.y()<btn8_maxposy )
    {
        ui->pushButton_9->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_9->setPalette(palWhite);
    }
    else
    {
        ui->pushButton_9->setStyleSheet("border-image: url(:/button20.png);");
        ui->pushButton_9->setPalette(palBlack);
    }
/**/

    if(temp.x()<btn1_minposx || temp.y()<btn1_minposy || temp.x()>btn8_maxposx || temp.y()>btn8_maxposy)
    {
        ui->pushButton_2->setStyleSheet("border-image: url(:/yellow.png);");
        ui->pushButton_2->setPalette(palWhite);
    }


}

void Princekin::mousePressEvent(QMouseEvent *e)
{
    isPress = true;
    move_point = e->pos();;

    const int widget3_x=ui->widget_3->pos().x();
    const int widget3_y=ui->widget_3->pos().y();

    const int w=ui->pushButton_10->width();
    const int h=ui->pushButton_10->height();

    const int minBtn_x=ui->pushButton_11->pos().x();
    const int minBtn_y=ui->pushButton_11->pos().y();

    const int minBtn_minposx=widget3_x+minBtn_x;
    const int minBtn_minposy=widget3_y+minBtn_y;

    const int minBtn_maxposx=minBtn_minposx+w;
    const int minBtn_maxposy=minBtn_minposy+h;

    QPoint temp = e->pos();
    if ( temp.x()>minBtn_minposx && temp.x()<minBtn_maxposx && temp.y()>minBtn_minposy && temp.y()<minBtn_maxposy )
    {
        //ui->pushButton_11->setIcon(QIcon(":/setup_hover.png"));
    }
    else
    {
       if(pulldownWidget!=NULL)
           pulldownWidget->hide();
    }
}
void Princekin::mouseReleaseEvent(QMouseEvent *e)
{
    isPress=false;
}


void Princekin::on_pushButton_clicked()
{
    if(recordPointer.isNull())
    {
        recordUi=new Record();
        recordPointer=recordUi;
        recordUi->setAttribute(Qt::WA_QuitOnClose,false);
        recordUi->setWindowFlags(Qt::Window);
        recordUi->showInit();
        recordPointer->show();


    }
    else
    {
        recordPointer->setWindowState(Qt::WindowActive);
        recordUi->showInit();
        recordPointer->show();
        recordPointer->raise();
        recordPointer->activateWindow();
    }
    connect(this,SIGNAL(SendDevicesSignal(QStringList)),recordUi,SLOT(RecieveDevicesSigal(QStringList)));
}

void Princekin::on_pushButton_4_clicked()
{
    /*if(performancePointer.isNull())
    {
        performanceDialog=new PerformanceWidget();
        performanceDialog->setAttribute(Qt::WA_QuitOnClose,false);
        performanceDialog->setWindowFlags(Qt::Window);
        performancePointer=performanceDialog;
        performancePointer->show();
    }
    else
    {
        performancePointer->show();
    }*/
    if(performanceDialog==NULL)
    {
        performanceDialog=new PerformanceWidget();
        performanceDialog->setAttribute(Qt::WA_QuitOnClose,false);
        performanceDialog->setWindowFlags(Qt::Window);
    }
    performanceDialog->setWindowState(Qt::WindowActive);

    performanceDialog->show();
    performanceDialog->raise();
    connect(this,SIGNAL(SendDevicesSignal(QStringList)),performanceDialog,SLOT(RecieveDevicesSigal(QStringList)));
    performanceDialog->showInit();

}

void Princekin::on_pushButton_5_clicked()
{
    /*if(monkeyPointer.isNull())
    {
        monkeyDialog=new MonkeyWidget();
        monkeyDialog->setAttribute(Qt::WA_QuitOnClose,false);
        monkeyDialog->setWindowFlags(Qt::Window);
        monkeyPointer=monkeyDialog;
        monkeyPointer->show();
    }
    else
    {
        monkeyPointer->show();
    } */
    if(monkeyDialog==NULL)
    {
        monkeyDialog=new MonkeyWidget();
        monkeyDialog->setAttribute(Qt::WA_QuitOnClose,false);
        monkeyDialog->setWindowFlags(Qt::Window);
    }

    monkeyDialog->setWindowState(Qt::WindowActive);
    monkeyDialog->showInit();
    monkeyDialog->show();
    monkeyDialog->raise();
    connect(this,SIGNAL(SendDevicesSignal(QStringList)),monkeyDialog,SLOT(RecieveDevicesSigal(QStringList)));
}

void Princekin::on_pushButton_3_clicked()
{
    close();
}

void Princekin::closeEvent(QCloseEvent *e)
{
    //返回之前保存下全局变量，主要是工作目录，因为在各个模块中都能改变gworkspace，所以在关闭时保存一下 hechang
    //saveConfigFile(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()+QDir::separator()+"config"+QDir::separator()+"generalconfig.xml"),gWorkSpace,gisMemCheck,gisCpuCheck,gisBatteryCheck,gisCpuTempCheck,gisWifiCheck,gisMobileCheck, gMemThres,gCpuThres,gBatteryThres,gCpuTempThres,gWifiThres,gMobileThres);//***********20170713

    //***********20170713***************//
    QString ss=gConfigDir+QDir::separator()+"generalconfig.xml";
    ss=QDir::toNativeSeparators(ss);
    saveConfigFile( gConfigDir+QDir::separator()+"generalconfig.xml",gWorkSpace,gisMemCheck,gisCpuCheck,gisBatteryCheck,gisCpuTempCheck,gisWifiCheck,gisMobileCheck, gMemThres,gCpuThres,gBatteryThres,gCpuTempThres,gWifiThres,gMobileThres);
    //***********20170713***************//

    QMessageBox::StandardButton resBtn= QMessageBox::information(this,"提示","关闭后所有模块将全部关闭！",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(resBtn==QMessageBox::Yes)
    {
        ExeCmd::runCmd("taskkill /F /IM adb.exe");//结束前杀死adb进程,否则无法修改路径 hechang
        e->accept();
    }
    else
        e->ignore();
}

void Princekin::on_pushButton_11_clicked()
{
    if(pulldownWidget==NULL)
    {
        pulldownWidget=new PulldownWidget;
        pulldownWidget->setAttribute(Qt::WA_QuitOnClose,false);
        connect(pulldownWidget,SIGNAL(setupSignal()),this,SLOT(setupBtn_clicked()));
        connect(pulldownWidget,SIGNAL(helpSignal()),this,SLOT(helpBtn_clicked()));
        connect(pulldownWidget,SIGNAL(aboutSignal()),this,SLOT(aboutBtn_clicked()));
    }
    pulldownWidget->show();
    pulldownWidget->raise();
    pulldownWidget->move(ui->pushButton_11->mapToGlobal(QPoint(0,0)).x()+ui->pushButton_11->width()-pulldownWidget->width(),ui->pushButton_11->mapToGlobal(QPoint(0,0)).y()+ui->pushButton_11->height()+5);
}

void Princekin::on_pushButton_6_clicked()
{
    if(behaviourDialog==NULL)
    {
        behaviourDialog=new BehaviourWidget();
        behaviourDialog->setAttribute(Qt::WA_QuitOnClose,false);
        behaviourDialog->setWindowFlags(Qt::Window);
    }

    behaviourDialog->setWindowState(Qt::WindowActive);
    behaviourDialog->showInit();
    behaviourDialog->showMaximized();
    behaviourDialog->raise();
    connect(this,SIGNAL(SendDevicesSignal(QStringList)),behaviourDialog,SLOT(RecieveDevicesSigal(QStringList)));

}

void Princekin::on_pushButton_2_clicked()
{

    if(replayPointer.isNull())
    {
        replayFormInstance=new ReplayForm();
        replayPointer=replayFormInstance;

        replayFormInstance->setAttribute(Qt::WA_QuitOnClose,false);
        replayFormInstance->setWindowFlags(Qt::Window);
        replayPointer=replayFormInstance;

        replayPointer->show();
    }
    else
    {
        replayPointer->setWindowState(Qt::WindowActive);
        replayPointer->show();
        replayPointer->raise();
        replayPointer->activateWindow();
    }



}

void Princekin::on_pushButton_7_clicked()
{
   /* if(temperatureDialog==NULL)
    {
        temperatureDialog=new TemperatureWidget();
        temperatureDialog->setAttribute(Qt::WA_QuitOnClose,false);
        temperatureDialog->setWindowFlags(Qt::Window);
    }

    temperatureDialog->setWindowState(Qt::WindowActive);
    temperatureDialog->showInit();
    temperatureDialog->show();
    temperatureDialog->raise();

    connect(this,SIGNAL(SendDevicesSignal(QStringList)),temperatureDialog,SLOT(RecieveDevicesSigal(QStringList)));*/
    if(autoTravelDialog==NULL)
    {
        autoTravelDialog=new AutoTravelWidget();
        autoTravelDialog->setAttribute(Qt::WA_QuitOnClose,false);
        autoTravelDialog->setWindowFlags(Qt::Window);
    }

    autoTravelDialog->setWindowState(Qt::WindowActive);
    autoTravelDialog->showInit();
    autoTravelDialog->show();
    autoTravelDialog->raise();

    connect(this,SIGNAL(SendDevicesSignal(QStringList)),autoTravelDialog,SLOT(RecieveDevicesSigal(QStringList)));
}


void Princekin::RecieveData(QString filePath,QString androidPath, bool isMemCheck,bool isCpuCheck,bool isBatteryCheck,bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
                            QString memThres,QString cpuThres,QString batteryThres,QString cpuTempThres, QString wifiThres,QString mobileThres)
{
    gAndroidJarDir = androidPath;

    //***********20170713***********//
    QString ss=gConfigDir+QDir::separator()+"config.xml";
    ss=QDir::toNativeSeparators(ss);
    saveAndroidPath(ss,androidPath);
    //***********20170713***********//

    gWorkSpace=QDir::toNativeSeparators(filePath);

    gReportDir=gWorkSpace + QDir::separator() + "report";
    gReportDir=QDir::toNativeSeparators(gReportDir);
    Helper::createPath(gReportDir);

    gisMemCheck = isMemCheck;
    gisCpuCheck = isCpuCheck;
    gisBatteryCheck = isBatteryCheck;
    gisCpuTempCheck = isCpuTempCheck;
    gisWifiCheck = isWifiCheck;
    gisMobileCheck = isMobileCheck;

    gMemThres=memThres;
    gCpuThres=cpuThres;
    gBatteryThres=batteryThres;
    gCpuTempThres=cpuTempThres;
    gWifiThres=wifiThres;
    gMobileThres=mobileThres;

    //**********************20170713*****************//
    gNewWorkSpaceDir=filePath;
    gNewWorkSpaceDir=QDir::toNativeSeparators(gNewWorkSpaceDir);

    QString strFile=gNewConfigDir+QDir::separator()+"workspace.txt";
    QFileInfo info(gNewWorkSpaceDir);
    if(info.isDir())
    {
        gWorkSpace=gNewWorkSpaceDir.trimmed();
        Helper::writeStr(strFile,gNewWorkSpaceDir);
    }
    else
    {
        gNewWorkSpaceDir=gNewDisk + QDir::separator() + "princekinWorkspace";
        gNewWorkSpaceDir=QDir::toNativeSeparators(gNewWorkSpaceDir);
        gWorkSpace=gNewWorkSpaceDir;
        Helper::writeStr(strFile,gNewWorkSpaceDir);
    }
    ss=gConfigDir+QDir::separator()+"generalconfig.xml";
    ss=QDir::toNativeSeparators(ss);
    saveConfigFile(ss,gWorkSpace,isMemCheck,isCpuCheck,isBatteryCheck,isCpuTempCheck,isWifiCheck,isMobileCheck, memThres,cpuThres,batteryThres,cpuTempThres,wifiThres,mobileThres);

    QString ss1=gNewConfigDir + QDir::separator() + "js";
    QString ss2=gWorkSpace + QDir::separator() + "js";
    Helper::copyModify(ss1,ss2,false);
    //***********20170713***********//
}

void Princekin::readConfigFile(QString fileName)
{
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(!file.exists())
        {
            QMessageBox::warning(this,"警告",fileName+" 文件不存在！");
            file.close();
            return;
        }
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,"警告",fileName+" 打开错误！");
            file.close();
            return;
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

        while(!node.isNull())
        {
            if(node.toElement().tagName()=="workspace")
            {
                //gWorkSpace=node.toElement().attributeNode("value").value();
            }
            if(node.toElement().tagName()=="memOption")
            {
                if(node.toElement().attributeNode("value").value()=="true")
                    gisMemCheck=true;
                else
                    gisMemCheck=false;
                gMemThres=node.toElement().attributeNode("thres").value();
            }
            if(node.toElement().tagName()=="cpuOption")
            {
                if(node.toElement().attributeNode("value").value()=="true")
                    gisCpuCheck=true;
                else
                    gisCpuCheck=false;
                gCpuThres=node.toElement().attributeNode("thres").value();
            }
            if(node.toElement().tagName()=="batteryOption")
            {
                if(node.toElement().attributeNode("value").value()=="true")
                    gisBatteryCheck=true;
                else
                    gisBatteryCheck=false;
                gBatteryThres=node.toElement().attributeNode("thres").value();
            }
            if(node.toElement().tagName()=="cpuTempOption")
            {
                if(node.toElement().attributeNode("value").value()=="true")
                    gisCpuTempCheck=true;
                else
                    gisCpuTempCheck=false;
                gCpuTempThres=node.toElement().attributeNode("thres").value();
            }
            if(node.toElement().tagName()=="wifiOption")
            {
                if(node.toElement().attributeNode("value").value()=="true")
                    gisWifiCheck=true;
                else
                    gisWifiCheck=false;
                gWifiThres=node.toElement().attributeNode("thres").value();
            }
            if(node.toElement().tagName()=="mobileOption")
            {
                if(node.toElement().attributeNode("value").value()=="true")
                    gisMobileCheck=true;
                else
                    gisMobileCheck=false;
                gMobileThres=node.toElement().attributeNode("thres").value();
            }

            node = node.nextSibling();//读取兄弟节点
        }
        qDebug()<<gWorkSpace;
    }
}

void Princekin::saveConfigFile(QString fileName,QString filePath, QVariant isMemCheck, QVariant isCpuCheck, QVariant isBatteryCheck, QVariant isCpuTempCheck,QVariant isWifiCheck,QVariant isMobileCheck,QString memThres,QString cpuThres,QString batteryThres,QString cpuTempThres,QString wifiThres,QString mobileThres)
{
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(!file.exists())
        {
            QMessageBox::warning(this,"警告",fileName+" 文件不存在！");
            file.close();
            return;
        }
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,"警告",fileName+" 打开错误！");
            file.close();
            return;
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
        QDomElement docElem = document.documentElement();
        QDomNodeList nodeList = docElem.elementsByTagName("workspace");
        if (nodeList.count() >0 )
        {
            QDomElement el = nodeList.at(0).toElement();
            el.setAttribute("value",filePath);  //设置属性值
        }
        nodeList = docElem.elementsByTagName("memOption");
        if (nodeList.count() >0 )
        {
            QDomElement el = nodeList.at(0).toElement();
            el.setAttribute("value",isMemCheck.toString());  //设置属性值
            el.setAttribute("thres",memThres);  //设置属性值
        }
        nodeList = docElem.elementsByTagName("cpuOption");
        if (nodeList.count() >0 )
        {
            QDomElement el = nodeList.at(0).toElement();
            el.setAttribute("value",isCpuCheck.toString());  //设置属性值
            el.setAttribute("thres",cpuThres);  //设置属性值
        }
        nodeList = docElem.elementsByTagName("batteryOption");
        if (nodeList.count() >0 )
        {
            QDomElement el = nodeList.at(0).toElement();
            el.setAttribute("value",isBatteryCheck.toString());  //设置属性值
            el.setAttribute("thres",batteryThres);  //设置属性值
        }
        nodeList = docElem.elementsByTagName("cpuTempOption");
        if (nodeList.count() >0 )
        {
            QDomElement el = nodeList.at(0).toElement();
            el.setAttribute("value",isCpuTempCheck.toString());  //设置属性值
            el.setAttribute("thres",cpuTempThres);  //设置属性值
        }
        nodeList = docElem.elementsByTagName("wifiOption");
        if (nodeList.count() >0 )
        {
            QDomElement el = nodeList.at(0).toElement();
            el.setAttribute("value",isWifiCheck.toString());  //设置属性值
            el.setAttribute("thres",wifiThres);  //设置属性值
        }
        nodeList = docElem.elementsByTagName("mobileOption");
        if (nodeList.count() >0 )
        {
            QDomElement el = nodeList.at(0).toElement();
            el.setAttribute("value",isMobileCheck.toString());  //设置属性值
            el.setAttribute("thres",mobileThres);  //设置属性值
        }
        QFile filexml(fileName);
        if(!filexml.exists())
        {
            QMessageBox::warning(this,"警告",fileName+" 文件不存在！");
            filexml.close();
            return;
        }
        if(!filexml.open(QFile::WriteOnly | QFile::Truncate))
        {
            QMessageBox::warning(this,"警告",fileName+" 打开错误！");
            filexml.close();
            return;
        }
        QTextStream ts(&filexml);
        ts.reset();
        ts.setCodec("utf-8");
        document.save(ts, 4, QDomNode::EncodingFromTextStream);
        filexml.close();
    }
}

void Princekin::saveAndroidPath(QString fileName,QString filePath)
{
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(!file.exists())
        {
            QMessageBox::warning(this,"警告",fileName+" 文件不存在！");
            file.close();
            return;
        }
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,"警告",fileName+" 打开错误！");
            file.close();
            return;
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
        QDomElement docElem = document.documentElement();
        QDomNodeList nodeList = docElem.elementsByTagName("androidJarDir");

        if (nodeList.count() >0 )
        {
            QDomNode node=nodeList.at(0);
            QDomNode oldnode = node.firstChild();     //标签之间的内容作为节点的子节点出现，得到原来的子节点
            node.firstChild().setNodeValue(filePath);   //用提供的value值来设置子节点的内容
            QDomNode newnode = node.firstChild();     //值修改过后
            node.replaceChild(newnode,oldnode);      //调用节点的replaceChild方法实现修改功能
        }

        QFile filexml(fileName);
        if(!filexml.exists())
        {
            QMessageBox::warning(this,"警告",fileName+" 文件不存在！");
            filexml.close();
            return;
        }
        if(!filexml.open(QFile::WriteOnly | QFile::Truncate))
        {
            QMessageBox::warning(this,"警告",fileName+" 打开错误！");
            filexml.close();
            return;
        }
        QTextStream ts(&filexml);
        ts.reset();
        ts.setCodec("utf-8");
        document.save(ts, 4, QDomNode::EncodingFromTextStream);
        filexml.close();
    }
}

void Princekin::on_pushButton_8_clicked()
{
    if(interfaceDialog==NULL)
    {
        interfaceDialog=new InterfaceWidget();
        interfaceDialog->setAttribute(Qt::WA_QuitOnClose,false);
        interfaceDialog->setWindowFlags(Qt::Window);
    }

    interfaceDialog->setWindowState(Qt::WindowActive);

    interfaceDialog->showMaximized();
    interfaceDialog->raise();
    interfaceDialog->showInit();

    connect(this,SIGNAL(SendDevicesSignal(QStringList)),interfaceDialog,SLOT(RecieveDevicesSigal(QStringList)));

}

void Princekin::on_pushButton_9_clicked()
{
    if(toolDialog==NULL)
    {
        toolDialog=new ToolBoxWidget();
        toolDialog->setAttribute(Qt::WA_QuitOnClose,false);
    }

    toolDialog->setWindowState(Qt::WindowActive);
    toolDialog->show();
    toolDialog->raise();

    connect(this,SIGNAL(SendDevicesSignal(QStringList)),toolDialog,SLOT(RecieveDevicesSigal(QStringList)));

}


void Princekin::on_pushButton_10_clicked()
{
    showMinimized();
}

void Princekin::setupBtn_clicked()
{
    settingDialog->setWindowState(Qt::WindowActive);
    settingDialog->setAttribute(Qt::WA_QuitOnClose,false);
    //settingDialog->setWindowModality(Qt::ApplicationModal);
    settingDialog->ReadEmailAdd();
    settingDialog->dataInit();
    settingDialog->show();
    settingDialog->raise();
}

void Princekin::helpBtn_clicked()
{
    qDebug()<<"help";
    QString cmdStr=gConfigDir+QDir::separator() +"helper.pdf\n";
    qDebug()<<cmdStr;
    QProcess *p=new QProcess(this);
    if(p->state())
        p->write(cmdStr.toLatin1());
    else
    {
        p->start("cmd", QStringList()<<"/c"<< cmdStr );
    }
}

void Princekin::aboutBtn_clicked()
{
    if(aboutWidget==NULL)
    {
        aboutWidget=new PrincekinAbout();
        aboutWidget->setAttribute(Qt::WA_QuitOnClose,false);
    }
    aboutWidget->show();
    aboutWidget->raise();
}
