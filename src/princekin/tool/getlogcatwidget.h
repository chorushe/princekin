#ifndef GETLOGCATWIDGET_H
#define GETLOGCATWIDGET_H

#include "globalvar.h"
#include "execmd.h"
#include "helper.h"
#include <QWidget>
#include <QFile>
#include <QProcess>
#include <QScrollBar>
#include <QStringListModel>
#include <QCloseEvent>
#include <QTextCodec>
#include "getlogcatworker.h"


namespace Ui {
class GetLogcatWidget;
}

class GetLogcatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GetLogcatWidget(QWidget *parent = 0);
    ~GetLogcatWidget();

private slots:
    void on_startBtn_clicked();
    void RecieveLogcatOutput(QString tempStr);

    void on_clearBtn_clicked();

    void on_checkLogBtn_clicked();

    void RecieveDevicesSigal(QStringList devicesList);

    void on_equipListView_clicked(const QModelIndex &index);

    void on_keyWordLineEdit_textChanged(const QString &arg1);

    void RecieveStopSignal();

private:
    Ui::GetLogcatWidget *ui;

    QFile *fileLogcat=NULL;
    QFile *errorFileLogcat=NULL;
    bool isStartFlag=false;
    QString qStartTime;
    QString fileNameForReport;
    QString deviceName;

    int numForDeviceSignal=0;
    QStringList equipList;
    QStringListModel *equipModel=NULL;

    QStringList keyqWords;

    bool errorFlag=false;

    void closeEvent(QCloseEvent *e);

signals:
    void sendStopSignal();
};

#endif // GETLOGCATWIDGET_H
