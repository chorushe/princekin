#include "xmleditdialog.h"
#include <QColor>
#include <QStyle>

XmlEditDialog::XmlEditDialog(QWidget *parent, QString filePath) : QWidget(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMinimizeButtonHint);

    setWindowTitle("编辑XML文件");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    this->filePath=filePath;
    xmlEditText=new QTextEdit;

    mainLayout=new QHBoxLayout(this);
    mainLayout->addWidget(xmlEditText);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect deskRect = desktopWidget->availableGeometry();

    int g_nActScreenX = deskRect.width();
    int g_nActScreenY = deskRect.height();
    //设置屏幕大小为桌面3/4
    this->setFixedSize((int)(g_nActScreenX*0.65),(int)(g_nActScreenY*0.65));

    connect(xmlEditText,SIGNAL(textChanged()),this,SLOT(XmlTextChanged()));
    saveFlag=true;

    ReadXMLFile(filePath);

}

XmlEditDialog::~XmlEditDialog()
{
    delete xmlEditText;
}

void XmlEditDialog::ReadXMLFile(QString filePath)
{
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        xmlEditText->append( "打开失败");
        return ;
    }

    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        if(lineStr.contains("<code")||lineStr.contains("<td"))
        {
            int num=lineStr.indexOf("<code")+5;
            QString left=lineStr.left(num);
            xmlEditText->append(left);
            lineStr=lineStr.mid(num);
            xmlEditText->insertPlainText(lineStr);
        }
        else
            xmlEditText->append(lineStr);
    }

    f.close();
    saveFlag=true;
}

void XmlEditDialog::closeEvent(QCloseEvent *event)
{
    if(!saveFlag)
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("关闭窗口"),
                                       QString(tr("是否保存当前页面?")),
                                       QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);

        if (button == QMessageBox::Yes) {
            QFile f(filePath);
            if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this,"提示" ,"保存失败");
                return ;
            }

            QTextStream txtOutput(&f);
            QString s1=xmlEditText->toPlainText();

            txtOutput << s1;

            f.close();
            event->accept();  //接受退出信号，程序退出
        }
        else if (button == QMessageBox::No) {
            event->accept();  //接受退出信号，程序退出
        }
    }
    else
    {
        event->accept();
    }
}

void XmlEditDialog::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == ( Qt::ControlModifier) && e->key() == Qt::Key_S)
    {
        //pressed
        QFile f(filePath);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,"提示" ,"保存失败");
            return ;
        }

        QTextStream txtOutput(&f);
        QString s1=xmlEditText->toPlainText();

        txtOutput << s1;

        f.close();
        QMessageBox::information(this,"提示","保存成功");
        saveFlag=true;
    }

}

void XmlEditDialog::XmlTextChanged()
{
    saveFlag=false;
}
