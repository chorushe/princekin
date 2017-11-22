#ifndef AUTOTRAVELSETUP_H
#define AUTOTRAVELSETUP_H

#include <QMainWindow>
#include "globalvar.h"
#include <QFileDialog>
#include <QToolTip>
#include <QMessageBox>
#include <QVariant>
#include <QMetaType>
#include <QDebug>
#include "parsexml.h"
#include "staticticsherper.h"
#include <QScrollBar>
#include <QHeaderView>
#include "globalvar.h"

namespace Ui {
class AutoTravelSetup;
}

typedef struct __signalAutotravelData_s
{
    QString logKeyWord;
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
    QString clickInternalTime;
}signalAutotravelData_s;

Q_DECLARE_METATYPE(signalAutotravelData_s)
class AutoTravelSetup : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoTravelSetup(QWidget *parent = 0);
    ~AutoTravelSetup();

    bool isDebug;

private slots:
    //void on_browseBtn_clicked();
    void recieveSettingSig(bool isMemCheck, bool isCpuCheck, bool isBatteryCheck, bool isCpuTempCheck, bool isWifiCheck, bool isMobileCheck,
                           QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres);
    void stateChanged(bool state);

    void on_memLineEdit_textChanged(const QString &arg1);

    void on_cpuLineEdit_textChanged(const QString &arg1);

    void on_batteryLineEdit_textChanged(const QString &arg1);

    void on_cpuTempLineEdit_textChanged(const QString &arg1);

    void on_wifiLineEdit_textChanged(const QString &arg1);

    void on_mobileLineEdit_textChanged(const QString &arg1);

    void on_logKeyWordLineEdit_textChanged(const QString &arg1);

    void on_debugRBtn_clicked();

    void on_releaseRBtn_clicked();

    void on_selectXmlBtn_clicked();

    void treeItemChanged(QTreeWidgetItem *item, int column);

    void on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_statCBox_clicked();

signals:
    void SendData(QVariant var);

private:
    Ui::AutoTravelSetup *ui;

    QStringList xmlFilePaths;
    QString filePath;//xml文件夹地址

    void closeEvent(QCloseEvent *e);
};

#endif // AUTOTRAVELSETUP_H
