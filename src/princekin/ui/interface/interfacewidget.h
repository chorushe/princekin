#ifndef INTERFACEWIDGET_H
#define INTERFACEWIDGET_H

#include <QMainWindow>
#include <QScrollBar>
#include <QStringListModel>
#include <QProcess>
#include <QDebug>
#include <QThread>
#include <QHash>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QResizeEvent>
#include <QToolBar>
#include <QToolButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>
#include <QFile>
#include <QtXlsx>
#include <QDesktopWidget>
#include <QColor>
#include "behaviorworker.h"
#include "helper.h"
#include "parsexml.h"
#include "interdataclass.h"
#include "interfacesetup.h"
#include "assistmeasure.h"
#include "globalvar.h"
#include "generatexlsx.h"
#include "framescontroller.h"
#include "xlsxformat.h"
#include "xlsxdocument.h"
#include "xlsxabstractsheet.h"
#include "adbshellcontroller.h"
#include "excelcontroller.h"
#include "execmd.h"

using namespace QXlsx;
using namespace ExcelController;
using namespace AdbShellController;


namespace Ui {
class InterfaceWidget;
}

class InterfaceWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit InterfaceWidget(QWidget *parent = 0);
    ~InterfaceWidget();

    void showInit();

private slots:
    void on_startBtn_clicked();

    void RecieveDevicesSigal(QStringList devicesList);
    void ReadStandardOutput(QString res);
    void ReadStandardError(QString res);

    void on_equipListView_clicked(const QModelIndex &index);

    void on_interDataTable_itemClicked(QTableWidgetItem *item);

    void on_equipBtn_clicked();

    void on_packageListView_clicked(const QModelIndex &index);

    void on_packageCombo_currentIndexChanged(const QString &arg1);

    void SetupBtnClicked();

    void RecieveData(QVariant var);

    void RecieveMemOverThresSignal(bool flag,QString deviceName);
    void RecieveCpuOverThresSignal(bool flag,QString deviceName);
    void RecieveBatteryOverThresSignal(bool flag, QString deviceName);
    void RecieveCpuTempOverThresSignal(bool flag,QString deviceName);
    void RecieveWifiOverThresSignal(bool flag,QString deviceName);
    void RecieveMobileOverThresSignal(bool flag,QString deviceName);

    void RecieveLogcatOutput();

    void on_clearBtn_clicked();

    void on_reportDetailBtn_clicked();

    void on_timeWarningLineEdit_textChanged(const QString &arg1);

    void on_filterAfterLineEdit_textChanged(const QString &arg1);

    //*****************20170606*************************//
    void receiveFramesResult(const QString&,const QString&);
    //*****************20170606*************************//

    void TableHeaderClicked(int num );
    void receiveWorkerFinished();
    void receiveWorkerResult1(const QString&,const QString&);
    void receiveWorkerResult2(const QString&,const QString&);

    void RecieveStopSignal();

    void on_interDataTable_itemChanged(QTableWidgetItem *item);

private:
    Ui::InterfaceWidget *ui;

    QThread *processThread=NULL;

    int numForDeviceSignal=0;
    QStringList equipList;
    QStringListModel *equipModel=NULL;
    QStringList packageList;
    QStringListModel *packageModel=NULL;
    QString deviceName;
    QString packageName;
    int numForPackageIndexChange=0;

    bool isStartFlag=false;

    int interNum=1;
    QHash<int,interDataClass> interHash;

    QToolBar *toolBar;
    QToolButton *settingButton;

    InterfaceSetup *setupDialog=NULL;

    AssistMeasure *assist=NULL;

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

    bool isAllTest=true;
    QString method;
    QString filterStr;
    QString filterStrAfter;
    QString fileNameForReport;
    QString qStartTime;
    QString qReportTxtPath;
    QString qErrorReportPath;

    QProcess *pLogcat=NULL;
    QFile *fileLogcat=NULL;
    bool isStartLogat=false;

    bool isLatest=true;

    int timeWarning=0;
    int interfaceTotalNum=0;
    int aveReponseTime=0;
    double timeOutRate=0;
    int maxTime=0;
    int minTime=0;

    bool endflag;
    QString ContentRes="";

    QString UrlDecode(QString str);
    void createToolBar();
    void SplitInterfaceParam(QString url);
    void StopRun();
    void exportReport();

    QString indent(int number)  ;
    QString formatJson(QString json);
    QString formatXml(QString xml);

    void ShowInterfaceData(interDataClass tempData);
    bool JudgeFilterRes(interDataClass tempData);

    QString SPACE = "   ";
    QString orderFlag="time";

    //*****************20170606*************************//
    void createExcel();

    QString qMemWarningValue;
    QString qCpuWarningValue;
    QString qCpuTempWarningValue;
    QString qBatteryTempWarningValue;
    QString qWifiWarningValue;
    QString qMobileWarningValue;

    QString qLunchTime;
    QString qFrames;

    void getXXX();
    void getFrames();
    bool isActive();
    void getLunchTime();
    //*****************20170606*************************//

    //*****************20170630*************************//
    void createBaseExcel();
    void createInterfaceExcel();
    Format getNormalFormat();
    //*****************20170630*************************//
    //*****************20170717*****************//
    QString qOldPackageName;
    //*****************20170717*****************//

protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void sendStateChange(bool state);
    void sendSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck,
                        QString memThres,QString cpuThres,QString batteryThres,QString cpuTempThres,QString wifiThres,QString mobileThres);
    void sendStopStatSignal();
};

#endif // INTERFACEWIDGET_H
