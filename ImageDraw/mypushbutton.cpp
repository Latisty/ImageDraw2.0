#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent), myGradientShift(0.0), myAnimation(new QPropertyAnimation(this, "gradientShift", this))
{
    myAnimation->setDuration(700);
    myAnimation->setStartValue(0.0);
    myAnimation->setEndValue(0.15);
    baseGradient = QLinearGradient(0,0,width(),0);
    baseGradient.setColorAt(0,QColor(169,169,169));
    baseGradient.setColorAt(.07,QColor(221,221,221));
    baseGradient.setColorAt(.155556,QColor(146,146,146));
    baseGradient.setColorAt(.35,QColor(221,221,221));
    baseGradient.setColorAt(.544444,QColor(169,169,169));
    baseGradient.setColorAt(.666667,QColor(221,221,221));
    baseGradient.setColorAt(.827778,QColor(127,123,127));
    baseGradient.setColorAt(.99999,QColor(99,99,99));

    connect(myAnimation, &QPropertyAnimation::finished, this, &MyPushButton::onAnimationFinished);
}

float MyPushButton::gradientShift() const
{
    return myGradientShift;
}

void MyPushButton::setGradientShift(float shift)
{
    if(myGradientShift == shift)
    {
        return;
    }
    myGradientShift = shift;
    emit gradientShiftChanged(myGradientShift);
    update();
}

void MyPushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    QLinearGradient newGradient(0,0,width(),0);
    float position = myGradientShift;
    int loop_count=0;
    int stops_count = baseGradient.stops().size();
    for(const QGradientStop& stop : baseGradient.stops())
    {

        if(loop_count < stops_count-1)
        {
            double new_value = stop.first + position;
            new_value = new_value - static_cast<int>(new_value);
            newGradient.setColorAt(new_value, stop.second);
        }
        else
        {
            newGradient.setColorAt(stop.first, stop.second);
        }
        loop_count++;

    }
    //painter.setBrush(newGradient);
    //painter.drawRect(rect());
    QPen pen;
    pen.setBrush(newGradient);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(rect());


}

void MyPushButton::onAnimationFinished()
{
    if(myAnimation->direction()==QAbstractAnimation::Forward)
    {
        myAnimation->setDirection(QAbstractAnimation::Backward);
    }
    else
    {
        myAnimation->setDirection(QAbstractAnimation::Forward);
    }
    myAnimation->start();
}

void MyPushButton::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);
    myAnimation->setDirection(QAbstractAnimation::Forward);
    myAnimation->start();



}

void MyPushButton::leaveEvent(QEvent *event)
{
    QPushButton::enterEvent(event);
    myAnimation->pause();
}
