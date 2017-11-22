#include "editscriptfilewidget.h"
#include "ui_editscriptfilewidget.h"

EditScriptFileWidget::EditScriptFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditScriptFileWidget)
{
    ui->setupUi(this);
    setWindowTitle("编辑测试套文件");
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    ui->scriptTreeWidget->setStyleSheet("QTreeWidget{background:#ffffff;border:1px #ffffff;}");
    ui->scriptTreeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border:0px solid grey;width: 10px;}"
                                                             "QScrollBar::handle:vertical {background: grey;border: 3px solid grey;border-radius:5px;min-height: 20px;}");

    QTreeWidgetItem *fileItem1 = new QTreeWidgetItem(QStringList() << "脚本文件列表", 1);   //1表示是文件
    ui->scriptTreeWidget->addTopLevelItem(fileItem1);
    ui->scriptTreeWidget->setHeaderHidden(true);
    ui->scriptTreeWidget->setMouseTracking(true);

    connect(ui->scriptTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
}

void EditScriptFileWidget::InitVar(QString arg_filepath)
{
    filePath=arg_filepath;

    suiteContent = readSuiteFile(filePath);
    addScript();
}

EditScriptFileWidget::~EditScriptFileWidget()
{
    delete ui;
}

void EditScriptFileWidget::addScript()
{
    QString filePath=gRepositoryDir;
    if(filePath=="")
        return;
    filePath=filePath + QDir::separator() + "testcase";
    filePath=QDir::toNativeSeparators(filePath);

    //加入判断testcase文件夹是否存在，若不存在则不是正确的脚本库，所以gmoduleDir2赋值为当前目录，在加载脚本库的时候也显示当前目录
    if(!QDir(filePath).exists())
    {
        QMessageBox::information(0,"提示","所选路径不是脚本库路径！");
        return;
    }
    filePath=filePath + QDir::separator() + "script";
    filePath=QDir::toNativeSeparators(filePath);

    ui->scriptTreeWidget->clear();

    QTreeWidgetItem *fileItem = new QTreeWidgetItem();
    StaticticsHerper::LoadFiles(filePath,ui->scriptTreeWidget,fileItem);
    QTreeWidgetItem *item1=ui->scriptTreeWidget->itemAt(0,0);
    ui->scriptTreeWidget->setCurrentItem(item1);

    QTreeWidgetItemIterator item(ui->scriptTreeWidget);
    while(*item)
    {
        if((*item)->childCount()==0 && (*item)->parent()!=NULL )
        {
            QString str=(*item)->parent()->text(0)+"="+(*item)->text(0);
            if(str.endsWith(".txt"))
                str=str.left(str.length()-4);
            if(suiteContent.contains(str))
                (*item)->setCheckState(0,Qt::Checked);
        }
        ++item;
    }
}

QStringList EditScriptFileWidget::readSuiteFile(QString filePath)
{
    QStringList suiteList;
    QFile file(filePath);
    if(file.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream data(&file);
        QString line;
        while(!data.atEnd())
        {
            line=data.readLine();
            line.remove('\n');
            line=line.trimmed();
            if(line==""||line.startsWith("//"))
            {}
            else
            {
                if(line.contains(",")&&line.contains("="))
                {
                    QString moudel=line.split("=").at(0);
                    QString scripts=line.split("=").at(1);
                    QStringList script=scripts.split(",",QString::SkipEmptyParts);
                    for(int i=0;i<script.length();i++)
                    {
                        if( script[i].endsWith(".txt") )
                        {
                            QString str=moudel+"="+script[i];
                            str=str.left(str.length()-4);
                            suiteList.append(str);
                        }
                        else
                            suiteList.append(moudel+"="+script[i]);
                    }
                }
                else
                {
                    if( line.endsWith(".txt") )
                    {
                        line=line.left(line.length()-4);
                        suiteList.append(line);
                    }
                    else
                        suiteList.append(line);
                }
            }
        }
    }
    file.close();
    return suiteList;
}

void EditScriptFileWidget::treeItemChanged(QTreeWidgetItem *item, int column)
{
    QString itemText = item->text(0);
    if (item->checkState(0) == Qt::Checked)
    {
        int cnt = item->childCount();
        if (cnt >0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Checked);
            }
        }
        else
        {
            StaticticsHerper::updateParentItem(item);
        }
    }
    else if (item->checkState(0) == Qt::Unchecked)
    {
        int cnt = item->childCount();
        if (cnt > 0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
        else
        {
            StaticticsHerper::updateParentItem(item);
        }
    }
}

void EditScriptFileWidget::on_scriptTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if(item->childCount()==0&&item->parent()!=NULL)
    {
        if(item->checkState(0)==Qt::Checked)
        {
            QString str=item->parent()->text(0)+"="+item->text(0);
            if(str.endsWith(".txt"))
                str=str.left(str.length()-4);
            if(!suiteContent.contains(str))
                suiteContent.append(str);
        }
        else
        {
            QString str=item->parent()->text(0)+"="+item->text(0);
            if(str.endsWith(".txt"))
                str=str.left(str.length()-4);
            int index=suiteContent.indexOf(str);
            suiteContent.removeAt( index );
        }
    }
}

void EditScriptFileWidget::on_okBtn_clicked()
{
    QFile f(filePath);
    if(f.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream txtout(&f);
        for(int i=0;i<suiteContent.count();i++)
        {
            txtout<<suiteContent[i]<<endl;
        }
    }
    f.close();

    this->close();
}

void EditScriptFileWidget::on_cancelBtn_clicked()
{
    this->close();
}
