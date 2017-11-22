#include "filehandle.h"

FileHandle::FileHandle()
{

}


void FileHandle::createJava(const QString &arg_javafile)
{
    QFile file(arg_javafile);
    if(file.exists())
    {
        file.close();
        return;
    }
    file.open(QIODevice::WriteOnly);
    file.close();
}

void FileHandle::createPath(const QString &arg_path)
{
    QDir mDir(arg_path);
    if(mDir.exists())
    {
    }
    else
    {
        mDir.mkpath(arg_path);
    }
}


QStringList FileHandle::parsePath(const QString &arg_path)
{
    int len1=gProjectDir.length();
    int len2=arg_path.length();

    QString mRightStr=arg_path.right(len2-len1);
    QStringList mList=mRightStr.split(QDir::separator());
    return mList;
}

QStringList FileHandle::parsePath(const QString &arg_path,const QString &arg_saveDir)
{
    int len1=arg_saveDir.length();
    int len2=arg_path.length();

    QString mRightStr=arg_path.right(len2-len1);
    QStringList mList=mRightStr.split(QDir::separator());
    return mList;
}

QString FileHandle::getBaseName(const QString &currentOpenFile)
{
    QFileInfo info(currentOpenFile);
    return info.baseName();
}

QString FileHandle::getSubPath(const QString &arg_currentOpenFile)
{
    int len1=gProjectDir.length();
    int len2=arg_currentOpenFile.length();

    QString mRightStr=arg_currentOpenFile.right(len2-len1);
    QStringList mList=mRightStr.split(QDir::separator());

    QString s1=mList.at(1);
    QString s2=mList.at(2);
    QString subPath=s1 + QDir::separator() + s2;

    return subPath;
}




bool FileHandle::isD(const QString &arg_path)
{
    QFileInfo info(arg_path);
    if(info.isDir())
    {
        return true;
    }
    else
    {
        return false;
    }
}

