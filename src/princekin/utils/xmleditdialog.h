#ifndef XMLEDITDIALOG_H
#define XMLEDITDIALOG_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QCloseEvent>

class XmlEditDialog : public QWidget
{
    Q_OBJECT
public:
    explicit XmlEditDialog(QWidget *parent = 0,QString filePath="");
    ~XmlEditDialog();

signals:

protected:
    void closeEvent(QCloseEvent *event);

private:
    QTextEdit *xmlEditText;
    QHBoxLayout *mainLayout;

    QString filePath;
    bool saveFlag;

    void ReadXMLFile(QString filePath);

public slots:
    void keyPressEvent(QKeyEvent *e);
    void XmlTextChanged();
};

#endif // XMLEDITDIALOG_H
