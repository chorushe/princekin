#ifndef ALLSETTING_H
#define ALLSETTING_H

#include <QWidget>
#include <QFileDialog>
#include <QCloseEvent>
#include "globalvar.h"

namespace Ui {
class AllSetting;
}

class AllSetting : public QWidget
{
    Q_OBJECT

public:
    explicit AllSetting(QWidget *parent = 0);
    ~AllSetting();
    void dataInit();

    void WriteEmailToFile(QString emailAdds);
    void ReadEmailAdd();

private slots:
    void on_pushButton_clicked();

    void on_memCBtn_clicked();

    void on_cpuCBtn_clicked();

    void on_batteryCBtn_clicked();

    void on_cpuTempCBtn_clicked();

    void on_wifiCBtn_clicked();

    void on_mobileCBtn_clicked();

    void on_androidBtn_clicked();

private:
    Ui::AllSetting *ui;

    bool isMemCheck;
    bool isCpuCheck;
    bool isCpuTempCheck;
    bool isBatteryCheck;
    bool isWifiCheck;
    bool isMobileCheck;

    QString workSpace;

protected:
    void closeEvent(QCloseEvent *e);

signals:
    void SendData(QString filePath,QString androidPath, bool isMemCheck,bool isCpuCheck,bool isBatteryCheck,bool isCpuTempCheck,bool isWifiCheck,bool isMobileCheck
                  ,QString memThres,QString cpuThres,QString batteryThres,QString cpuTempThres, QString wifiThres,QString mobileThres);
};

#endif // ALLSETTING_H
