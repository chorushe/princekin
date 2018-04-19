#include "renamedialog2.h"
#include "ui_renamedialog2.h"

RenameDialog2::RenameDialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog2)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
}

RenameDialog2::~RenameDialog2()
{
    delete ui;
}

void RenameDialog2::setFilePath(const QString &arg_filePath)
{
    qFilePath=arg_filePath;
    qFilePath=QDir::toNativeSeparators(qFilePath);
    QString baseName=getBaseName(qFilePath);
    ui->lineEdit->setText(baseName);
}

void RenameDialog2::on_buttonBox_accepted()
{
    QString nameStr=ui->lineEdit->text().trimmed();
    if(nameStr.isEmpty())
    {
    }
    else
    {
        QString baseName;
        baseName=getBaseName(qFilePath);
        if(nameStr==baseName)
        {
        }
        else
        {
            QString absolutePath;
            QString modifyPath;
            QString moduleName;
            QString filePath;

            absolutePath=getAbsolutePath(qFilePath);
            modifyPath=absolutePath + QDir::separator() + nameStr+".txt";
            modifyPath=QDir::toNativeSeparators(modifyPath);

            moduleName=getBaseName(absolutePath);
            QFile::rename(qFilePath,modifyPath);

            filePath=gUiautomatorDir+QDir::separator()+moduleName+QDir::separator()+baseName+".java";
            filePath=QDir::toNativeSeparators(filePath);
            removeFile(filePath);


            QString str="bin\\classes\\com\\sohu\\test\\";
            filePath=gUiautomatorDir+QDir::separator()+moduleName+QDir::separator()+str+baseName+".class";
            filePath=QDir::toNativeSeparators(filePath);
            removeFile(filePath);

            filePath=gUiautomatorDir+QDir::separator()+moduleName+QDir::separator()+str+baseName+"$Receive.class";
            filePath=QDir::toNativeSeparators(filePath);
            removeFile(filePath);

        }
    }
}

QString RenameDialog2::getBaseName(const QString &arg_path)
{
    QFileInfo fileInfo(arg_path);
    QString baseName=fileInfo.baseName();
    return baseName;
}

QString RenameDialog2::getAbsolutePath(const QString &arg_path)
{
    QFileInfo fileInfo(arg_path);
    QString absolutePath=fileInfo.absolutePath();
    return absolutePath;
}

void RenameDialog2::removeFile(const QString &arg_path)
{
    QFile file(arg_path);
    file.remove();
}
