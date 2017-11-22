#ifndef BEHAVIOURWIDGET_H
#define BEHAVIOURWIDGET_H

#include <QMainWindow>
#include <QApplication>
#include <QStringListModel>
#include <QProcess>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QTextCodec>
#include <QByteArray>
#include <QDomElement>
#include <QDomNode>
#include <QFileDialog>
#include <QStringList>
#include <QStringListModel>
#include <QScrollBar>
#include <QColor>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QMap>
#include <QThread>
#include <QSettings>
#include <QCloseEvent>
#include <QToolBar>
#include <QToolButton>
#include <QUrl>
#include <QClipboard>
#include <QToolTip>
#include <QStandardItemModel>
#include <QtXlsx>
#include <QLabel>
#include <QThreadPool>
#include <QPointer>

#include "dataclass.h"
#include "ui/setting/behavioursetup.h"
#include "utils/xmleditdialog.h"
#include "assistmeasure.h"
#include "servercontrol.h"
#include "helper.h"
#include "urltoxml.h"
#include "globalvar.h"
#include "editmultixml.h"
#include "staticticsherper.h"
#include "SwitchControl.h"
#include "replayforbehaviorworker.h"
#include "replayforbehaviorcontroller.h"
#include "form.h"
#include "behaviorworker.h"
#include "adbshellcontroller.h"
#include "excelcontroller.h"
#include "execmd.h"
#include "generatexlsx.h"
#include "framescontroller.h"


using namespace QXlsx;
using namespace ExcelController;
using namespace AdbShellController;

namespace Ui {
class BehaviourWidget;
}

typedef struct _urlTimeStruct
{
    QString url;
    int time;
}urlTimeStruct;

Q_DECLARE_METATYPE(urlTimeStruct)

class BehaviourWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit BehaviourWidget(QWidget *parent = 0);
    ~BehaviourWidget();

    void showInit();

private:
    Ui::BehaviourWidget *ui;

    QToolBar *toolBar;
    QToolBar *toolBar1;
    QToolBar *toolBar2;
    QToolBar *toolBar3;
    QToolButton *settingButton;
    QToolButton *addXmlButton;
    QToolButton *createXmlButton;
    QToolButton *editXmlButton;
    QToolButton *addScriptButton;

    QList<DataClass> xmlData;
    QMap<QString,QList<DataClass> > allXmlData;
    BehaviourSetup *setupDialog;
    QString currentPath;
    QString revelantHead;
    QString unique;
    QList<QString> urlList;
    QList<bool> urlErrorList;

    int focusFlag;//0-url 1-statistics
    bool isSuccess;//所检测的url与xml文件对比，完全符合true，不符合false

    UrlToXml *urlToXmlDialog;
    QString xmlSavePath;
    EditMultiXML *editMultiXmlDialog;

    int numForDeviceSignal=0;
    QStringList equipList;
    QStringListModel *equipModel=NULL;

    int urlTotalNum=0;
    int urlErrorNum=0;


    void createToolBar();
    QString UrlDecode(QString strInput);
    QList<DataClass> GetXML(QString fileName);
    void ParseXML(const QDomElement &element);
    QString ParseURL(QString url, bool isLatest);
    void ShowURL(QString url,QString unique,QString key);

    AssistMeasure *assist=NULL;
    QString deviceName;

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

    bool isServerPattern;
    QString UID;
    bool isDebugVersion;
    int xmlCount;

    QThread *processThread=NULL;

    QVector<ServerControl*> server;
    QMap<QString,QString> xmlMatch;

    QSettings *iniContent;

    QProcess *pLogcat=NULL;
    QFile *fileLogcat=NULL;

    bool isStartFlag=false;

    QString fileNameForReport;

   // QStringList statistics;//用来存储统计点信息的变量

    //脚本执行统计点检测相关变量
    QString xmlRootPath;
    QList<urlTimeStruct> urlTimeList;
    int numForUrl=0;
    QList<DataClass> singleXmlData;
    QList<QString> urlListFour;
    QList<QList<DataClass>> allXmlDataForCheck;//装载所有要检测的xml信息，为了点击之前的url时对其进行解析
    QStringList statRes;
    QMap<QString, bool> statResForScript;//用来统计每个脚本中统计点的对错情况
    bool tempFlagForStat=true;

    QString ParseURLForCheck(QString url,QList<DataClass> xmlData, bool isLatest);
    void GetFourSecData();
    bool FindMatchUrl(QString xmlFile);

    QList<int> lineNumberList;

    void closeEvent(QCloseEvent *e);

    void exportReport();
    void stopRun();

    //*****************20170331*****************//
    void createExcel();

    QString qStartTime;
    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qCpuTempWarningValue;
    QString qBatteryTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;
    //*****************20170331*****************//

    //*****************20170505*****************//
    QString qLogDir;
    QString qReportXlsxPath;
    QString qReportTxtPath;
    QString qStatisticsTxtPath;
    QString qUrlTxtPath;
    QString qErrorReportPath;
    QString qLunchTime;
    QString qFrames;
    QString qPackageName;

    bool scriptPatternFlag=true;
    SwitchControl *scriptSC;
    QLabel *scriptLabel;
    QString currentScriptPath;
    QStringList qScriptList;

    QPointer<Form> formPointer=NULL;
    Form *formInstance=NULL;

    void startRunScript();

    void getXXX();
    void getFrames();
    bool isActive();
    void getLunchTime();

    QStringList qStatisticsDataList;
    //*****************20170505*****************//

    //*****************20170630*************************//
    void createBaseExcel();
    //*****************20170630*************************//

    QMap<QString,bool> qmap;


signals:
    void sendStateChange(bool state);
    void sendSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
                        QString memThres,QString cpuThres,QString batteryThres,QString cpuTempThres,QString wifiThres,QString mobileThres);
    void sendStopReplay();
    void sendStopStatSignal();

private slots:

    void ReadStandardOutput(QString res);
    void ReadStandardError(QString res);
    void RecieveStopSignal();
    void SetupBtnClicked();
    void CreateXmlBtnClicked();
    void AddXmlBtnClicked();
    void EditXmlBtnClicked();
    void AddScriptBtnClicked();

    void RecieveData(QString xmlRootPath, bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck, bool isMobileCheck, QString UID, bool isServerPattern, bool isDebugVersion, QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres);

    bool eventFilter(QObject *,QEvent *);    //注意这里
    void keyPressEvent(QKeyEvent *e);

    void treeItemChanged(QTreeWidgetItem *item, int column);

    void readFromServerClass(QString str);

    void RecieveLogcatOutput();

    void RecieveMemOverThresSignal(bool flag,QString deviceName);
    void RecieveCpuOverThresSignal(bool flag,QString deviceName);
    void RecieveBatteryOverThresSignal(bool flag, QString deviceName);
    void RecieveCpuTempOverThresSignal(bool flag,QString deviceName);
    void RecieveWifiOverThresSignal(bool flag,QString deviceName);
    void RecieveMobileOverThresSignal(bool flag,QString deviceName);


    void on_xmlTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column);
    void on_urlListWidget_itemPressed(QListWidgetItem *item);
    void on_xmlTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_statisticsListWidget_itemClicked(QListWidgetItem *item);
   // void on_exportBtn_clicked();
    void on_clearBtn_clicked();
    void on_startBtn_clicked();
    void on_reportDetailBtn_clicked();

    void RecieveDevicesSigal(QStringList devicesList);

    void on_equipListView_clicked(const QModelIndex &index);
    void on_equipBtn_clicked();
    void receiveWorkerFinished();
    void receiveWorkerResult1(const QString&,const QString&);
    void receiveWorkerResult2(const QString&,const QString&);

    void recieveOneScriptFinish(const QString &, bool flag);//一个脚本执行结束后，清空标志位，计量每个脚本中统计点的情况；

    //*****************20170505*************************//
    void receiveFramesResult(const QString&,const QString&);
    //*****************20170505*************************//
    void on_xmlTreeWidget_itemEntered(QTreeWidgetItem *item, int column);
    void on_scriptTreeWidget_itemChanged(QTreeWidgetItem *item, int column);
    void ScriptSwitch(bool state);

    void receiveRunScriptResult(const QString &arg_result);
    void receiveReplayResult(const QString &arg_deviceId, const QString &arg_two, QMap<QString, bool> arg_resmap, int arg_sec);
    void receiveErrorResult(const QString &arg_SecondLevelDirName,const QString &arg_error);
    void on_scriptTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_scriptTextEdit_cursorPositionChanged();
};

#endif // BEHAVIOURWIDGET_H
