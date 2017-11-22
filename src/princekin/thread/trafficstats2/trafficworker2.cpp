#include "trafficworker2.h"

namespace TrafficWorker2
{

Worker::Worker()
{
}

void Worker::setDeviceId(const QString &arg_text)
{
    qDeviceId=arg_text;
}

void Worker::setCmdLine(const QString &arg_text)
{
    qCmdLine=arg_text;
}

void Worker::initVar()
{
    c=1;
    qIsStop=false;
    gIsStopTraffic=false;
}

void Worker::setSec(int arg_sec)
{
    qSec=arg_sec;
}

void Worker::receiveStopWorker()
{
    qIsStop=true;
}


void Worker::startWorker()
{

    QString srw;
        QString stw;

        QString srm;
        QString stm;

        QString str1;
        QString str2;
        QString str3;
        QString str4;

        QString sss;




    int count;

    double doubleValue;

    double double_lrxWifi;
    double double_ltxWifi;
    double double_lrxMobile;
    double double_ltxMobile;

    double double_current_lrxWifi;
    double double_current_ltxWifi;
    double double_current_lrxMobile;
    double double_current_ltxMobile;


    double double_lrxWifi_M=0.0;
    double double_ltxWifi_M=0.0;
    double double_lrxMobile_M=0.0;
    double double_ltxMobile_M=0.0;


    double double_first_lrxWifi=0.0;
    double double_first_ltxWifi=0.0;
    double double_first_lrxMobile=0.0;
    double double_first_ltxMobile=0.0;


    QString str_lrxWifi;
    QString str_ltxWifi;
    QString str_lrxMobile;
    QString str_ltxMobile;

    QString strLine;
    QString strTemp;
    QString strTemp2;
    QString netState;
    QStringList splitResult;

    QByteArray byteArray;

    QHash<QString,double> firstValueHash;

    while(true)
    {
        if(gIsStopTraffic)
        {
            return;
        }

        doubleValue=0.0;

        double_lrxWifi=0.0;
        double_ltxWifi=0.0;

        double_lrxMobile=0.0;
        double_ltxMobile=0.0;

        double_first_lrxWifi=0.0;
        double_first_ltxWifi=0.0;
        double_first_lrxMobile=0.0;
        double_first_ltxMobile=0.0;



        QProcess proc;
        proc.setReadChannelMode(QProcess::MergedChannels);
        proc.start(qCmdLine);
        proc.waitForStarted();

        while(proc.waitForFinished()==false)
        {
        }

        while(proc.canReadLine())
        {
            byteArray=proc.readLine();
            strLine=qTc->toUnicode(byteArray).trimmed();

            if(strLine.contains("wlan0"))
            {
                splitResult=strLine.split(" ");
                count=splitResult.size();

                if(count>=8)
                {
                    strTemp=splitResult.at(5);
                    doubleValue=strTemp.toDouble();
                    double_lrxWifi=double_lrxWifi + doubleValue;

                    strTemp=splitResult.at(7);
                    doubleValue=strTemp.toDouble();
                    double_ltxWifi=double_ltxWifi + doubleValue;
                }
            }
            if(strLine.contains("rmnet0"))
            {
                splitResult=strLine.split(" ");
                count=splitResult.size();
                if(count>=8)
                {
                    strTemp=splitResult.at(5);
                    doubleValue=strTemp.toDouble();
                    double_lrxMobile=double_lrxMobile + doubleValue;

                    strTemp=splitResult.at(7);
                    doubleValue=strTemp.toDouble();
                    double_ltxMobile=double_ltxMobile + doubleValue;
                }
            }
        }//while(proc.canReadLine())

        proc.close();


        double_lrxWifi_M=(double)double_lrxWifi/1024/1024;
        str_lrxWifi=QString::number(double_lrxWifi_M,'f',2);

        double_ltxWifi_M=(double)double_ltxWifi/1024/1024;
        str_ltxWifi=QString::number(double_ltxWifi_M,'f',2);

        double_lrxMobile_M=(double)double_lrxMobile/1024/1024;
        str_lrxMobile=QString::number(double_lrxMobile_M,'f',2);

        double_ltxMobile_M=(double)double_ltxMobile/1024/1024;
        str_ltxMobile=QString::number(double_ltxMobile_M,'f',2);


        netState=gNetStateHash.value(qDeviceId);
        if(c==1)
        {
            double_first_lrxWifi=double_lrxWifi;
            double_first_ltxWifi=double_ltxWifi;
            double_first_lrxMobile=double_lrxMobile;
            double_first_ltxMobile=double_ltxMobile;

            if(netState=="currentStateWifi")
            {
                emit sendResult_wifi1(str_lrxWifi,str_ltxWifi);
            }
            else if(netState=="currentStateMobile")
            {
                emit sendResult_mobile1(str_lrxMobile,str_ltxMobile);
            }
            else
            {
                //其它状态暂不处理
            }
            c=2;
        }
        else
        {
            doubleValue=double(double_lrxWifi-double_current_lrxWifi)/1024;
            srw=QString::number(doubleValue,'f',2);

            doubleValue=double(double_ltxWifi-double_current_ltxWifi)/1024;
            stw=QString::number(doubleValue,'f',2);

            doubleValue=double(double_lrxMobile-double_current_lrxMobile)/1024;
            srm=QString::number(doubleValue,'f',2);

            doubleValue=double(double_ltxMobile-double_current_ltxMobile)/1024;
            stm=QString::number(doubleValue,'f',2);


            doubleValue=double(double_lrxWifi-double_first_lrxWifi)/1024/1024;
            str1=QString::number(doubleValue,'f',2);


            doubleValue=double(double_ltxWifi-double_first_ltxWifi)/1024/1024;
            str2=QString::number(doubleValue,'f',2);

            doubleValue=double(double_lrxMobile-double_first_lrxMobile)/1024/1024;
            str3=QString::number(doubleValue,'f',2);

            doubleValue=double(double_ltxMobile-double_first_ltxMobile)/1024/1024;
            str4=QString::number(doubleValue,'f',2);

            if(netState=="currentStateWifi")
            {
                sss=str_lrxWifi+"=" + str_ltxWifi+"=" + srw+"=" + stw+"=" + str1+"=" + str2;
                emit sendResult_wifi2(str_lrxWifi,sss);
            }
            else if(netState=="currentStateMobile")
            {
                sss=str_lrxMobile+"=" + str_ltxMobile+"=" + srm+"=" + stm+"=" + str3+"=" + str4;
                emit sendResult_mobile2(str_lrxMobile,sss);
            }
            else
            {
                //其它状态暂不处理
            }
        }

        double_current_lrxWifi=double_lrxWifi;
        double_current_ltxWifi=double_ltxWifi;
        double_current_lrxMobile=double_lrxMobile;
        double_current_ltxMobile=double_ltxMobile;

        Sleep(qSec);

    }//while(true)
}

}
