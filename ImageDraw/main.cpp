#include "imagedraw.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);    
    ImageDraw w;
    w.show();
    return a.exec();
}
