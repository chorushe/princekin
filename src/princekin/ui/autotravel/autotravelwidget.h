#ifndef AUTOTRAVELWIDGET_H
#define AUTOTRAVELWIDGET_H

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
#include <QScrollBar>
#include <QThread>
#include <QtXlsx>

#include "ui/setting/monkeysetup.h"
#include "assistmeasure.h"
#include "globalvar.h"
#include "autotravelsetup.h"
#include "cmdworker.h"
#include "travelworker.h"
#include "helper.h"
#include "parsexml.h"
#include "assistbehaviorworker.h"
#include "generatexlsx.h"
#include "framescontroller.h"
#include "adbshellcontroller.h"
#include "excelcontroller.h"
#include "execmd.h"

using namespace QXlsx;
using namespace ExcelController;
using namespace AdbShellController;
namespace Ui {
class AutoTravelWidget;
}

class AutoTravelWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoTravelWidget(QWidget *parent = 0);
    ~AutoTravelWidget();
    void showInit();

private slots:
    void on_equipBtn_clicked();

    void RecieveDevicesSigal(QStringList devicesList);

    void on_packageCombo_currentIndexChanged(int index);

    void on_equipListView_clicked(const QModelIndex &index);

    void on_packageListView_clicked(const QModelIndex &index);

    void on_startBtn_clicked();

    void on_clearBtn_clicked();

    void on_reportDetailBtn_clicked();

    //void ReadOutput();
    //void ReadError();
    void ReadOutput(QString recieveStr);
    void ReadError(QString recieveStr);
    void travelerNotRunning();
    void ShowSetupWindow();
    void RecieveData(QVariant var);

    void RecieveLogcatOutput();
    void RecieveMemOverThresSignal(bool flag,QString deviceName);
    void RecieveCpuOverThresSignal(bool flag,QString deviceName);
    void RecieveBatteryOverThresSignal(bool flag, QString deviceName);
    void RecieveCpuTempOverThresSignal(bool flag,QString deviceName);
    void RecieveWifiOverThresSignal(bool flag,QString deviceName);
    void RecieveMobileOverThresSignal(bool flag,QString deviceName);

    void copyErrorPicture();

    void ReadStatisticsData(QStringList statistics);

    //*****************20170505*************************//
    void receiveFramesResult(const QString&,const QString&);
    //*****************20170505*************************//
    void receiveWorkerFinished();
    void receiveWorkerResult1(const QString&,const QString&);
    void receiveWorkerResult2(const QString&,const QString&);


signals:
    void sendSettingSig( bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck, bool isMobileCheck, QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres);
    void sendStateChange(bool state);
    void travelStop();

private:
    Ui::AutoTravelWidget *ui;

    QToolBar *toolBar;
    QToolButton *settingButton;
    QStringList equipList;
    QStringListModel *equipModel=NULL;
    QStringList packageList;
    QStringListModel *packageModel=NULL;

    QString deviceName;
    QString packageName;

    QProcess *p=NULL;
    bool isStartFlag=false;

    bool isLogEnd=false;
    QString fileNameForReport="";
    QString logFilePath="";
    QString reportFilePath="";
    QString logcatFilePath="";
    QFile *logFile=NULL;
    QFile *reportFile=NULL;

    AutoTravelSetup *setupDialog=NULL;

    QString systemStr;
    QString errorStr="点击异常详情：\r\n";

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

    bool isBatteryOverThres;
    bool isCpuTempOverThres;
    bool isMemOverThres;
    bool isCpuOverThres;
    bool isWifiOverThres;
    bool isMobileOverThres;

    AssistMeasure *assist=NULL;
    QProcess *pLogcat=NULL;
    QFile *fileLogcat=NULL;

    int numForPackageIndexChange=0;
    int numForDeviceSignal=0;

    bool logcatErrorFlag=false;
    bool clickStartEndFlag=false;
    QVector<QString> errorLogcatPngList;

    QString idBlackString="";
    QStringList logKeyWordList;
    QStringList idBlackList;

    QStringList xmlPaths;
    bool isDebug;
    QString IP;
    QString Port;

    QString clickInternalTime="3000";

    void createToolBar();
    QString getpsuid(const QString &arg_cmdline);
    void StopUiautomatorByHand();
    void CreateReportFile();
    void closeEvent(QCloseEvent *e);
    void stopAutoTravel();
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);

    //*****************20170331*****************//
    void createExcel();

    QString qStartTime;
    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qCpuTempWarningValue;
    QString qBatteryTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;

    QString qTravelStartTime;
    QString qTravelEndTime;
    QString qTravelTime;
    QString qCheckNumber;
    QString qClickNumber;
    QString qErrorNumber;
    QString qTravelActivityNumber;
    //*****************20170331*****************//

    //*****************20170505*************************//
    QString qLunchTime;
    QString qFrames;

    void getXXX();
    void getFrames();
    bool isActive();
    void getLunchTime();

    QStringList qStatisticsDataList;
    //*****************20170505*************************//

    //*****************20170630*************************//
    void createBaseExcel();
    //*****************20170630*************************//
    //*****************20170717*****************//
    QString qOldPackageName;
    //*****************20170717*****************//


};

#endif // AUTOTRAVELWIDGET_H
