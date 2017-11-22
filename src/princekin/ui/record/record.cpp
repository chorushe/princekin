#include "record.h"
#include "ui_record.h"

using namespace std;

static int n=0;
static bool isChanged=false;
static bool isopen=false;
static bool isReplaying=false;
static bool isRecording=false;
static bool isCompileing=false;
static QStringList fileNameList;
static QStringList glIndexList;
static QStringList glTextModifyList;
static QList<QTextEdit *> textEditList;
static QHash<QString,QTextEdit *> textEditHash;

Record::Record(QWidget *parent) :QMainWindow(parent),ui(new Ui::Record)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0,10);//右
    // ui->splitter->setStretchFactor(1,3);

    ui->splitter_2->setStretchFactor(0,1);//左
    ui->splitter_2->setStretchFactor(1,7);
    ui->splitter_2->setStretchFactor(2,2);

    ui->splitter_3->setStretchFactor(0,3);//全
    ui->splitter_3->setStretchFactor(1,7);


    initFont();

    createToolBars();
    createToolButtons();


    createTree(gTreeRootDir);//获取currentRepository.txt文件中的上次打开的记录
    initPopupMenu();

    connect(ui->treeView, SIGNAL(objectNameChanged(const QString&)), this, SLOT(myslot(QString)));
    connect(ui->newMoudleBtn,SIGNAL(clicked(bool)),this,SLOT(createModuleSlot()));

    ui->textEdit->setText("<font color=gray>脚本输出显示区</font>");

    strCodec="gb2312";

    //设置设备显示框的属性
    ui->listWidget->setStyleSheet("QListWidget{font-size:12px;background:#ffffff;border:1px #ffffff;}"
                                  "QListWidget::item:selected:active{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                  "QListWidget::item:selected{background:#e4e4e4;color:#000000;padding-left:8px;}"
                                  "QListWidget::item{height:30px;color:#4d4d4d}");
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                       "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                     "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->treeView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                     "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    ui->treeView->setStyleSheet("QTreeView{border:1px #ffffff;background:#ffffff;}");


    ui->newMoudleBtn->setToolTip("新建模块");
}

Record::~Record()
{
    delete ui;
}

void Record::myslot(QString arg_str)
{
}


void MyThread::initArg(const QString &mFilePath,QFileSystemModel *mMmodel,QModelIndex mIndex)
{
    filePath=mFilePath;
    model=mMmodel;
    index=mIndex;
}
void MyThread::run()
{
    if( !index.isValid() )
    {
    }
    bool ok;
    if( model->fileInfo(index).isDir() )
    {
        ok = model->remove(index);
    }

    else
    {
        ok=model->remove(index);
    }

    QProcess p;
    QString s1="rd /S /Q ";
    QString s2=s1 + QDir::toNativeSeparators(filePath);

    QStringList list;
    list<<"/c"<<s2;
    p.start("cmd", QStringList()<<"/c"<<s2 );
    p.waitForStarted();
    p.waitForFinished();
}


void Record::initFont()
{
    myFont.setPointSize(14);
}


void Record::createToolBars()
{
    toolBar1=addToolBar("record");
    toolBar2=addToolBar("compile");

    toolBar4=addToolBar("script");

    toolBar1->setStyleSheet("QToolBar{background:#ffffff;}");
    toolBar2->setStyleSheet("QToolBar{background:#ffffff;}");
    toolBar4->setStyleSheet("QToolBar{background:#ffffff;}");
}

void Record::createToolButtons()
{
    recordButton=new QToolButton(this);
    recordButton->setToolTip("录制");
    recordButton->setIcon(QIcon(":/record_j.png"));
    recordButton->setFixedWidth(50);
    recordButton->setStyleSheet("QToolButton{background:#ffffff;}");
    connect(recordButton,SIGNAL(clicked()),this,SLOT(recordSlot1()));
    toolBar1->addWidget(recordButton);

    replayButton=new QToolButton(this);
    replayButton->setToolTip("回放");
    replayButton->setIcon(QIcon(":/replay_j.png"));
    replayButton->setFixedWidth(50);
    replayButton->setStyleSheet("QToolButton{background:#ffffff;}");
    connect(replayButton,SIGNAL(clicked()),this,SLOT(replaySlot()));
    toolBar1->addWidget(replayButton);


    compileButton=new QToolButton(this);
    compileButton->setToolTip("编译");
    compileButton->setIcon(QIcon(":/compilemore_j.png"));
    compileButton->setFixedWidth(50);
    compileButton->setStyleSheet("QToolButton{background:#ffffff;}");
    connect(compileButton,SIGNAL(clicked()),this,SLOT(compileSlot()));
    toolBar2->addWidget(compileButton);

    packJarButton=new QToolButton(this);
    packJarButton->setToolTip("打包");
    packJarButton->setIcon(QIcon(":/wrap_j.png"));
    packJarButton->setFixedWidth(50);
    packJarButton->setStyleSheet("QToolButton{background:#ffffff;}");
    connect(packJarButton,SIGNAL(clicked()),this,SLOT(packJarSlot()));
    toolBar2->addWidget(packJarButton);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spacer->setStyleSheet("QWidget{background:#ffffff;}");

    newScriptButton=new QToolButton(this);
    newScriptButton->setToolButtonStyle((Qt::ToolButtonTextBesideIcon));
    newScriptButton->setToolTip("新建脚本库");
    newScriptButton->setText("新建脚本库");
    newScriptButton->setIcon(QIcon(":/newscript.png"));
    newScriptButton->setFixedWidth(120);
    newScriptButton->setFixedHeight(30);
    newScriptButton->setStyleSheet("QToolButton{background:#ffffff;}");
    addScriptButton=new QToolButton(this);
    addScriptButton->setToolButtonStyle((Qt::ToolButtonTextBesideIcon));
    addScriptButton->setToolTip("加载脚本库");
    addScriptButton->setText("加载脚本库");
    addScriptButton->setIcon(QIcon(":/addscript.png"));
    addScriptButton->setFixedWidth(120);
    addScriptButton->setFixedHeight(30);
    addScriptButton->setStyleSheet("QToolButton{background:#ffffff;}");
    toolBar4->addWidget(newScriptButton);
    toolBar4->addWidget(addScriptButton);
    toolBar4->addWidget(spacer);
    //toolBar4->addWidget(settingButton);
    connect(newScriptButton,SIGNAL(clicked(bool)),this,SLOT(NewScriptSlot()));
    connect(addScriptButton, SIGNAL(clicked(bool)), this, SLOT(loadRepositorySlot()));
}

void Record::NewScriptSlot()
{
    if(newScriptDialog==NULL)
    {
        newScriptDialog=new NewScriptWidget();
        connect(newScriptDialog,SIGNAL(sendRepositoryResult(QString,QString)),this,SLOT(receiveRepositoryResult(QString,QString)));
    }

    newScriptDialog->setScriptDir(gWorkSpace);
    newScriptDialog->show();
}

void Record::createTree(const QString &arg_scriptDir)
{
    if(arg_scriptDir.isEmpty())
    {
        return;
    }
    model=new QFileSystemModel(this);
    model->setNameFilterDisables(false);
    model->setRootPath(arg_scriptDir);

    QStringList mFilterList;
    mFilterList<<"*.txt";
    model->setNameFilters(mFilterList);

    ui->treeView->setModel(NULL);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(arg_scriptDir));
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);

    ui->treeView->setHeaderHidden(true);
}

void Record::initPopupMenu()
{
    popMenu1=new QMenu;
    action=new QAction("新建模块",this);
    popMenu1->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(createModuleSlot()));

    popMenu2=new QMenu;
    action=new QAction("新建脚本",this);
    popMenu2->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(createScriptSlot()));

    popMenu3=new QMenu;
    action=new QAction("删除脚本",this);
    popMenu3->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(deleteProjectSlot()));

    popMenuNewSuite=new QMenu;
    action=new QAction("新建测试套文件夹",this);
    popMenuNewSuite->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(createSuiteModuleSlot()));
    action=new QAction("新建测试套文件",this);
    popMenuNewSuite->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(createSuiteFileSlot()));

    popMenuNewSuiteFile=new QMenu;
    action=new QAction("新建测试套文件",this);
    popMenuNewSuiteFile->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(createSuiteFileSlot()));

    popMenuEditSuiteFile=new QMenu;
    action=new QAction("编辑测试套文件",this);
    popMenuEditSuiteFile->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(editSuiteFileSlot()));
    action=new QAction("删除测试套文件",this);
    popMenuEditSuiteFile->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(deleteProjectSlot()));

}

void Record::createModuleSlot()
{
    if(selectTreePath=="")
    {
        return;
    }

    NewWidget *newDialog=new NewWidget;
    newDialog->setLabelText("请输入模块名称:");
    newDialog->InitVar(selectTreePath,1);
    newDialog->show();
}

void Record::createSuiteModuleSlot()
{
    if(selectTreePath=="")
    {
        return;
    }

    NewWidget *newDialog=new NewWidget;
    newDialog->setLabelText("请输入测试套文件夹名称:");
    newDialog->InitVar(selectTreePath,2);
    newDialog->show();
}

void Record::createSuiteFileSlot()
{
    if(selectTreePath=="")
    {
        return;
    }

    NewWidget *newDialog=new NewWidget;
    newDialog->setLabelText("请输入测试套文件名称:");
    newDialog->InitVar(selectTreePath,3);
    newDialog->show();
}

void Record::editSuiteFileSlot()
{
    if(editSuiteDialog==NULL)
    {
        editSuiteDialog=new EditScriptFileWidget();
    }
    editSuiteDialog->InitVar(selectTreePath);
    editSuiteDialog->show();
    editSuiteDialog->raise();
}

void Record::on_treeView_customContextMenuRequested(const QPoint &pos)//右键菜单触发
{
    qModelIndex=ui->treeView->indexAt(pos);
    if(qModelIndex.isValid())
    {
        selectTreePath=model->filePath(qModelIndex);
        selectTreePath=QDir::toNativeSeparators(selectTreePath);
        QStringList mList=parsePath(QDir::toNativeSeparators(selectTreePath));

        //"" testcase script play a.txt
        //"" testcase invoke a.txt
        bool okDir=isD(selectTreePath);
        int mCount=mList.count();

        switch (mCount)
        {
        case 3:
            if(okDir && mList.at(2)=="script")
            {
                popMenu1->exec(QCursor::pos());//新建模块
            }
            else if(okDir && mList.at(2)=="invoke")
            {
                popMenu2->exec(QCursor::pos());//新建脚本
            }
            else if(okDir && mList.at(2)=="suite")
            {
                popMenuNewSuite->exec(QCursor::pos());
            }
            break;
        case 4:
            if(okDir && mList.at(2)=="script")
            {
                popMenu2->exec(QCursor::pos());
            }
            else if(okDir && mList.at(2)=="suite")
            {
                popMenuNewSuiteFile->exec(QCursor::pos());
            }

            else if(!okDir && mList.at(2)=="invoke")
            {
                popMenu3->exec(QCursor::pos());//删除脚本
            }

            else if(!okDir && mList.at(2)=="suite")
            {
                popMenuEditSuiteFile->exec(QCursor::pos());
            }

            break;
        case 5:
            if(!okDir)
            {
                if(mList.at(2)=="suite")
                    popMenuEditSuiteFile->exec(QCursor::pos());
                else
                    popMenu3->exec(QCursor::pos());
            }
            break;
        default:
            break;
        }
    }
}

void Record::on_treeView_doubleClicked(const QModelIndex &index)
{
    currentOpenFile=model->filePath(index);
    currentOpenFile=QDir::toNativeSeparators(currentOpenFile);
    QFileInfo mFileInfo(currentOpenFile);
    if(mFileInfo.isFile())
    {
        QStringList mList=parsePath(currentOpenFile);
        int mCount=mList.count();

        if(mCount==5)
        {
            doubleClickOpenFile(currentOpenFile);
        }
        if(mCount==4 &&(mList.at(2)=="invoke" || mList.at(2)=="var" || mList.at(2)=="command" || mList.at(2)=="suite") ||mList.at(2)=="方法库")
        {
            openFile(currentOpenFile);
        }
        else
        {
            return;
        }
    }
}



void Record::on_tabWidget_tabBarClicked(int index)
{
    n=0;
    currentIndex=index;
    currentOpenFile=openedFileList.at(index);
    currentOpenFile=QDir::toNativeSeparators(currentOpenFile);

}


void Record::on_tabWidget_tabCloseRequested(int index)
{
    if(index != currentIndex)
    {
        ui->tabWidget->setCurrentIndex(index);
    }

    QString closeingFileName=openedFileList.at(index);

    if(glTextModifyList.contains(closeingFileName))
    {
        QMessageBox::StandardButton jValue=QMessageBox::information(NULL, "提示", "   文件已修改,是否保存?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(jValue==QMessageBox::Yes)
        {
            saveAndClose(index,closeingFileName);
        }
        glTextModifyList.removeAt( glTextModifyList.indexOf(closeingFileName) );
    }




    ui->tabWidget->removeTab(index);
    openedFileList.removeAt(index);

    textEditHash.remove(closeingFileName);

    currentIndex=ui->tabWidget->currentIndex();
    if(currentIndex==-1)
    {
    }
    else
    {
        currentOpenFile=openedFileList.at(currentIndex);
        currentOpenFile=QDir::toNativeSeparators(currentOpenFile);
    }
    n=0;
}

bool Record::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::FocusIn)//在edit界面点击鼠标时，把 点击文字编辑脚本 文字去掉
    {
        QWidget * mWidget=ui->tabWidget->currentWidget();
        QTextEdit *mEdit=qobject_cast<QTextEdit *>(mWidget);
        if(mEdit->toPlainText()=="点击文字编辑脚本")
        {
            mEdit->clear();
        }
    }


    if(event->type() == QEvent::KeyPress)//Ctrl S按键
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_S && (keyEvent->modifiers() & Qt::ControlModifier))
        {
            ctrlS();
        }
    }
    /*else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_A && (keyEvent->modifiers() & Qt::ControlModifier))
        {
            ui->treeView->selectAll();
        }
    }
    else
    {
        return false;
    }*/
    return QWidget::eventFilter(watched,event);

}

void Record::ctrlS()
{
    QWidget * mWidget=ui->tabWidget->currentWidget();
    QTextEdit *mEdit=qobject_cast<QTextEdit *>(mWidget);
    QString mStr=mEdit->toPlainText();

    currentOpenFile=openedFileList.at(currentIndex);
    currentOpenFile=QDir::toNativeSeparators(currentOpenFile);
    QFile mFile(currentOpenFile);
    if(!mFile.exists())
    {
        if(glTextModifyList.contains(currentOpenFile))
        {
            int mIndex=glTextModifyList.indexOf(currentOpenFile);
            glTextModifyList.removeAt(mIndex);
        }
        return;
    }
    if(!mFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        mFile.close();
        return;
    }
    QTextStream out(&mFile);

    //QTextCodec *tc=QTextCodec::codecForName("GBK");
    //QTextCodec *tc=QTextCodec::codecForName("UTF-8");
    out.setCodec(tc);
    out<<mStr;
    mFile.close();

    if(glTextModifyList.contains(currentOpenFile))
    {
        int mIndex=glTextModifyList.indexOf(currentOpenFile);
        glTextModifyList.removeAt(mIndex);
    }
}

void Record::saveAll()
{
    QList<QString> mList=textEditHash.keys();
    foreach (QString sss, mList)
    {
        QTextEdit *mEdit=textEditHash.value(sss);

        QString mStr=mEdit->toPlainText();

        QFile mFile(sss);
        if(!mFile.exists())
        {
            if(glTextModifyList.contains(sss))
            {
                int mIndex=glTextModifyList.indexOf(sss);
                glTextModifyList.removeAt(mIndex);
            }
            continue;
        }
        if(!mFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            mFile.close();
            return;
        }
        QTextStream out(&mFile);

        //QTextCodec *tc=QTextCodec::codecForName("GBK");
        //QTextCodec *tc=QTextCodec::codecForName("UTF-8");

        out.setCodec(tc);
        out<<mStr;
        mFile.close();

        if(glTextModifyList.contains(sss))
        {
            int mIndex=glTextModifyList.indexOf(sss);
            glTextModifyList.removeAt(mIndex);
        }

    }
}

void Record::saveAndClose(int index,const QString &closeingFileName)
{
    QWidget * mWidget=ui->tabWidget->currentWidget();
    QTextEdit *mEdit=qobject_cast<QTextEdit *>(mWidget);
    QString mStr=mEdit->toPlainText();

    QFile mFile(closeingFileName);
    if(!mFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        mFile.close();
        return;
    }
    QTextStream out(&mFile);

    //QTextCodec *tc=QTextCodec::codecForName("GBK");
    //QTextCodec *tc=QTextCodec::codecForName("UTF-8");

    out.setCodec(tc);
    out<<mStr;
    mFile.close();
}


void Record::on_tabWidget_currentChanged(int index)
{
}


void Record::deleteProjectSlot()
{
    QMessageBox::StandardButton jValue=QMessageBox::information(NULL, "提示", "   是否删除？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(jValue==QMessageBox::Yes)
    {
        deleteFile(selectTreePath);
    }
}
void Record::deleteFile(const QString & mFilePaths)
{
    QModelIndex mIndex = ui->treeView->currentIndex();

    MyThread t;
    t.initArg(mFilePaths,model,mIndex);
    t.start();
    t.wait();
}



void Record::setButtonIconSlot1(int)
{
    recordButton->setIcon(QIcon(myMovie->currentPixmap()));
}



void Record::slot_OutPutProxy(const QString & arg)
{
    ui->textEdit->append(arg);
}



void Record::editChangeSlot()
{
    if(glTextModifyList.contains(currentOpenFile))
    {
    }
    else
    {
        glTextModifyList.append(currentOpenFile);
    }

    if(n==0)
    {
        n=1;
    }
}





void Record::createScriptSlot()
{
    NewWidget *newDialog=new NewWidget;
    newDialog->setLabelText("请输入脚本名称:");
    newDialog->InitVar(selectTreePath,0);
    newDialog->show();
}


//************************************以下录制回放********************************************//
void Record::recordSlot1()
{
    if(isRecording)
    {
        ExeCmd::runCmd("taskkill /F /IM adb.exe");
        setRecordButtonJ();
        isRecording=false;
    }
    else
    {
        if(ui->tabWidget->currentIndex()==-1)
        {
            QMessageBox::information(NULL,"提示","请打开脚本文件");
            return;
        }

        QList<QListWidgetItem*> mListWidgetItemList=ui->listWidget->selectedItems();
        int mDeviceCount=mListWidgetItemList.count();
        if(mDeviceCount==0)
        {
            QMessageBox::information(NULL,"提示","请选择设备");
            return;
        }
        selectDeviceID=ExeCmd::getDeviceSN( mListWidgetItemList.at(0)->text());


        QString ver1=ExeCmd::getVersion("adb -s " + selectDeviceID + " shell getprop");

        QString ver2="4.3";
        if(ver1<ver2)
        {
            QMessageBox::information(NULL,"提示","系统版本太低啦");
            return;
        }


        myMovie = new QMovie(":/record_d.gif");
        connect(myMovie,SIGNAL(frameChanged(int)),this,SLOT(setButtonIconSlot1(int)));
        myMovie->start();

        QWidget *w=ui->tabWidget->currentWidget();
        qEdit=qobject_cast<QTextEdit *>(w);

        isRecording=true;
        record1();
    }
}


void Record::record1()
{
    QString mCmdLine;

    mCmdLine="adb -s " + selectDeviceID  + " push " + gConfigDir + QDir::separator() +  "script_Dump.jar /sdcard/";
    ExeCmd::runCmd(mCmdLine);

    mCmdLine="adb -s " + selectDeviceID  + " shell uiautomator runtest /sdcard/script_Dump.jar -c com.sohu.test.Dump";
    ExeCmd::runCmd(mCmdLine);

    mCmdLine="adb -s " + selectDeviceID  + " pull /data/local/tmp/local/tmp/dump.xml " + gConfigDir + QDir::separator();
    ExeCmd::runCmd(mCmdLine);



    mCmdLine="adb -s " + selectDeviceID  + " shell getevent -qlt";
    workThread=new QThread();
    ThreadInstance=new SohuThread();
    ThreadInstance->moveToThread(workThread);
    ThreadInstance->setN(1);
    ThreadInstance->setDeviceId(selectDeviceID);
    ThreadInstance->setCmdLine(mCmdLine);

    connect(ThreadInstance, SIGNAL(mySignal(const QString&)), this, SLOT(slot_OutPutRecord(const QString&)));
    connect(workThread, SIGNAL(started()), ThreadInstance, SLOT(slot_getEvent()));
    connect(workThread, SIGNAL(finished()), ThreadInstance, SLOT(deleteLater()));
    workThread->start();

}



void Record::slot_OutPutRecord(const QString & arg_line)
{
    qEdit->textCursor().insertText(arg_line);
    qEdit->moveCursor(QTextCursor::EndOfLine);
}







void Record::replaySlot()
{
    QList<QListWidgetItem*> mListWidgetItemList=ui->listWidget->selectedItems();
    int mDeviceCount=mListWidgetItemList.count();
    if(mDeviceCount==0)
    {
        QMessageBox::information(NULL,"提示","请选择设备");
        return;
    }

    if(isRecording)
    {
        QMessageBox::information(NULL,"提示","请停止录制");
        return;
    }

    selectDeviceID=ExeCmd::getDeviceSN( mListWidgetItemList.at(0)->text());

    replayUiautomator();
}
void Record::setButtonIconSlot3(int)
{
    replayButton->setIcon(QIcon(myMovie->currentPixmap()));
}


void Record::replayUiautomator()
{
    if(isReplaying)
    {
        killUiautomator();
        emit signal_closeProc();
        //setReplayButtonJ();
        isReplaying=false;
    }
    else
    {
        if(ui->tabWidget->currentIndex()==-1)
        {
            QMessageBox::information(NULL,"提示","请打开脚本文件");
            return;
        }

        QString mBaseName;
        QString mCmdLine;
        QString mModelName;
        QString mProjectName;
        QString mJarName;
        QString mAutomatorJar;
        QString mJavaFileSaveDir;
        QString mJarDir;

        QStringList mList=parsePath(currentOpenFile);
        //"" testcase script play a.txt
        if(mList.count()!=5)
        {
            ui->textEdit->setText("选择脚本有误");
            return;
        }

        QString s1=mList.at(2);
        if(s1=="invoke" || s1=="var" || s1=="方法库" || s1=="command")
        {
            ui->textEdit->setText("选择脚本有误");
            return;
        }



        mBaseName=getBaseName(currentOpenFile);

        mModelName=mList.at(3);
        mJarName=mModelName+".jar";

        mJarDir=gUiautomatorDir + QDir::separator() + mModelName + QDir::separator() + "bin" + QDir::separator() + mJarName;



        ExeCmd::runCmd("adb -s " + selectDeviceID + " shell mkdir /sdcard/temppng/");
        createPath(gDebugDir + QDir::separator() + selectDeviceID);


        mCmdLine="adb -s " + selectDeviceID + " push " + mJarDir + " /sdcard/";
        QString mResult=ExeCmd::runCmd(mCmdLine);
        ui->textEdit->append(mResult);

        //mCmdLine="adb -s "+selectDeviceID+" shell uiautomator runtest /sdcard/" + mJarName;
        mCmdLine="adb -s "+selectDeviceID+" shell uiautomator runtest /sdcard/" + mJarName + " -c com.sohu.test." + mBaseName  + " -e checkStat no";


        isReplaying=true;

        myMovie = new QMovie(":/replay_d.png");
        connect(myMovie,SIGNAL(frameChanged(int)),this,SLOT(setButtonIconSlot3(int)));
        myMovie->start();

        ui->listWidget->setEnabled(false);//开始执行时不允许更换设备   hechang

        ui->textEdit->clear();

        startReplay(mCmdLine);
    }

}

void Record::startReplay(const QString &arg_cmdline)
{
    Controller *control=new Controller();
    control->setDeviceId(selectDeviceID);
    connect(control, SIGNAL(sendReplayResult(const QString&)), this, SLOT(receiveReplayResult(const QString&)));
    control->startReplay(arg_cmdline);
}

void Record::receiveReplayResult(const QString &arg_line)
{
    if(arg_line.contains("replayFinished"))
    {
        ui->textEdit->append(arg_line);
        setReplayButtonJ();
        isReplaying=false;
    }
    else
    {
        ui->textEdit->append(arg_line);
    }
}

void Record::slot_OutPutReplay(const QString & arg_line)//这个函数好像没用
{
    if(arg_line.contains("INSTRUMENTATION_STATUS_CODE: -1"))
    {
        ui->textEdit->append(arg_line);
        setReplayButtonJ();
        isReplaying=false;
    }
    else
    {
        ui->textEdit->append(arg_line);
    }
}

void Record::setRecordButtonJ()
{
    myMovie->stop();
    delete myMovie;
    recordButton->setIcon(QIcon(":/record_j.png"));
}


void Record::setReplayButtonJ()
{

    myMovie->stop();
    delete myMovie;
    replayButton->setIcon(QIcon(":/replay_j.png"));

    ui->listWidget->setEnabled(true);//执行结束后设备可用   hechang
}






void Record::killUiautomator()
{
    QString mCmdLine="adb -s " + selectDeviceID + " shell ps | grep uiautomator";
    QString mPid=ExeCmd::getLinuxPid(mCmdLine,"uiautomator");
    if(mPid.isEmpty())
    {
    }
    else
    {
        mCmdLine="adb -s " + selectDeviceID + " shell kill " + mPid;
        ExeCmd::runCmd(mCmdLine);
    }
}

//************************************以上录制回放 处理方法********************************************//













void Record::on_treeView_clicked(const QModelIndex &index)
{
    clickTreePath=model->filePath(index);
}

//**************************************文件编译处理 slot***********************************************//

bool Record::isStruct(QModelIndexList arg_selectedIndexList)
{
    bool okDir;
    bool isSelectDir=false;
    bool isSelectFile=false;

    int mCount;
    int dirCount=0;
    QString mStr;
    QString mParentPath;
    QString mSelectedItem;
    QStringList mdirList;
    QStringList mSplistResult;

    mCount=arg_selectedIndexList.count();

    for(int i=0;i<mCount;i++)
    {
        okDir=model->fileInfo(arg_selectedIndexList.at(i)).isDir();
        if(okDir)
        {
            dirCount++;
        }
    }
    if(dirCount>=2)//文件夹超过两个错误，只能对一个文件夹即一个模块完整编译
    {
        QMessageBox::warning(this,"提示","两个模块不能同时编译");
        return false;
    }

    //**********************************************************//
    for(int i=0;i<mCount;i++)
    {
        mSelectedItem=model->filePath(arg_selectedIndexList.at(i));
        mSelectedItem=QDir::toNativeSeparators(mSelectedItem);
        mSplistResult=parsePath(mSelectedItem);

        if( mSplistResult.at(2)=="invoke")
        {
            QMessageBox::warning(this,"提示","invoke文件不能编译");
            return false;
        }
        else if( mSplistResult.at(2)=="var")
        {
            QMessageBox::warning(this,"提示","var文件不能编译");
            return false;
        }
        else if( mSplistResult.at(2)=="方法库" || mSplistResult.at(2)=="command")
        {
            QMessageBox::warning(this,"提示","方法库文件不能编译");
            return false;
        }
        else if(mSplistResult.at(2)=="suite")
        {
            QMessageBox::warning(this,"提示","测试套文件不能编译");
            return false;
        }

        QFileInfo mInfo(mSelectedItem);
        okDir=mInfo.isDir();
        if(okDir)
        {
            if(mSplistResult.count()==4 && mSplistResult.at(2)=="script")//只能编译script文件夹下的文件，不能编译invoke文件
            {
            }
            else if(mSplistResult.count()==3 && mSplistResult.at(2)=="script")//只能编译script文件夹下的文件，不能编译invoke文件
            {
                QMessageBox::warning(this,"提示","不能对script文件夹直接编译");
                return false;
            }
            else
            {
                return false;
            }

            isSelectDir=true;
        }
        else
        {

            if(mSplistResult.count()==5 && mSplistResult.at(2)=="script")
            {
            }
            else
            {
                return false;
            }

            mParentPath=mInfo.absolutePath();
            mParentPath=QDir::toNativeSeparators(mParentPath);
            if(!mdirList.contains(mParentPath))
            {
                mdirList.append(mParentPath);
            }
            isSelectFile=true;
        }
    }//for(int i=0;i<count;i++)

    if(mdirList.count()>=2)//如果选择的文件中包含两个模块中的文件，结构也错误
    {
        QMessageBox::warning(this,"提示","两个模块不能同时编译");
        return false;
    }
    if(isSelectDir && isSelectFile)//既有文件又有文件夹也是结构错误
    {
        QMessageBox::warning(this,"提示","两个模块不能同时编译");
        return false;
    }
    return true;
}


void Record::compileSlot()
{
    if(isCompileing)
    {
        emit sendStopCompileMore();

        isCompileing=false;
        compileButton->setIcon(QIcon(":/compilemore_j.png"));
    }
    else
    {
        if(ui->treeView->rootIndex().row()==-1)
        {
            QMessageBox::information(NULL,"提示","请加载脚本库");
            return;
        }

        QModelIndexList mSelectedTree=ui->treeView->selectionModel()->selectedRows();
        int mCount=mSelectedTree.count();
        if(mCount==0)
        {
            QMessageBox::information(NULL,"提示", "请选择脚本或脚本所在目录");
            return;
        }

        saveAll();

        bool isok=isStruct(mSelectedTree);
        if(!isok)
        {
            //ui->textEdit->setText("选择结构有误");
            //以上文字提示不明确，更换明确的提示hechang
            ui->textEdit->setText("编译出错");
            return;
        }

        convertSelects(mSelectedTree);

        QString mSelectedNode;
        QString mSelectedScript;
        QString mSelectedItem;
        QString mModuleName;

        QString mCmdLine="";
        QStringList mListScipt;

        QFileInfo mInfo;

        mSelectedItem=model->filePath(mSelectedTree.at(0));
        mInfo.setFile(mSelectedItem);

        if(mInfo.isDir())
        {
            mModuleName=mSelectedTree.at(0).data().toString();
            mCmdLine=getJavacCmd1(mModuleName);
        }
        else
        {
            for(int i=0;i<mCount;i++)
            {
                mSelectedNode=model->filePath(mSelectedTree.at(i));
                mSelectedScript=QDir::toNativeSeparators(mSelectedNode);

                mInfo.setFile(mSelectedScript);
                mListScipt.append(mInfo.baseName());
            }//for(int i=0;i<mCount;i++)

            mSelectedNode=model->filePath(mSelectedTree.at(0));
            mSelectedScript=QDir::toNativeSeparators(mSelectedNode);
            mInfo.setFile(mSelectedScript);

            QString mAbsolutePath=mInfo.absolutePath();
            QDir mDir(mAbsolutePath);
            QString mParentName=mDir.dirName();

            mModuleName=mParentName;
            mCmdLine=getJavacCmd2(mListScipt,mModuleName);
        }

        //QString mStr=getJavacCmd2(mListScipt);

        compileButton->setIcon(QIcon(":/compilemore_d.png"));
        isCompileing=true;

        Controller *control=new Controller();
        connect(control, SIGNAL(sendCompileMoreResult(const QString&)), this, SLOT(receiveCompileMoreResult(const QString&)));
        connect(this, SIGNAL(sendStopCompileMore()), control, SLOT(receiveStopCompileMore()));

        control->startCompileMore2(mCmdLine);
        ui->textEdit->setText("编译中...");
    }//if(isCompileing)
}

void Record::convertSelects(QModelIndexList arg_modelIndexList)
{
    ui->textEdit->setText("转换中......");

    QString mSelectedItem;
    QFileInfo mInfo;
    //QModelIndexList mSelectedTree=ui->treeView->selectionModel()->selectedRows();

    int mCount=arg_modelIndexList.count();
    mSelectedItem=model->filePath(arg_modelIndexList.at(0));
    mInfo.setFile(mSelectedItem);
    if(mInfo.isDir())//判断是文件夹还是文件
    {
        QStringList mFilters;
        mFilters<<QString("*.txt");

        QDir mDir(mSelectedItem);
        mDir.setFilter(QDir::Files | QDir::NoSymLinks);
        QStringList mList=mDir.entryList();//获得文件夹下所有txt文件，并转换成java文件
        int num=mList.count();

        QString mTempStr;
        QString mFileName;
        for(int i=0;i<num;i++)
        {
            mFileName=mList.at(i);
            mTempStr=mSelectedItem + QDir::separator() + mFileName;
            mTempStr=QDir::toNativeSeparators(mTempStr);
            convertMoreToUiautomator(mTempStr);
        }

    }
    else
    {
        for(int i=0;i<mCount;i++)
        {
            mSelectedItem=model->filePath(arg_modelIndexList.at(i));
            mSelectedItem=QDir::toNativeSeparators(mSelectedItem);

            mInfo.setFile(mSelectedItem);
            if(mInfo.isFile())
            {
                convertMoreToUiautomator(mSelectedItem);
            }
        }
    }

    ui->textEdit->append("转换完成");
}

void Record::convertMoreToUiautomator(const QString &arg_file)
{
   // QString loop=loopEdit->text().trimmed();
    QString loop="";

    bool ok;
    int loopNumber=loop.toInt(&ok, 10);//10是十进制
    if(ok)
    {
    }
    else
    {
        loopNumber=1;
    }
    setCurrentOpenFile(arg_file);

    setLoopNumber(loopNumber);

    scriptToUiautomator(arg_file);
}

QString Record::getJavacCmd1(const QString &arg_moduleName)
{
    QString mCmdLine;
    QString mJavaFileSaveDir;
    QString mClassSaveDir;
    QString mBaseName;
    QString mSourcefile;
    QString mJavaFile;


    QString ss=gUiautomatorDir + QDir::separator() + arg_moduleName + QDir::separator() + "*.java";
    ss=QDir::toNativeSeparators(ss);


    mJavaFileSaveDir=gUiautomatorDir + QDir::separator() + arg_moduleName;
    mJavaFileSaveDir=QDir::toNativeSeparators(mJavaFileSaveDir);

    mClassSaveDir=mJavaFileSaveDir + QDir::separator() + "bin" + QDir::separator() + "classes";
    mClassSaveDir=QDir::toNativeSeparators(mClassSaveDir);

    createPath(mClassSaveDir);



    mCmdLine="javac  -encoding " + strCodec + " -d " + mClassSaveDir + " -cp " + ".;" + gAndroidJarDir + QDir::separator() + "android.jar;" + gAndroidJarDir + QDir::separator() + "uiautomator.jar " + ss;
    //mCmdLine="javac  -encoding GBK -d " + mClassSaveDir + " -cp " + ".;" + gAndroidJarDir + QDir::separator() + "android.jar;" + gAndroidJarDir + QDir::separator() + "uiautomator.jar " + ss;
    //mCmdLine="javac  -encoding utf-8 -d " + mClassSaveDir + " -cp " + ".;" + gAndroidJarDir + QDir::separator() + "android.jar;" + gAndroidJarDir + QDir::separator() + "uiautomator.jar " + "@"+mSourcefile;


    return mCmdLine;
}



QString Record::getJavacCmd2(QStringList arg_scriptNameList,const QString &arg_moduleName)
{
    QString mCmdLine;
    QString mJavaFileSaveDir;
    QString mClassSaveDir;
    QString mBaseName;
    QString mSourcefile;
    QString mJavaFile;


    mJavaFileSaveDir=gUiautomatorDir + QDir::separator() + arg_moduleName;
    mJavaFileSaveDir=QDir::toNativeSeparators(mJavaFileSaveDir);

    mClassSaveDir=mJavaFileSaveDir + QDir::separator() + "bin" + QDir::separator() + "classes";
    mClassSaveDir=QDir::toNativeSeparators(mClassSaveDir);

    createPath(mClassSaveDir);
    mSourcefile=mJavaFileSaveDir + QDir::separator() + "sourcefile.txt";

    QFile mFile(mSourcefile);
    if(!mFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        mFile.close();
        return "";
    }
    QTextStream out(&mFile);

    for(int i=0;i<arg_scriptNameList.size();i++)
    {
        out<<mJavaFileSaveDir + QDir::separator() + arg_scriptNameList.at(i)+".java";
        out<<endl;
    }
    mFile.close();


    mCmdLine="javac  -encoding " + strCodec + " -d " + mClassSaveDir + " -cp " + ".;" + gAndroidJarDir + QDir::separator() + "android.jar;" + gAndroidJarDir + QDir::separator() + "uiautomator.jar " + "@"+mSourcefile;
    //mCmdLine="javac  -encoding GBK -d " + mClassSaveDir + " -cp " + ".;" + gAndroidJarDir + QDir::separator() + "android.jar;" + gAndroidJarDir + QDir::separator() + "uiautomator.jar " + "@"+mSourcefile;
    //mCmdLine="javac  -encoding utf-8 -d " + mClassSaveDir + " -cp " + ".;" + gAndroidJarDir + QDir::separator() + "android.jar;" + gAndroidJarDir + QDir::separator() + "uiautomator.jar " + "@"+mSourcefile;


    return mCmdLine;
}



void Record::setCompileMoreButtonIconSlot(int)
{
    compileButton->setIcon(QIcon(myMovie->currentPixmap()));
}

void Record::receiveCompileMoreResult(const QString &arg_result)
{
    ui->textEdit->append(arg_result);
    if(arg_result=="停止编译")
    {
    }
    else if(arg_result=="编译完成")
    {
        /*
        myMovie->stop();
        delete myMovie;
        */
        isCompileing=false;
        compileButton->setIcon(QIcon(":/compilemore_j.png"));
    }
    QTextCursor cursor =  ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}
//*******************************************************************************************************//






void Record::packJarSlot()
{
    if(ui->tabWidget->currentIndex()==-1)
    {
        QMessageBox::information(NULL,"提示","请打开脚本");
        return;
    }

    packUiautomatorJar();
}






void Record::openUiautomatorFile()
{
    QStringList mList=parsePath(currentOpenFile);
    if(mList.count()!=5)
    {
        return;
    }

    QString s1=mList.at(3);//script var check
    if(s1=="check" || s1=="var")
    {
        return;
    }
    QString mBaseName=getBaseName(currentOpenFile);
    QString mSubPath=getSubPath(currentOpenFile);
    QString mJavaFile=gUiautomatorDir + QDir::separator() + mSubPath + QDir::separator() + mBaseName + ".java";

    openFile(mJavaFile);
}



void Record::openFile(const QString &arg_openfile)
{
    QFile mFile(arg_openfile);
    if(!mFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    n=0;

    QFileInfo mInfo(arg_openfile);
    QString mFileName=mInfo.fileName();

    QTextStream mStream(&mFile);

    //QTextCodec *tc=QTextCodec::codecForName("GBK");
    //QTextCodec *tc=QTextCodec::codecForName("UTF-8");
    mStream.setCodec(tc);
    QString ss=mStream.readAll();


    QTextEdit *mEdit=new QTextEdit;
    mEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                              "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    mEdit->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");
    mEdit->setFont(myFont);
    mEdit->setWordWrapMode(QTextOption::NoWrap);
    mEdit->installEventFilter(this);



    int x=openedFileList.indexOf(arg_openfile);
    if(x==-1)
    {
        isopen=false;
        //mEdit->setPlainText(mStream.readAll());
        //以下是对invoke或var为空的文件也加入“点击文字编辑脚本”

        if(ss.length()==0)
        {
            mEdit->append("<font color=gray><em>点击文字编辑脚本</em></font>");
        }
        else
        {
            mEdit->setPlainText(ss);
        }
        //以上是对invoke或var为空的文件也加入“点击文字编辑脚本”

        int mTabIndex=ui->tabWidget->addTab(mEdit,mFileName);
        openedFileList.append(arg_openfile);
        ui->tabWidget->setCurrentIndex(mTabIndex);
        connect(mEdit, SIGNAL(textChanged()), this, SLOT(editChangeSlot()));
        textEditHash.insert(arg_openfile,mEdit);
    }
    else
    {
        ui->tabWidget->setCurrentIndex(x);
    }

    //内容有更新时，双击后刷新
    if(arg_openfile.contains("suite"))
    {
        QWidget * mWidget=ui->tabWidget->currentWidget();
        QTextEdit *mEdit1=qobject_cast<QTextEdit *>(mWidget);
        mEdit1->setText(ss);
    }

    mFile.close();

    currentIndex=ui->tabWidget->currentIndex();
    currentOpenFile=arg_openfile;
    currentOpenFile=QDir::toNativeSeparators(currentOpenFile);
}


void Record::doubleClickOpenFile(const QString &arg_openfile)
{
    QFile mFile(arg_openfile);
    if(!mFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        mFile.close();
        return;
    }

    n=0;

    QFileInfo mInfo(arg_openfile);
    QString mFileName=mInfo.fileName();

    QTextStream mStream(&mFile);

    //QTextCodec *tc=QTextCodec::codecForName("GBK");
    //QTextCodec *tc=QTextCodec::codecForName("UTF-8");
    mStream.setCodec(tc);

    QTextEdit *mEdit=new QTextEdit;
    mEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                              "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");
    mEdit->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid grey;height: 9px;}"
                                                "QScrollBar::handle:horizontal { background: grey; min-width: 20px;  border-radius: 3px;border: none;}");
    mEdit->setFont(myFont);
    mEdit->setWordWrapMode(QTextOption::NoWrap);
    mEdit->installEventFilter(this);

    //textEditList.append(mEdit);

    QString ss=mStream.readAll();
    if(ss.length()==0)
    {
        //mStream<<"startApp=sohuvideo"<<endl;
        //mEdit->append("startApp=sohuvideo");
        mEdit->append("<font color=gray><em>点击文字编辑脚本</em></font>");
    }
    else
    {
        mEdit->setPlainText(ss);
    }

    int x=openedFileList.indexOf(arg_openfile);
    if(x==-1)
    {
        isopen=false;

        int mTabIndex=ui->tabWidget->addTab(mEdit,mFileName);
        openedFileList.append(arg_openfile);
        ui->tabWidget->setCurrentIndex(mTabIndex);
        connect(mEdit, SIGNAL(textChanged()), this, SLOT(editChangeSlot()));
        //connect(mEdit, SIGNAL(textChanged()), this, SLOT(editChangeSlot(mEdit)));
        textEditHash.insert(arg_openfile,mEdit);

    }
    else
    {
        ui->tabWidget->setCurrentIndex(x);
    }
    mFile.close();

    if(arg_openfile.contains("suite"))
    {
        QWidget * mWidget=ui->tabWidget->currentWidget();
        QTextEdit *mEdit1=qobject_cast<QTextEdit *>(mWidget);

        mEdit1->setText(ss);
    }

    currentIndex=ui->tabWidget->currentIndex();
    currentOpenFile=arg_openfile;
    currentOpenFile=QDir::toNativeSeparators(currentOpenFile);
}



void Record::receiveCompileResult(const QString & arg_line)
{
    if(arg_line.contains("编译结束"))
    {
        ui->textEdit->append(arg_line);
        compileOneJavaButton->setEnabled(true);
    }
    else
    {
        ui->textEdit->append(arg_line);
    }
}



void Record::packUiautomatorJar()
{
    QString mProjectName;
    QString mModelName;
    QString mJavaFileSaveDir;
    QString mBaseName;
    QString mSubPath;
    QString mClassSaveDir;
    QString mModuleName;
    QFileInfo mInfo;

    currentOpenFile=QDir::toNativeSeparators(currentOpenFile);
    QStringList mList=parsePath(currentOpenFile);
    //"" testcase script play a.txt
    if(mList.count()!=5)
    {
        return;
    }

    QString s1=mList.at(2);//script var check
    if(s1=="invoke" || s1=="var" || s1=="方法库" || s1=="command" || s1=="suite")//打包时如果当前打开的脚本不是script中的TXT，打包无效
    {
        QMessageBox::warning(this,"提示","选择打包文件错误");
        return;
    }


    mInfo.setFile(currentOpenFile);//打包时都是当前打开页面的文件，和选择的文件夹无关 hechang

    QString mAbsolutePath=mInfo.absolutePath();
    QDir mDir(mAbsolutePath);
    QString mParentName=mDir.dirName();

    mModuleName=mParentName;

    mJavaFileSaveDir=gUiautomatorDir + QDir::separator() + mModuleName;
    mJavaFileSaveDir=QDir::toNativeSeparators(mJavaFileSaveDir);

    mClassSaveDir=mJavaFileSaveDir + QDir::separator() + "bin" + QDir::separator() + "classes";
    mClassSaveDir=QDir::toNativeSeparators(mClassSaveDir);

    createPath(mClassSaveDir);


    packJarButton->setEnabled(false);
    ui->textEdit->clear();



    //***********************//
    QString mCmdLine;
    QStringList mCmdList;

    mCmdLine="android create uitest-project -n " + mModuleName + " -t 1 -p " + mJavaFileSaveDir;
    mCmdList<<mCmdLine;

    mCmdLine="ant build -buildfile " + mJavaFileSaveDir + QDir::separator() + "build.xml";
    mCmdList<<mCmdLine;


    Controller *control=new Controller();
    connect(control, SIGNAL(sendPackResult(const QString&)), this, SLOT(receivePackResult(const QString&)));
    control->startPack(mCmdList);
    ui->textEdit->setText("打包中...");

}
void Record::receivePackResult(const QString &arg_result)
{
    ui->textEdit->append(arg_result);
    if(arg_result=="打包结束")
    {
        packJarButton->setEnabled(true);
    }
    QTextCursor cursor =  ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}



void Record::slot_quit()
{
}



QString Record::getJavaFileSaveDir1()
{
    QString mSubPath;
    QString mJavaFileSaveDir;

    mSubPath=getSubPath(currentOpenFile);
    mJavaFileSaveDir=gUiautomatorDir + QDir::separator() + mSubPath;
    mJavaFileSaveDir=QDir::toNativeSeparators(mJavaFileSaveDir);
    return mJavaFileSaveDir;
}

QString Record::getClassSaveDir1()
{
    QString mJavaFileSaveDir;
    QString mClassSaveDir;

    mJavaFileSaveDir=getJavaFileSaveDir1();
    mClassSaveDir=mJavaFileSaveDir + QDir::separator() + "bin" + QDir::separator() + "classes";
    mClassSaveDir=QDir::toNativeSeparators(mClassSaveDir);
    return mClassSaveDir;
}


QString Record::getJavaFileSaveDir2()
{
    QString mJavaFileSaveDir;
    QFileInfo mInfo(currentOpenFile);
    mJavaFileSaveDir=mInfo.absolutePath();
    mJavaFileSaveDir=QDir::toNativeSeparators(mJavaFileSaveDir);
    return mJavaFileSaveDir;
}
QString Record::getProjectName2()
{
    QString mProjectName;
    QString mJavaFileSaveDir;
    QStringList mList;

    mJavaFileSaveDir=getJavaFileSaveDir2();
    mList=mJavaFileSaveDir.split(QDir::separator());
    mProjectName=mList.at(mList.count()-2);
    return mProjectName;
}
QString Record::getModelName2()
{
    QString mModelName;
    QString mJavaFileSaveDir;
    QStringList mList;

    mJavaFileSaveDir=getJavaFileSaveDir2();
    mList=mJavaFileSaveDir.split(QDir::separator());
    mModelName=mList.at(mList.count()-1);
    return mModelName;
}
QString Record::getClassSaveDir2()
{
    QString mJavaFileSaveDir;
    QString mClassSaveDir;
    mJavaFileSaveDir=getJavaFileSaveDir2();
    mClassSaveDir=mJavaFileSaveDir + QDir::separator() + "bin" + QDir::separator() + "classes";
    return mClassSaveDir;
}
//**************************************文件编译处理 方法***********************************************//


void Record::receiveRepositoryResult(const QString &arg_repositoryName, const QString &arg_repositorySavePath)
{
    createTree(gTreeRootDir);
}

QString Record::getWorkspace()
{
    QString strFile=gConfigDir+QDir::separator()+"currentRepository.txt";

    QFile file;
    file.setFileName(strFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        QMessageBox::information(NULL,"提示", "currentRepository 文件异常");


        return "";
    }
    QTextStream inStream(&file);
    QString strPath=inStream.readLine();
    file.close();

    QFileInfo fileInfo(strPath);
    gMyWorkSpace=fileInfo.absolutePath();
    return fileInfo.absolutePath();;

}


void Record::loadRepositorySlot()
{
    QString mWorkSpace=getWorkspace();

    QString directory=QFileDialog::getExistingDirectory(this,("Find Files"),mWorkSpace);
    if(directory.isEmpty())
    {
    }
    else
    {
        //判断该脚本库是不是2.11版本之气前建的，如果是，则脚本库里没有suite文件夹，此处新建
        QString suitePath=directory+QDir::separator()+"testcase"+QDir::separator()+"suite";
        QDir dir(suitePath);
        if(!dir.exists())
        {
            dir.mkdir(suitePath);
        }

        gRepositoryDir=directory;
        Helper::loadRepository(gRepositoryDir);

        createTree(gTreeRootDir);

        QFileInfo mFile(directory);
        gProjectDir=mFile.absolutePath();
        gProjectDir=QDir::toNativeSeparators(gProjectDir);
        gProjectDir=gRepositoryDir;
        //加载脚本库后写入currentRespository.txt，下次打开可记录hechang
        QString strFile=gConfigDir+QDir::separator()+"currentRepository.txt";
        strFile=QDir::toNativeSeparators(strFile);
        Helper::writeStr(strFile,directory);
    }
}

void Record::recieveAddRepositoryPath(QString filepath)//加载脚本库by hechang
{
    gRepositoryDir=filepath;
    Helper::loadRepository(gRepositoryDir);

    createTree(gTreeRootDir);

    QFileInfo mFile(filepath);
    gProjectDir=mFile.absolutePath();
    gProjectDir=QDir::toNativeSeparators(gProjectDir);
    gProjectDir=gRepositoryDir;
}

void Record::on_pushButton_clicked()
{
    QStringList mlist=getDeviceList();
    ui->listWidget->clear();
    ui->listWidget->addItems(mlist);

    if(mlist.count()>0)//原来的逻辑是有一个设备时默认选中第一个，现在改成设备数大于0，默认选中第一个
    {
        ui->listWidget->setCurrentRow(0);
    }
}

QStringList Record::getDeviceList()
{
    QProcess proc;
    QString cmdLine="adb devices";
    proc.setReadChannelMode(QProcess::MergedChannels);
    proc.start(cmdLine);
    proc.waitForStarted();
    while(proc.waitForFinished()==false)
    {
    }

    QString mStr;
    QStringList mSplitResult;
    QStringList mDeviceList;

    while(proc.canReadLine())
    {
        mStr=proc.readLine();
        if(mStr.contains("device") && !mStr.contains("devices"))
        {
            mSplitResult=mStr.split("device");
            if(!mSplitResult.at(0).contains("error"))
            {
                QString tempStr=ExeCmd::GetDeviceModel(mSplitResult.at(0).trimmed())+" + "+mSplitResult.at(0).trimmed();
                mDeviceList.append(tempStr);
            }
        }
    }
    proc.close();
    return mDeviceList;
}

void Record::showInit()
{
    //on_pushButton_clicked();
}

void Record::RecieveDevicesSigal(QStringList devicesList)
{
    if(numForDeviceSignal<3)
        numForDeviceSignal++;
    QString currentDevice = ui->listWidget->currentIndex().data().toString();
    ui->listWidget->clear();
    ui->listWidget->addItems(devicesList);

    int row=devicesList.indexOf(currentDevice);
    ui->listWidget->setCurrentItem(ui->listWidget->item(row));
    if(numForDeviceSignal==1)
        ui->listWidget->setCurrentItem(ui->listWidget->item(0));
}



void Record::on_newMoudleBtn_clicked()
{
    if(gTreeRootDir=="")
    {
        QMessageBox::information(this,"提示","请新建或加载脚本库");
        selectTreePath="";
        return;
    }
    selectTreePath=gTreeRootDir+QDir::separator()+"script";
}
