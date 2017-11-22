#ifndef PRINCEKINABOUT_H
#define PRINCEKINABOUT_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

namespace Ui {
class PrincekinAbout;
}

class PrincekinAbout : public QWidget
{
    Q_OBJECT

public:
    explicit PrincekinAbout(QWidget *parent = 0);
    ~PrincekinAbout();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PrincekinAbout *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint dragPosition;
};

#endif // PRINCEKINABOUT_H
