#ifndef NEWSCRIPTWIDGET_H
#define NEWSCRIPTWIDGET_H

#include <QWidget>

namespace Ui {
class NewScriptWidget;
}

class NewScriptWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewScriptWidget(QWidget *parent = 0);
    ~NewScriptWidget();

private slots:
    void on_browseBtn_clicked();

    void on_scriptPathLineEdit_textChanged(const QString &arg1);

    void on_scriptNameLineEdit_textChanged(const QString &arg1);

private:
    Ui::NewScriptWidget *ui;

    void textChange();
    QString repositoryName;
    QString workSpace;

    //QString getWorkspace();
protected:
    void closeEvent(QCloseEvent *event);

signals:
    void sendRepositoryResult(const QString&,const QString&);

    //***************20170713***************//
public:
    void setScriptDir(const QString &);

    //***************20170713***************//
};

#endif // NEWSCRIPTWIDGET_H
