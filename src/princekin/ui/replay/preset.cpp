#include "preset.h"
#include <QDebug>
PreSet::PreSet()
{
}


QStringList PreSet::getDeviceIdList(QList<QListWidgetItem *> arg_selectedDeviceIdItemList)
{
    int count;
    QString deviceIdName;
    QStringList deviceIdList;

    count=arg_selectedDeviceIdItemList.size();
    for(int i=0;i<count;i++)
    {
        deviceIdName=getDeviceSN( arg_selectedDeviceIdItemList.at(i)->text());
        deviceIdList.append(deviceIdName);
    }
    return deviceIdList;
}


QStringList PreSet::getScriptList(QFileSystemModel *arg_model,QModelIndexList arg_scriptModelIndexList)
{
    int count;
    QString scriptBaseName;
    QString moduleName;
    QString selectedNode;
    QStringList scriptList;

    QFileInfo info;

    count=arg_scriptModelIndexList.size();
    for(int i=0;i<count;i++)
    {
        selectedNode=arg_model->filePath(arg_scriptModelIndexList.at(i));
        info.setFile(selectedNode);
        if(info.isFile())
        {
            scriptBaseName=getBaseName(selectedNode);
            moduleName=getModuleName(selectedNode);
            scriptList.append(moduleName+"="+scriptBaseName);
        }
        else
        {
            scriptList.append("findfolder");
        }
    }

    return scriptList;
}



QStringList PreSet::getModuleNameList(QFileSystemModel *arg_model,QModelIndexList arg_scriptModelIndexList)
{
    int count;

    QString moduleName;
    QString selectedNode;
    QStringList moduleNameList;

    QFileInfo info;

    count=arg_scriptModelIndexList.size();

    for(int i=0;i<count;i++)
    {
        selectedNode=arg_model->filePath(arg_scriptModelIndexList.at(i));
        info.setFile(selectedNode);
        if(info.isFile())
        {
            moduleName=getModuleName(selectedNode);
            if(!moduleNameList.contains(moduleName))
            {
                moduleNameList.append(moduleName);
            }
        }
    }
    return moduleNameList;
}


QStringList PreSet::getSecondLevelDirNameList(QList<QListWidgetItem *> arg_selectedDeviceItemList)
{
    int count;
    QString str;
    QStringList list;
    QStringList spliResult;

    count=arg_selectedDeviceItemList.size();
    for(int i=0;i<count;i++)
    {
        str=arg_selectedDeviceItemList.at(i)->text();
        spliResult=str.split("+");
        list.append(spliResult.at(0).trimmed()+ "-" + spliResult.at(1).trimmed());
    }
    return list;
}


QHash<QString,int> PreSet::getModule_FileCount_Hash(QFileSystemModel *arg_model,QModelIndexList arg_scriptModelIndexList)
{
    int count;
    QString moduleName;
    QString selectedNode;
    QHash<QString,int> module_FileCount_Hash;

    QFileInfo info;

    count=arg_scriptModelIndexList.size();
    for(int i=0;i<count;i++)
    {
        selectedNode=arg_model->filePath(arg_scriptModelIndexList.at(i));
        info.setFile(selectedNode);
        if(info.isFile())
        {
            moduleName=getModuleName(selectedNode);
            int x=module_FileCount_Hash.value(moduleName);
            module_FileCount_Hash.insert(moduleName,x+1);
        }
    }
    return module_FileCount_Hash;
}


QStringList PreSet::getModule_fileCount_List(QStringList arg_moduleNameList,QHash<QString,int> arg_module_FileCount_Hash)
{
    int x;
    QStringList module_FileCount_List;
    foreach(QString var,arg_moduleNameList)
    {
        x=arg_module_FileCount_Hash.value(var);
        module_FileCount_List.append(var + "=" + QString::number(x));
    }
    return module_FileCount_List;
}

QHash<QString,QString> PreSet::getDevice_TopIndex_Hash(QStringList arg_deviceIdList)
{
    int count;
    QString deviceIdName;
    QStringList list;
    QHash<QString,QString> device_TopIndex_Hash;


    QString s="C D E F G H I J K L M N O P Q R S T U V W X Y Z";
    list=s.split(" ");

    count=arg_deviceIdList.size();
    for(int i=0;i<count;i++)
    {
        deviceIdName=arg_deviceIdList.at(i);
        device_TopIndex_Hash.insert(deviceIdName,list.at(i));
    }
    int x=3;
    for(QString var:list)
    {
        gLetterHash.insert(var,x);
        x++;
    }



    return device_TopIndex_Hash;
}





QString PreSet::getBaseName(const QString &arg_selectedScriptNode)
{
    QFileInfo info(arg_selectedScriptNode);
    return info.baseName();
}

QString PreSet::getModuleName(const QString &arg_selectedScriptNode)
{
    QString moduleName;
    QString absolutePath;
    QString parentName;
    QFileInfo info;

    info.setFile(arg_selectedScriptNode);
    absolutePath=info.absolutePath();

    QDir dir(absolutePath);
    parentName=dir.dirName();
    moduleName=parentName;

    return moduleName;
}

QString PreSet::getDeviceSN(const QString &arg_deviceModel)
{
    QString tmp="+";
    int num=arg_deviceModel.indexOf(tmp)+1;
    return arg_deviceModel.mid(num).trimmed();
}


int PreSet::strToInt1(const QString &arg_str)
{
    int d;
    bool isOK;

    d=arg_str.toInt(&isOK,10);
    if(!isOK)
    {
        d=1;
    }
    return d;
}

int PreSet::strToInt2(const QString &arg_str)
{
    int d;
    bool isOK;

    d=arg_str.toInt(&isOK,10);
    if(isOK)
    {
        d=d*1000;
    }
    else
    {
        d=6*1000;
    }
    return d;
}

QStringList PreSet::_getMoudleName(QStringList scriptlist)
{
    QStringList moudleName;
    foreach (QString var, scriptlist)
    {
        moudleName.append( var.split("=").at(0));
    }
    return moudleName;
}

QHash<QString,int> PreSet::_getModule_FileCount_Hash(QStringList moudleNameList)
{
    QHash<QString,int> module_FileCount_Hash;
    foreach(QString var,moudleNameList)
    {
        int x=module_FileCount_Hash.value(var);
        module_FileCount_Hash.insert(var,x+1);
    }
    return module_FileCount_Hash;
}
