#ifndef MONKEYWIDGET_H
#define MONKEYWIDGET_H

#include <QMainWindow>
#include <QApplication>
#include <QStringListModel>
#include <QProcess>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QDateTime>
#include <QCloseEvent>
#include <QToolBar>
#include <QToolButton>
#include <qDebug>
#include <QDir>
#include <QTextCodec>
#include <QtXlsx>

#include "ui/setting/monkeysetup.h"
#include "assistmeasure.h"
#include "globalvar.h"
#include "helper.h"
#include "assistbehaviorworker.h"
#include "generatexlsx.h"
#include "adbshellcontroller.h"
#include "excelcontroller.h"
#include "framescontroller.h"
#include "execmd.h"
#include "monkeyworker.h"

using namespace QXlsx;
using namespace ExcelController;
using namespace AdbShellController;

namespace Ui {
class MonkeyWidget;
}

class MonkeyWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit MonkeyWidget(QWidget *parent = 0);
    ~MonkeyWidget();
    void showInit();

private:
    Ui::MonkeyWidget *ui;

    QToolBar *toolBar;
    QToolButton *settingButton;
    QStringList equipList;
    QStringListModel *equipModel=NULL;
    QStringList packageList;
    QStringListModel *packageModel=NULL;

    QString deviceName;
    QString packageName;

    QString cmdStr;
    QProcess *pLogcat;
    QProcess *pbugReport;
    QProcess *pDumpsys;
    QProcess *pPull;
    QProcess *pPull_1;

    QFile *fileMonkeyLog=NULL;
    QFile *fileMonkeyReport=NULL;
    QFile *fileLogcat=NULL;
    QFile *fileBugReport=NULL;
    QFile *fileDumpsys=NULL;
    QString reportFilePath;

    QDateTime timeStart;
    QDateTime timeEnd;

    bool isMemCheck;
    bool isCpuCheck;
    bool isCpuTempCheck;
    bool isBatteryCheck;
    bool isWifiCheck;
    bool isMobileCheck;

    QString memThres;
    QString cpuThres;
    QString batteryThres;
    QString cpuTempThres;
    QString wifiThres;
    QString mobileThres;

    QString recieveStr;//用来接收p返回的数据，使其为全局变量的目的仅在于记录monkeyreport中最后一行的发现

    MonkeySetUp *setupDialog;

    AssistMeasure *assist;
    QProcess *proLogcat;
    QFile *fLogcat=NULL;

    bool isStartFlag=false;
    bool isLogcatFlag=false;

    bool isBatteryOverThres;
    bool isCpuTempOverThres;
    bool isMemOverThres;
    bool isCpuOverThres;
    bool isWifiOverThres;
    bool isMobileOverThres;

    int numForPackageIndexChange=0;
    int numForDeviceSignal=0;

    QString ANRErrorStr="";
    QString CrashErrorStr="";
    QString ExceptionErrorStr="";
    QString OtherErrorStr="";

    QStringList logKeyWordList;

    QStringList xmlPaths;
    bool isDebug=true;
    QString IP;
    QString port;

    void widgetInit();
    void createToolBar();
    void sleep(unsigned int msec);
    void CreateMonkeyFiles();
    void CreateRootFiles();
    void CreateLogcatFiles();
    void CreateBugReportFiles();

    void closeEvent(QCloseEvent *e);

    void exportBugreport();
    void catchLogcat();
    void setEnabledTrue();
    void killMonkey();
    void createExcel();

    QString qStartTime;
    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qCpuTempWarningValue;
    QString qBatteryTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;

    QString qMonkeyTime;
    QString qLunchTime;
    QString qFrames;

    void getXXX();
    void getFrames();
    bool isActive();
    void getLunchTime();
    QStringList qStatisticsDataList;
    QString qOldPackageName;


signals:
    void sendStateChange(bool state);
    void sendSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck,bool isMobileCheck,
                        QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres,QString wifiThres,QString mobileThres);
    void monkeyStop();

private slots:

    void ShowSetupWindow();

    void RecieveData(QVariant var);

    void ReadOutput(QString recieveStr);
    void ReadOutputLogcat();
    void ReadOutputBugreport(QString tempStr);
    void monkeyNotRunning();

    void BugReportNotRunning();
    void ReadOutputDumpsys(QString tempStr);
    void DumpsysNotRunning();
    void ReadError(QString recieveStr);

    void RecieveMemOverThresSignal(bool flag,QString deviceName);
    void RecieveCpuOverThresSignal(bool flag,QString deviceName);
    void RecieveBatteryOverThresSignal(bool flag,QString deviceName);
    void RecieveCpuTempOverThresSignal(bool flag,QString deviceName);
    void RecieveWifiOverThresSignal(bool flag,QString deviceName);
    void RecieveMobileOverThresSignal(bool flag,QString deviceName);

    void RecieveLogcatOutput();

    void RecieveDevicesSigal(QStringList devicesList);

    void on_equipBtn_clicked();
    void on_packageCombo_currentIndexChanged(int index);
    void on_equipListView_clicked(const QModelIndex &index);
    void on_packageListView_clicked(const QModelIndex &index);
    void on_startBtn_clicked();
    void on_clearBtn_clicked();
    //void on_logcatBtn_clicked();
    //void on_bugreportBtn_clicked();
    void on_detailReportBtn_clicked();

    void ReadStatisticsData(QStringList statistics);

    void receiveFramesResult(const QString&,const QString&);
    void createBaseExcel();
    void receiveWorkerFinished();
    void receiveWorkerResult1(const QString&,const QString&);
    void receiveWorkerResult2(const QString&,const QString&);

};

#endif // MONKEYWIDGET_H
