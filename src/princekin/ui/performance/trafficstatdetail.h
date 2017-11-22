#ifndef TRAFFICSTATDETAIL_H
#define TRAFFICSTATDETAIL_H

#include <QWidget>
#include <QVector>
#include <QScrollBar>

namespace Ui {
class TrafficStatDetail;
}

class TrafficStatDetail : public QWidget
{
    Q_OBJECT

public:
    explicit TrafficStatDetail(QWidget *parent = 0);
    ~TrafficStatDetail();

    QVector<QString> wifiData;
    QVector<QString> mobileData;

private:
    Ui::TrafficStatDetail *ui;




public slots:
    void receiveTrafficResult_wifi1(const QString &arg_receiveAll,const QString &arg_upAll );
    void receiveTrafficResult_wifi2(const QString &arg_receiveAll,const QString &arg_upAll );
    void receiveTrafficResult_mobile1(const QString &arg_receiveAll,const QString &arg_upAll );
    void receiveTrafficResult_mobile2(const QString &arg_receiveAll,const QString &arg_upAll );

    void drawTable();
};

#endif // TRAFFICSTATDETAIL_H
