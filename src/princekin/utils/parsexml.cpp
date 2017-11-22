#include "parsexml.h"
#include "globalvar.h"
#include "everything.h"
#include <QByteArray>
using namespace com_ximpleware;

void clearPvHash();
void clearVvHash();




ParseXml::ParseXml()
{
}

void ParseXml::saveConfigXmlIP_Port(QString IP,QString port)
{
    QString fileName=gConfigDir + QDir::separator() + "config.xml";
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(!file.exists())
        {
            QMessageBox::warning(NULL,"警告",fileName+" 文件不存在！");
            file.close();
            return;
        }
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(NULL,"警告",fileName+" 打开错误！");
            file.close();
            return;
        }

        QDomDocument document;
        QString error;
        int row = 0, column = 0;
        if(!document.setContent(&file, false, &error, &row, &column))
        {
            QMessageBox::information(NULL, QString("提示"), QString("解析文件错误，行： ") + QString::number(row, 10) + QString(" ,列： ") + QString::number(column, 10));
            return ;
        }
        if(document.isNull())
        {
            QMessageBox::information(NULL, QString("提示"), QString("文件为空!"));
            return ;
        }
        QDomElement root = document.documentElement();
        QDomElement docElem = document.documentElement();
        QDomNodeList nodeList = docElem.elementsByTagName("IP");
        qDebug()<<nodeList.count();
        if (nodeList.count() >0 )
        {
            QDomNode node = nodeList.at(0);
            QDomNode oldnode = node.firstChild();
            node.firstChild().setNodeValue(IP);
            QDomNode newnode = node.firstChild();
            node.replaceChild(newnode,oldnode);
        }
        nodeList = docElem.elementsByTagName("Port");
        if (nodeList.count() >0 )
        {
            QDomNode node = nodeList.at(0);
            QDomNode oldnode = node.firstChild();
            node.firstChild().setNodeValue(port);
            QDomNode newnode = node.firstChild();
            node.replaceChild(newnode,oldnode);
        }

        QFile filexml(fileName);
        if(!filexml.exists())
        {
            QMessageBox::warning(NULL,"警告",fileName+" 文件不存在！");
            filexml.close();
            return;
        }
        if(!filexml.open(QFile::WriteOnly | QFile::Truncate))
        {
            QMessageBox::warning(NULL,"警告",fileName+" 打开错误！");
            filexml.close();
            return;
        }
        QTextStream ts(&filexml);
        ts.reset();
        ts.setCodec("utf-8");
        document.save(ts, 4, QDomNode::EncodingFromTextStream);
        filexml.close();
    }
}

bool ParseXml::readPvXml(const QString & xmlfile,const QString & xmlfile2)
{
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


    QFile file2(xmlfile2);
    if(!file2.exists())
    {
        QMessageBox::information(NULL,"提示",xmlfile2 +"不存在");
        file.close();
        return false;
    }
    if(!file2.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,"提示",xmlfile2 + "打开失败");
        file.close();
        return false;
    }








    //QString mGbkDesValue;
    //QTextCodec *mUtf8 = QTextCodec::codecForName("utf-8");
    QString mNodeName;
    QString mNodeValue;
    QString mDesValue;

    QString urlLineJoin;
    QString otherLineJoin;

    QString fgf=">>";

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*/*");
        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            gPvStringList.clear();
            vn=vg.getNav();
            ap.bind(vn);
            while(ap.evalXPath()!=-1)
            {
                urlLineJoin="";
                otherLineJoin="";

                t1=vn->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mNodeName=QString::fromWCharArray(tmpString);
                    delete tmpString;

                    if(mNodeName=="url")
                    {
                        urlLineJoin=urlLineJoin + mNodeName + fgf;

                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);
                            urlLineJoin=urlLineJoin + mNodeValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"des");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            urlLineJoin=urlLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"serial");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            urlLineJoin=urlLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"onlyCheck");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            urlLineJoin=urlLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }


                        gPvStringList.append(urlLineJoin);
                    }//if(mNodeName="url")
                    else
                    {
                        otherLineJoin=otherLineJoin + mNodeName + fgf;

                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mNodeValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"des");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"onlyCheck");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        gPvStringList.append(otherLineJoin);
                    }//else
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)

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

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    char *filename2;
    wchar_t *tmpString2;
    VTDNav *vn2=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*/*");

        QByteArray byteArray=xmlfile2.toLatin1();
        filename2=byteArray.data();

        if(vg.parseFile(true,filename2))
        {

            gPvStringList2.clear();
            vn2=vg.getNav();
            ap.bind(vn2);
            while(ap.evalXPath()!=-1)
            {
                urlLineJoin="";
                otherLineJoin="";

                t1=vn2->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString2=vn2->toNormalizedString(t1);
                    mNodeName=QString::fromWCharArray(tmpString2);
                    delete tmpString2;

                    urlLineJoin=urlLineJoin + mNodeName + fgf;

                    t2=vn2->getText();//节点值
                    if(t2!=-1)
                    {
                        tmpString2=vn2->toNormalizedString(t2);
                        mNodeValue=QString::fromWCharArray(tmpString2);
                        urlLineJoin=urlLineJoin + mNodeValue + fgf;
                        delete tmpString2;
                    }

                    t2=vn2->getAttrVal(L"contains");
                    if(t2!=-1)
                    {
                        tmpString2=vn2->toNormalizedString(t2);
                        mDesValue=QString::fromWCharArray(tmpString2);
                        urlLineJoin=urlLineJoin + mDesValue + fgf;
                        delete tmpString2;
                    }

                    t2=vn2->getAttrVal(L"serial");
                    if(t2!=-1)
                    {
                        tmpString2=vn2->toNormalizedString(t2);
                        mDesValue=QString::fromWCharArray(tmpString2);
                        urlLineJoin=urlLineJoin + mDesValue + fgf;
                        delete tmpString2;
                    }
                    gPvStringList2.append(urlLineJoin);
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)

            delete filename2;
            delete vn2;
            delete tmpString2;
        }
    }
    catch(...)
    {
        delete filename2;
        delete vn2;
        delete tmpString2;
    }





    return true;
}

bool ParseXml::readVvXml(const QString & xmlfile)
{
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


    QString mNodeName;
    QString mNodeValue;
    QString mDesValue;

    QString urlLineJoin;
    QString otherLineJoin;

    QString fgf=">>";

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*/*");

        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            gVvStringList.clear();
            vn=vg.getNav();
            ap.bind(vn);
            while(ap.evalXPath()!=-1)
            {
                urlLineJoin="";
                otherLineJoin="";

                t1=vn->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mNodeName=QString::fromWCharArray(tmpString);
                    delete tmpString;



                        otherLineJoin=otherLineJoin + mNodeName + fgf;

                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mNodeValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"des");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"onlyCheck");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }
                        gVvStringList.append(otherLineJoin);
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)
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


bool ParseXml::readBdXml(const QString & xmlfile)
{
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


    QString mNodeName;
    QString mNodeValue;
    QString mDesValue;

    QString urlLineJoin;
    QString otherLineJoin;

    QString fgf=">>";

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*/*");

        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            gBdStringList.clear();
            vn=vg.getNav();
            ap.bind(vn);
            while(ap.evalXPath()!=-1)
            {
                urlLineJoin="";
                otherLineJoin="";

                t1=vn->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mNodeName=QString::fromWCharArray(tmpString);
                    delete tmpString;



                        otherLineJoin=otherLineJoin + mNodeName + fgf;

                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mNodeValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"des");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"onlyCheck");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        gBdStringList.append(otherLineJoin);
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)
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


bool ParseXml::readCatonXml(const QString & xmlfile)
{
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


    QString mNodeName;
    QString mNodeValue;
    QString mDesValue;

    QString urlLineJoin;
    QString otherLineJoin;

    QString fgf=">>";

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*/*");

        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            gCatonStringList.clear();
            vn=vg.getNav();
            ap.bind(vn);
            while(ap.evalXPath()!=-1)
            {
                urlLineJoin="";
                otherLineJoin="";

                t1=vn->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mNodeName=QString::fromWCharArray(tmpString);
                    delete tmpString;



                        otherLineJoin=otherLineJoin + mNodeName + fgf;

                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mNodeValue + fgf;
                            delete tmpString;
                        }









                        t2=vn->getAttrVal(L"des");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        t2=vn->getAttrVal(L"onlyCheck");
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mDesValue=QString::fromWCharArray(tmpString);
                            otherLineJoin=otherLineJoin + mDesValue + fgf;
                            delete tmpString;
                        }

                        gCatonStringList.append(otherLineJoin);
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)
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

bool ParseXml::readConfigXml(const QString & xmlfile)
{

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


    gHashConfig.clear();

    QString mNodeName;
    QString mNodeValue;

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*");

        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            gCatonStringList.clear();
            vn=vg.getNav();
            ap.bind(vn);
            while(ap.evalXPath()!=-1)
            {
                t1=vn->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);
                    mNodeName=QString::fromWCharArray(tmpString);
                    delete tmpString;

                    t2=vn->getText();//节点值
                    if(t2!=-1)
                    {
                        tmpString=vn->toNormalizedString(t2);
                        mNodeValue=QString::fromWCharArray(tmpString);

                        gHashConfig.insert(mNodeName,mNodeValue);
                        delete tmpString;
                    }
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)
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

QStringList ParseXml::readConfigXmlForBlackList(const QString & xmlfile)
{
    QStringList blackList;
    QFile file(xmlfile);
    if(!file.exists())
    {
        QMessageBox::information(NULL,"提示", xmlfile + "不存在");
        file.close();
        return blackList;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,"提示",xmlfile + "打开失败");
        file.close();
        return blackList;
    }

    QString mNodeName;
    QString mNodeValue;

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*/*");

        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            gCatonStringList.clear();
            vn=vg.getNav();
            ap.bind(vn);
            while(ap.evalXPath()!=-1)
            {
                t1=vn->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);

                    mNodeName=QString::fromWCharArray(tmpString);
                    delete tmpString;
                    if(mNodeName=="item")
                    {
                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);

                            blackList.append(mNodeValue);
                            delete tmpString;
                        }
                    }
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)
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
    return blackList;
}

QStringList ParseXml::readConfigXmlForIP_Port(const QString & xmlfile)
{
    QStringList strList;
    QString IP;
    QString Port;
    QFile file(xmlfile);
    if(!file.exists())
    {
        QMessageBox::information(NULL,"提示", xmlfile + "不存在");
        file.close();
        return strList;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,"提示",xmlfile + "打开失败");
        file.close();
        return strList;
    }

    QString mNodeName;
    QString mNodeValue;

    char *filename;
    wchar_t *tmpString;
    VTDNav *vn=NULL;
    try
    {
        int t1;
        int t2;

        VTDGen vg;
        AutoPilot ap;

        ap.selectXPath(L"/*/*");

        QByteArray byteArray=xmlfile.toLatin1();
        filename=byteArray.data();

        if(vg.parseFile(true,filename))
        {
            gCatonStringList.clear();
            vn=vg.getNav();
            ap.bind(vn);
            while(ap.evalXPath()!=-1)
            {
                t1=vn->getCurrentIndex();//节点名称
                if(t1!=-1)
                {
                    tmpString=vn->toNormalizedString(t1);

                    mNodeName=QString::fromWCharArray(tmpString);
                    delete tmpString;
                    if(mNodeName=="IP")
                    {
                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);

                            IP=mNodeValue;
                            delete tmpString;
                        }
                    }
                    else if(mNodeName=="Port")
                    {
                        t2=vn->getText();//节点值
                        if(t2!=-1)
                        {
                            tmpString=vn->toNormalizedString(t2);
                            mNodeValue=QString::fromWCharArray(tmpString);

                            Port=mNodeValue;
                            delete tmpString;
                        }
                    }
                }//if(t1!=-1)
            }//while(ap.evalXPath()!=-1)
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
    strList.append(IP);
    strList.append(Port);
    return strList;
}

QString ParseXml::EditMultiXml(QString filePath,QStringList listData)
{
    QString res="";
    QString fileName=filePath;
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(!file.exists())
        {
            QMessageBox::warning(NULL,"警告",fileName+" 文件不存在！");
            file.close();
            return "";
        }
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(NULL,"警告",fileName+" 打开错误！");
            file.close();
            return "";
        }

        QDomDocument document;
        QString error;
        int row = 0, column = 0;
        if(!document.setContent(&file, false, &error, &row, &column))
        {
            QMessageBox::information(NULL, QString("提示"), QString("解析文件错误，行： ") + QString::number(row, 10) + QString(" ,列： ") + QString::number(column, 10));
            return "";
        }
        if(document.isNull())
        {
            QMessageBox::information(NULL, QString("提示"), QString("文件为空!"));
            return "";
        }
        QDomElement docElem = document.documentElement();

        for(int i=0;i<listData.count();i++)
        {
            QStringList paramData=listData[i].split("=");// paramdata 字段 参数 描述 验证
            QDomNodeList nodeList = docElem.elementsByTagName(paramData[0]);
            if (nodeList.count() >0 )
            {
                if(paramData[3]=="2"||paramData[3]=="3")//需要验证
                {
                    if(paramData[1].contains(";"))//区分多字段情况
                    {
                        QStringList paramList=paramData[1].split(";");
                        QStringList desList=paramData[2].split(";");
                        qDebug()<<"nodeList.count()"<<nodeList.count();
                        qDebug()<<"paramList.length()"<<paramList.length();
                        if(paramList.length()<=nodeList.count())//修改的个数比原有个数小，直接改
                        {
                            for(int j=0;j<paramList.count();j++)
                            {
                                QDomNode node = nodeList.at(j);
                                QDomNode oldnode = node.firstChild();
                                node.firstChild().setNodeValue(paramList[j]);
                                QDomNode newnode = node.firstChild();
                                node.replaceChild(newnode,oldnode);
                                QDomElement el = nodeList.at(j).toElement();//设置属性值
                                if(desList.count()-1>=j)
                                    el.setAttribute("des",desList[j]);
                                else
                                    el.setAttribute("des",desList[desList.count()-1]);
                                el.setAttribute("onlyCheck","Y");
                            }
                        }
                        else//修改个数比原有个数大，在原有基础上直接改了之后，在新添加子节点
                        {
                            for(int j=0;j<nodeList.count();j++)//原有基础直接改
                            {
                                QDomNode node = nodeList.at(j);
                                QDomNode oldnode = node.firstChild();
                                node.firstChild().setNodeValue(paramList[j]);
                                QDomNode newnode = node.firstChild();
                                node.replaceChild(newnode,oldnode);
                                QDomElement el = nodeList.at(j).toElement();//设置属性值
                                if(desList.count()-1>=j)
                                    el.setAttribute("des",desList[j]);
                                else
                                    el.setAttribute("des",desList[desList.count()-1]);
                                el.setAttribute("onlyCheck","Y");
                            }
                            //添加新节点
                            qDebug()<<"nodeList.count()"<<nodeList.count();
                            qDebug()<<"paramList.length()"<<paramList.length();
                            for(int j=nodeList.count();j<paramList.length();j++)
                            {
                                QDomElement uid = document.createElement(paramData[0]);
                                if(desList.count()-1>=j)
                                    uid.setAttribute("des",desList[j]);
                                else
                                    uid.setAttribute("des",desList[desList.count()-1]);
                                uid.setAttribute("onlyCheck","Y");
                                QDomText node_text;
                                node_text = document.createTextNode(paramList[j]);
                                uid.appendChild(node_text);
                                QDomElement data=nodeList.at(0).parentNode().toElement();
                                data.appendChild(uid);
                                docElem.appendChild(data);
                            }
                        }
                    }
                    else//没有包含多字段
                    {
                        QDomNode node = nodeList.at(0);
                        QDomNode oldnode = node.firstChild();
                        node.firstChild().setNodeValue(paramData[1]);
                        QDomNode newnode = node.firstChild();
                        node.replaceChild(newnode,oldnode);
                        QDomElement el = nodeList.at(0).toElement();//设置属性值
                        el.setAttribute("des",paramData[2]);
                        el.setAttribute("onlyCheck","Y");
                    }
                }
                else if(paramData[3]=="0"||paramData[3]=="1")//不验证或验证非空都没有多字段情况
                {
                    QDomNode node = nodeList.at(0);
                    QDomNode oldnode = node.firstChild();
                    if(paramData[3]=="0")//不验证
                        node.firstChild().setNodeValue("unknown");
                    else if(paramData[3]=="1")//验证非空
                        node.firstChild().setNodeValue("unnull");
                    QDomNode newnode = node.firstChild();
                    node.replaceChild(newnode,oldnode);
                    QDomElement el = nodeList.at(0).toElement();//设置属性值
                    el.setAttribute("des",paramData[2]);
                    el.setAttribute("onlyCheck","N");
                }
                if(paramData[3]=="3")//如果某一个值改为了唯一识别值,要把第一个data节点的unique属性改掉
                {
                    QDomNodeList nodeListUnique = docElem.elementsByTagName("data");
                    if(nodeListUnique.count()>0)
                    {
                        QDomElement elUnique = nodeListUnique.at(0).toElement();//设置属性值
                        elUnique.setAttribute("unique",paramData[0]+"="+paramData[1]);
                    }
                }
            }
            else
            {
                res+=paramData[0]+",";
            }
        }

        QFile filexml(fileName);
        if(!filexml.exists())
        {
            QMessageBox::warning(NULL,"警告",fileName+" 文件不存在！");
            filexml.close();
            return "ERROR";
        }
        if(!filexml.open(QFile::WriteOnly | QFile::Truncate))
        {
            QMessageBox::warning(NULL,"警告",fileName+" 打开错误！");
            filexml.close();
            return "ERROR";
        }
        QTextStream ts(&filexml);
        ts.reset();
        ts.setCodec("utf-8");
        document.save(ts, 4, QDomNode::EncodingFromTextStream);
        filexml.close();
        if(res!=""&&res!="ERROR")
            res=res.left(res.length()-1);
        return res;
    }
}


