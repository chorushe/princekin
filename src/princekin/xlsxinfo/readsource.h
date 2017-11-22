#ifndef READSOURCE_H
#define READSOURCE_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QtXlsx>
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QVector>

#include "xlsxformat.h"
#include "xlsxdocument.h"
#include "xlsxabstractsheet.h"

using namespace QXlsx;

class ReadSource
{
public:
    ReadSource();
public:
    static void readData(const QString &);

    static void read_memory_time(Document &);
    static void read_cpu_time(Document &);
    static void read_cputemp_time(Document &);
    static void read_batterytemp_time(Document &);
    static void read_wifi_time(Document &);
    static void read_mobile_time(Document &);


    static void read_memory_warning(Document &);
    static void read_cpu_warning(Document &);
    static void read_cputemp_warning(Document &);
    static void read_batterytemp_warning(Document &);
    static void read_wifi_warning(Document &);
    static void read_mobile_warning(Document &);



    static void read_memory_data(Document &);
    static void read_cpu_data(Document &);
    static void read_cputemp_data(Document &);
    static void read_batterytemp_data(Document &);
    static void read_wifi_data(Document &);
    static void read_mobile_data(Document &);


    static QVector<QString> get_memory_time();
    static QVector<QString> get_cpu_time();
    static QVector<QString> get_cputemp_time();
    static QVector<QString> get_batterytemp_time();
    static QVector<QString> get_wifi_time();
    static QVector<QString> get_mobile_time();


    static QVector<QString> get_memory_warning();
    static QVector<QString> get_cpu_warning();
    static QVector<QString> get_cputemp_warning();
    static QVector<QString> get_batterytemp_warning();
    static QVector<QString> get_wifi_warning();
    static QVector<QString> get_mobile_warning();


    static QVector<QString> get_memory_data();
    static QVector<QString> get_cpu_data();
    static QVector<QString> get_cputemp_data();
    static QVector<QString> get_batterytemp_data();
    static QVector<QString> get_wifi_data();
    static QVector<QString> get_mobile_data();

};

#endif // READSOURCE_H
