#ifndef PRESET_H
#define PRESET_H

#include "globalvar.h"
#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QListWidgetItem>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QModelIndexList>


class PreSet:public QObject
{
    Q_OBJECT
public:
    PreSet();
public:
    static int strToInt1(const QString &);
    static int strToInt2(const QString &);

    static void setModel(QFileSystemModel*);
    static QStringList getDeviceIdList(QList<QListWidgetItem *>);
    static QStringList getScriptList(QFileSystemModel *,QModelIndexList);
    static QString getBaseName(const QString &);
    static QString getModuleName(const QString &);
    static QStringList getModuleNameList(QFileSystemModel *,QModelIndexList);
    static QStringList getSecondLevelDirNameList(QList<QListWidgetItem *>);
    static QString getDeviceSN(const QString &);
    static QHash<QString,int> getModule_FileCount_Hash(QFileSystemModel *,QModelIndexList);
    static QStringList getModule_fileCount_List(QStringList,QHash<QString,int>);
    static QHash<QString,QString> getDevice_TopIndex_Hash(QStringList);

    static QStringList _getMoudleName(QStringList scriptlist);
    static QHash<QString,int> _getModule_FileCount_Hash(QStringList moudleNameList);
private:
    //
};

#endif // PRESET_H
