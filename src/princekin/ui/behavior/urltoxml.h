#ifndef URLTOXML_H
#define URLTOXML_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QHash>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QtXml>
#include <QDomDocument>
#include <QDateTime>
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QScrollBar>
#include <QtXlsx>
#include <QListView>
#include "globalvar.h"
#include "execmd.h"
#include <QAxObject>
#include <QWheelEvent>


using namespace QXlsx;

namespace Ui {
class UrlToXml;
}

class UrlToXml : public QMainWindow
{
    Q_OBJECT

public:
    explicit UrlToXml(QWidget *parent = 0);
    ~UrlToXml();

    QStringList statisticList;
    QString domainName;
    QStringList descList;
    QList<int> verifyList;
    int uniqueNum=0;
    QString uniqueStr="";
    QStringList uniqueList;

    int saveXml(QString sXmlFilePath);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::UrlToXml *ui;
    QString UrlDecode(QString str);
    QString xmlPath;
    QString rightXmlPath;
    bool isMultiPattern=false;
    QList<QPair<QString,QString>> fileInfoList;
    QPair<QString,QString> filepair;
    QStringList fileNameList;//装载xml文件名，用来判断list中是否有重复的文件名
    bool repeatFlag=false;//判断生成的文件名中是否有重复文件
    bool errorFlag=true;//在生成过程中是否有失败，如有失败就不提示生成成功了

    QVariant excelVar;

    void readExcelData(QString fileName);
    void ShowFileInfo();
    void changeUnique(QString filePath, QString uniqueStr, QString des, QString nullnote);

public slots:
    void CalculateUnique(int index);
private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_changeBtn_clicked();
    void on_openFileBtn_clicked();

    void on_multiCBox_clicked();

    void on_browseBtn_clicked();

    void on_fileTableWidget_itemChanged(QTableWidgetItem *item);

    void on_downLoadExcelBtn_clicked();

    void on_importExcelBtn_clicked();

    void on_xmlPathLineEdit_textChanged(const QString &arg1);

    void on_xmlPathLineEdit_editingFinished();

protected:


};

#endif // URLTOXML_H
