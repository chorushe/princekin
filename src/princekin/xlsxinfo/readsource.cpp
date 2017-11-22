#include "readsource.h"

static QVector<QString> memory_time_vector;
static QVector<QString> cpu_time_vector;
static QVector<QString> cputemp_time_vector;
static QVector<QString> batterytemp_time_vector;
static QVector<QString> wifi_time_vector;
static QVector<QString> mobile_time_vector;

static QVector<QString> memory_warning_vector;
static QVector<QString> cpu_warning_vector;
static QVector<QString> cputemp_warning_vector;
static QVector<QString> batterytemp_warning_vector;
static QVector<QString> wifi_warning_vector;
static QVector<QString> mobile_warning_vector;


static QVector<QString> memory_data_vector;
static QVector<QString> cpu_data_vector;
static QVector<QString> cputemp_data_vector;
static QVector<QString> batterytemp_data_vector;
static QVector<QString> wifi_data_vector;
static QVector<QString> mobile_data_vector;


ReadSource::ReadSource()
{
}


void ReadSource::readData(const QString &arg_targetXlsx)
{
    memory_time_vector.clear();
    cpu_time_vector.clear();
    cputemp_time_vector.clear();
    batterytemp_time_vector.clear();
    wifi_time_vector.clear();
    mobile_time_vector.clear();



    memory_warning_vector.clear();
    cpu_warning_vector.clear();
    cputemp_warning_vector.clear();
    batterytemp_warning_vector.clear();
    wifi_warning_vector.clear();
    mobile_warning_vector.clear();


    memory_data_vector.clear();
    cpu_data_vector.clear();
    cputemp_data_vector.clear();
    batterytemp_data_vector.clear();
    wifi_data_vector.clear();
    mobile_data_vector.clear();


    Document xlsxDoc(arg_targetXlsx);
    xlsxDoc.selectSheet("sourceData");

    read_memory_time(xlsxDoc);
    read_cpu_time(xlsxDoc);
    read_cputemp_time(xlsxDoc);
    read_batterytemp_time(xlsxDoc);
    read_wifi_time(xlsxDoc);
    read_mobile_time(xlsxDoc);


    read_memory_warning(xlsxDoc);
    read_cpu_warning(xlsxDoc);
    read_cputemp_warning(xlsxDoc);
    read_batterytemp_warning(xlsxDoc);
    read_wifi_warning(xlsxDoc);
    read_mobile_warning(xlsxDoc);


    read_memory_data(xlsxDoc);
    read_cpu_data(xlsxDoc);
    read_cputemp_data(xlsxDoc);
    read_batterytemp_data(xlsxDoc);
    read_wifi_data(xlsxDoc);
    read_mobile_data(xlsxDoc);


}

void ReadSource::read_memory_time(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,1);
        if(!var.isNull())
        {
            cellValue=var.toString();
            memory_time_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_cpu_time(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,4);
        if(!var.isNull())
        {
            cellValue=var.toString();
            cpu_time_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_cputemp_time(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,7);
        if(!var.isNull())
        {
            cellValue=var.toString();
            cputemp_time_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_batterytemp_time(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,10);
        if(!var.isNull())
        {
            cellValue=var.toString();
            batterytemp_time_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}


void ReadSource::read_wifi_time(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,13);
        if(!var.isNull())
        {
            cellValue=var.toString();
            wifi_time_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_mobile_time(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,16);
        if(!var.isNull())
        {
            cellValue=var.toString();
            mobile_time_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}


void ReadSource::read_memory_warning(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,2);
        if(!var.isNull())
        {
            cellValue=var.toString();
            memory_warning_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_cpu_warning(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,5);
        if(!var.isNull())
        {
            cellValue=var.toString();
            cpu_warning_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_cputemp_warning(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,8);
        if(!var.isNull())
        {
            cellValue=var.toString();
            cputemp_warning_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_batterytemp_warning(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,11);
        if(!var.isNull())
        {
            cellValue=var.toString();
            batterytemp_warning_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_wifi_warning(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,14);
        if(!var.isNull())
        {
            cellValue=var.toString();
            wifi_warning_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_mobile_warning(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,17);
        if(!var.isNull())
        {
            cellValue=var.toString();
            mobile_warning_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}









void ReadSource::read_memory_data(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,3);
        if(!var.isNull())
        {
            cellValue=var.toString();
            memory_data_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_cpu_data(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,6);
        if(!var.isNull())
        {
            cellValue=var.toString();
            cpu_data_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_cputemp_data(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,9);
        if(!var.isNull())
        {
            cellValue=var.toString();
            cputemp_data_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_batterytemp_data(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,12);
        if(!var.isNull())
        {
            cellValue=var.toString();
            batterytemp_data_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_wifi_data(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,15);
        if(!var.isNull())
        {
            cellValue=var.toString();
            wifi_data_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}

void ReadSource::read_mobile_data(Document &arg_xlsxDoc)
{
    int rowIndex=2;
    QString cellValue;
    QVariant var;

    while(1)
    {
        var=arg_xlsxDoc.read(rowIndex,18);
        if(!var.isNull())
        {
            cellValue=var.toString();
            mobile_data_vector.append(cellValue);
        }
        else
        {
            return;
        }
        rowIndex++;
    }
}


QVector<QString> ReadSource::get_memory_time()
{
    return memory_time_vector;
}

QVector<QString> ReadSource::get_cpu_time()
{
    return cpu_time_vector;
}

QVector<QString> ReadSource::get_cputemp_time()
{
    return cputemp_time_vector;
}

QVector<QString> ReadSource::get_batterytemp_time()
{
    return batterytemp_time_vector;
}

QVector<QString> ReadSource::get_wifi_time()
{
    return wifi_time_vector;
}

QVector<QString> ReadSource::get_mobile_time()
{
    return mobile_time_vector;
}


QVector<QString> ReadSource::get_memory_warning()
{
    return memory_warning_vector;
}
QVector<QString> ReadSource::get_cpu_warning()
{
    return cpu_warning_vector;
}
QVector<QString> ReadSource::get_cputemp_warning()
{
    return cputemp_warning_vector;
}

QVector<QString> ReadSource::get_batterytemp_warning()
{
    return batterytemp_warning_vector;
}
QVector<QString> ReadSource::get_wifi_warning()
{
    return wifi_warning_vector;
}

QVector<QString> ReadSource::get_mobile_warning()
{
    return mobile_warning_vector;
}


QVector<QString> ReadSource::get_memory_data()
{
    return memory_data_vector;
}

QVector<QString> ReadSource::get_cpu_data()
{
    return cpu_data_vector;
}

QVector<QString> ReadSource::get_cputemp_data()
{
    return cputemp_data_vector;
}

QVector<QString> ReadSource::get_batterytemp_data()
{
    return batterytemp_data_vector;
}

QVector<QString> ReadSource::get_wifi_data()
{
    return wifi_data_vector;
}

QVector<QString> ReadSource::get_mobile_data()
{
    return mobile_data_vector;
}
