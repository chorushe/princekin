#include "princekin.h"

#include <QApplication>
#include <QDir>
#include <QtMsgHandler>
#include <QFileInfo>
#include <QTextCodec>

#include "globalvar.h"
#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>


void getDisk();
void initEnv();

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QString currentPath=QCoreApplication::applicationDirPath();
    qDebug()<<"currentPath"<<currentPath;
    if(currentPath.contains(" "))
    {
        QMessageBox::critical(0,"错误","小王子工具不能放到含有空格的路径下，请更换位置！");
        a.exit(0);
        return 0;
    }

    initEnv();

    Princekin w;
    w.setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint|Qt::FramelessWindowHint);
    w.setAttribute(Qt::WA_QuitOnClose,true);
    w.show();

    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    int ret=a.exec();

    return ret;
}


void initEnv()
{
    Helper::initVar();
    Helper::initEvent();
    Helper::copyXml();

    QString currentVersion="2.11.2";
    bool d=Helper::isCopyConfig(currentVersion);

    if(d)
    {
        Helper::copyNormal(gtempConfigDir + QDir::separator() + "normalconfig",gNewConfigDir, false);
        Helper::copyModify(gtempConfigDir + QDir::separator() + "modifyconfig",gNewConfigDir, false);

        QFileInfo info(gNewConfigDir + QDir::separator() + "js");
        if(!info.exists())
        {
            Helper::copyModify(gtempConfigDir + QDir::separator() + "lib",gNewConfigDir, false);
        }
    }

    Helper::getWorkspace();
    Helper::initConfig();
}

