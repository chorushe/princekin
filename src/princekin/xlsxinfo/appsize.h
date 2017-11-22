#ifndef APPSIZE_H
#define APPSIZE_H

#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QFile>

#include "execmd.h"
#include "globalvar.h"

class AppSize
{
public:
    AppSize();
    QString getSize(const QString &);
};

#endif // APPSIZE_H
