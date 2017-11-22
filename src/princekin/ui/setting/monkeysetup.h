#ifndef MonkeySetUp_H
#define MonkeySetUp_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <paramclass.h>
#include <QTime>
#include <QLabel>
#include <QMetaType>
#include <QVariant>
#include "parsexml.h"
#include "globalvar.h"
#include <QTreeWidget>
#include "staticticsherper.h"

typedef struct __signalMonkeyData_s
{
    ParamClass data;
    QString logKeyWords;
    bool isMemCheck;
    bool isCpuCheck;
    bool isBatteryCheck;
    bool isCpuTempCheck;
    bool isWifiCheck;
    bool isMobileCheck;
    QString memThres;
    QString cpuThres;
    QString batteryThres;
    QString cpuTempThres;
    QString wifiThres;
    QString mobileThres;
    QStringList xmlPaths;
    bool isDebug;
}signalMonkeyData_s;

Q_DECLARE_METATYPE(signalMonkeyData_s)
class MonkeySetUp : public QWidget
{
    Q_OBJECT
public:
    explicit MonkeySetUp(QWidget *parent = 0);
    ~MonkeySetUp();

private:
    QGroupBox *logLevelGroup;
    QRadioButton *levelZeroRBtn;
    QRadioButton *levelOneRBtn;
    QRadioButton *levelTwoRBtn;
    QRadioButton *levelThreeRBtn;
    QHBoxLayout *logLevelLayout;

    QGroupBox *basicEventGroup;
    QCheckBox *countCBtn;
    QCheckBox *seedCBtn;
    QCheckBox *throttleCBtn;
    QCheckBox *randomCBtn;
    QLineEdit *countLineEdit;
    QLineEdit *seedLineEdit;
    QLineEdit *throttleLineEdit;
    QGridLayout *basicEventLayout;

    QGroupBox *eventPercGroup;
    QCheckBox *touchCBtn;
    QCheckBox *motionCBtn;
    QCheckBox *trackBallCBtn;
    QCheckBox *syskeysCBtn;
    QCheckBox *navCBtn;
    QCheckBox *majorNavCBtn;
    QCheckBox *appSwitchCBtn;
    QCheckBox *flipCBtn;
    QCheckBox *pinchZoomCBtn;
    QCheckBox *anyEventCBtn;

    QLineEdit *touchLineEdit;
    QLineEdit *motionLineEdit;
    QLineEdit *trackBallLineEdit;
    QLineEdit *syskeysLineEdit;
    QLineEdit *navLineEdit;
    QLineEdit *majorNavLineEdit;
    QLineEdit *appSwitchLineEdit;
    QLineEdit *flipLineEdit;
    QLineEdit *pinchZoomLineEdit;
    QLineEdit *anyEventLineEdit;
    QGridLayout *eventPercLayout;

    QGroupBox *classGroup;
    QTextEdit *className;
    QHBoxLayout *classLayout;

    QGroupBox *logKeyWordGroup;
    QLineEdit *logKeyWordName;
    QHBoxLayout *logKeyWordLayout;

    QGroupBox *debugGroup;
    QCheckBox *iCrashesCBtn;
    QCheckBox *iTimeoutCBtn;
    QCheckBox *isecurityCBtn;
    QCheckBox *dbgNoEventCBtn;
    QCheckBox *killProcessCBtn;
    QCheckBox *monitorCBtn;
    QCheckBox *waitDBGNavCBtn;
    QCheckBox *hprofCBtn;
    QVBoxLayout *debugLayout;

   /* QGroupBox *pathGroup;
    QLineEdit *pathLineEdit;
    QPushButton *browseBtn;
    QHBoxLayout *pathLayout;*/

    QGroupBox *cmdGroup;
    QTextEdit *cmdTextEdit;
    QHBoxLayout *cmdLayout;

    QHBoxLayout *mainlayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;

    //参数测量
    QCheckBox *memCBtn;
    QCheckBox *cpuCBtn;
    QCheckBox *cpuTempCBtn;
    QCheckBox *batteryCBtn;
    QCheckBox *wifiCBtn;
    QCheckBox *mobileCBtn;

    QLabel *cpuLabel;
    QLabel *memLabel;
    QLabel *cpuTempLabel;
    QLabel *batteryLabel;
    QLabel *wifiLabel;
    QLabel *mobileLabel;

    QLineEdit *cpuLineEdit;
    QLineEdit *memLineEdit;
    QLineEdit *cpuTempLineEdit;
    QLineEdit *batteryLineEdit;
    QLineEdit *wifiLineEdit;
    QLineEdit *mobileLineEdit;

    QGridLayout *measureLayout;
    QGroupBox *measureGroup;

    bool isMemCheck;
    bool isCpuCheck;
    bool isCpuTempCheck;
    bool isBatteryCheck;
    bool isWifiCheck;
    bool isMobileCheck;

    ParamClass data;

    //统计点
    QVBoxLayout *statisticsLayout;
    QGroupBox *statisticsGroup;
    QLabel *appVersionLabel;
    QRadioButton *debugRBtn;
    QRadioButton *releaseRBtn;
    QTreeWidget *xmlTreeWidget;
    QPushButton *selectXmlBtn;
    QHBoxLayout *ipPortLayout;
    QHBoxLayout *versionLayout;
    //QHBoxLayout *treeLayout;
    QLabel *ipLable;
    QLabel *portLabel;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QCheckBox *statCBox;

    bool isDebug=true;
    QStringList xmlFilePaths;
    QString filePath;//xml文件夹地址

    void widgetInit();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void SendData(QVariant var);

private slots:
    void level0Changed();
    void level1Changed();
    void level2Changed();
    void level3Changed();
    void countChanged();
    void countTextChanged();
    void seedChanged();
    void seedTextChanged();
    void throttleChanged();
    void throttleTextChanged();
    void randomChanged();

    void touchChanged();
    void touchTextChanged();
    void motionChanged();
    void motionTextChanged();
    void trackballChanged();
    void trackballTextChanged();
    void syskeysChanged();
    void syskeysTextChanged();
    void navChanged();
    void navTextChanged();
    void majornavChanged();
    void majornavTextChanged();
    void appswitchChanged();
    void appswitchTextChanged();
    void flipChanged();
    void flipTextChanged();
    void pinchzoomChanged();
    void pinchzoomTextChanged();
    void anyeventChanged();
    void anyeventTextChanged();

    void logTextChanged();

    void classNameChanged();
    void iCrashChanged();
    void iTimeoutChanged();
    void iSecurityChanged();

    void dbgNoEventChanged();
    void killProcessChanged();
    void monitorChanged();
    void waitDngChanged();
    void hprofChanged();

    void cmdMonkeyChanged();

    void memCBtnCheck();
    void cpuCBtnCheck();
    void wifiCBtnCheck();
    void mobileCBtnCheck();
    void batteryCBtnCheck();
    void cpuTempCBtnCheck();

    void stateChanged(bool state);
    void recieveSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck, bool isMobileCheck,
                           QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres);

    void TextChanged();

    void debugRBtnClick();
    void releaseRBtnClick();
    void selectXmlBtnClick();
    void treeItemChanged(QTreeWidgetItem*,int);
    void on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column);
    void statClick();
};

#endif // MonkeySetUp_H
