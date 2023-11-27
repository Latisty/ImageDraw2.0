#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QPainter>

class MyPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(float gradientShift READ gradientShift WRITE setGradientShift NOTIFY gradientShiftChanged)
public:
    MyPushButton(QWidget* parent = nullptr);
    float gradientShift() const;

signals:
    void gradientShiftChanged(float shift);

public slots:
    void setGradientShift(float shift);
    void onAnimationFinished();
protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;
private:
    float myGradientShift;
    QPropertyAnimation *myAnimation;
    QLinearGradient baseGradient;

};

#endif // MYPUSHBUTTON_H
