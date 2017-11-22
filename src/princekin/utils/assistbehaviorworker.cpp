#include "assistbehaviorworker.h"

AssistBehaviorWorker::AssistBehaviorWorker(QObject *parent) : QObject(parent)
{
    p=new QProcess(this);
    connect(p,SIGNAL(readyReadStandardOutput()),this,SLOT(ReadOutput()));
    //connect(p,SIGNAL(readyReadStandardError()),this,SLOT(ReadError()));
}

void AssistBehaviorWorker::startStatistics()
{
    GetXMLs();
    QString cmdStr="mitmdump -b "+IP+" -p "+Port;
    //cmdStr="mitmdump -b 10.2.8.94 -p 7788";
    if(p->state())
        p->write(cmdStr.toLatin1());
    else
    {
        p->start("cmd", QStringList()<<"/c"<< cmdStr.toLatin1());
    }
    qDebug()<<"start";
}

void AssistBehaviorWorker::ReadOutput()
{
    while(p->canReadLine())
    {
        QString res(p->readLine());
        res=UrlDecode(res);

        QMap<QString,QList<DataClass> >::iterator it;
        for(it=allXmlData.begin();it!=allXmlData.end();++it)
        {
            if(res.contains(it.value()[0].UniqueIdenti)&&res.contains(it.value()[0].revelantHead))//unique是全局检测的检测标准，但单个检测时需要revelant
            {
                isSuccess=true;
                QString errorStr = ParseURL(res,it.value()[0].UniqueIdenti,it.key());
                qDebug()<<isSuccess;
                if(isSuccess)
                    statistics.append(it.key()+"&√");
                else
                    statistics.append(it.key()+"&×"+errorStr);
                //emit sendStatisticsData(statistics);
                break;
            }
        }
    }
}

void AssistBehaviorWorker::stopStatistics()
{
    qDebug()<<"stop";
    if(p->state()!=QProcess::NotRunning)
    {
        p->close();
    }
    ExeCmd::runCmd("taskkill /im mitmdump.exe /f");
    emit sendStopSignal();
    //找到哪些xml文件没有被遍历到，并加到statistics最后
    QMap<QString,QList<DataClass> >::iterator it;
    for(it=allXmlData.begin();it!=allXmlData.end();++it)
    {
        bool flag=false;
        for(int i=0;i<statistics.length();i++)
        {
            if(statistics[i].contains(it.key()))
            {
                flag=true;
                break;
            }
        }
        if(!flag)
        {
            statistics.append(it.key()+"&…");
        }
    }
    //把statistics里的xml路径换成文件
    for(int i=0;i<statistics.length();i++)
    {
        QStringList list=statistics[i].split("&");
        QString after=statistics[i].mid(list[0].length());
        QFileInfo f=list[0];
        statistics[i] = f.fileName() + after;
    }
    emit sendStatisticsData(statistics);
}

QString AssistBehaviorWorker::ParseURL(QString url,QString unique,QString key)//点击url列表时槽函数，解析url变成统计点
{
    //isLatest，如果当前url不是最新一条，就是false，其他都是true,比如点击某一条URL解析时
    //url="10.7.36.41:53926: GET http://stat.m.tv.sohu.com/mobiledata_test/mcc/mc.gif?uid=c9e25b6c5931d72ac0a8f77ac6c611d1&tkey=472c80786bc2941025c46ad1f115965004572956&vid=&tvid=&mtype=6&mfov=NX531J&memo=&passport=4BEC8B9477DA5AAB14B235D4F08F1E3E@qq.sohu.com&ltype=&cv=6.3.0&mos=2&mosv=6.0.1&pro=1&mfo=nubia&webtype=WiFi&time=1493972334292&channelid=93&sim=0&playlistid=&catecode=&preid=&newuser=0&enterid=0&startid=1493972327094&loc=&mnc=-2&build=02232253&url=2001&type=1&value=&androidId=b6dc20841c665c08&imei=869465023061802&timer=12f900b6b0421aca1d62e0b2044db36c3b963ce9&xid=RpLaFhOVejLuADC+vM6FprUPsAd6dRRNTRNyxBr27vfdw8pdXlu0hZOZpEd4Rz/cdR33Rh5cIH96XYIxg9k6p23QBpdo0gnlO3Q0I33BXfV80eTrGj8VDvrD1b8KKV24kCrDhljbKPsoujmFSk96D58wTDrQuM4o0XfxAMHyY2E=";
    QString errorStr="";
    QString tmp="";
    bool isFind=false;//下面的for循环是找
    QMap<QString,QList<DataClass> >::iterator it;
    for(it=allXmlData.begin();it!=allXmlData.end();++it)
    {
        if(url.contains(it.value()[0].UniqueIdenti))
        {
            isFind=true;
            tmp=it.value()[0].revelantHead;
            url=url.mid(tmp.length());//把get前缀去掉
            tmp="";
            int num;
            QString tagName="";
            QString value="";

            while(url!="")
            {
                tmp="=";
                num=url.indexOf(tmp);
                tagName=url.left(num);
                url=url.mid(num+1);
                if(tagName=="memo")//memo这个统计点中可能会带有&字符，这截取的内容就会不完整，考虑到该字段由{}括着，所以选取}作为结尾判断
                    tmp="}";
                else
                    tmp="&";
                num=url.indexOf(tmp);
                if(num==-1)
                {
                    if(tagName=="memo")//有的memo是空
                    {
                        tmp="&";
                        num=url.indexOf(tmp);
                        value=url.left(num);
                        url=url.mid(num+1);
                    }
                    else
                    {
                        value=url;
                        url="";
                    }
                }
                else
                {
                    if(tagName=="memo")//以}结尾的，需要把}放到value值中，位置往后扩一位
                    {
                        value=url.left(num+1);
                        url=url.mid(num+2);
                    }
                    else
                    {
                        value=url.left(num);
                        url=url.mid(num+1);
                    }
                }
                //把每个字段和从xml中读出来的数据对比
                //多个字段的onlyCheck是Y，2.7版本修改，但此处的逻辑无需改变
                int flag=0;//xml文件和解析出来的url字段对比，正确或错误的标志
                for(int i=1;i<it.value().length();i++)//list<dataclass>的第一个值是存revelanthead头的，所以遍历的时候从1开始
                {
                    if(tagName==it.value()[i].tagName)//每一个字段在for循环中遇到tagName相同之后，就不会再有相同的了，会跳出for循环了
                    {
                        flag=1;
                        if(it.value()[i].onlyCheck=="Y")
                        {
                            bool mflag=false;//onlycheck是Y时，可能有多个值，一个对不上时，要判断下其他值是不是都对不上
                            while(tagName==it.value()[i].tagName)
                            {
                                if(it.value()[i].value==value)
                                {
                                    mflag=true;
                                    flag=2;
                                   // QString tempStr=tagName+"="+value+"="+it.value()[i].des+"=ok";
                                   // statistics.append(tempStr);
                                    break;//如果一样就break,不用考虑顺序问题，都不一样就打叉
                                }
                                i++;
                            }

                            if(!mflag)
                            {
                                isSuccess=false;
                                //出错信息~~~~
                                qDebug()<<"XX  "<<tagName<<"　　"<<value;
                                QString tempStr=tagName+"="+value+"="+it.value()[i-1].value;
                                //statistics.append(tempStr);
                                errorStr+="&"+tempStr;
                                flag=2;
                            }
                            break;
                        }
                        else if(it.value()[i].onlyCheck=="N")
                        {
                            if(value==it.value()[i].value||it.value()[i].value=="unknown"||it.value()[i].value=="hotvrs获取")//
                            {
                                flag=2;
                                //QString tempStr=tagName+"="+value+"="+it.value()[i].des+"=ok";
                                //statistics.append(tempStr);
                                break;
                            }
                            if(it.value()[i].value=="unnull")//非空字段
                            {
                                if(value!="")
                                {
                                    flag=2;
                                    //QString tempStr=tagName+"="+value+"="+it.value()[i].des+"=ok";
                                    //statistics.append(tempStr);
                                    break;
                                }
                                else
                                {
                                    isSuccess=false;
                                    //出错信息~~
                                    QString tempStr=tagName+"="+value+"="+it.value()[i].value;
                                    errorStr+="&"+tempStr;
                                    //statistics.append(tempStr);
                                    //qDebug()<<"XXXX  "<<tagName<<"　　"<<value;
                                    flag=2;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(flag==1)//有这个名，但值对不上，url很多值都对不上，但也是错了。。。
                {
                    isSuccess=false;
                    //出错信息~~~
                    QString tempStr=tagName+"="+value+"=";
                    errorStr+="&"+tempStr;

                }
            }
            break;
        }
    }
    if(!isFind)
    {
        /*QMap<QString,QList<DataClass> >::iterator iit;
        for(iit=allXmlData.begin();iit!=allXmlData.end();++iit)
        {
            qDebug()<< iit.value()[0].UniqueIdenti;
        }*/
        qDebug()<<"!!没能正确匹配xml文件";
    }
    return errorStr;
}

void AssistBehaviorWorker::GetXMLs()
{
    for(int i=0;i<xmlPaths.length();i++)
    {
        qDebug()<<xmlPaths[i];
        QList<DataClass> tempData=GetXML(xmlPaths[i]);
        allXmlData.insert(xmlPaths[i],tempData);
    }
    qDebug()<<"allxmldata num: "<<allXmlData.count();
}

QList<DataClass> AssistBehaviorWorker::GetXML(QString fileName)//打开XML文件，判断路径或文件格式是否正确
{
    QList<DataClass> tempData;
    if(!fileName.isEmpty())
    {
        //return NULL;
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::information(NULL, QString("提示"), QString("打开错误"));
            return tempData;
        }
        QDomDocument document;
        QString error;
        int row = 0, column = 0;
        if(!document.setContent(&file, false, &error, &row, &column))
        {
            QMessageBox::information(NULL, QString("提示"), QString("解析文件错误，行： ") + QString::number(row, 10) + QString(" ,列： ") + QString::number(column, 10));
            return tempData;
        }
        if(document.isNull())
        {
            QMessageBox::information(NULL, QString("提示"), QString("文件为空"));
            return tempData;
        }
        QDomElement root = document.documentElement();
        QDomNode node = root.firstChild();

        if(isDebugVersion)
            revelantHead=node.toElement().attributeNode("behaviour_debug").value();
        else if(!isDebugVersion)
            revelantHead=node.toElement().attributeNode("behaviour_release").value();

        xmlData.clear();
        DataClass tempData;
        tempData.revelantHead=revelantHead;
        QString unique=node.toElement().attributeNode("unique").value();
        if(unique!="")
        {
            tempData.UniqueIdenti="&"+unique+"&";
        }
        xmlData<<tempData;

        ParseXML(root);

        return xmlData;
    }
}

void AssistBehaviorWorker::ParseXML(const QDomElement &element)//解析xml文件，把节点分离出来
{
    QDomNode node = element.firstChild();
    while(!node.isNull())
    {
        if(node.toElement().tagName()!="data")
        {
            DataClass tempData;
            tempData.tagName=node.toElement().tagName();
            if(node.toElement().hasAttribute("des"))
            {
                tempData.onlyCheck = node.toElement().attributeNode("onlyCheck").value();
            }
            if(node.toElement().hasAttribute("des"))
            {
                tempData.des = node.toElement().attributeNode("des").value();
            }
            tempData.value = node.toElement().text();
            xmlData<<tempData;
        }
        if(node.toElement().tagName()=="data")
        {
            ParseXML(node.toElement());
        }
        node = node.nextSibling();//读取兄弟节点
    }
}

QString AssistBehaviorWorker::UrlDecode(QString str)
{
    if(str.isEmpty())//两种方法都可以对url解码
    {
        return "";
    }
    QByteArray byteArrayGB2312 = str.toUtf8();
    QByteArray byteArrayPercentEncoded = QByteArray::fromPercentEncoding(byteArrayGB2312);
    return QString(byteArrayPercentEncoded);
}
