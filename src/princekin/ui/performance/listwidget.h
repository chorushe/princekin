#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDebug>

class ListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QString model,QWidget *parent = 0);

    QLabel *lab1;
    QString filePath;
    QString GetLabelStr();
    void SetLabelStr(QString str,QString str1);
    QString model;

    static int num;

signals:

public slots:
    void Close();
};

#endif // LISTWIDGET_H
