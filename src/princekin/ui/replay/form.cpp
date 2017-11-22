#include "form.h"
#include "ui_form.h"
#include <QLabel>
#include <QTextCodec>
Form::Form(QWidget *parent) :QWidget(parent),ui(new Ui::Form)
{
    ui->setupUi(this);

   // QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));//

}

Form::~Form()
{
    delete ui;
}

void Form::setScriptText(const QString &arg_Text)
{
   // this->setWindowFlags(this->windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinimizeButtonHint);
    ui->textEdit->setText(arg_Text);
}
