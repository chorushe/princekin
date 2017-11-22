#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include "commonhead.h"
#include "globalvar.h"

class FileHandle
{
public:
    FileHandle();


public:

    void createPath(const QString&);
    void createJava(const QString&);
    QString getBaseName(const QString &);
    QString getSubPath(const QString &);
    QStringList parsePath(const QString &);
    QStringList parsePath(const QString &,const QString &);

    bool isD(const QString &);

};

#endif // FILEHANDLE_H
