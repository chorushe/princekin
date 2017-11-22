#ifndef EDITSCRIPTFILEWIDGET_H
#define EDITSCRIPTFILEWIDGET_H

#include <QWidget>
#include "helper.h"
#include "globalvar.h"
#include "staticticsherper.h"
#include <QScrollBar>
#include <QTextStream>
#include <QFile>

namespace Ui {
class EditScriptFileWidget;
}

class EditScriptFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditScriptFileWidget( QWidget *parent = 0);
    ~EditScriptFileWidget();

    static QStringList readSuiteFile(QString filePath);

    void InitVar(QString arg_filepath);
    QString filePath;
    QStringList suiteContent;

private:
    Ui::EditScriptFileWidget *ui;


    void addScript();


private slots:
    void treeItemChanged(QTreeWidgetItem *item, int column);


    void on_scriptTreeWidget_itemChanged(QTreeWidgetItem *item, int column);
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
};

#endif // EDITSCRIPTFILEWIDGET_H
