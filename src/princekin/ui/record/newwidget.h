#ifndef NEWWIDGET_H
#define NEWWIDGET_H

#include <QWidget>
#include "filemanager.h"

namespace Ui {
class NewWidget;
}

class NewWidget : public QWidget,FileManager
{
    Q_OBJECT

public:
    explicit NewWidget(QWidget *parent = 0);
    ~NewWidget();
    void setLabelText(QString textContent);

    QString filePath;
    int flag;//1表示建模块，0表示建脚本;2表示建测试套文件夹，3表示建测试套文件

   // bool createScript(QString selectTreePath,QString name);
    void InitVar(QString filePath,int flag);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:



private:
    Ui::NewWidget *ui;
};

#endif // NEWWIDGET_H
