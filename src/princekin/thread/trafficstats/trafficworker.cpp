#include "trafficworker.h"

namespace TrafficWorker
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
    qIsStop=false;
}

void Worker::receiveStopWorker()
{
    qIsStop=true;
}

void Worker::startWorker()
{
    int count;

    double doubleValue;
    double double_lrxWifi;
    double double_ltxWifi;
    double double_lrxMobile;
    double double_ltxMobile;


    double double_lrxWifi_M=0.0;
    double double_ltxWifi_M=0.0;
    double double_lrxMobile_M=0.0;
    double double_ltxMobile_M=0.0;


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
        if(qIsStop)
        {
            return;
        }

        doubleValue=0.0;

        double_lrxWifi=0.0;
        double_ltxWifi=0.0;

        double_lrxMobile=0.0;
        double_ltxMobile=0.0;

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

        if(!firstValueHash.keys().contains("lrxWifi"))
        {
            firstValueHash.insert("lrxWifi",double_lrxWifi);
        }
        if(!firstValueHash.keys().contains("ltxWifi"))
        {
            firstValueHash.insert("ltxWifi",double_ltxWifi);
        }
        if(!firstValueHash.keys().contains("lrxMobile"))
        {
            firstValueHash.insert("lrxMobile",double_lrxMobile);
        }
        if(!firstValueHash.keys().contains("ltxMobile"))
        {
            firstValueHash.insert("ltxMobile",double_ltxMobile);
        }

        doubleValue=double_lrxWifi - firstValueHash.value("lrxWifi");
        double_lrxWifi_M=(double)doubleValue/1024;
        str_lrxWifi=QString::number(double_lrxWifi_M,'f',2);

        doubleValue=double_ltxWifi-firstValueHash.value("ltxWifi");
        double_ltxWifi_M=(double)doubleValue/1024;
        str_ltxWifi=QString::number(double_ltxWifi_M,'f',2);

        doubleValue=double_lrxMobile-firstValueHash.value("lrxMobile");
        double_lrxMobile_M=(double)doubleValue/1024;
        str_lrxMobile=QString::number(double_lrxMobile_M,'f',2);

        doubleValue=double_ltxMobile-firstValueHash.value("ltxMobile");
        double_ltxMobile_M=(double)doubleValue/1024;
        str_ltxMobile=QString::number(double_ltxMobile_M,'f',2);

        netState=gNetStateHash.value(qDeviceId);

        if(netState=="currentStateWifi" || netState=="currentStateMobile")
        {
            strTemp=qDeviceId + "=" + netState;
            strTemp2=str_lrxWifi + "=" + str_ltxWifi + "=" + str_lrxMobile + "=" + str_ltxMobile;

            emit sendResult(strTemp,strTemp2);
        }
        else
        {
            //其它状态暂不处理
        }

        Sleep(gdInterval);

    }//while(true)
}

}
