#include "newscriptwidget.h"
#include "ui_newscriptwidget.h"
#include <QMessageBox>
#include "globalvar.h"
#include <QFileDialog>
#include "helper.h"

NewScriptWidget::NewScriptWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewScriptWidget)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);

    ui->scriptPathLineEdit->setText(gWorkSpace);
}

NewScriptWidget::~NewScriptWidget()
{
    delete ui;
}

void NewScriptWidget::on_browseBtn_clicked()
{
    QString directory=QFileDialog::getExistingDirectory(this,("选择目录"),workSpace);
    if(directory.isEmpty())
    {
    }
    else
    {
        ui->scriptPathLineEdit ->setText(directory);
    }
}

void NewScriptWidget::textChange()
{
    repositoryName=ui->scriptNameLineEdit->text().trimmed();
    workSpace=ui->scriptPathLineEdit->text().trimmed();

    workSpace=QDir::toNativeSeparators(workSpace);

    QString s=workSpace + QDir::separator() + repositoryName;
    QFileInfo info(s);
    bool isOk=info.exists();

    gMyWorkSpace=workSpace;

}
void NewScriptWidget::on_scriptPathLineEdit_textChanged(const QString &arg1)
{
    textChange();
}

void NewScriptWidget::on_scriptNameLineEdit_textChanged(const QString &arg1)
{
    textChange();
}

void NewScriptWidget::closeEvent(QCloseEvent *event)
{
    if(ui->scriptNameLineEdit->text()==""||ui->scriptPathLineEdit->text()=="")//选择创建的同时名称路径有一个是空的
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("提示"),
                                       QString(tr("请完善脚本库名称及路径")),
                                       QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);

        if (button == QMessageBox::Yes)
        {
            event->ignore();  //忽略退出信号，程序继续运行，填写名称或路径
        }
        else if (button == QMessageBox::No)
        {
            event->accept();//程序退出，不创建也不新建
        }
    }
    else//如果名称路径是完整的
    {

        QString str;
        if(workSpace.endsWith("\\"))
        {
            str=workSpace+repositoryName;
        }
        else
        {
            str=workSpace+QDir::separator()+repositoryName;
        }

        //*****************20170315*************************//
        str=workSpace + QDir::separator() + repositoryName;
        //*****************20170315*************************//

        str=QDir::toNativeSeparators(str);
        qDebug()<<"脚本库地址"<<str;
        Helper::createRepository(str);

        QString strFile=gConfigDir+QDir::separator()+"currentRepository.txt";
        strFile=QDir::toNativeSeparators(strFile);
        Helper::writeStr(strFile,str);

        emit sendRepositoryResult(ui->scriptNameLineEdit->text(),ui->scriptPathLineEdit->text());
        event->accept();
    }

}

//***************20170713***************//
void NewScriptWidget::setScriptDir(const QString &arg_dir)
{
    ui->scriptPathLineEdit->setText(arg_dir);
}
