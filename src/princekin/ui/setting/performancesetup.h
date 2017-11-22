#ifndef PERFORMANCESETUP_H
#define PERFORMANCESETUP_H

#include <QWidget>
#include <QFileDialog>
#include "globalvar.h"
#include <QDebug>
#include <QVariant>
#include <QMetaType>
#include "parsexml.h"
#include "staticticsherper.h"
#include <QTreeWidgetItem>
#include <QScrollBar>

namespace Ui {
class PerformanceSetup;
}

typedef struct __signalData_s
{
  int time;
  QString memThres;
  QString cpuThres;
  QString batteryThres;
  QString cpuTempThres;
  QString wifiThres;
  QString mobileThres;
  QStringList xmlPaths;
  QString port;
  bool isDebug;
}signalData_s;
Q_DECLARE_METATYPE(signalData_s)

class PerformanceSetup : public QWidget
{
    Q_OBJECT

public:
    explicit PerformanceSetup(QWidget *parent = 0);
    ~PerformanceSetup();

private:
    Ui::PerformanceSetup *ui;

    int time;
    int flag;

    QString memThres;
    QString cpuThres;
    QString batteryThres;
    QString cpuTempThres;
    QString wifiThres;
    QString mobileThres;

    QString workspace;

    bool isDebug=true;
    QString IP;

    QStringList xmlFilePaths;
    QString filePath;//xml文件夹地址

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void SendData(QVariant signalData);//showflag 0代表文本，1代表图表


private slots:

    void on_timeLineEdit_textChanged(const QString &arg1);

    void on_batteryLineEdit_textChanged(const QString &arg1);
    void on_cpuTempLineEdit_textChanged(const QString &arg1);
    void on_wifiLineEdit_textChanged(const QString &arg1);
    void on_mobileLineEdit_textChanged(const QString &arg1);

    void recieveSettingSig(QString memThres, QString cpuThres, QString batteryThres, QString cpuTempThres, QString wifiThres, QString mobileThres);
    void on_memLineEdit_textChanged(const QString &arg1);
    void on_cpuLineEdit_textChanged(const QString &arg1);
    //void on_xmlbrowseBtn_clicked();
    void on_debugRBtn_clicked();
    void on_releaseRBtn_clicked();
    void on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column);
    void on_selectXmlBtn_clicked();
    void treeItemChanged(QTreeWidgetItem *item, int column);
    void on_statCBox_clicked();
};

#endif // PERFORMANCESETUP_H
