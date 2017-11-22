#ifndef ASSISTBEHAVIORWORKER_H
#define ASSISTBEHAVIORWORKER_H

#include <QObject>
#include <QWidget>
#include <QProcess>
#include "dataclass.h"
#include <QDomElement>
#include <QDomNode>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "execmd.h"


class AssistBehaviorWorker: public QObject
{
    Q_OBJECT
public:
    explicit AssistBehaviorWorker(QObject *parent = 0);

    QStringList xmlPaths;
    bool isDebugVersion=true;
    QString IP;
    QString Port;

    QList<DataClass> GetXML(QString fileName);
    void ParseXML(const QDomElement &element);
    void GetXMLs();
    QString UrlDecode(QString str);
    QString ParseURL(QString url,QString unique,QString key);

private:

    QProcess *p=NULL;
    QList<DataClass> xmlData;
    QMap<QString,QList<DataClass> > allXmlData;
    bool isSuccess=true;
    QStringList statistics;
    QString revelantHead;


public slots:
    void startStatistics();
    void ReadOutput();
    void stopStatistics();

signals:
    void sendStatisticsData(QStringList statistics);
    void sendStopSignal();
};

#endif // ASSISTBEHAVIORWORKER_H
