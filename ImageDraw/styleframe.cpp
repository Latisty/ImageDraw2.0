#include "styleframe.h"

StyleFrame::StyleFrame(QWidget *parent) : QFrame(parent)
{
    QPixmap pixmap(612, 612);
    pixmap.fill(QColor(50,50,50));
    QGraphicsScene s;
    QGraphicsPixmapItem item;
    item.setPixmap(pixmap);
    QGraphicsDropShadowEffect *dpsEffect = new QGraphicsDropShadowEffect();
    dpsEffect->setBlurRadius(10);
    dpsEffect->setXOffset(4);
    dpsEffect->setYOffset(4);
    dpsEffect->setColor(QColor(0,0,0,150));
    item.setGraphicsEffect(dpsEffect);
    s.addItem(&item);

    QPixmap b(pixmap.width()+40, pixmap.height()+40);
    b.fill(Qt::transparent);
    QPainter sp(&b);
    s.render(&sp);

    pm = b;



}

void StyleFrame::paintEvent(QPaintEvent * event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.drawPixmap(0,0,pm);


}
