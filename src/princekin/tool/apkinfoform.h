#ifndef APKINFOFORM_H
#define APKINFOFORM_H

#include "helper.h"
#include "execmd.h"
#include "globalvar.h"

#include <QWidget>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QProcess>
#include <QFileDialog>

namespace Ui {
class ApkInfoForm;
}

class ApkInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit ApkInfoForm(QWidget *parent = 0);
    ~ApkInfoForm();

private:
    void showList();
    bool isSelected();

private slots:
    void on_pushButton_clicked();
    void refreshApk();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::ApkInfoForm *ui;
    QTextCodec *tcUtf8=QTextCodec::codecForName("utf-8");
};

#endif // APKINFOFORM_H
