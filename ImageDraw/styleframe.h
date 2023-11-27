#ifndef STYLEFRAME_H
#define STYLEFRAME_H

#include <QFrame>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class StyleFrame : public QFrame
{
    Q_OBJECT
public:
    StyleFrame(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap pm;
};

#endif // STYLEFRAME_H
