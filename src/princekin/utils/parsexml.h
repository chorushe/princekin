#ifndef PARSEXML_H
#define PARSEXML_H

#include "commonhead.h"
#include "componenthead.h"
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>



class ParseXml
{
public:
    ParseXml();
public:
    static bool readPvXml(const QString &,const QString &);
    static bool readVvXml(const QString &);
    static bool readBdXml(const QString &);
    static bool readCatonXml(const QString &);
    static bool readConfigXml(const QString &);
    static QStringList readConfigXmlForBlackList(const QString & xmlfile);
    static QStringList readConfigXmlForIP_Port(const QString & xmlfile);
    static void saveConfigXmlIP_Port(QString IP, QString port);
    static QString EditMultiXml(QString filePath, QStringList listData);
    static void parseIP_PortToBat(QString fileName);

    //void clearPvHash();
    //void clearVvHash();
};

#endif // PARSEXML_H
