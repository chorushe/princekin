#ifndef PERFORMANCEDETAIL_H
#define PERFORMANCEDETAIL_H

#include <QWidget>
#include <QScrollBar>
#include "globalvar.h"

namespace Ui {
class performanceDetail;
}

class performanceDetail : public QWidget
{
    Q_OBJECT

public:
    explicit performanceDetail(QWidget *parent = 0);
    ~performanceDetail();

private:
    Ui::performanceDetail *ui;

public slots:
    void WriteEdit();
    void RecieveMemResult(QString numStr,QString time);
    void RecieveCpuResult(QString numStr, QString time);
};

#endif // PERFORMANCEDETAIL_H
