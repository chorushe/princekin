#ifndef REPLAYMEASUREHELP_H
#define REPLAYMEASUREHELP_H
#include <QFile>
#include <QProcess>


class ReplayMeasureHelp : public QObject
{
    Q_OBJECT

public:
    ReplayMeasureHelp();
    ~ReplayMeasureHelp();

    QString deviceName;

    bool isMemOverThres;
    bool isCpuOverThres;
    bool isBatteryOverThers;
    bool isCpuTempOverThres;
    bool isWifiOverThres;
    bool isMobileOverThres;

    QFile *fileLogcat;
    QString filePath;
    QProcess *pLogcat;

public slots:
    void RecieveLogcatOutput();

signals:
    void SendProcessPutput(QString str,QString deviceName);

};

#endif // REPLAYMEASUREHELP_H
