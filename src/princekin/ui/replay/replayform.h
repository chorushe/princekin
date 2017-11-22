#ifndef REPLAYFORM_H
#define REPLAYFORM_H

#include <QWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QThread>
#include <QCloseEvent>
#include <QTimer>
#include <QPointer>
#include <QString>
#include <QHash>
#include <QStringList>
#include <QFileSystemModel>
#include <QThreadPool>
#include <QFile>
#include <QTextStream>
#include "globalvar.h"
#include "helper.h"
#include "execmd.h"
#include "filehandle.h"
#include "initenv.h"
#include "form.h"
#include "replaysetup.h"
#include "preset.h"
#include "createreport.h"
#include "createcrashreport.h"
#include "writeresult.h"
#include "performance.h"
#include "replaycontroller.h"
#include "memcontroller.h"
#include <QScrollBar>
#include "assistbehaviorworker.h"
#include "parsexml.h"
#include "adbshellcontroller.h"
#include "staticticsherper.h"
#include "editscriptfilewidget.h"


namespace Ui {
class ReplayForm;
}

class MyThread2 : public QThread
{
public:
    QString myid;

public:
    void initArg(const QString &);
    virtual void run();
};


class ReplayForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReplayForm(QWidget *parent = 0);
    ~ReplayForm();

private:
    void createMenuBar();
    void createToolBar();
    void createMenuItems();

    void loadRepository();
    void createTree();

    void showDeviceList();

    void preSets();

    void initDeviceEnv(QStringList,QStringList);
    void setNetState(const QString &,const QString &);
    void setDeviceEnv(QStringList,QStringList);

    void startRunScript(QStringList,QStringList,QStringList);
    void startReplay(const QString &,const QString &,QStringList);
    void startPerformance(QStringList);


    void createDetailReport(const QString &,QStringList,QStringList);
    void createReportSaveDir();
    void createCrashReport();

    void createWriteResult(const QString &);


    void stopTrafficservice(const QString &);
    void stopNetState(const QString &);

    void closeEvent(QCloseEvent *event);

    QString getWorkspace();


    void doWork(QList<QListWidgetItem*>);

    void writeBaseInfo(QStringList);
    void getIcon(const QString &,const QString &,const QString &);
    void getStatisticsData();
    void createUrl();
    void mailTimer();
    void runexe(const QString &,const QString &);

    void createDaily(const QString &,QStringList,QStringList,const QString &,QStringList);
    void createPerformance(const QString &,QStringList,QStringList);
    QString getMinMaxMemory(const QString &,QHash<QString,int>);
    QString getMinMaxCpu(const QString &,QHash<QString,int>);
    QString getMinMaxBatterytemp(const QString &,QHash<QString,int>);
    QString getMinMaxCputemp(const QString &,QHash<QString,int>);

    QString getMemoryAveValue(const QString &,QStringList);
    QString getCpuAveValue(const QString &,QStringList);
    QString getBatterytempAveValue(const QString &,QStringList);
    QString getCputempAveValue(const QString &,QStringList);
    QString getWifiMobile(const QString &);


    QImage getImage();
    Format getNormalFormat();
    Format getHyperlinkFormat();

    QString getTreeRootText(QTreeWidgetItem *item);
    void restartscript();

signals:
    void sendStopReplay();
    void sendStopPerformance(const QString &);
    void sendReplayResultToWrite(const QString &,const QString &,QStringList,QStringList,const QString &);
    void sendSaveXlsx();
    void sendClearPerformance();
    void sendStopNetState();
    void sendSec(int);
    void sendBaseInfoToWrite(QStringList);
    void sendBaseInfoToWrite(QString);
    void sendIcon(QString);
    void sendStopReplayForStatistics();//执行结束时发送信号给统计
    void sendCrashResult(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,QStringList);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void loadScriptSlot();
    void settingSlot();
    void getDeviceSlot();
    void recieveData(QVariant var);
    void receiveRunScriptResult(const QString &);
    void receiveReplayResult(const QString &,const QString &,QStringList,QStringList,int,QStringList,const QString &);
    void receiveCrashResult(const QString &,const QString &,const QString &,const QString &);
    void receiveProcFinished(const QString &);
    void receiveInitEnvFinished();
    void receiveErrorResult(const QString &,const QString &);
    void receiveHtmlFinished(const QString &);
    void receiveIconFinished(const QString &);
    void ReadStatisticsData(QStringList statistics);
    void mailTimerSlot();
    void sendMailSlot();

    void treeItemChanged(QTreeWidgetItem* item,int);

    void on_scriptTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_scriptTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::ReplayForm *ui;

    QAction *qAction;
    QMenuBar *qMyMenuBar;
    QMenu *qMenuSetting;
    QToolBar *toolBar;
    QToolBar *toolBar4;
    QToolButton *settingButton;
    QToolButton *addScriptButton;
   // QFileSystemModel *qModel;

    bool isMemCheck;
    bool isCpuCheck;
    bool isCpuTempCheck;
    bool isBatteryCheck;
    bool isWifiCheck;
    bool isMobileCheck;
    bool isRun=false;
    bool isDebug;

    int qSec=0;
    int qNumber;
    int qNumber2;
    int qSelectedDeviceCount;
    int qRowIndex;
    int qPerRowIndex;

    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qBatteryWarningValue;
    QString qCpuTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;

    QString qDeviceId;
    QString qCurrentDeviceId;
    QString qFirstLevelDirName;
    QString qEverytimeFolderName;
    QString qDetailedFolderName;
    QString qDetailedPath;
    QString qTargetXlsx;
    QString qCrashTargetXlsx;


    QString qDetailSourceXlsx;
    QString qDetailedTargetXlsx;

    QString qDailySourceXlsx;
    QString qDailyTargetXlsx;


    QString qEverytimePath;

    QString qSourceXlsx;

    QString qCrashSourceXlsx;

    QString qRepoName;
    QString IP;
    QString port;
    QString qAppInfo;
    QString qAppVersion;
    QString qAppName;
    QString qMailContent;
    QStringList qMobileInfoList;


    QStringList qScriptList;
    QStringList qDeviceIdList;
    QStringList qDevice_list;
    QStringList qModule_FileCount_List;
    QStringList qDeviceModelIdNameList;
    QStringList qModuleNameList;
    QStringList xmlPaths;
    QStringList qStatisticsDataList;

    QHash<QString,int> qModule_FileCount_Hash;
    QHash<QString,QString> qDevice_TopIndex_Hash;
    QHash<QString,bool> qDeviceInitedHash;

    CreateReport report;

    QPointer<Form> formPointer=NULL;
    ReplaySetup *setupDialog=NULL;
    Form *formInstance=NULL;
    WriteResult *qWriteResultInstance=NULL;
    Performance *performanceInstance=NULL;
    CreateCrashReport *qCrashreportInstance=NULL;
    QTimer *testTimer;
    QTimer *tt;
    QTimer *qMailtimer;
    Document *qXlsx;

    QString currentScriptPath="";
    QTextStream *qOutStream;

    //QFile *qfile;
    QHash<QString,QFile*> qfileHash;
    QHash<QString,QTextStream *> qfilePointerHash;
    QString qTempDeviceId;
    QString qTempDeviceModelIdName;
    QStringList qTempSplitResult;
};

#endif // REPLAYFORM_H
