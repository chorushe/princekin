#include "newwidget.h"
#include "ui_newwidget.h"


NewWidget::NewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewWidget)
{
    ui->setupUi(this);
    ui->label_2->setVisible(false);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);

    connect(ui->lineEdit,SIGNAL(returnPressed()),ui->pushButton,SLOT(click()), Qt::UniqueConnection);
}

NewWidget::~NewWidget()
{
    delete ui;
}

void NewWidget::InitVar(QString filePath, int flag)
{
    this->filePath=filePath;
    this->flag=flag;

}

void NewWidget::on_pushButton_clicked()
{
    if(ui->lineEdit->text()=="")
    {
        ui->label_2->setVisible(true);
        ui->label_2->setText("<font color=red>名称不能为空</font>");
    }
    else
    {
        ui->label_2->setVisible(false);
        if(flag==0)
        {
            bool isok=createScript(filePath,ui->lineEdit->text().trimmed());
            if(!isok)
            {
                ui->label_2->setVisible(true);
                ui->label_2->setText("<font color=red>该脚本已存在</font>");
            }
            else
                this->close();
        }
        else if(flag==1)
        {
            bool isok=createModule(filePath,ui->lineEdit->text().trimmed());
            if(!isok)
            {
                ui->label_2->setVisible(true);
                ui->label_2->setText("<font color=red>该模块已存在</font>");
            }
            else
                this->close();
        }
        else if(flag==2)
        {
            bool isok=createSuiteModule(filePath,ui->lineEdit->text().trimmed());
            if(!isok)
            {
                ui->label_2->setVisible(true);
                ui->label_2->setText("<font color=red>该测试套文件夹已存在</font>");
            }
            else
                this->close();
        }
        else if(flag==3)
        {
            bool isok=createScript(filePath,ui->lineEdit->text().trimmed());
            if(!isok)
            {
                ui->label_2->setVisible(true);
                ui->label_2->setText("<font color=red>该测试套文件已存在</font>");
            }
            else
                this->close();
        }

    }
}

void NewWidget::on_pushButton_2_clicked()
{
    this->close();
}

void NewWidget::setLabelText(QString textContent)
{
    ui->label->setText(textContent);
}
