#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QFrame>
#include <QMouseEvent>

class TitleBar : public QFrame
{
    Q_OBJECT
public:
    TitleBar(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QPoint mouseStartPos;
    bool isDragging;
};

#endif // TITLEBAR_H
