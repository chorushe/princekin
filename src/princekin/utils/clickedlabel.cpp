#include "clickedlabel.h"

ClickedLabel::ClickedLabel(QWidget *parent) : QLabel(parent)
{

}

void ClickedLabel::mousePressEvent(QMouseEvent *e)
{
    isClick=true;
}

void ClickedLabel::mouseMoveEvent(QMouseEvent *e)
{
    isClick=false;
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if(isClick)
    {
        if(e->button()==Qt::LeftButton)
        {
            Q_UNUSED(e)
            emit clicked();
            isClick=false;
        }
    }
}
