#include "RenameDialog.h"
#include "ui_RenameDialog.h"
#include <QDebug>
RenameDialog::RenameDialog(QString labelText,QWidget *parent) :QDialog(parent),ui(new Ui::RenameDialog)
{
    ui->setupUi(this);
    ui->label->setText(labelText);
    ui->lineEdit->setFocus();
}

RenameDialog::~RenameDialog()
{
    delete ui;
}


void RenameDialog::on_buttonBox_accepted()
{

    inputName=ui->lineEdit->text();
}

void RenameDialog::on_buttonBox_rejected()
{
    inputName="";
}

void RenameDialog::set(const QString &inputText)
{
    inputName=inputText;
    ui->lineEdit->setText(inputText);
}
