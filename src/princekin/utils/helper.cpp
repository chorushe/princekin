#include "helper.h"
#include <QCoreApplication>
#include "everything.h"
#include <QByteArray>
using namespace com_ximpleware;

static QStringList glDeviceList;
static QProcess proc;

Helper::Helper()
{
}
Helper::~Helper()
{
}



bool Helper::readConfigXml(const QString & xmlfile)
{
    gText.clear();
    gContentDesc.clear();
    gResourceId.clear();
    gClass.clear();
    gBounds.clear();


    QFile file(xmlfile);
    if(!file.exists())
    {
        QMessageBox::information(NULL,"提示", xmlfile + "不存在");
        file.close();
        return false;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,"提示",xmlfile + "打开失败");
        file.close();
        return false;
    }

    QString mAttrValue;

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"//@bounds");

        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            vn=vg.getNav();
            ap.bind(vn);

            ap.selectXPath(L"//@text");
            while(ap.evalXPath()!=-1)
            {
                t1=vn->getAttrVal(L"text");
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mAttrValue=QString::fromWCharArray(tmpString);
                    gText.append(mAttrValue);
                    delete tmpString;
                }
            }//while(ap.evalXPath()!=-1)
            ap.resetXPath();

            ap.selectXPath(L"//@content-desc");
            while(ap.evalXPath()!=-1)
            {
                t1=vn->getAttrVal(L"content-desc");
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mAttrValue=QString::fromWCharArray(tmpString);
                    gContentDesc.append(mAttrValue);
                    delete tmpString;
                }
            }//while(ap.evalXPath()!=-1)
            ap.resetXPath();


            ap.selectXPath(L"//@resource-id");
            while(ap.evalXPath()!=-1)
            {
                t1=vn->getAttrVal(L"resource-id");
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mAttrValue=QString::fromWCharArray(tmpString);
                    gResourceId.append(mAttrValue);
                    delete tmpString;
                }
            }//while(ap.evalXPath()!=-1)
            ap.resetXPath();


            ap.selectXPath(L"//@class");
            while(ap.evalXPath()!=-1)
            {
                t1=vn->getAttrVal(L"class");
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mAttrValue=QString::fromWCharArray(tmpString);
                    gClass.append(mAttrValue);
                    delete tmpString;
                }
            }//while(ap.evalXPath()!=-1)
            ap.resetXPath();


            while(ap.evalXPath()!=-1)
            {
                t1=vn->getAttrVal(L"bounds");
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mAttrValue=QString::fromWCharArray(tmpString);
                    gBounds.append(mAttrValue);
                    delete tmpString;
                }
            }//while(ap.evalXPath()!=-1)
            ap.resetXPath();

            delete filename;
            delete vn;
            delete tmpString;
        }
    }
    catch(...)
    {
        delete filename;
        delete vn;
        delete tmpString;
    }

    return true;
}

void Helper::createRepository(const QString &arg_repoDir)
{
    QString mStr="";

    gRepositoryDir=QDir::toNativeSeparators(arg_repoDir);
    createPath(gRepositoryDir);

   /* mStr=gRepositoryDir + QDir::separator() + "result";
    mStr=QDir::toNativeSeparators(mStr);
    gResultDir=mStr;
    createPath(mStr);*/

    mStr=gRepositoryDir + QDir::separator() + "uiautomatorjava";
    mStr=QDir::toNativeSeparators(mStr);
    gUiautomatorDir=mStr;
    createPath(mStr);

    mStr=gRepositoryDir + QDir::separator() + "debug";
    mStr=QDir::toNativeSeparators(mStr);
    gDebugDir=mStr;
    createPath(mStr);

    mStr=gRepositoryDir + QDir::separator() + "testcase";
    mStr=QDir::toNativeSeparators(mStr);
    gTreeRootDir=mStr;
    createPath(mStr);

    mStr=gTreeRootDir + QDir::separator() + "script";
    mStr=QDir::toNativeSeparators(mStr);
    gModuleDir=mStr;
    createPath(gModuleDir);

    mStr=gTreeRootDir + QDir::separator() + "invoke";
    mStr=QDir::toNativeSeparators(mStr);
    gInvokeDir=mStr;
    createPath(mStr);

    mStr=gTreeRootDir + QDir::separator() + "command";
    mStr=QDir::toNativeSeparators(mStr);
    gCommandDir=mStr;
    createPath(mStr);

    mStr=gTreeRootDir + QDir::separator() + "suite";
    mStr=QDir::toNativeSeparators(mStr);
    gSuiteDir=mStr;
    createPath(mStr);

    QStringList mFilterList;
    mFilterList<<"*.txt";

    mStr=gConfigDir + QDir::separator() + "command";
    QDir dir(mStr);
    QStringList mstrList=dir.entryList(mFilterList);
    foreach (QString filename, mstrList)
    {
        QFile::copy(mStr + QDir::separator() + filename,gCommandDir + QDir::separator() + filename);
    }

    mStr=gTreeRootDir + QDir::separator() + "var";
    mStr=QDir::toNativeSeparators(mStr);
    gVarDir=mStr;
    QString mVarDir=mStr;
    createPath(mStr);


    QFile file;
    file.setFileName(mVarDir + QDir::separator() + "var.txt");
    if(file.exists())
    {
        file.close();
        return;
    }
    file.open(QIODevice::WriteOnly);
    file.close();

    file.setFileName(mVarDir + QDir::separator() + "var.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.close();
        return;
    }
}

void Helper::loadRepository(const QString &arg_repoDir)//录制时涉及到的
{
    QString mStr="";

    gRepositoryDir=QDir::toNativeSeparators(arg_repoDir);

    mStr=gRepositoryDir + QDir::separator() + "result";
    mStr=QDir::toNativeSeparators(mStr);
    gResultDir=mStr;


    mStr=gRepositoryDir + QDir::separator() + "uiautomatorjava";
    mStr=QDir::toNativeSeparators(mStr);
    gUiautomatorDir=mStr;


    mStr=gRepositoryDir + QDir::separator() + "debug";
    mStr=QDir::toNativeSeparators(mStr);
    gDebugDir=mStr;


    mStr=gRepositoryDir + QDir::separator() + "testcase";
    mStr=QDir::toNativeSeparators(mStr);
    //加入判断testcase文件夹是否存在，若不存在则不是正确的脚本库，所以gmoduleDir2赋值为当前目录，在加载脚本库的时候也显示当前目录
    if(!QDir(mStr).exists())
    {
        gTreeRootDir=arg_repoDir;
        qDebug()<<"gTreeRootDir: "<<gTreeRootDir;
        QMessageBox::information(0,"提示","所选路径不是脚本库路径！");
        return;
    }
    gTreeRootDir=mStr;
    qDebug()<<"gTreeRootDir: "<<gTreeRootDir;


    mStr=gTreeRootDir + QDir::separator() + "script";
    mStr=QDir::toNativeSeparators(mStr);
    gModuleDir=mStr;


    mStr=gTreeRootDir + QDir::separator() + "invoke";
    mStr=QDir::toNativeSeparators(mStr);
    gInvokeDir=mStr;

    mStr=gTreeRootDir + QDir::separator() + "command";
    mStr=QDir::toNativeSeparators(mStr);
    gCommandDir=mStr;

    mStr=gTreeRootDir + QDir::separator() + "suite";
    mStr=QDir::toNativeSeparators(mStr);
    gSuiteDir=mStr;

    mStr=gTreeRootDir + QDir::separator() + "var";
    mStr=QDir::toNativeSeparators(mStr);
    gVarDir=mStr;

    /*
    QFile file;
    createfile(gConfigDir + QDir::separator() + "loadRepoPath.txt");
    file.setFileName(gConfigDir + QDir::separator() + "loadRepoPath.txt");
    if(!file.open(QIODevice::WriteOnly))
    {
        file.close();
        return;
    }

    QTextStream mStream(&file);
    mStream<<gRepositoryDir<<endl;
    file.close();
    */
}

void Helper::loadRepository2(const QString &arg_repositoryDir)//运行时涉及到的
{
    QString mStr="";

    gRepositoryDir2=QDir::toNativeSeparators(arg_repositoryDir);


    mStr=gRepositoryDir2 + QDir::separator() + "result";
    mStr=QDir::toNativeSeparators(mStr);
    gResultDir2=mStr;


    mStr=gRepositoryDir2 + QDir::separator() + "uiautomatorjava";
    mStr=QDir::toNativeSeparators(mStr);
    gUiautomatorDir2=mStr;


    mStr=gRepositoryDir2 + QDir::separator() + "debug";
    mStr=QDir::toNativeSeparators(mStr);
    gDebugDir2=mStr;


    mStr=gRepositoryDir2 + QDir::separator() + "testcase";
    mStr=QDir::toNativeSeparators(mStr);
    gTreeRootDir2=mStr;

    //加入判断testcase文件夹是否存在，若不存在则不是正确的脚本库，所以gmoduleDir2赋值为当前目录，在加载脚本库的时候也显示当前目录
    if(!QDir(gTreeRootDir2).exists())
    {
        gModuleDir2=arg_repositoryDir;
        qDebug()<<"gModuleDir2: "<<gModuleDir2;
        QMessageBox::information(0,"提示","所选路径不是脚本库路径！");
        return;
    }

    mStr=gTreeRootDir2 + QDir::separator() + "script";
    mStr=QDir::toNativeSeparators(mStr);
    gModuleDir2=mStr;
    qDebug()<<"gModuleDir2: "<<gModuleDir2;


    mStr=gTreeRootDir2 + QDir::separator() + "invoke";
    mStr=QDir::toNativeSeparators(mStr);
    gInvokeDir2=mStr;

    mStr=gTreeRootDir2 + QDir::separator() + "方法库";
    mStr=QDir::toNativeSeparators(mStr);
    gCommandDir2=mStr;

    mStr=gTreeRootDir2 + QDir::separator() + "var";
    mStr=QDir::toNativeSeparators(mStr);
    gVarDir2=mStr;
}



void Helper::createPath(const QString &arg_path)
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


void Helper::createfile(const QString &arg_filename)
{
    QFile file(arg_filename);
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}


void Helper::initVar()
{
    gDeviceId="";
    gAppDir=QCoreApplication::applicationDirPath() + QDir::separator();
    gConfigDir=gAppDir + "config";
    gConfigDir=QDir::toNativeSeparators(gConfigDir);    
    gtempConfigDir=gAppDir + "config";
    gtempConfigDir=QDir::toNativeSeparators(gtempConfigDir);
    gStatisticsConfigDir=gAppDir + "statistics" + QDir::separator() + "config" + QDir::separator();
    gStatisticsXmlDir=gAppDir + "statistics" + QDir::separator() + "xml" + QDir::separator();

    gApkDir=gConfigDir + QDir::separator() + "apk";
    gApkDir=QDir::toNativeSeparators(gApkDir);
    gStatisticsConfigDir=gAppDir + "statistics" + QDir::separator() + "config" + QDir::separator();
    gStatisticsXmlDir=gAppDir + "statistics" + QDir::separator() + "xml" + QDir::separator();
}

void Helper::initEvent()
{
    gHashEvent.insert("KEY_BACK","back");
    gHashEvent.insert("KEY_HOMEPAGE","home");
    gHashEvent.insert("KEY_HOME","home");
    gHashEvent.insert("KEY_MENU","menu");
    gHashEvent.insert("KEY_VOLUMEUP","volumeUp");
    gHashEvent.insert("KEY_VOLUMEDOWN","volumeDown");
    gHashEvent.insert("KEY_POWER","power");

    gEventList.append("KEY_BACK");
    gEventList.append("KEY_HOMEPAGE");
    gEventList.append("KEY_HOME");
    gEventList.append("KEY_MENU");
}

void Helper::initConfig()
{
    QString xmlPath=gConfigDir + QDir::separator() + "config.xml";
    bool f=ParseXml::readConfigXml(xmlPath);
    if(f)
    {
    }
    else
    {
        return;
    }

    gAndroidJarDir=gHashConfig.value("androidJarDir");
    gAndroidJarDir=QDir::toNativeSeparators(gAndroidJarDir);

    QHash<QString,QString>::iterator i;
    for(i=gHashConfig.begin();i!=gHashConfig.end();++i)
        qDebug()<<i.key()<<":"<<i.value();

    //gRepositoryDir=gHashConfig.value("repositoryDir");
    //createRepository(gRepositoryDir);

}

void Helper::initCurrentRepository()
{
    QString strFile=gNewConfigDir+QDir::separator()+"currentRepository.txt";
    QFile file;
    file.setFileName(strFile);

    if(file.exists())
    {
        if(!file.open(QIODevice::ReadOnly))
        {
            file.close();
            QMessageBox::information(NULL,"提示", "currentRepository 文件异常");
            return;
        }
        QTextStream inStream(&file);
        QString strPath=inStream.readLine();
        file.close();
        strPath=QDir::toNativeSeparators(strPath);
        QFileInfo fileInfo(strPath);
        if(fileInfo.exists() && fileInfo.isDir())
        {
            gRepositoryName=fileInfo.baseName();
            gMyWorkSpace=fileInfo.absolutePath();
            gRepositoryDir=strPath;
            createRepository(strPath);
        }
        else
        {
            gTreeRootDir="";
        }
    }
    else
    {
        gTreeRootDir="";
        createfile(strFile);
    }
}

void Helper::writeStr(const QString &arg_filename, const QString &arg_str)
{
    QFile file(arg_filename);
    QTextStream outStream(&file);
    if(!file.open(QIODevice::WriteOnly))
    {
        file.close();
        QMessageBox::information(NULL,"提示", arg_filename + "文件异常");
        return;
    }
    outStream<<arg_str<<endl;
    file.close();
}

QString Helper::getPackageName(const QString &arg_file)
{
    QString mstrLine;
    QString mPackageName;
    QString mStr=arg_file;
    QFile mFile;
    mFile.setFileName(mStr);
    mFile.open(QIODevice::ReadOnly);


    QTextStream mInSteam(&mFile);
    while(!mInSteam.atEnd())
    {
        mstrLine=mInSteam.readLine();
        if(!mstrLine.startsWith("//"))
        {
            mPackageName=mstrLine;
        }
    }
    mFile.close();

    return mPackageName;
}


QString Helper::getTime()
{
    QDateTime currentTime=QDateTime::currentDateTime();
    QString time=currentTime.toString("yyyy-MM-dd_hh-mm-ss");
    return time;
}

QString Helper::getTime2(const QString &arg_format)
{
    QDateTime currentTime=QDateTime::currentDateTime();
    QString time=currentTime.toString(arg_format);
    return time;
}


QHash<QString,QString> Helper::getBrand(const QString &arg_file)
{
    QString strLine;
    QString strTemp;

    QStringList splitResult;
    QHash<QString,QString> hash;

    strTemp=arg_file;
    strTemp=QDir::toNativeSeparators(strTemp);

    QFile file(strTemp);

    if(file.open(QIODevice::ReadOnly))
    {
        QTextCodec *tc=QTextCodec::codecForName("gb2312");
        QTextStream inStream(&file);
        inStream.setCodec(tc);

        while(!inStream.atEnd())
        {
            strLine=inStream.readLine();
            if(!strLine.startsWith("//"))
            {
                splitResult=strLine.split("=");
                if(splitResult.size()==2)
                {
                    hash.insert(splitResult.at(0),splitResult.at(1));
                }
            }
        }
    }
    file.close();

    return hash;
}

QStringList Helper::getList(const QString &arg_file)
{
    QString strLine;
    QString strTemp;

    QStringList list;

    strTemp=arg_file;
    strTemp=QDir::toNativeSeparators(strTemp);

    QFile file(strTemp);

    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream inStream(&file);
        while(!inStream.atEnd())
        {
            strLine=inStream.readLine();
            if(!strLine.startsWith("//"))
            {
                list.append(strLine.trimmed());
            }
        }
    }

    file.close();

    return list;
}


bool Helper::copyDir(const QString &arg_source, const QString &arg_target, bool override)
{
    QDir directory(arg_source);
    if (!directory.exists())
    {
        return false;
    }

    QString srcPath = QDir::toNativeSeparators(arg_source);
    if (!srcPath.endsWith(QDir::separator()))
    {
        srcPath += QDir::separator();
    }

    QString dstPath = QDir::toNativeSeparators(arg_target);
    if (!dstPath.endsWith(QDir::separator()))
    {
        dstPath += QDir::separator();
    }


    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;

        QDir srcDir(srcFilePath);
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            QDir dir(dstFilePath);
            QFileInfo info(dstFilePath);
            if(info.exists())
            {
                QString parentDir=info.absolutePath();
                QString baseName=info.baseName();
                QString strTime=getTime();
                dir.rename(dstFilePath,parentDir+QDir::separator()+baseName+"_"+strTime+".txt");
                dir.remove(dstFilePath);
            }


            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);

            srcDir.remove(srcFilePath);
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }

    return !error;
}


QString Helper::getFirstLine(const QString &arg_file)
{
    QString mstrLine;
    QString mStr=arg_file;
    QFile mFile;

    mFile.setFileName(mStr);
    if( mFile.open(QIODevice::ReadOnly) )
    {
        QTextStream mInSteam(&mFile);
        mstrLine=mInSteam.readLine();
    }

    mFile.close();

    return mstrLine;
}

QString Helper::getAllText(const QString &arg_file)
{
    QString mstrLine;
    QString mStr=arg_file;
    QFile mFile;

    mFile.setFileName(mStr);
    if( mFile.open(QIODevice::ReadOnly) )
    {
        QTextStream mInSteam(&mFile);
        mstrLine=mInSteam.readAll();
    }

    mFile.close();

    return mstrLine;
}


void Helper::createDir(const QString &arg_dir)
{
    QDir dir(arg_dir);
    bool b=dir.mkdir(arg_dir);
}


void Helper::getWorkspace()
{
    QString strFile=gNewConfigDir+QDir::separator()+"workspace.txt";
    QFile file;
    file.setFileName(strFile);

    if(file.exists())
    {
        gNewWorkSpaceDir=getFirstLine(strFile);
        gNewWorkSpaceDir=QDir::toNativeSeparators(gNewWorkSpaceDir);

        gWorkSpace=gNewWorkSpaceDir;
    }
    else
    {
        gNewWorkSpaceDir=gNewDisk + QDir::separator() + "princekinWorkspace";
        gNewWorkSpaceDir=QDir::toNativeSeparators(gNewWorkSpaceDir);
        gWorkSpace=gNewWorkSpaceDir;
        createDir(gWorkSpace);
        writeStr(strFile,gNewWorkSpaceDir);
    }
}


bool Helper::isPackageName1(const QString &arg_selectedPackageName)
{
    bool ok=true;
    QString savedPackageName=getFirstLine(gConfigDir + QDir::separator() + "packageName.txt");
    if(arg_selectedPackageName!=savedPackageName && arg_selectedPackageName!="")
    {
        QMessageBox::StandardButton jValue=QMessageBox::information(NULL, "提示", "所选应用与配置信息不一致,会导致(APP图标,启动时间,安装包大小,版本号)等信息获取为空.是否继续测试?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(jValue==QMessageBox::Yes)
        {
            ok=true;
        }
        else
        {
            ok=false;
        }
    }
    return ok;
}

bool Helper::isCopyConfig(const QString &arg_version)
{
    bool isCopy=false;
    QString versionFile;
    QString oldVersion;

    versionFile=gNewConfigDir + QDir::separator() + "version.txt";
    QFileInfo info(versionFile);

    if(info.exists())
    {
        oldVersion=getFirstLine(versionFile);
        if(oldVersion==arg_version)
        {
            isCopy=false;
        }
        else
        {
            writeStr(versionFile,arg_version);
            isCopy=true;
        }
    }
    else
    {
        writeStr(versionFile,arg_version);
        isCopy=true;
    }
    return isCopy;
}


void Helper::copyXml()
{
    QString srcFilePath;
    QString targetFilePath;

    QDir dir("d:");
    if(dir.exists())
    {
        gNewDisk="D:";
    }
    else
    {
        gNewDisk="C:";
    }

    QString localDir=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    int index=localDir.lastIndexOf("/");
    localDir=localDir.left(index);

    //gNewConfigDir=gNewDisk + QDir::separator() + "princekinConfig";
    gNewConfigDir=localDir + QDir::separator() + "princekinConfig";
    gNewConfigDir=QDir::toNativeSeparators(gNewConfigDir);
    Helper::createDir(gNewConfigDir);

    srcFilePath=gConfigDir + QDir::separator() + "modifyconfig" + QDir::separator() + "config.xml";
    QFileInfo info(srcFilePath);
    if(info.exists())
    {
        targetFilePath=gNewConfigDir + QDir::separator() + "config.xml";
        copyXml(srcFilePath,targetFilePath,"config_");
    }

    srcFilePath=gConfigDir + QDir::separator() + "modifyconfig" + QDir::separator() + "generalconfig.xml";
    info.setFile(srcFilePath);
    if(info.exists())
    {
        targetFilePath=gNewConfigDir + QDir::separator() + "generalconfig.xml";
        copyXml(srcFilePath,targetFilePath,"generalconfig_");
    }

    gConfigDir=gNewConfigDir;
    gApkDir=gConfigDir + QDir::separator() + "apk";
    gApkDir=QDir::toNativeSeparators(gApkDir);
}

void Helper::copyXml(const QString &arg_srcFilePath,const QString &arg_targetFilePath,const QString &arg_mark)
{
    QDir dir(arg_targetFilePath);
    QFileInfo info(arg_targetFilePath);
    if(info.exists())
    {
        /*
        QString strTime=Helper::getTime();
        dir.rename(arg_targetFilePath,gNewConfigDir+QDir::separator() + arg_mark + strTime + ".xml");
        dir.remove(arg_targetFilePath);
        */
    }
    else
    {
        QFile::copy(arg_srcFilePath, arg_targetFilePath);
    }

}


bool Helper::copyNormal(const QString &arg_source, const QString &arg_target, bool override)
{
    QDir directory(arg_source);
    if (!directory.exists())
    {
        return false;
    }

    QString srcPath = QDir::toNativeSeparators(arg_source);
    if (!srcPath.endsWith(QDir::separator()))
    {
        srcPath += QDir::separator();
    }

    QString dstPath = QDir::toNativeSeparators(arg_target);
    if (!dstPath.endsWith(QDir::separator()))
    {
        dstPath += QDir::separator();
    }


    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;

        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            QDir dir(dstFilePath);
            QFileInfo info(dstFilePath);
            if(info.exists())
            {
                dir.remove(dstFilePath);
            }


            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyNormal(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }

    return !error;
}

bool Helper::copyModify(const QString &arg_source, const QString &arg_target, bool override)
{
    QDir directory(arg_source);
    if (!directory.exists())
    {
        return false;
    }

    QString srcPath = QDir::toNativeSeparators(arg_source);
    if (!srcPath.endsWith(QDir::separator()))
    {
        srcPath += QDir::separator();
    }

    QString dstPath = QDir::toNativeSeparators(arg_target);
    if (!dstPath.endsWith(QDir::separator()))
    {
        dstPath += QDir::separator();
    }

    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;

        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
            QFileInfo info(dstFilePath);
            if(!info.exists())
            {
                QFile::copy(srcFilePath, dstFilePath);
            }
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyModify(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }

    return !error;
}

