#ifndef RECORD_H
#define RECORD_H

#include <QTextCursor>
#include <QObject>
#include <QByteArray>
#include <QTextOption>
#include <QMenuBar>
#include <QFileDialog>
#include <QScrollBar>
#include <QMainWindow>
#include <QPointer>
#include <QToolButton>
#include <QLineEdit>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QFileSystemModel>
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>

#include "deletedialog.h"
#include "createproject.h"
#include "globalvar.h"
#include "parsexml.h"
#include "newwidget.h"
#include "execmd.h"
#include "filemanager.h"
#include "renamedialog.h"
#include "sohuthread.h"
#include "controller.h"
#include "worker.h"
#include "helper.h"
#include "newscriptwidget.h"
#include "editscriptfilewidget.h"

namespace Ui {
class Record;
}


class MyThread : public QThread
{
public:
    QString filePath;
    QFileSystemModel *model;
    QModelIndex index;
public:
    void initArg(const QString &,QFileSystemModel *,QModelIndex);
    virtual void run();
};

class Record : public QMainWindow,FileManager
{
    Q_OBJECT

public:
    explicit Record(QWidget *parent = 0);
    ~Record();


public:
    void showInit();

private:
    void initFont();
    void createMenuBar();
    void createMenuItems();

    void createToolBars();
    void createToolButtons();

    void initConfig();

    void createTree(const QString &);
    void initPopupMenu();

    void openFile(const QString &);
    void doubleClickOpenFile(const QString &);

    void deleteFile(const QString &);

    void saveAndClose(int,const QString &);
    void setRecordButtonJ();
    void convertSelects(QModelIndexList);
    void convertMoreToUiautomator(const QString &);
    bool eventFilter(QObject *,QEvent *);

    void record1();

    void socketClient();
    void replayUiautomator();
    void killUiautomator();
    void compileOneUiautomator();
    QString compileMoreUiautomator(const QString &);
    QString getJavacCmd(const QString &);
    QString getJavacCmd1(const QString &);
    QString getJavacCmd2(QStringList,const QString &);

    void packUiautomatorJar();
    void startReplay(const QString&);

    QString getJavaFileSaveDir1();
    QString getClassSaveDir1();
    QString getJavaFileSaveDir2();
    QString getProjectName2();
    QString getModelName2();
    QString getClassSaveDir2();

    void setReplayButtonJ();

    void openUiautomatorFile();
    void convertToUiautomator();

    void ctrlS();

    void newCreateScriptSave(const QString &);

    void saveAll();
    bool isStruct(QModelIndexList);

    QString getWorkspace();
    QStringList getDeviceList();


signals:
    void signal_closeProc();
    void sendStopCompileMore();

private slots:
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_tabCloseRequested(int index);

    void on_tabWidget_tabBarClicked(int index);

    void compileSlot();

    void createScriptSlot();
    void deleteProjectSlot();
    void setButtonIconSlot1(int);

    void slot_OutPutProxy(const QString &);

    void recordSlot1();
    void replaySlot();

    void editChangeSlot();
    //void editChangeSlot(QTextEdit *);
    void slot_OutPutRecord(const QString &);
    void setButtonIconSlot3(int);

    void slot_OutPutReplay(const QString &);

    void createModuleSlot();
    void createSuiteModuleSlot();
    void createSuiteFileSlot();
    void editSuiteFileSlot();

    void loadRepositorySlot();

    void on_treeView_clicked(const QModelIndex &index);
    void setCompileMoreButtonIconSlot(int);

    void receiveCompileMoreResult(const QString &);
    void receivePackResult(const QString &);
    void packJarSlot();
    void receiveReplayResult(const QString &);

    void receiveCompileResult(const QString &);

    void slot_quit();

    void myslot(QString);

    void on_pushButton_clicked();
    void receiveRepositoryResult(const QString&,const QString&);

    void recieveAddRepositoryPath(QString filepath);

    void RecieveDevicesSigal(QStringList devicesList);
    void NewScriptSlot();

    void on_newMoudleBtn_clicked();

private:
    Ui::Record *ui;

    QAction *action;
    QMenu *popMenu0;
    QMenu *popMenu1;
    QMenu *popMenu2;
    QMenu *popMenu3;
    QMenu *popMenuNewSuite;
    QMenu *popMenuNewSuiteFile;
    QMenu *popMenuEditSuiteFile;

    QMenuBar *myMenuBar;
    QMenu *menuSetting;

    QToolBar *toolBar1;
    QToolButton *recordButton;
    QLineEdit *loopEdit;
    QToolButton *replayButton;

    QToolBar *toolBar2;
    QToolButton *compileButton;
    QToolButton *packJarButton;

    QToolButton *compileMoreJavaButton;
    QFileSystemModel *model;
    QModelIndex qModelIndex;

    QToolButton *compileOneJavaButton;

    QToolButton *settingButton;

    QToolBar *toolBar4;
    QToolButton *newScriptButton;
    QToolButton *addScriptButton;
    NewScriptWidget *newScriptDialog=NULL;

    QFont myFont;

    QMovie *myMovie;

    int currentIndex;

    QString selectDeviceID;
    QString currentOpenFile;
    QString selectTreePath;
    QString clickTreePath;
    QString projectDir;
    QString uiautomatorDir;

    QStringList openedFileList;

    QThread *workThread;

    SohuThread *ThreadInstance;

    QThread *workThread1;
    QThread *workThread2;

    QTextEdit *qEdit;

    QString strCodec;
    QTextCodec *tc=QTextCodec::codecForName("gb2312");
    //QTextCodec *tc=QTextCodec::codecForName("GBK");
    //QTextCodec *tc=QTextCodec::codecForName("UTF-8");

    bool isCompileing=false;

    int handOpened=0;
    int numForDeviceSignal=0;

    EditScriptFileWidget *editSuiteDialog=NULL;

};

#endif // RECORD_H
