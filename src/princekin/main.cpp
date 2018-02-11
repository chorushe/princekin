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
#include "sendsignalclass.h"
#include "DbgHelp.h"

SendSignalClass *sendSignal=new SendSignalClass;

static LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    //And output crash information
    EXCEPTION_RECORD *record = pException->ExceptionRecord;

    //Create the dump file
    HANDLE hDumpFile = CreateFile((LPCWSTR)QString("D:/QtWorkSpace/build-testrelease-Desktop_Qt_5_7_1_MinGW_32bit-Release/release/crash.dmp").utf16(),
             GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hDumpFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
        CloseHandle(hDumpFile);
    }

    sendSignal->emitCrash();
    //这里弹出一个错误对话框并退出程序
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QMessageBox::critical(NULL,"小王子崩溃",QString("<div>错误代码：%1</div><div>错误地址：%2</div></FONT>").arg(errCode).arg(errAdr),
                          QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}

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
    //qInstallMessageHandler(myMessageOutput);

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

    a.connect(sendSignal,SIGNAL(sendCrashSignal()),&w,SLOT(recieveCrashSignal()));
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    int ret=a.exec();

    return ret;
}


void initEnv()
{
    Helper::initVar();
    Helper::initEvent();
    Helper::copyXml();

    QString currentVersion="3.2";
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

