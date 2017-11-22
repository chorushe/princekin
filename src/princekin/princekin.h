#ifndef PRINCEKIN_H
#define PRINCEKIN_H

#include <QDomDocument>
#include <QDomNode>
#include <QPalette>
#include <QPushButton>

#include "commonhead.h"
#include "componenthead.h"
#include "record.h"
#include "replayform.h"
#include "execmd.h"
#include "performancewidget.h"
#include "monkeywidget.h"
#include "behaviourwidget.h"
#include "ui/autotravel/autotravelwidget.h"
#include "allsetting.h"
#include "pulldownwidget.h"
#include "princekinabout.h"
#include "ui/interface/interfacewidget.h"
#include "tool/toolboxwidget.h"

namespace Ui {
class Princekin;
}

class Princekin : public QMainWindow
{
    Q_OBJECT

public:
    explicit Princekin(QWidget *parent = 0);
    ~Princekin();

private:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);


private slots:
    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_7_clicked();

    void RecieveData(QString filePath, QString androidPath, bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck,
                     bool isMobileCheck, QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres);


    void on_pushButton_9_clicked();

    void TimerSlot();
    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void setupBtn_clicked();
    void helpBtn_clicked();
    void aboutBtn_clicked();

private:
    Ui::Princekin *ui;

    QPointer<Record> recordPointer=NULL;
    Record *recordUi=NULL;

    QPointer<ReplayForm> replayPointer=NULL;
    ReplayForm *replayFormInstance=NULL;

    QPalette   palWhite;
    QPalette   palBlack;


    QPoint pos0;
    QPoint last;
    bool isPress=false;

    QPoint move_point;

    QTimer *timer=NULL;
    QThread *timerThread=NULL;

    int isDeviceNull=0;

    PerformanceWidget *performanceDialog=NULL;
    MonkeyWidget *monkeyDialog=NULL;
    BehaviourWidget *behaviourDialog=NULL;
    AutoTravelWidget *autoTravelDialog=NULL;
    InterfaceWidget *interfaceDialog=NULL;
    ToolBoxWidget *toolDialog=NULL;

    PulldownWidget *pulldownWidget=NULL;
    PrincekinAbout *aboutWidget=NULL;


    //*******************************************************//
    AllSetting *settingDialog=NULL;

    void readConfigFile(QString fileName);
    void saveConfigFile(QString fileName, QString filePath, QVariant isMemCheck, QVariant isCpuCheck, QVariant isBatteryCheck, QVariant isCpuTempCheck, QVariant isWifiCheck, QVariant isMobileCheck, QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres);
    void saveAndroidPath(QString fileName,QString filePath);

    QString GetDeviceModel(QString sn);
signals:
    void SendDevicesSignal(QStringList devicesList);

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // PRINCEKIN_H
