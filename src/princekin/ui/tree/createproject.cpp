#include "createproject.h"
#include "ui_createproject.h"
#include <QDebug>
CreateProject::CreateProject(QString labelText,QWidget *parent) :QDialog(parent),ui(new Ui::CreateProject)
{
    ui->setupUi(this);
    ui->label->setText(labelText);
    ui->lineEdit->setFocus();
}

CreateProject::~CreateProject()
{
    delete ui;
}


void CreateProject::on_buttonBox_accepted()
{

    inputName=ui->lineEdit->text();
}

void CreateProject::on_buttonBox_rejected()
{
    inputName="";
}
