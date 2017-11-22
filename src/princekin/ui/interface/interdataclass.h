#ifndef INTERDATACLASS_H
#define INTERDATACLASS_H
#include <QString>


class interDataClass
{
public:
    interDataClass();

    int ID;
    QString url;
    QString response;
    QString httpVersion;
    QString method;
    int time;
    QString statusCode;
    QString contentType;
    QString contentLength;
    QString requestParam;
};

#endif // INTERDATACLASS_H
