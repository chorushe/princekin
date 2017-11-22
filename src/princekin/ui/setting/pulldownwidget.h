#ifndef PULLDOWNWIDGET_H
#define PULLDOWNWIDGET_H

#include <QWidget>

namespace Ui {
class PulldownWidget;
}

class PulldownWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PulldownWidget(QWidget *parent = 0);
    ~PulldownWidget();

private slots:
    void on_setupBtn_clicked();

    void on_helpBtn_clicked();

    void on_aboutBtn_clicked();

private:
    Ui::PulldownWidget *ui;

signals:
    void setupSignal();
    void helpSignal();
    void aboutSignal();
};

#endif // PULLDOWNWIDGET_H
