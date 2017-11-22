#ifndef REPLAYSETUP_H
#define REPLAYSETUP_H

#include <QWidget>
#include <QMessageBox>
#include <QVariant>
#include <QMetaType>
#include <QDebug>
#include "parsexml.h"
#include <QTreeWidgetItem>
#include "staticticsherper.h"
#include <QScrollBar>

namespace Ui {
class ReplaySetup;
}

typedef struct __signalReplayData_s
{
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
}signalReplayData_s;

Q_DECLARE_METATYPE(signalReplayData_s)

class ReplaySetup : public QWidget
{
    Q_OBJECT

public:
    explicit ReplaySetup(QWidget *parent = 0);
    ~ReplaySetup();

    QStringList xmlFilePaths;
    bool isDebug;
    QString filePath;

    void initUI();


private slots:
    void on_timesLineEdit_textChanged(const QString &arg1);

    void on_intervalLineEdit_textChanged(const QString &arg1);

    void on_memLineEdit_textChanged(const QString &arg1);

    void on_cpuLineEdit_textChanged(const QString &arg1);

    void on_batteryLineEdit_textChanged(const QString &arg1);

    void on_cpuTempLineEdit_textChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_wifiLineEdit_textChanged(const QString &arg1);

    void on_mobileLineEdit_textChanged(const QString &arg1);

    void on_debugRBtn_clicked();

    void on_releaseRBtn_clicked();

    void on_selectXmlBtn_clicked();

    void on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void treeItemChanged(QTreeWidgetItem *item, int column);

    void on_statCBox_clicked();

private:
    Ui::ReplaySetup *ui;

    void textChange();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void sendAddRepositoryPath(const QString&);
    void sendData(QVariant var);
    void sendLoadResiporitySignal();
};

#endif // REPLAYSETUP_H
