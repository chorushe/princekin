#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "commonhead.h"
#include "componenthead.h"
#include "globalvar.h"


class FileManager
{
public:
    FileManager();
public:

    void createProject(const QString &);
    bool createModule(const QString &,const QString &);
    bool createSuiteModule(const QString &,const QString &);
    bool createScript(const QString &,const QString &);

    void setCurrentOpenFile(const QString &);
    void setLoopNumber(int);


    void scriptToUiautomator(const QString&);
    void createPath(const QString&);
    void createJava(const QString&);
    void createUiautomator(const QString &,QStringList);

    void readVar(const QString &);
    void readKeyCode(const QString &);

    void testDemo(QStringList);
    void parseTempScript(QStringList);

    QString readAllFile(const QString &);
    QStringList convertScriptToTemp(const QString &);
    QStringList parseInvoke(const QString &);

    bool isD(const QString &);
    QString getBaseName(const QString &);
    QString getSubPath(const QString &);
    QStringList parsePath(const QString &);


    //*************************事件处理***************************//
    void pressBack(const QString &);
    void fastBack(const QString &);
    void pressHome(const QString &);
    void pressMenu(const QString &);


    void pushApk(const QString &,const QString &,const QString &,const QString &);
    void unOrInstallOrStart(const QString &,const QString &,const QString &,const QString &);
    void startApp(const QString &,const QString &,const QString &,const QString &);
    void checkStartApp(const QString &,const QString &,const QString &,const QString &);

    void checkStatPoint(const QString &);

    void createString(const QString &,const QString &,const QString &,const QString &);
    void createStringNotExit(const QString &,const QString &,const QString &);


    void clickIdOrClass(const QString &,const QString &,const QString &,const QString &);
    void clickIdOrClassNotExit(const QString &,const QString &,const QString &);

    void scrollIdOrClass(const QString &,const QString &,const QString &,const QString &);
    void scrollIdOrClassNotExit(const QString &,const QString &,const QString &);



    void createIntBool(const QString &,const QString &,const QString &,const QString &);
    void createIntBoolNotExit(const QString &,const QString &,const QString &);


    void createNoParameter(const QString &,const QString &);
    void createNoParameterNotExit(const QString &);

    void createIntBool_scroll(const QString &,const QString &,const QString &,const QString &);
    void createIntBool_scrollNotReturn(const QString &,const QString &,const QString &);


    void sohuPrint(const QString &,const QString &);

    void takeScreen(const QString &,const QString &);


    void writeOK(const QString &);
    void writeOKStart(const QString &);
    void sleep(const QString &,const QString &);
    void otherLine(const QString &);



    QStringList getSpaceLine(const QString &);
    QStringList getSplist(const QString &,const QString &);



    //*************************事件处理***************************//

public:
    int loopNumber;
    QString subPath;
    QString baseName;
    QString currentOpenFile;
    QTextStream *stream;

    QString strClickOK="isClickOK";
    QString strCheckOK="isCheckOK";
    QString strScrollOK="isScrollOK";
    QString strStartOK="isStartOK";
    QString strInstallOK="isInstallOK";
    QString strUninstallOK="isUninstallOK";
    QString strOK="isOK";

    QTextCodec *tc=QTextCodec::codecForName("gb2312");




};

#endif // FILEMANAGER_H
