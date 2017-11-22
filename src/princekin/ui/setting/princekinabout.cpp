#include "princekinabout.h"
#include "ui_princekinabout.h"

PrincekinAbout::PrincekinAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrincekinAbout)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    setWindowTitle("关于");

    ui->pushButton->setStyleSheet("QPushButton{border-image:url(:/close-normal.png);}"
                                  "QPushButton:hover{border-image:url(:/close-hightlight.png);}");
}

PrincekinAbout::~PrincekinAbout()
{
    delete ui;
}

void PrincekinAbout::on_pushButton_clicked()
{
    this->close();
}

void PrincekinAbout::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {

        dragPosition = event->globalPos() - frameGeometry().topLeft();

        event->accept();

    }

}

void PrincekinAbout::mouseMoveEvent(QMouseEvent *event)
{

    if (event->buttons() & Qt::LeftButton) {

        move(event->globalPos() - dragPosition);

        event->accept();

    }

}
