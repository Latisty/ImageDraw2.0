#include "windowbar.h"

WindowBar::WindowBar(QWidget *parent) : QFrame(parent), isDragging(false)
{

}

void WindowBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        mouseStartPos = event->globalPos()-this->window()->geometry().topLeft();
        isDragging = true;
    }

}

void WindowBar::mouseMoveEvent(QMouseEvent *event)
{
    if(isDragging)
    {
        QWidget *parentWindow = this->window();
        parentWindow->move(event->globalPos() - mouseStartPos);
        //mouseStartPos = event->globalPos();
    }

}

void WindowBar::mouseReleaseEvent(QMouseEvent *)
{
    isDragging = false;
}
