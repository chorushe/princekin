#include "staticticsherper.h"

StaticticsHerper::StaticticsHerper()
{

}

/*void StaticticsHerper::treeItemChanged(QTreeWidgetItem *item, int column)
{
    QString itemText = item->text(0);
    if (item->checkState(0) == Qt::Checked)
    {
        QTreeWidgetItem *parent = item->parent();
        int cnt = item->childCount();
        if (cnt >0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Checked);
            }
        }
        else
        {
            updateParentItem(item);
        }
    }
    else if (item->checkState(0) == Qt::Unchecked)
    {
        int cnt = item->childCount();
        if (cnt > 0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
        else
        {
            updateParentItem(item);
        }
    }
}*/

void StaticticsHerper::updateParentItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if (parent == NULL)
    {
        return;
    }
    //选中的子节点个数
    int selectedCount = 0;
    int childCount = parent->childCount();
    for (int i = 0; i < childCount; i++)
    {
        QTreeWidgetItem *childItem = parent->child(i);
        if (childItem->checkState(0) == Qt::Checked)
        {
            selectedCount++;
        }
    }
    if (selectedCount <= 0)
    {
        //未选中状态
        parent->setCheckState(0, Qt::Unchecked);
    }
    else if (selectedCount > 0 && selectedCount < childCount)
    {
        //部分选中状态
        parent->setCheckState(0, Qt::PartiallyChecked);
    }
    else if (selectedCount == childCount)
    {
        //选中状态
        parent->setCheckState(0, Qt::Checked);
    }
}

void StaticticsHerper::LoadFiles(QString path, QTreeWidget *treewidget, QTreeWidgetItem *item)
{
    QDir dir(path);
    if (!dir.exists()) return;

    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    int size = list.size();
    for (int i = 0; i < size; i++)
    {
        QFileInfo info = list.at(i);
        if (info.fileName() == "." || info.fileName() == "..") continue;

        if (info.isDir()) {
            QTreeWidgetItem *fileItem = new QTreeWidgetItem(QStringList() << info.fileName(), 0);  //0表示目录
            fileItem->setIcon(0,QIcon(":/folder.png"));
            fileItem->setCheckState(0,Qt::Unchecked);
            if (treewidget == NULL)
                item->addChild(fileItem);
            else
                treewidget->addTopLevelItem(fileItem);
            LoadFiles(info.filePath(), NULL, fileItem);
        }
    }
    QDir *dirr=new QDir(path);
    QStringList filter;
    filter<<"*.xml";
    filter<<"*.txt";
    dirr->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dirr->entryInfoList(filter));
    for(int i=0;i<fileInfo->count();i++)
    {
        //xmlListWidget->addItem(fileInfo->at(i).fileName());
        QTreeWidgetItem *fileItem = new QTreeWidgetItem(QStringList() << fileInfo->at(i).fileName(), 1);   //1表示是文件
        fileItem->setIcon(0,QIcon(":/file.png"));
        fileItem->setCheckState(0,Qt::Unchecked);
        if (treewidget == NULL)
            item->addChild(fileItem);
        else
            treewidget->addTopLevelItem(fileItem);
    }
}

QString StaticticsHerper::getBasePath(QTreeWidgetItem *itemfile)
{
    /*得到文件路径*/
    QStringList filepath;
    //QTreeWidgetItem *itemfile=xmlTreeWidget->currentItem(); //获取被点击的item
    while(itemfile!=NULL)
    {
        filepath<<itemfile->text(0); //获取itemfile名称
        itemfile=itemfile->parent(); //将itemfile指向父item
    }
    QString strpath;
    for(int i=(filepath.size()-1);i>=0;i--) //QStringlist类filepath反向存着初始item的路径
    { //将filepath反向输出，相应的加入’/‘
        strpath+=filepath.at(i);
        if(i!=0)
            strpath+=QDir::separator();
    }
    return strpath;

}

QString StaticticsHerper::getTreeRootText(QTreeWidgetItem *item)//寻找根路径的名称
{
    QString rootText;
    while(item!=NULL)
    {
        rootText=item->text(0);
        item=item->parent(); //将itemfile指向父item
    }
    return rootText;
}
