#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent) : QFrame(parent), isDragging(false)
{

}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        mouseStartPos = event->globalPos()-this->window()->geometry().topLeft();
        isDragging = true;
    }

}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(isDragging)
    {
        QWidget *parentWindow = this->window();
        parentWindow->move(event->globalPos() - mouseStartPos);
        //mouseStartPos = event->globalPos();
    }

}

void TitleBar::mouseReleaseEvent(QMouseEvent *)
{
    isDragging = false;
}
