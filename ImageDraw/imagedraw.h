#ifndef IMAGEDRAW_H
#define IMAGEDRAW_H

#include <QMainWindow>
#include <QTimer>
#include <QCursor>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QAbstractNativeEventFilter>
#include <windows.h>
#include <QFileDialog>
#include <QFile>
#include <QPropertyAnimation>
#include <QCheckBox>
#include <QString>
#include <QThread>
#include <QAtomicInt>
#include <drawalgo.h>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QColorDialog>
#include <QSettings>
#include <QtMath>
#include <QElapsedTimer>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class ImageDraw; }
QT_END_NAMESPACE

class ImageDraw : public QMainWindow
{
    Q_OBJECT

public:
    ImageDraw(QWidget *parent = nullptr);
    ~ImageDraw();

public slots:
    void refreshZoomWindow();
private slots:
    void pointClickReceived();

    void on_SetCoordButton_clicked();

    void on_Exit_clicked();

    void on_SelectImage_clicked();

    void on_StartButton_clicked();

    void onDrawingComplete();

    void onDrawingTerminated();

    void on_FittoFrameCB_stateChanged(int arg1);

    void on_algodrop_currentIndexChanged(int index);

    void on_artboardrop_currentIndexChanged(int index);

    void on_cropAndCentercb_stateChanged(int arg1);

    void on_CropComboBox_currentIndexChanged(int index);

    void on_scaletoartCB_stateChanged(int arg1);

    void on_BgFillEdit_textChanged(const QString &arg1);

    void on_blockboundsCB_stateChanged();

    void emitPauseSignal();

    void drawNeon(QColor c1, QColor c2);

    void on_color1But_clicked();

    void on_coordConfirm_clicked();

    void on_changeToCoordsBut_clicked();

    void on_prepareButton_clicked();

    void on_stayTopCheck_stateChanged(int arg1);

    void on_endDrawing_clicked();

    void on_color2But_clicked();

    void on_clearButton_clicked();

    void on_SaveState_clicked();

    void loadSaveState();

    void on_clicksEdit_textChanged();

    void updateColors();

    void on_globEdit_textChanged();

    void on_uisleepinterval_textChanged();

    void on_minimize_clicked();

    void on_helpbutton_clicked();

    void on_letsgobutton_clicked();

    void incrementProgressBar();

    void currentCoordsPreview();

    void on_OS_currentIndexChanged(int index);

    void on_disableProg_stateChanged();

    void on_defaultState_clicked();

    void on_rotateline_textChanged();

    void on_rotateslide_valueChanged();

    void rotateImageToDraw();

    bool checkForPr3();

    void on_clearMan_clicked();

signals:
    void startWorker();
private:
    Ui::ImageDraw *ui;
    QTimer *timer;
    QTimer *pauseTimer;
    QElapsedTimer stopwatch;
    QAtomicInt shouldRun;
    QAtomicInt shouldStop;
    bool isDrawing;

    //if exit clicked while running
    bool closed;

    bool pr3Detected;

    bool manualCoordsSet;
    int manLeftPr3;
    int manTopPr3;
    int leftPr3;
    int topPr3;

    //for windows 7
    int yOffset;
    bool coordsFound;

    QPoint colorBox;
    QString imageLoc;
    QPixmap artboardT;



    DrawAlgo *imageDrawObject;
    QThread *imageDrawThread;

    //base image incase we revert changes to imageToDraw. Follows chain down.
    QPixmap rawImage;

    //After Base, rotates.
    QPixmap rotatedImage;

    //For when we want to crop out alpha or white.
    QPixmap croppedImage;

    //for when we draw image onto bg.
    QPixmap imageWithBG;

    //for when we want to scale image.
    QPixmap imageScaled;

    //for when an image is too large.
    QPixmap imageCut;

    //final output (crop, bg, scale, alignment).
    QPixmap imageToDraw;

    int imageToDrawWidth;
    int imageToDrawHeight;

    //for the customizable neon bar
    QColor c1;
    QColor c2;


    int imagesPrepared;
};
#endif // IMAGEDRAW_H
