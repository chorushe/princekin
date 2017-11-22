#include "pulldownwidget.h"
#include "ui_pulldownwidget.h"

PulldownWidget::PulldownWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PulldownWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);


    ui->setupBtn->setStyleSheet("QPushButton{background:#ffffff;color:#000000;border: 0px;padding:6px;}"
                                "QPushButton::hover{background:#fffcff;color:#c1904d}");
    ui->helpBtn->setStyleSheet("QPushButton{background:#ffffff;color:#000000;border: 0px;padding:6px;}"
                               "QPushButton::hover{background:#fffcff;color:#c1904d}");
    ui->aboutBtn->setStyleSheet("QPushButton{background:#ffffff;color:#000000;border: 0px;padding:6px;}"
                                "QPushButton::hover{background:#fffcff;color:#c1904d}");
}

PulldownWidget::~PulldownWidget()
{
    delete ui;
}

void PulldownWidget::on_setupBtn_clicked()
{
    this->hide();
    emit setupSignal();
}

void PulldownWidget::on_helpBtn_clicked()
{
    this->hide();
    emit helpSignal();
}

void PulldownWidget::on_aboutBtn_clicked()
{
    this->hide();
    emit aboutSignal();
}
