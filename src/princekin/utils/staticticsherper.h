#ifndef STATICTICSHERPER_H
#define STATICTICSHERPER_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QFileInfo>


class StaticticsHerper
{
public:
    StaticticsHerper();

    static void updateParentItem(QTreeWidgetItem *item);
    static void LoadFiles(QString path, QTreeWidget *treewidget, QTreeWidgetItem *item);
    static QString getBasePath(QTreeWidgetItem *itemfile);
    static QString getTreeRootText(QTreeWidgetItem *item);
};

#endif // STATICTICSHERPER_H
