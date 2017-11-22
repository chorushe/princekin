#include "assistmeasure.h"
#include "windows.h"
#include "globalvar.h"

AssistMeasure::AssistMeasure(QWidget *parent) : QWidget(parent)
{
    timerTime=5000;
    isMemCheck=false;
    isCpuCheck=false;
    isCpuTempCheck=false;
    isBatteryCheck=false;
    isWifiCheck=false;
    isMobileCheck=false;

    isMemOverThres=false;
    isCpuOverThres=false;
    isBatteryOverThres=false;
    isCpuTempOverThres=false;
    isWifiOverThres=false;
    isMobileOverThres=false;

    packageName="";

}

void AssistMeasure::StartMeasure()
{
    MeasureReport newMeasure;

    QString tmp="+";
    int num=deviceModel.indexOf(tmp)+1;
    deviceName=deviceModel.mid(num).trimmed();

    newMeasure.deviceModel=deviceModel;
    newMeasure.deviceName=deviceName;
    if(packageName=="")
        newMeasure.measureContent="整体手机";
    else
        newMeasure.measureContent=packageName;
    newMeasure.startTime = QDateTime::currentDateTime();
    reportInfo.append(newMeasure);

    assistTimer = new QTimer;
    assistTimerThread=new QThread;
    assistTimer->start(timerTime);
    assistTimer->moveToThread(assistTimerThread);
    connect(assistTimerThread,SIGNAL(finished()),assistTimer,SLOT(deleteLater()));

    getMemoryCpuIndex();

    if(isMemCheck)
        connect( assistTimer, SIGNAL(timeout()),this, SLOT(getMemory()),Qt::DirectConnection );
    if(isCpuCheck)
        connect( assistTimer, SIGNAL(timeout()),this, SLOT(getCPU()),Qt::DirectConnection );
    if(isBatteryCheck)
        connect( assistTimer,SIGNAL(timeout()),this,SLOT(getBattery()),Qt::DirectConnection);
    if(isCpuTempCheck)
        connect( assistTimer,SIGNAL(timeout()),this,SLOT(getCpuTemp()),Qt::DirectConnection);
    if(isWifiCheck||isMobileCheck)
        connect(assistTimer,SIGNAL(timeout()),this,SLOT(getTrafficstats()),Qt::DirectConnection);
    assistTimerThread->start();

    stopFlag=false;

    c=1;
}

void AssistMeasure::getMemoryCpuIndex()
{
    if(packageName!="")
    {
        QString cmdStrGetMem="adb -s "+deviceName+" shell top -m 1 -n 1 | grep CPU%";
        QString res=ExeCmd::runCmd(cmdStrGetMem).trimmed();
        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        for(int i=0;i<resList.count();i++)
        {
            if(resList[i]=="CPU%")
                cpuIndex=i;
            else if(resList[i]=="RSS")
                memIndex=i;
        }
    }
}

void AssistMeasure::getMemory()
{
    QString cmdStrGetMem;
    QString res="",memRes="";
    int num=0;
    QString tmp="";

    if(packageName!="")
    {
        cmdStrGetMem="adb -s "+deviceName+" shell top -n 1 | grep "+packageName;
        res=ExeCmd::runCmd(cmdStrGetMem).trimmed();
        if(res=="")
        {
            return;
        }
        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        if(resList.count()>7)
        {
            memRes=resList.at(memIndex);
            memRes=memRes.left(memRes.length()-1);
        }
        qDebug()<<memRes;
    }
    else
    {
        cmdStrGetMem="adb -s "+deviceName+" shell dumpsys meminfo | grep Used\n";
        res=ExeCmd::runCmd(cmdStrGetMem);

        if(res=="")
        {
            return;
        }

        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        if(resList.count()>2)
            memRes=resList.at(2);
        qDebug()<<memRes;
    }

    reportInfo[reportInfo.length()-1].memData.append(memRes.toDouble()/1024);
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    reportInfo[reportInfo.length()-1].memTime.append(currentTime);

    if(!reportInfo.isEmpty())
    {
        if(reportInfo[reportInfo.length()-1].memInfo[3]==0)
        {
            reportInfo[reportInfo.length()-1].memInfo[0]=memRes.toDouble()/1024;
            reportInfo[reportInfo.length()-1].memInfo[1]=memRes.toDouble()/1024;
            reportInfo[reportInfo.length()-1].memInfo[2]=memRes.toDouble()/1024;
            reportInfo[reportInfo.length()-1].memInfo[3]=1;
        }
        else
        {
            if(memRes.toDouble()/1024>reportInfo[reportInfo.length()-1].memInfo[0])
                reportInfo[reportInfo.length()-1].memInfo[0]=memRes.toDouble()/1024;
            if(memRes.toDouble()/1024<reportInfo[reportInfo.length()-1].memInfo[1])
                reportInfo[reportInfo.length()-1].memInfo[1]=memRes.toDouble()/1024;
            reportInfo[reportInfo.length()-1].memInfo[2]=(reportInfo[reportInfo.length()-1].memInfo[2]*reportInfo[reportInfo.length()-1].memInfo[3]+memRes.toDouble()/1024)/(++reportInfo[reportInfo.length()-1].memInfo[3]);
        }

        qDebug()<<reportInfo[reportInfo.length()-1].memInfo[0];
        qDebug()<<reportInfo[reportInfo.length()-1].memInfo[1];

        qDebug()<<memRes.toDouble()/1024<<"  "<<memThres.toDouble();
        if(memThres!="")
        {
            if(memRes.toDouble()/1024>memThres.toDouble())
            {
                if(isMemOverThres==false)
                {
                    memTime.start();
                    isMemOverThres=true;
                }
                sendMemOverThresSignal(true,deviceName);
            }
            else
            {
                if(isMemOverThres==true)
                {
                    int timeinternal=memTime.elapsed();
                    reportInfo[reportInfo.length()-1].memOverThresTime += timeinternal;
                    isMemOverThres=false;
                }
                sendMemOverThresSignal(false,deviceName);
            }
        }
    }

}

void AssistMeasure::getCPU()
{
    QString cmdStrGetCPU="";
    QString tmp="";
    QString res="",cpuRes="";
    int num=0;

    if(packageName!="")
    {
        cmdStrGetCPU="adb -s "+deviceName+" shell top -n 1 | grep "+packageName;
        res=ExeCmd::runCmd(cmdStrGetCPU).trimmed();
        if(res=="")
        {
            return;
        }
        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        if(resList.count()>7)
        {
            cpuRes=resList.at(cpuIndex);
            cpuRes=cpuRes.left(cpuRes.length()-1);
        }
        qDebug()<<cpuRes;
    }
    else
    {
        cmdStrGetCPU="adb -s "+deviceName+" shell dumpsys cpuinfo | grep TOTAL\n";
        res=ExeCmd::runCmd(cmdStrGetCPU);
        if(res=="")//如果没有返回结果就继续
        {
            return;
        }
        QStringList resList=res.split(" ",QString::SkipEmptyParts);
        if(resList.count()>0)
        {
            cpuRes=resList.at(0);
            cpuRes=cpuRes.left(cpuRes.length()-1);
        }
    }

    reportInfo[reportInfo.length()-1].cpuData.append(cpuRes.toDouble());
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    reportInfo[reportInfo.length()-1].cpuTime.append(currentTime);

    qDebug()<<"cpu: "+cpuRes;
    if(!reportInfo.isEmpty())
    {
        if(reportInfo[reportInfo.length()-1].cpuInfo[3]==0)
        {
            reportInfo[reportInfo.length()-1].cpuInfo[0]=cpuRes.toDouble();
            reportInfo[reportInfo.length()-1].cpuInfo[1]=cpuRes.toDouble();
            reportInfo[reportInfo.length()-1].cpuInfo[2]=cpuRes.toDouble();
            reportInfo[reportInfo.length()-1].cpuInfo[3]=1;
        }
        else
        {
            if(cpuRes.toDouble()>reportInfo[reportInfo.length()-1].cpuInfo[0])
                reportInfo[reportInfo.length()-1].cpuInfo[0]=cpuRes.toDouble();
            if(cpuRes.toDouble()<reportInfo[reportInfo.length()-1].cpuInfo[1])
                reportInfo[reportInfo.length()-1].cpuInfo[1]=cpuRes.toDouble();
            reportInfo[reportInfo.length()-1].cpuInfo[2]=(reportInfo[reportInfo.length()-1].cpuInfo[2]*reportInfo[reportInfo.length()-1].cpuInfo[3]+cpuRes.toDouble())/(++reportInfo[reportInfo.length()-1].cpuInfo[3]);
        }
        qDebug()<<reportInfo[reportInfo.length()-1].cpuInfo[1];

        qDebug()<<cpuRes.toDouble()<<"  "<<cpuThres.toDouble();
        if(cpuThres!="")
        {
            if(cpuRes.toDouble()>cpuThres.toDouble())
            {
                if(isCpuOverThres==false)
                {
                    cpuTime.start();
                    isCpuOverThres=true;
                }
                sendCpuOverThresSignal(true,deviceName);
            }
            else
            {
                if(isCpuOverThres==true)
                {
                    int timeinternal=cpuTime.elapsed();
                    reportInfo[reportInfo.length()-1].cpuOverThresTime += timeinternal;
                    isCpuOverThres=false;
                }
                sendCpuOverThresSignal(false,deviceName);
            }
        }
    }
}

void AssistMeasure::getBattery()
{
    QString getBatteryCmd="adb -s "+deviceName+" shell dumpsys battery | grep \"temperature\"";
    QString res=ExeCmd::runCmd(getBatteryCmd).trimmed();
    qDebug()<<res;
    QString tmp="temperature:";
    if(res.contains(tmp))
    {
        res=res.mid(tmp.length());
        reportInfo[reportInfo.length()-1].batteryData.append(res.toDouble()/10);
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        reportInfo[reportInfo.length()-1].batteryTime.append(currentTime);

        if(!reportInfo.isEmpty())
        {
            if(reportInfo[reportInfo.length()-1].batteryInfo[3]==0)
            {
                reportInfo[reportInfo.length()-1].batteryInfo[0]=res.toDouble()/10;
                reportInfo[reportInfo.length()-1].batteryInfo[1]=res.toDouble()/10;
                reportInfo[reportInfo.length()-1].batteryInfo[2]=res.toDouble()/10;
                reportInfo[reportInfo.length()-1].batteryInfo[3]=1;
            }
            else
            {
                if(res.toDouble()/10>reportInfo[reportInfo.length()-1].batteryInfo[0])
                    reportInfo[reportInfo.length()-1].batteryInfo[0]=res.toDouble()/10;
                if(res.toDouble()/10<reportInfo[reportInfo.length()-1].batteryInfo[1])
                    reportInfo[reportInfo.length()-1].batteryInfo[1]=res.toDouble()/10;
                reportInfo[reportInfo.length()-1].batteryInfo[2]=(reportInfo[reportInfo.length()-1].batteryInfo[2]*reportInfo[reportInfo.length()-1].batteryInfo[3]+res.toDouble()/10)/(++reportInfo[reportInfo.length()-1].batteryInfo[3]);
            }
            qDebug()<<reportInfo[reportInfo.length()-1].batteryInfo[1];

            qDebug()<<res.toDouble()/10<<"  "<<batteryThres.toDouble();
            if(batteryThres!="")
            {
                if(res.toDouble()/10>batteryThres.toDouble())
                {
                    if(isBatteryOverThres==false)
                    {
                        batteryTime.start();
                        isBatteryOverThres=true;
                    }
                    sendBatteryOverThresSignal(true,deviceName);
                }
                else
                {
                    if(isBatteryOverThres==true)
                    {
                        int timeinternal=batteryTime.elapsed();
                        reportInfo[reportInfo.length()-1].batteryOverThresTime += timeinternal;
                        isBatteryOverThres=false;
                    }
                    sendBatteryOverThresSignal(false,deviceName);
                }
            }
        }
    }


}

void AssistMeasure::getCpuTemp()
{
    QString getCPUCmd="adb -s "+deviceName+" shell cat /sys/class/thermal/thermal_zone0/temp";
    QString res=ExeCmd::runCmd(getCPUCmd).trimmed();
    //qDebug()<<res;

    QString tmp="error:";
    if(!res.contains(tmp))
    {
        double resNum=0;bool ok;
        resNum=res.toDouble(&ok);
        if(!ok||resNum<0)
            return;
        if(resNum>1000)
            resNum=resNum/1000;

        reportInfo[reportInfo.length()-1].cpuTempData.append(resNum);
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        reportInfo[reportInfo.length()-1].cpuTempTime.append(currentTime);

        if(!reportInfo.isEmpty())
        {
            if(reportInfo[reportInfo.length()-1].cpuTempInfo[3]==0)
            {
                reportInfo[reportInfo.length()-1].cpuTempInfo[0]=resNum;
                reportInfo[reportInfo.length()-1].cpuTempInfo[1]=resNum;
                reportInfo[reportInfo.length()-1].cpuTempInfo[2]=resNum;
                reportInfo[reportInfo.length()-1].cpuTempInfo[3]=1;
            }
            else
            {
                if(res.toDouble()>reportInfo[reportInfo.length()-1].cpuTempInfo[0])
                    reportInfo[reportInfo.length()-1].cpuTempInfo[0]=resNum;
                if(res.toDouble()<reportInfo[reportInfo.length()-1].cpuTempInfo[1])
                    reportInfo[reportInfo.length()-1].cpuTempInfo[1]=resNum;
                reportInfo[reportInfo.length()-1].cpuTempInfo[2]=(reportInfo[reportInfo.length()-1].cpuTempInfo[2]*reportInfo[reportInfo.length()-1].cpuTempInfo[3]+resNum)/(++reportInfo[reportInfo.length()-1].cpuTempInfo[3]);
            }
            qDebug()<<reportInfo[reportInfo.length()-1].cpuTempInfo[1];

            qDebug()<<resNum<<"  "<<cpuTempThres.toDouble();
            if(cpuTempThres!="")
            {
                if(resNum>cpuTempThres.toDouble())
                {
                    if(isCpuTempOverThres==false)
                    {
                        cpuTempTime.start();
                        qDebug()<<"cpuTempTime.start()"<<cpuTempTime.currentTime().toString();
                        isCpuTempOverThres=true;
                    }
                    sendCpuTempOverThresSignal(true,deviceName);
                }
                else
                {
                    if(isCpuTempOverThres==true)
                    {
                        reportInfo[reportInfo.length()-1].cpuTempOverThresTime += cpuTempTime.elapsed();
                        qDebug()<<"timeinternal: "<<reportInfo[reportInfo.length()-1].cpuTempOverThresTime;
                        isCpuTempOverThres=false;
                    }
                    sendCpuTempOverThresSignal(false,deviceName);
                }
            }
        }
    }
}

void AssistMeasure::getTrafficstats()
{
    QString mStr;
    QString s1;
    QString s2;
    QString s3;
    QString s4;
    QString s5;
    QString s6;
    QString s7;

    QString srw;
    QString stw;

    QString srm;
    QString stm;

    QString str1;
    QString str2;
    QString str3;
    QString str4;

    double lrxWifi;
    double lrxMobile;

    double ltxWifi;
    double ltxMobile;

    double lrxW=0;
    double ltxW=0;
    double lrxM=0;
    double ltxM=0;

    double l;
    double l1;
    double l2;
    double l3;
    double l4;
    double l5;
    double l6;
    double l7;
    double l8;

    QStringList mSplitResult;

    QString packageForTraffistats="";
    if(packageName!="")
        packageForTraffistats=packageName;
    else
    {
        QString mPackName=gConfigDir + QDir::separator() + "packageName.txt";
        QFile mf(mPackName);
        mf.open(QIODevice::ReadOnly);
        QTextStream minSteam(&mf);
        packageForTraffistats=minSteam.readLine();
        mf.close();
    }
    qDebug()<<"baoming: "<<packageForTraffistats;

    QString mCmdLine="cmd /c adb -s " + deviceName + " shell dumpsys package " + packageForTraffistats + " | findstr userId=";
    QString uid=ExeCmd::getUid(mCmdLine);

    QString qcmdLine="cmd /c adb -s " + deviceName + " shell cat /proc/net/xt_qtaguid/stats | findstr "+uid;

    QProcess proc;
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.closeReadChannel(QProcess::StandardOutput);
    proc.closeReadChannel(QProcess::StandardError);
    proc.closeWriteChannel();
    proc.start(qcmdLine);
    proc.waitForStarted();

    if(proc.waitForFinished(-1))
    {
        l=0;
        lrxWifi=0;
        lrxMobile=0;
        ltxWifi=0;
        ltxMobile=0;

        QString mStrLine;
        QTextStream mStream(&proc);
        while(!mStream.atEnd())
        {
            mStrLine=mStream.readLine();
            if(mStrLine.contains("wlan0"))
            {
                mSplitResult=mStrLine.split(" ");
                mStr=mSplitResult.at(5);
                l=mStr.toDouble();
                lrxWifi=lrxWifi+l;


                mStr=mSplitResult.at(7);
                l=mStr.toDouble();
                ltxWifi=ltxWifi+l;
            }
            if(mStrLine.contains("rmnet0"))
            {
                mSplitResult=mStrLine.split(" ");
                mStr=mSplitResult.at(5);
                l=mStr.toDouble();
                lrxMobile=lrxMobile+l;


                mStr=mSplitResult.at(7);
                l=mStr.toDouble();
                ltxMobile=ltxMobile+l;
            }
        }
        proc.close();

        lrxW=(double)lrxWifi/1024/1024;

        ltxW=(double)ltxWifi/1024/1024;

        s1=QString::number(lrxW,'f',2);
        s2=QString::number(ltxW,'f',2);


        lrxM=(double)lrxMobile/1024/1024;
        ltxM=(double)ltxMobile/1024/1024;
        s3=QString::number(lrxM,'f',2);
        s4=QString::number(ltxM,'f',2);

        if(c==1)
        {
            one_rxWifi_M=(double)lrxWifi/1024/1024;
            one_rxWifi2=lrxWifi;

            one_txWifi_M=(double)ltxWifi/1024/1024;
            one_txWifi2=ltxWifi;

            one_rxMobile_M=(double)lrxMobile/1024/1024;
            one_rxMobile2=lrxMobile;

            one_txMobile_M=(double)ltxMobile/1024/1024;
            one_txMobile2=ltxMobile;

            c=2;
        }
        else
        {

            l1=double(lrxWifi-currentLrxWifi)/1024;
            l2=double(ltxWifi-currentLtxWifi)/1024;
            srw=QString::number(l1,'f',2);
            stw=QString::number(l2,'f',2);

            l3=double(lrxMobile-currentLrxMobile)/1024;
            l4=double(ltxMobile-currentLtxMobile)/1024;
            srm=QString::number(l3,'f',2);
            stm=QString::number(l4,'f',2);

            l5=double(lrxWifi-one_rxWifi2)/1024/1024;
            l6=double(ltxWifi-one_txWifi2)/1024/1024;

            double wifiAll=l5+l6;
            qDebug()<<"wifi总流量："<<wifiAll;

            l7=double(lrxMobile-one_rxMobile2)/1024/1024;
            l8=double(ltxMobile-one_txMobile2)/1024/1024;

            double mobileAll=l7+l8;

            if(isWifiCheck)
            {
                reportInfo[reportInfo.length()-1].wifiData.append(wifiAll);
                QDateTime current_date_time = QDateTime::currentDateTime();
                QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
                reportInfo[reportInfo.length()-1].wifiTime.append(currentTime);

                reportInfo[reportInfo.length()-1].wifiTrafficstats+=wifiAll;
                if(wifiThres!="")
                {

                    if(wifiAll>wifiThres.toDouble())
                    {
                        if(isWifiOverThres==false)
                        {
                            wifiTime.start();
                            isWifiOverThres=true;
                        }
                        sendWifiOverThresSignal(true,deviceName);
                    }
                    else
                    {
                        if(isWifiOverThres==true)
                        {
                            reportInfo[reportInfo.length()-1].wifiOverThresTime += wifiTime.elapsed();
                            qDebug()<<"timeinternal: "<<reportInfo[reportInfo.length()-1].wifiOverThresTime;
                            isWifiOverThres=false;
                        }
                        sendWifiOverThresSignal(false,deviceName);
                    }
                }
            }

            if(isMobileCheck)
            {
                reportInfo[reportInfo.length()-1].mobileData.append(mobileAll);
                QDateTime current_date_time = QDateTime::currentDateTime();
                QString currentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
                reportInfo[reportInfo.length()-1].mobileTime.append(currentTime);

                reportInfo[reportInfo.length()-1].mobileTrafficstats+=mobileAll;
                if(mobileThres!="")
                {
                    if(mobileAll>mobileThres.toDouble())
                    {
                        if(isMobileOverThres==false)
                        {
                            mobileTime.start();
                            isMobileOverThres=true;
                        }
                        sendMobileOverThresSignal(true,deviceName);
                    }
                    else
                    {
                        if(isMobileOverThres==true)
                        {
                            reportInfo[reportInfo.length()-1].mobileOverThresTime += mobileTime.elapsed();
                            qDebug()<<"timeinternal: "<<reportInfo[reportInfo.length()-1].mobileOverThresTime;
                            isMobileOverThres=false;
                        }
                        sendMobileOverThresSignal(false,deviceName);
                    }
                }
            }
        }

        currentLrxWifi=lrxWifi;
        currentLtxWifi=ltxWifi;

        currentLrxMobile=lrxMobile;
        currentLtxMobile=ltxMobile;


    }
}

void AssistMeasure::StopMeasure()
{
    if(assistTimerThread!=NULL)
    {
        if(assistTimerThread->isRunning())
        {
            assistTimerThread->quit();
            assistTimerThread->wait();
        }
    }
    stopFlag=true;
    if(!reportInfo.isEmpty())
    {
        reportInfo[reportInfo.length()-1].endTime = QDateTime::currentDateTime();
        if(memThres!="")
        {
            if(isMemOverThres==true)
            {
                int timeinternal=memTime.elapsed();
                reportInfo[reportInfo.length()-1].memOverThresTime += timeinternal;
                qDebug()<<"timeinternal: "<<timeinternal;
                qDebug()<<"timeinternal: "<<reportInfo[reportInfo.length()-1].memOverThresTime;
            }
        }
        if(cpuThres!="")
        {
            if(isCpuOverThres==true)
            {
                int timeinternal=cpuTime.elapsed();
                reportInfo[reportInfo.length()-1].cpuOverThresTime += timeinternal;
            }
        }
        if(batteryThres!="")
        {
            if(isBatteryOverThres==true)
            {
                int timeinternal=batteryTime.elapsed();
                reportInfo[reportInfo.length()-1].batteryOverThresTime += timeinternal;
            }
        }
        if(cpuTempThres!="")
        {
            if(isCpuTempOverThres==true)
            {
                int timeinternal=cpuTempTime.elapsed();
                reportInfo[reportInfo.length()-1].cpuTempOverThresTime += timeinternal;
            }
        }
        if(wifiThres!="")
        {
            if(isWifiOverThres==true)
            {
                int timeinternal=wifiTime.elapsed();
                reportInfo[reportInfo.length()-1].wifiOverThresTime += timeinternal;
            }
        }
        if(mobileThres!="")
        {
            if(isMobileOverThres==true)
            {
                int timeinternal=mobileTime.elapsed();
                reportInfo[reportInfo.length()-1].mobileOverThresTime += timeinternal;
            }
        }
    }
    Sleep(500);
}

void AssistMeasure::ExportReport(QString filePath)//0-温度；1-性能；2-全部
{
    if(reportInfo.isEmpty())
        return;
    if(reportInfo.count()>0)
    {
        if(stopFlag==false)
            reportInfo[reportInfo.length()-1].endTime = QDateTime::currentDateTime();
        QFile f2(filePath);
        f2.open(QIODevice::WriteOnly|QIODevice::Text);
        int i=0;
        for(i=0;i<reportInfo.length();i++)
        {
            reportInfo[i].internalTime = QString::number( reportInfo[i].startTime.daysTo(reportInfo[i].endTime))+"天"
                    +QString::number( reportInfo[i].startTime.msecsTo(reportInfo[i].endTime)/3600000)+"小时"
                    +QString::number(reportInfo[i].startTime.msecsTo(reportInfo[i].endTime)%3600000/60000)+"分"
                    +QString::number(reportInfo[i].startTime.msecsTo(reportInfo[i].endTime)%60000/1000)+"秒";/**/

            QString cmdLine="adb -s "+reportInfo[i].deviceName+" shell getprop ro.build.version.release";
            qDebug()<<cmdLine;
            QString systemStr=ExeCmd::runCmd(cmdLine);

            QString tempStr;
            tempStr="手机型号： "+ reportInfo[i].deviceModel +"\r\n";
            f2.write(tempStr.toStdString().c_str());
            tempStr="系统版本： Android "+ systemStr ;
            f2.write(tempStr.toStdString().c_str());
            tempStr="测试项目： "+ reportInfo[i].measureContent +"\r\n";
            f2.write(tempStr.toStdString().c_str());
            tempStr="测量开始时间： "+ reportInfo[i].startTime.toString("yyyy-MM-dd hh:mm:ss")+"\r\n";
            f2.write(tempStr.toStdString().c_str());
            tempStr="测量结束时间： "+ reportInfo[i].endTime.toString("yyyy-MM-dd hh:mm:ss")+"\r\n";
            f2.write(tempStr.toStdString().c_str());
            tempStr="测量时长： "+ reportInfo[i].internalTime+"\r\n-----------------------------------------------------"+"\r\n";
            f2.write(tempStr.toStdString().c_str());
            if(isCpuCheck)
            {
                tempStr="CPU信息：\r\n最大值： "+ QString::number( reportInfo[i].cpuInfo[0])+"%\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="最小值： "+ QString::number( reportInfo[i].cpuInfo[1])+"%\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="平均值： "+ QString::number( reportInfo[i].cpuInfo[2])+"%\r\n";
                f2.write(tempStr.toStdString().c_str());

                if(cpuThres!="")
                {
                    tempStr="警戒值： "+ cpuThres +"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                    tempStr="超过警戒值时间： "+ calculateTimeFromMS(reportInfo[i].cpuOverThresTime) +"\r\n"+"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
                else
                {
                    tempStr="\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
            }
            if(isMemCheck)
            {
                tempStr="内存信息(M)：\r\n最大值： "+ QString::number( reportInfo[i].memInfo[0] )+"\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="最小值： "+ QString::number( reportInfo[i].memInfo[1] )+"\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="平均值： "+ QString::number( reportInfo[i].memInfo[2] )+"\r\n";
                f2.write(tempStr.toStdString().c_str());

                if(memThres!="")
                {
                    tempStr="警戒值： "+ memThres +"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                    tempStr="超过警戒值时间： "+ calculateTimeFromMS(reportInfo[i].memOverThresTime) +"\r\n"+"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
                else
                {
                    tempStr="\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
            }
            if(isCpuTempCheck)
            {
                tempStr="CPU温度信息(℃)：\r\n最大值： "+ QString::number( reportInfo[i].cpuTempInfo[0])+"\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="最小值： "+ QString::number( reportInfo[i].cpuTempInfo[1])+"\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="平均值： "+ QString::number( reportInfo[i].cpuTempInfo[2])+"\r\n";
                f2.write(tempStr.toStdString().c_str());

                if(cpuTempThres!="")
                {
                    tempStr="警戒值： "+ cpuTempThres +"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                    tempStr="超过警戒值时间： "+ calculateTimeFromMS(reportInfo[i].cpuTempOverThresTime) +"\r\n"+"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
                else
                {
                    tempStr="\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
            }
            if(isBatteryCheck)
            {
                tempStr="电池温度信息(℃)：\r\n最大值： "+ QString::number( reportInfo[i].batteryInfo[0] )+"\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="最小值： "+ QString::number( reportInfo[i].batteryInfo[1] )+"\r\n";
                f2.write(tempStr.toStdString().c_str());
                tempStr="平均值： "+ QString::number( reportInfo[i].batteryInfo[2] )+"\r\n";
                f2.write(tempStr.toStdString().c_str());

                if(batteryThres!="")
                {
                    tempStr="警戒值： "+ batteryThres +"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                    tempStr="超过警戒值时间： "+ calculateTimeFromMS(reportInfo[i].batteryOverThresTime) +"\r\n"+"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
                else
                {
                    tempStr="\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
            }
            if(isWifiCheck)
            {
                tempStr="Wifi流量信息(M)：\r\n总值： "+ QString::number( reportInfo[i].wifiTrafficstats )+"\r\n";
                f2.write(tempStr.toStdString().c_str());

                if(wifiThres!="")
                {
                    tempStr="警戒值： "+ wifiThres +"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                    tempStr="超过警戒值时间： "+ calculateTimeFromMS(reportInfo[i].wifiOverThresTime) +"\r\n"+"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
                else
                {
                    tempStr="\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
            }
            if(isMobileCheck)
            {
                tempStr="Mobile流量信息(M)：\r\n总值： "+ QString::number( reportInfo[i].mobileTrafficstats )+"\r\n";
                f2.write(tempStr.toStdString().c_str());

                if(mobileThres!="")
                {
                    tempStr="警戒值： "+ mobileThres +"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                    tempStr="超过警戒值时间： "+ calculateTimeFromMS(reportInfo[i].mobileOverThresTime) +"\r\n"+"\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
                else
                {
                    tempStr="\r\n";
                    f2.write(tempStr.toStdString().c_str());
                }
            }
            tempStr="======================================================================\r\n";
            f2.write(tempStr.toStdString().c_str());
        }
        f2.close();
    }
}

QString AssistMeasure::calculateTimeFromMS(int time)
{
    time=time/1000;
    int h=time/60/60;
    int m=(time-h*60*60)/60;
    int s=time-h*60*60-m*60;
    QString timeStr=QString::number(h)+"小时"+QString::number(m)+"分"+QString::number(s)+"秒";
    return timeStr;
}
