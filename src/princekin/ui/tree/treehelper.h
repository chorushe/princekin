#ifndef TREEHELPER_H
#define TREEHELPER_H

#include "commonhead.h"
#include "componenthead.h"
#include "filemanager.h"

class TreeHelper : public FileManager
{
public:
    TreeHelper();
public:
    void createProject(const QString &);
    void createModel(const QString &,const QString &);
    void createScript(const QString &,const QString &);
};

#endif // TREEHELPER_H
