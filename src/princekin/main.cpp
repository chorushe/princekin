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
#include "windows.h"
#include "DbgHelp.h"
#include "singleapplication.h"

/*void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
  {
      QByteArray localMsg = msg.toLocal8Bit();
      switch (type) {
      case QtDebugMsg:
          fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtInfoMsg:
          fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtWarningMsg:
          fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtCriticalMsg:
          fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtFatalMsg:
          fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          abort();
      }
  }*/

void getDisk();
void initEnv();

int main(int argc, char *argv[])
{
    //SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注冊异常捕获函数
   // qInstallMessageHandler(myMessageOutput);

   SingleApplication a(argc, argv, "some unique key string");
    if (a.isRunning())
    {
        a.sendMessage("message from other instance.");
        return 0;
    }

   // QApplication a(argc, argv);
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
    a.connect(&a, SIGNAL(messageAvailable(QString)), &w, SLOT(receiveMessage(QString)));
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));

    w.setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint|Qt::FramelessWindowHint);
    w.setAttribute(Qt::WA_QuitOnClose,true);
    w.show();


    int ret=a.exec();

    return ret;
}


void initEnv()
{
    Helper::initVar();
    Helper::initEvent();
    Helper::copyXml();

    QString currentVersion="3.3";
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

