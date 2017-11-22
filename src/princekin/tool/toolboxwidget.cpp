#include "toolboxwidget.h"
#include "ui_toolboxwidget.h"

ToolBoxWidget::ToolBoxWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBoxWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("工具箱");

    ui->closeBtn->setStyleSheet("QPushButton{border-image:url(:/close-normal.png);}"
                                "QPushButton::hover{border-image:url(:/close-hightlight.png);}");

    ui->minBtn->setStyleSheet("QPushButton{border-image:url(:/narrow-normal.png);}"
                              "QPushButton::hover{border-image:url(:/narrow-highlight.png);}");

    ui->logcatBtn->setStyleSheet("QPushButton{background:#e5e5e5;border: 0px;border-radius:3px;}"
                                 "QPushButton::hover{background-color:#c1904d;color:#ffffff}");

    ui->apkBtn->setStyleSheet("QPushButton{background:#e5e5e5;border: 0px;border-radius:3px;}"
                              "QPushButton::hover{background-color:#c1904d;color:#ffffff}");
}

ToolBoxWidget::~ToolBoxWidget()
{
    delete ui;
}

void ToolBoxWidget::on_apkBtn_clicked()
{
    if(apkinfo==NULL)
    {
        apkinfo=new ApkInfoForm();
        apkinfo->setAttribute(Qt::WA_QuitOnClose,false);
    }

    apkinfo->setWindowState(Qt::WindowActive);
    apkinfo->show();
    apkinfo->raise();
    emit refresh();
    connect(this,SIGNAL(refresh()),apkinfo,SLOT(refreshApk()));
}

void ToolBoxWidget::on_closeBtn_clicked()
{
    close();
}

void ToolBoxWidget::on_minBtn_clicked()
{
    showMinimized();
}

void ToolBoxWidget::on_logcatBtn_clicked()
{
    if(logcatDialog==NULL)
    {
        logcatDialog=new GetLogcatWidget();
        logcatDialog->setAttribute(Qt::WA_QuitOnClose,false);
    }

    logcatDialog->setWindowState(Qt::WindowActive);
    logcatDialog->show();
    logcatDialog->raise();
    connect(this,SIGNAL(SendDevicesSignal(QStringList)),logcatDialog,SLOT(RecieveDevicesSigal(QStringList)));
}

void ToolBoxWidget::RecieveDevicesSigal(QStringList deviceList)
{
    emit SendDevicesSignal(deviceList);
}

void ToolBoxWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void ToolBoxWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
