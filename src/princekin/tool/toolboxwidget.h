#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

#include "getlogcatwidget.h"
#include "apkinfoform.h"

namespace Ui {
class ToolBoxWidget;
}

class ToolBoxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBoxWidget(QWidget *parent = 0);
    ~ToolBoxWidget();

private slots:
    void on_apkBtn_clicked();

    void on_closeBtn_clicked();

    void on_minBtn_clicked();

    void on_logcatBtn_clicked();

    void RecieveDevicesSigal(QStringList deviceList);

private:
    Ui::ToolBoxWidget *ui;

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    QPoint dragPosition;

    GetLogcatWidget *logcatDialog=NULL;
    ApkInfoForm *apkinfo=NULL;

signals:
    void SendDevicesSignal(QStringList);
    void refresh();
};

#endif // TOOLBOXWIDGET_H
