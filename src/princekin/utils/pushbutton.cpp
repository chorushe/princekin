#include "pushbutton.h"
PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    status = NORMAL;
    mouse_press = false;
}
PushButton::~PushButton()
{
}
void PushButton::getIconPath(QString Icon_path)
{
    Icon_path="E:\project\QT\princekin6\qrc\images";
    this->icon_path = Icon_path;
    setFixedSize(QPixmap(Icon_path).size());
}

void PushButton::enterEvent(QEvent *)
{
    status = HOVER;
    update();
}
void PushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
        status = PRESS;
        update();
    }
}
void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(mouse_press&&this->rect().contains(event->pos()))
    {
        mouse_press = false;
        status = HOVER;
        update();
        emit clicked();
    }
}
void PushButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}
void PushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;
    switch(status)
    {
    case NORMAL:
        {
            pixmap.load(icon_path);
            break;
        }
    case HOVER:
        {
            pixmap.load(icon_path + QString("_hover"));
            break;
        }
    case PRESS:
        {
            pixmap.load(icon_path + QString("_press"));
            break;
        }
    default:
        pixmap.load(icon_path);
    }
    painter.drawPixmap(rect(), pixmap);
    painter.drawText(this->rect(), Qt::AlignCenter, this->text());
}
