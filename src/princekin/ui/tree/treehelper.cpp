#include "treehelper.h"

TreeHelper::TreeHelper()
{
}

void TreeHelper::createProject(const QString & newProjectDir)
{
    //创建项目
    //创建模块
    //创建script var invoked check
    QDir mDir(newProjectDir);
    if(mDir.exists())
    {
        QMessageBox::information(NULL,"提示", "当前项目已存在");
        return;
    }
    else
    {
         mDir.mkdir(newProjectDir);
         /*
         mDir.mkdir(newProjectDir + QDir::separator() + "模块");
         mDir.mkdir(newProjectDir + QDir::separator() + "模块" + QDir::separator() + "check");
         mDir.mkdir(newProjectDir + QDir::separator() + "模块" + QDir::separator() + "invoked");
         mDir.mkdir(newProjectDir + QDir::separator() + "模块" + QDir::separator() + "script");
         mDir.mkdir(newProjectDir + QDir::separator() + "模块" + QDir::separator() + "var");
         */
    }
}

void TreeHelper::createModel(const QString & selectTreePaths,const QString & inputName)
{
    QString selectTreePath=QDir::toNativeSeparators(selectTreePaths);
    QDir mDir(selectTreePath + QDir::separator() + inputName);
    if(mDir.exists())
    {
        QMessageBox::information(NULL,"提示", "当前模块已存在");
        return;
    }
    else
    {
         mDir.mkdir(selectTreePath + QDir::separator() + inputName);
         mDir.mkdir(selectTreePath + QDir::separator() + inputName + QDir::separator() + "check");
         mDir.mkdir(selectTreePath + QDir::separator() + inputName + QDir::separator() + "invoke");
         mDir.mkdir(selectTreePath + QDir::separator() + inputName + QDir::separator() + "script");
         mDir.mkdir(selectTreePath + QDir::separator() + inputName + QDir::separator() + "var");
    }
}


void TreeHelper::createScript(const QString & selectTreePaths,const QString & inputName)
{
    QString selectTreePath=QDir::toNativeSeparators(selectTreePaths);
    QFile file(selectTreePath + QDir::separator() + inputName + ".txt");
    if(file.exists())
    {
        file.close();
        QMessageBox::information(NULL,"提示", "当前脚本文件已存在");
        return;
    }

    file.open(QIODevice::WriteOnly);
    file.close();

    QString mSubPath=getSubPath(selectTreePath);
    createPath(gEmptyScriptDir + QDir::separator() + mSubPath);

    QString mStr=gEmptyScriptDir + QDir::separator() + mSubPath + QDir::separator() + inputName + ".txt";
    file.setFileName(mStr);
    file.open(QIODevice::WriteOnly);
    file.close();
}

