#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
class PushButton:public QPushButton
{
    Q_OBJECT
public:
   explicit PushButton(QWidget *parent = 0);
   ~PushButton();
   void getIconPath(QString icon_path);//得到图片的路径名字，并且设置按钮为图片的大小
   void enterEvent(QEvent *);//进入事件
   void leaveEvent(QEvent *);//离开事件
   void mousePressEvent(QMouseEvent *event);  //鼠标按下事件
   void mouseReleaseEvent(QMouseEvent *event); //鼠标释放事件
   void paintEvent(QPaintEvent *);//按钮绘图事件

private:
   enum ButtonStatus{NORMAL, HOVER, PRESS}; //枚举按钮的几种状态
   ButtonStatus status;
   QString icon_path;//路径名
   bool mouse_press; //按钮左键是否按下
};

#endif // PUSHBUTTON_H
