#include "listwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

int ListWidget::num=0;

ListWidget::ListWidget(QString model, QWidget *parent) : QWidget(parent)
{
   // this->setFixedSize(200,20);
    this->setStyleSheet("QWidget{background:#ffffff;}");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    lab1 = new QLabel;
    QPushButton *btn1 = new QPushButton();
    btn1->setIcon(QIcon(":/close-normal.png"));
   // lab1->setFixedHeight(30);
   // btn1->setFixedHeight(30);
    lab1->setStyleSheet("QLabel{background:#ffffff;padding-left: 25px;}");
    btn1->setStyleSheet("QPushButton{background:#ffffff;border-style: outset;padding:8px;text-align :right ;padding-right: 15px;}");

    layout->addWidget(lab1);
    layout->addWidget(btn1);
    lab1->setText(tr("behavior.xlsx"));
    this->setLayout(layout);
    connect(btn1,SIGNAL(clicked(bool)),this,SLOT(Close()));
    this->model=model;
    if(model=="performance")//在性能测试时才加入数量的统计，不能超过7个
        num++;
}

QString ListWidget::GetLabelStr()
{
    return filePath;
}

void ListWidget::SetLabelStr(QString str, QString str1)
{
    lab1->setText(str);
    filePath=str1;
}

void ListWidget::Close()
{
    if(model=="performance")
        num--;

    filePath="";
    this->close();
}
