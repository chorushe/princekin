#ifndef EDITMULTIXML_H
#define EDITMULTIXML_H

#include <QWidget>
#include <QFileDialog>
#include <QScrollBar>
#include <QScrollArea>
#include <QVector>
#include <QTableWidgetItem>
#include <QString>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QFileInfo>

#include "globalvar.h"
#include "listwidget.h"
#include "parsexml.h"
#include "staticticsherper.h"
#include "execmd.h"

namespace Ui {
class EditMultiXML;
}

class EditMultiXML : public QWidget
{
    Q_OBJECT

public:
    explicit EditMultiXML(QWidget *parent = 0);
    ~EditMultiXML();

private slots:

    void on_editBtn_clicked();

    void tableWidget_itemChanged(QTableWidgetItem *item);

    void on_clearBtn_clicked();

    void on_xmlTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

    void treeItemChanged(QTreeWidgetItem *item, int column);

    void on_detailBtn_clicked();

private:
    Ui::EditMultiXML *ui;

    QVector<QString> xmlFilePaths;
    bool unique=false;
    QString filePath;

    QStringList getEditDatas();

};

#endif // EDITMULTIXML_H
