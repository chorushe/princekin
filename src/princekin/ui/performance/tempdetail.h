#ifndef TEMPDETAIL_H
#define TEMPDETAIL_H

#include <QScrollBar>
#include <QWidget>
#include "globalvar.h"


namespace Ui {
class tempDetail;
}

class tempDetail : public QWidget
{
    Q_OBJECT

public:
    explicit tempDetail(QWidget *parent = 0);
    ~tempDetail();

private:
    Ui::tempDetail *ui;

public slots:
    void RecieveCpuTempResult(QString numStr, QString time);
    void RecieveBatteryResult(QString numStr, QString time);
    void WriteEdit();
};

#endif // TEMPDETAIL_H
