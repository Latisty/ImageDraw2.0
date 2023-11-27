#include "imagedraw.h"
#include "ui_imagedraw.h"
#include <QDebug>

ImageDraw* globalImageDrawInstance = nullptr;
//GLOBAL STUFF FOR MOUSE CLICK DETECTION (TO FIND TOPLEFT COORD OF DRAWBOX)
HHOOK mouseHook;
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        if(wParam == WM_LBUTTONDOWN)
        {
            if(globalImageDrawInstance)
            {
                QMetaObject::invokeMethod(globalImageDrawInstance, "pointClickReceived", Qt::QueuedConnection);
            }
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void SetHook()
{
    if(!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0)))
    {
        //Something went wrong
    }
}

//Should be called before program ends. Im guessing because it will just sit there until restart.
void ReleaseHook()
{
    UnhookWindowsHookEx(mouseHook);
}

ImageDraw::ImageDraw(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImageDraw)
    , imageDrawObject(new DrawAlgo())
    , imageDrawThread(new QThread(this))
{
    ui->setupUi(this);
    globalImageDrawInstance = this;
    ui->ZoomGlass->setFixedSize(396,396);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    //used for zoom gui refreshing.
    timer = new QTimer(this);
    pauseTimer = new QTimer(this);
    //timer emits timout, refresh on this recieves.
    //Apparently & helps find and catch errors? (according to chatgpt)
    connect(timer, &QTimer::timeout, this, &ImageDraw::refreshZoomWindow);
    connect(pauseTimer, &QTimer::timeout, this, &ImageDraw::emitPauseSignal);

    QPixmap windowLogo(":/Resources/Images/pr3.png");
    ui->Logo->setPixmap(windowLogo);
    artboardT = QPixmap(":/Resources/Images/checkered256.png");
    ui->myImage->setPixmap(artboardT);
    //dont start here, start when button is clicked
    //timer->start(100);
    ui->algodrop->addItem("Color Blend (Latisty)");
    //ui->algodrop->addItem("Per Pixel (Omicron)");
    ui->artboardrop->addItem("Block");
    ui->artboardrop->addItem("Stamp");
    ui->CropComboBox->addItem("No Cropping");
    ui->CropComboBox->addItem("By Alpha");
    ui->CropComboBox->addItem("By White");
    ui->CropComboBox->setCurrentIndex(1);
    ui->OS->addItem("Win 10+");
    ui->OS->addItem("Win 7");
    imageToDrawWidth = 40;
    imageToDrawHeight = 40;
    shouldRun = 1;
    shouldStop = 0;
    yOffset = 0;
    imageDrawObject->setAtomics(&shouldStop, &shouldRun);
    isDrawing = false;
    closed = false;
    QColor color1(0,17,212);
    QColor color2(212,118,0);
    c1 = color1;
    c2 = color2;
    drawNeon(c1, c2);
    ui->Console->setText("Hello! Click the ? for Help!\nPlease Select an Image");
    QScreen *screen = QApplication::primaryScreen();
    qreal logicalDPI = (screen->logicalDotsPerInch()/96);
    int dpiPerc = static_cast<int>(logicalDPI*100);
    QString dpiString = QString::number(dpiPerc);
    QString dpiText;
    if(dpiPerc==100)
    {
        dpiText = QString("Screen Dpi: <span style=' color:#00ff00;'>%1%</span>").arg(dpiString);
    }
    else
    {
        dpiText = QString("Screen Dpi: <span style=' color:#ca3b31;'>%1%</span>").arg(dpiString);
        ui->Console->setText("<span style='color:#ca3b31;'>WARNING:</span><br>DPI != 100% Detected.<br> Please change for guaranteed accuracy.");
    }

    ui->dpi->setText(dpiText);
    ui->pr3found->setText("Coords: <span style=' color:#00ff00;'>Auto</span>");
    loadSaveState();
    QIntValidator *valid1 = new QIntValidator(0,10000,this);
    QIntValidator *valid2 = new QIntValidator(0,10000,this);
    QIntValidator *valid3 = new QIntValidator(0,10000,this);
    QIntValidator *valid4 = new QIntValidator(0,359,this);
    ui->clicksEdit->setValidator(valid1);
    ui->globEdit->setValidator(valid2);
    ui->uisleepinterval->setValidator(valid3);
    ui->rotateline->setValidator(valid4);
    setWindowIcon(QIcon(":/Resources/Images/pr3.png"));
    coordsFound = false;
    manualCoordsSet = false;
    imagesPrepared = 0;
}

ImageDraw::~ImageDraw()
{
    imageDrawThread->quit();
    imageDrawThread->wait();
    delete imageDrawThread;
    delete imageDrawObject;
    delete ui;
    ReleaseHook();
    globalImageDrawInstance = nullptr;
}


void ImageDraw::refreshZoomWindow()
{
    QPoint mousePos = QCursor::pos();

    QScreen *screen = QGuiApplication::screenAt(mousePos);

    if(!screen)
    {
        return;
    }

    //Captures the pixels around the cursor and then scales
    QRect captureRect(mousePos.x() - 4, mousePos.y() - 4, 9, 9);
    QPixmap pixmap = screen->grabWindow(0, captureRect.x(), captureRect.y(), captureRect.width(), captureRect.height());
    pixmap = pixmap.scaled(396, 396, Qt::KeepAspectRatio, Qt::FastTransformation);
    QRect captureAreaRect(28,28,340,340);
    QRect ellipseRegion(20,20,320,320);

    //Draws the center pixel square
    QPainter reticlePainter(&pixmap);
    QPen pen(Qt::red,2);
    reticlePainter.setPen(pen);
    reticlePainter.drawRect(177,177,44,44);
    reticlePainter.end();

    //Creates a second pixmap and fills it with a circle.
    QPixmap circularPixmap(357,357);
    circularPixmap.fill(Qt::transparent);

    //The area we want to clip from captured image.
    QPainterPath paintPath;
    paintPath.addEllipse(ellipseRegion);

    //Whereever there is a pixel, it clips its, else, has nothing.
    QPainter zoomFramePainter(&circularPixmap);
    zoomFramePainter.setRenderHint(QPainter::Antialiasing); //Smooth edges
    zoomFramePainter.setClipPath(paintPath);
    zoomFramePainter.drawPixmap(ellipseRegion, pixmap, captureAreaRect);

    //Now disable clipping so we can paste the lens onto the image now.
    zoomFramePainter.setClipping(false);
    QPixmap lensFrame(":/Resources/Images/lens9.png");
    zoomFramePainter.drawPixmap(0,0,lensFrame);
    zoomFramePainter.end();

    ui->ZoomGlass->setPixmap(circularPixmap);


}


void ImageDraw::currentCoordsPreview()
{
    QPoint previewPoint;
    if(manualCoordsSet)
    {
        if(ui->artboardrop->currentIndex()==0)
        {
            previewPoint = QPoint(manLeftPr3, manTopPr3);
        }
        else
        {
            previewPoint = QPoint(manLeftPr3-108, manTopPr3-108);
        }
    }
    else
    {
        if(ui->artboardrop->currentIndex()==0)
        {
            previewPoint = QPoint(leftPr3, topPr3);
        }
        else
        {
            previewPoint = QPoint(leftPr3-108, topPr3-108);
        }
    }



    QScreen *screen = QGuiApplication::screenAt(previewPoint);

    if(!screen)
    {
        return;
    }

    //Captures the pixels around the cursor and then scales
    QRect captureRect(previewPoint.x() - 4, previewPoint.y() - 4, 9, 9);
    QPixmap pixmap = screen->grabWindow(0, captureRect.x(), captureRect.y(), captureRect.width(), captureRect.height());
    pixmap = pixmap.scaled(396, 396, Qt::KeepAspectRatio, Qt::FastTransformation);
    QRect captureAreaRect(28,28,340,340);
    QRect ellipseRegion(20,20,320,320);

    //Draws the center pixel square
    QPainter reticlePainter(&pixmap);
    QPen pen(Qt::red,2);
    reticlePainter.setPen(pen);
    reticlePainter.drawRect(177,177,44,44);
    reticlePainter.end();

    //Creates a second pixmap and fills it with a circle.
    QPixmap circularPixmap(357,357);
    circularPixmap.fill(Qt::transparent);

    //The area we want to clip from captured image.
    QPainterPath paintPath;
    paintPath.addEllipse(ellipseRegion);

    //Whereever there is a pixel, it clips its, else, has nothing.
    QPainter zoomFramePainter(&circularPixmap);
    zoomFramePainter.setRenderHint(QPainter::Antialiasing); //Smooth edges
    zoomFramePainter.setClipPath(paintPath);
    zoomFramePainter.drawPixmap(ellipseRegion, pixmap, captureAreaRect);

    //Now disable clipping so we can paste the lens onto the image now.
    zoomFramePainter.setClipping(false);
    QPixmap lensFrame(":/Resources/Images/lens9.png");
    zoomFramePainter.drawPixmap(0,0,lensFrame);
    zoomFramePainter.end();

    ui->ZoomGlass->setPixmap(circularPixmap);
}


void ImageDraw::pointClickReceived()
{
    QPoint topLeft = QCursor::pos();
    coordsFound = true;
    manLeftPr3 =  topLeft.x();
    manLeftPr3 = topLeft.y();
    imageDrawObject->set_ui_positions(topLeft.x(),topLeft.y());
    QString tlCoords = "(x,y) (" + QString::number(topLeft.x()) + "," + QString::number(topLeft.y()) + ")";
    ui->coordPosLine->setText(tlCoords);
    imageDrawObject->setTopLeftToUse(ui->artboardrop->currentIndex());
    timer->stop();
    ReleaseHook();
    manualCoordsSet = true;
    ui->pr3found->setText("Coords: <span style=' color:#eeee10;'>Manual</span>");
}


void ImageDraw::on_SetCoordButton_clicked()
{
    //this function has direct access to it since it was started in the constructor.
    timer->start(100);
    SetHook();

}


void ImageDraw::on_Exit_clicked()
{
    if(isDrawing)
    {
        shouldStop=1;
        closed = true;
    }
    else
    {
       qApp->exit();
    }

}


void ImageDraw::on_SelectImage_clicked()
{
    QFileDialog imgSelector(this);
    imgSelector.setNameFilter("Image Files (*.png *.jpg *.jpeg)");
    imgSelector.setWindowTitle("Please Select an Image");
    QString filename;
    if(imgSelector.exec())
    {
        filename = imgSelector.selectedFiles().first();
    }

    QPixmap myImage(filename);
    if(!myImage.isNull())
    {
        rawImage = myImage;
        //starts going through the update chain
        //rotate first.
        rotateImageToDraw();
    }
    ui->progressBar->setValue(0);

}


void ImageDraw::on_stayTopCheck_stateChanged(int state)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(50); // Duration in milliseconds
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);

    while (animation->state() == QPropertyAnimation::Running) {
        qApp->processEvents();
    }

    if(state == Qt::Checked)
    {
        setWindowFlag(Qt::WindowStaysOnTopHint, true);
    }
    else
    {
        setWindowFlag(Qt::WindowStaysOnTopHint, false);
    }

    this->hide();
    this->show();

    // Fade back in after the window is shown again
    animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(150);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start(QPropertyAnimation::DeleteWhenStopped);

}


void ImageDraw::on_prepareButton_clicked()
{
    //emit startWorker();
    //qDebug("signal emiited");
    if(rawImage.isNull())
    {
        ui->Console->setText("Please Select an Image");
        return;
    }

    if(ui->alphaCB->isChecked())
    {
        imageDrawObject->changeAlphaState(0);
        int status = imageDrawObject->prepare_Line_Image();
        if(status==0)
        {
            ui->Console->setText("Image Prepared with Alpha");
        }
    }
    else
    {
        imageDrawObject->changeAlphaState(1);
        int status = imageDrawObject->prepare_Line_Image();
        if(status==0)
        {
            ui->Console->setText("Image Prepared without Alpha");
        }
    }
    ui->progressBar->setValue(0);
    imagesPrepared++;

    //Refreshes commands count in console
    if(imagesPrepared>1)
    {
        QString imgPT = QString("Images Prepared: <span style=' color:#ca3b31;'>%1</span>").arg(imagesPrepared);
        ui->imgsprepared->setText(imgPT);
    }
    else
    {
        ui->imgsprepared->setText("Images Prepared: " + QString::number(imagesPrepared));
    }

    ui->lineXC->setText("LineX Cmds: " + QString::number(imageDrawObject->getShiftxCommands()));
    ui->lineYC->setText("LineY Cmds: " + QString::number(imageDrawObject->getShiftyCommands()));
    ui->singleC->setText("Single Cmds: " + QString::number(imageDrawObject->getSingleCommands()));
    ui->totalC->setText("Total Cmds: " + QString::number(imageDrawObject->getTotalCommands()));
    ui->alphaC->setText("Alpha Cmds: " + QString::number(imageDrawObject->getAlphaCommands()));



}


void ImageDraw::onDrawingComplete()
{
    //resources cleaned from within the file
    pauseTimer->stop();
    isDrawing = false;
    shouldStop = 0;
    shouldRun = 1;
    disconnect(this, &ImageDraw::startWorker, imageDrawObject, &DrawAlgo::start_line_drawing);
    disconnect(imageDrawObject, &DrawAlgo::finishedDrawing, this, &ImageDraw::onDrawingComplete);
    disconnect(imageDrawObject, &DrawAlgo::earlyHalt, this, &ImageDraw::onDrawingTerminated);
    disconnect(imageDrawObject, &DrawAlgo::progress, this, &ImageDraw::incrementProgressBar);
    qint64 eTime = stopwatch.elapsed();
    QTime time = QTime::fromMSecsSinceStartOfDay(eTime);
    QString consoleOut = "Finished: " + time.toString("hh:mm:ss.zzz");
    ui->Console->setText(consoleOut);
    ui->StartButton->setText("Start!");

    //enables buttons in options
    const auto disButs = ui->Options->findChildren<QPushButton*>();
    for(QPushButton* but : disButs)
    {
        but->setEnabled(true);
    }
    const auto disChecks = ui->Options->findChildren<QCheckBox*>();
    for(QCheckBox* check : disChecks)
    {
        check->setEnabled(true);
    }
    const auto disCombs = ui->Options->findChildren<QComboBox*>();
    for(QComboBox* comb : disCombs)
    {
        comb->setEnabled(true);
    }
    ui->BgFillEdit->setEnabled(true);
    ui->OS->setEnabled(true);
    ui->disableProg->setEnabled(true);
    //disables times in drawspeed
    const auto disLines= ui->drawOptionsFrame->findChildren<QLineEdit*>();
    for(QLineEdit* lin : disLines)
    {
        lin->setEnabled(true);
    }

    //disables preferences button
    const auto disPrefButs = ui->preferences->findChildren<QPushButton*>();
    for(QPushButton* but : disPrefButs)
    {
        but->setEnabled(true);
    }

    //disables preview checkboxes
    const auto disImgChecks = ui->ImageDetails->findChildren<QCheckBox*>();
    for(QCheckBox* check : disImgChecks)
    {
        check->setEnabled(true);
    }
    //final 2 buttons
    ui->SelectImage->setEnabled(true);
    ui->changeToCoordsBut->setEnabled(true);
    ui->StartButton->setText("Start!");
    imagesPrepared = 0;
    ui->imgsprepared->setText("Images Prepared: " + QString::number(imagesPrepared));
    ui->lineXC->setText("LineX Cmds: " + QString::number(imageDrawObject->getShiftxCommands()));
    ui->lineYC->setText("LineY Cmds: " + QString::number(imageDrawObject->getShiftyCommands()));
    ui->singleC->setText("Single Cmds: " + QString::number(imageDrawObject->getSingleCommands()));
    ui->totalC->setText("Total Cmds: " + QString::number(imageDrawObject->getTotalCommands()));
    ui->alphaC->setText("Alpha Cmds: " + QString::number(imageDrawObject->getAlphaCommands()));
}


void ImageDraw::onDrawingTerminated()
{
    //we need to clean the resources.
    pauseTimer->stop();
    isDrawing = false;
    shouldStop = 0;
    shouldRun = 1;
    disconnect(this, &ImageDraw::startWorker, imageDrawObject, &DrawAlgo::start_line_drawing);
    disconnect(imageDrawObject, &DrawAlgo::finishedDrawing, this, &ImageDraw::onDrawingComplete);
    disconnect(imageDrawObject, &DrawAlgo::earlyHalt, this, &ImageDraw::onDrawingTerminated);
    disconnect(imageDrawObject, &DrawAlgo::progress, this, &ImageDraw::incrementProgressBar);
    qint64 eTime = stopwatch.elapsed();
    QTime time = QTime::fromMSecsSinceStartOfDay(eTime);
    QString consoleOut = "Finished: " + time.toString("hh:mm:ss.zzz");
    ui->Console->setText(consoleOut);
    imageDrawObject->cleanResources();
    if(closed)
    {
        on_Exit_clicked();
    }
    else
    {
        //enables buttons in options
        const auto disButs = ui->Options->findChildren<QPushButton*>();
        for(QPushButton* but : disButs)
        {
            but->setEnabled(true);
        }
        const auto disChecks = ui->Options->findChildren<QCheckBox*>();
        for(QCheckBox* check : disChecks)
        {
            check->setEnabled(true);
        }
        const auto disCombs = ui->Options->findChildren<QComboBox*>();
        for(QComboBox* comb : disCombs)
        {
            comb->setEnabled(true);
        }
        ui->BgFillEdit->setEnabled(true);

        //disables times in drawspeed
        const auto disLines= ui->drawOptionsFrame->findChildren<QLineEdit*>();
        for(QLineEdit* lin : disLines)
        {
            lin->setEnabled(true);
        }

        //disables preferences button
        const auto disPrefButs = ui->preferences->findChildren<QPushButton*>();
        for(QPushButton* but : disPrefButs)
        {
            but->setEnabled(true);
        }

        //disables preview checkboxes
        const auto disImgChecks = ui->ImageDetails->findChildren<QCheckBox*>();
        for(QCheckBox* check : disImgChecks)
        {
            check->setEnabled(true);
        }
        //final 2 buttons
        ui->SelectImage->setEnabled(true);
        ui->changeToCoordsBut->setEnabled(true);
    }
    ui->StartButton->setText("Start!");
    imagesPrepared = 0;
    ui->imgsprepared->setText("Images Prepared: " + QString::number(imagesPrepared));
    ui->lineXC->setText("LineX Cmds: " + QString::number(imageDrawObject->getShiftxCommands()));
    ui->lineYC->setText("LineY Cmds: " + QString::number(imageDrawObject->getShiftyCommands()));
    ui->singleC->setText("Single Cmds: " + QString::number(imageDrawObject->getSingleCommands()));
    ui->totalC->setText("Total Cmds: " + QString::number(imageDrawObject->getTotalCommands()));
    ui->alphaC->setText("Alpha Cmds: " + QString::number(imageDrawObject->getAlphaCommands()));

}


void ImageDraw::on_StartButton_clicked()
{

    if(isDrawing)
    {
        if(shouldRun==0)
        {
            shouldRun=1;
        }
    }
    else
    {
        //not best way to code this.
        bool cont = false;
        if(!imageDrawObject->ready_to_draw())
        {
            ui->Console->setText("Please Prepare Image");
            return;
        }
        //fixes coordinates one last time incase accidently moved pr3 window.
        if(!manualCoordsSet&&checkForPr3())
        {
            cont = true;
        }
        else if(manualCoordsSet)
        {
            cont = true;
        }
        else
        {
            ui->Console->setText("<span style=' color:#ca3b31;'>Error: </span>PR3R Not Detected<br>Please Set Coords Manually");
            return;
        }
        if(cont)
        {
            if(ui->alphaCB->isChecked())
            {
                ui->Console->setText("Starting Alpha");

            }
            else
            {
                ui->Console->setText("Starting non-alpha");
            }

            //disables buttons in options
            const auto disButs = ui->Options->findChildren<QPushButton*>();
            for(QPushButton* but : disButs)
            {
                but->setEnabled(false);
            }
            const auto disChecks = ui->Options->findChildren<QCheckBox*>();
            for(QCheckBox* check : disChecks)
            {
                check->setEnabled(false);
            }
            const auto disCombs = ui->Options->findChildren<QComboBox*>();
            for(QComboBox* comb : disCombs)
            {
                comb->setEnabled(false);
            }
            ui->BgFillEdit->setEnabled(false);
            ui->OS->setEnabled(false);
            ui->disableProg->setEnabled(false);
            //disables times in drawspeed
            const auto disLines= ui->drawOptionsFrame->findChildren<QLineEdit*>();
            for(QLineEdit* lin : disLines)
            {
                lin->setEnabled(false);
            }

            //disables preferences buttons
            const auto disPrefButs = ui->preferences->findChildren<QPushButton*>();
            for(QPushButton* but : disPrefButs)
            {
                but->setEnabled(false);
            }

            //disables preview checkboxes
            const auto disImgChecks = ui->ImageDetails->findChildren<QCheckBox*>();
            for(QCheckBox* check : disImgChecks)
            {
                check->setEnabled(false);
            }
            //final 2 buttons
            ui->SelectImage->setEnabled(false);
            ui->changeToCoordsBut->setEnabled(false);



            ui->progressBar->setValue(0);
            ui->progressBar->setMaximum(imageDrawObject->getTotalCommands());
            pauseTimer->start(100);
            imageDrawObject->setMainThreadInstance(QCoreApplication::instance()->thread());
            imageDrawObject->moveToThread(imageDrawThread);
            connect(this, &ImageDraw::startWorker, imageDrawObject, &DrawAlgo::start_line_drawing);
            connect(imageDrawObject, &DrawAlgo::finishedDrawing, this, &ImageDraw::onDrawingComplete);
            connect(imageDrawObject, &DrawAlgo::earlyHalt, this, &ImageDraw::onDrawingTerminated);
            connect(imageDrawObject, &DrawAlgo::progress, this, &ImageDraw::incrementProgressBar);

            imageDrawThread->start();
            isDrawing=true;
            stopwatch.start();
            emit startWorker();
        }
    }



}


void ImageDraw::on_FittoFrameCB_stateChanged(int state)
{
    if(!imageToDraw.isNull())
    {
        QPixmap myImage;
        QPixmap empty = artboardT;
        QPainter myPainter(&empty);

        if(ui->FittoFrameCB->isChecked())
        {
            myImage = imageCut.scaled(256,256, Qt::KeepAspectRatio, Qt::FastTransformation);
            if(ui->cropAndCentercb->isChecked())
            {
                if(myImage.width()!=256||myImage.height()!=256)
                {
                    QPixmap centered(256, 256);
                    centered.fill(Qt::transparent);
                    int xOffset = (256 - myImage.width())/2;
                    int yOffset = (256 - myImage.height())/2;
                    QPainter centPaint(&centered);
                    centPaint.drawPixmap(xOffset, yOffset, myImage);
                    centPaint.end();
                    myPainter.drawPixmap(0,0,centered);
                }
                else
                {
                    myPainter.drawPixmap(0,0,myImage);
                }

            }
            else
            {
                myPainter.drawPixmap(0,0,myImage);
            }



        }
        else
        {
            if(ui->artboardrop->currentIndex()==0)
            {
                myPainter.drawPixmap(108,108,imageToDraw.width(), imageToDraw.height(), imageToDraw);
            }
            else
            {
                myPainter.drawPixmap(0,0, imageToDraw);
            }
        }




        myPainter.end();
        if(ui->blockboundsCB->isChecked())
        {
            QPainter blockBoundsPainter(&empty);
            blockBoundsPainter.setPen(QPen(Qt::black));
            blockBoundsPainter.setBrush(Qt::NoBrush);
            blockBoundsPainter.drawRect(108,108,40,40);
            blockBoundsPainter.end();

        }
        ui->myImage->setPixmap(empty);
    }

}


void ImageDraw::on_algodrop_currentIndexChanged(int index)
{
    //qDebug("test");
}


void ImageDraw::on_artboardrop_currentIndexChanged(int index)
{
    if(index==0)
    {
        imageToDrawWidth = 40;
        imageToDrawHeight = 40;
    }
    else
    {
        imageToDrawWidth = 256;
        imageToDrawHeight = 256;
    }
    imageDrawObject->setTopLeftToUse(ui->artboardrop->currentIndex());
    //starts image update process;
    on_BgFillEdit_textChanged("");
}


void ImageDraw::on_cropAndCentercb_stateChanged(int state)
{
    if(!rawImage.isNull())
    {
        if(imageScaled.width()>imageToDrawWidth||imageScaled.height()>imageToDrawHeight)
        {
            QPixmap cut(imageToDrawWidth, imageToDrawHeight);
            cut.fill(Qt::transparent);
            QPainter paintToCut(&cut);
            paintToCut.drawPixmap(0,0,imageScaled);
            paintToCut.end();
            imageCut = cut;
            int tmpWidth = imageToDrawWidth;
            int tmpHeight = imageToDrawHeight;
            if(imageScaled.width()<imageToDrawWidth)
            {
                tmpWidth = imageScaled.width();
            }
            if(imageScaled.height()<imageToDrawHeight)
            {
                tmpHeight = imageScaled.height();
            }
            QString dims = "Dimensions: (" + QString::number(tmpWidth) + "," + QString::number(tmpHeight) + ")";
            ui->DimLabel->setText(dims);
        }
        else
        {
            imageCut = imageScaled;
            QString dims = "Dimensions: (" + QString::number(imageScaled.width()) + "," + QString::number(imageScaled.height()) + ")";
            ui->DimLabel->setText(dims);
        }
        if(ui->cropAndCentercb->isChecked())
        {
            if(imageScaled.width()!=imageToDrawWidth||imageScaled.height()!=imageToDrawHeight)
            {
                QPixmap centered(imageToDrawWidth, imageToDrawHeight);
                centered.fill(Qt::transparent);
                int xOffset = (imageToDrawWidth - imageScaled.width())/2;
                int yOffset = (imageToDrawHeight - imageScaled.height())/2;
                QPainter centPaint(&centered);
                centPaint.drawPixmap(xOffset, yOffset, imageScaled);
                centPaint.end();
                imageToDraw = centered;
                if(imageScaled.width()>imageToDrawWidth||imageScaled.height()>imageToDrawHeight)
                {
                    imageCut = centered;
                }
            }
            else
            {
                imageToDraw = imageCut;
            }

        }
        else
        {
            imageToDraw = imageCut;
        }
        on_FittoFrameCB_stateChanged(ui->FittoFrameCB->isChecked());
        imageDrawObject->set_Image(imageToDraw);

    }
}


void ImageDraw::on_CropComboBox_currentIndexChanged(int index)
{
    if(!rawImage.isNull())
    {
        if(ui->CropComboBox->currentIndex()==0)
        {
            croppedImage = rotatedImage;
        }
        else if(ui->CropComboBox->currentIndex()==1)
        {
            QImage rawImageQI = rotatedImage.toImage();
            int min_width = rotatedImage.width();
            int min_height = rotatedImage.height();
            int max_width = 0;
            int max_height = 0;
            for(int y = 0; y < rotatedImage.height();y++)
            {
                for(int x = 0; x < rotatedImage.width(); x++)
                {
                    QColor pixColor(rawImageQI.pixelColor(x,y));
                    if(pixColor.alpha() !=0)
                    {
                        if(x < min_width) min_width = x;
                        if(x > max_width) max_width = x;
                        if(y < min_height) min_height = y;
                        if(y > max_height) max_height = y;

                    }
                }
            }

            QImage croppedImageQI = rawImageQI.copy(min_width, min_height, max_width-min_width+1, max_height-min_height+1);
            if(croppedImageQI.isNull()||croppedImageQI.width()==0||croppedImageQI.height()==0)
            {
                croppedImage = rotatedImage;
                qDebug("no cropping done");
            }
            else
            {
                QPixmap croppedImagePix(croppedImageQI.width(), croppedImageQI.height());
                croppedImagePix.fill(Qt::transparent);
                QPainter cropPainter(&croppedImagePix);
                cropPainter.drawImage(0, 0, croppedImageQI);
                cropPainter.end();

                croppedImage = croppedImagePix;
            }
        }
        else
        {
            QImage rawImageQI = rotatedImage.toImage();
            int min_width = rotatedImage.width();
            int min_height = rotatedImage.height();
            int max_width = 0;
            int max_height = 0;
            for(int y = 0; y < rotatedImage.height();y++)
            {
                for(int x = 0; x < rotatedImage.width(); x++)
                {
                    QColor pixColor(rawImageQI.pixelColor(x,y));
                    if(!(pixColor.red()==255&&pixColor.green()==255 && pixColor.blue() == 255))
                    {
                        if(x < min_width) min_width = x;
                        if(x > max_width) max_width = x;
                        if(y < min_height) min_height = y;
                        if(y > max_height) max_height = y;

                    }
                }
            }
            QImage croppedImageQI = rawImageQI.copy(min_width, min_height, max_width-min_width+1, max_height-min_height+1);
            if(croppedImageQI.isNull()||croppedImageQI.width()==0||croppedImageQI.height()==0)
            {
                croppedImage = rotatedImage;
                qDebug("no cropping done");
            }
            else
            {
                QPixmap croppedImagePix(croppedImageQI.width(), croppedImageQI.height());
                croppedImagePix.fill(Qt::transparent);
                QPainter cropPainter(&croppedImagePix);
                cropPainter.drawImage(0, 0, croppedImageQI);
                cropPainter.end();

                croppedImage = croppedImagePix;
            }

        }
        //to BG
        on_BgFillEdit_textChanged("");
    }

}


void ImageDraw::on_scaletoartCB_stateChanged(int state)
{
    if(!rawImage.isNull())
    {
        if(ui->scaletoartCB->isChecked())
        {
            imageScaled = imageWithBG.scaled(imageToDrawWidth,imageToDrawHeight, Qt::KeepAspectRatio, Qt::FastTransformation);


        }
        else
        {
            imageScaled = imageWithBG;
        }
        on_cropAndCentercb_stateChanged(1);

    }
}


void ImageDraw::on_BgFillEdit_textChanged(const QString &hex)
{
    if(!rawImage.isNull())
    {
        QColor bg;
        QString col = "#" + ui->BgFillEdit->text();
        bg.setNamedColor(col);
        if(bg.isValid())
        {
            QPixmap artboardColored(croppedImage.width(),croppedImage.height());
            artboardColored.fill(bg);
            QPainter placeBehindCrop(&artboardColored);
            placeBehindCrop.drawPixmap(0,0, croppedImage);
            placeBehindCrop.end();

            imageWithBG = artboardColored;
        }
        else
        {
            imageWithBG = croppedImage;
        }
        //to artboard scale
        on_scaletoartCB_stateChanged(ui->scaletoartCB->isChecked());

    }

}


void ImageDraw::on_blockboundsCB_stateChanged()
{
    on_FittoFrameCB_stateChanged(1);
}


void ImageDraw::on_endDrawing_clicked()
{
    if(isDrawing)
    {
        shouldStop=1;
    }
}


void ImageDraw::emitPauseSignal()
{
    if(GetAsyncKeyState(VK_ESCAPE))
    {
        if(shouldRun==1)
        {
            shouldRun=0;
            ui->StartButton->setText("Resume");
        }
    }
}


void ImageDraw::drawNeon(QColor c1, QColor c2)
{
    QPixmap pm(612,40);
    pm.fill(Qt::transparent);
    QPixmap pm2(612,40);
    pm2.fill(Qt::transparent);
    QPainter p2(&pm2);
    p2.setBrush(Qt::NoBrush);
    QLinearGradient grad(0,0,750,0);
    grad.setColorAt(0,c1);
    grad.setColorAt(1,c2);
    p2.setPen(QPen(grad,9));
    p2.drawRect(15,28,582,40);
    p2.end();

    QGraphicsScene s;
    QGraphicsPixmapItem item;
    item.setPixmap(pm2);
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(8);
    item.setGraphicsEffect(blurEffect);
    s.addItem(&item);

    QPixmap b(612,40);
    b.fill(Qt::transparent);
    QPainter sp(&b);
    s.render(&sp);

    QPainter tp(&pm);
    tp.drawPixmap(0,0, b);
    tp.setRenderHint(QPainter::Antialiasing, true);
    tp.setBrush(Qt::NoBrush);
    tp.setPen(QPen(QColor(255,255,255,230), 3));
    tp.drawRect(15,28,582,40);
    tp.end();
    sp.end();

    ui->neonBar->setPixmap(pm);
}


void ImageDraw::on_color1But_clicked()
{
    QColor color = QColorDialog::getColor(c1, this);
    if(color.isValid())
    {
        c1 = color;
        drawNeon(c1, c2);
        ui->color1But->setStyleSheet(QString(
                                         "QPushButton{"
                                         "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(116, 116, 116, 255));"
                                         "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255, 255, 255, 255), stop:0.452514 %1, stop:1 rgba(0, 0, 0, 255));"
                                         "}"
                                         "QPushButton:hover{"
                                         "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, 182, 182, 255), stop:1 rgba(139, 139, 139, 255));"
                                         "}").arg(color.name()));

    }
}


void ImageDraw::on_coordConfirm_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void ImageDraw::on_changeToCoordsBut_clicked()
{
    if(manualCoordsSet||checkForPr3())
    {
        currentCoordsPreview();
    }
    ui->stackedWidget->setCurrentIndex(1);
}


void ImageDraw::on_color2But_clicked()
{
    QColor color = QColorDialog::getColor(c2, this);
    if(color.isValid())
    {
        c2 = color;
        drawNeon(c1, c2);
        ui->color2But->setStyleSheet(QString(
                                         "QPushButton{"
                                         "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(116, 116, 116, 255));"
                                         "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255, 255, 255, 255), stop:0.452514 %1, stop:1 rgba(0, 0, 0, 255));"
                                         "}"
                                         "QPushButton:hover{"
                                         "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, 182, 182, 255), stop:1 rgba(139, 139, 139, 255));"
                                         "}").arg(color.name()));

    }
}


void ImageDraw::on_clearButton_clicked()
{
    imageDrawObject->cleanResources();
    ui->Console->setText("Commands Cleared\nPrepare an Image");
    ui->progressBar->setValue(0);
    imagesPrepared = 0;
    ui->imgsprepared->setText("Images Prepared: " + QString::number(imagesPrepared));
    ui->lineXC->setText("LineX Cmds: " + QString::number(imageDrawObject->getShiftxCommands()));
    ui->lineYC->setText("LineY Cmds: " + QString::number(imageDrawObject->getShiftyCommands()));
    ui->singleC->setText("Single Cmds: " + QString::number(imageDrawObject->getSingleCommands()));
    ui->totalC->setText("Total Cmds: " + QString::number(imageDrawObject->getTotalCommands()));
    ui->alphaC->setText("Alpha Cmds: " + QString::number(imageDrawObject->getAlphaCommands()));
}


void ImageDraw::on_SaveState_clicked()
{
    QSettings settings("usr", "ImageDraw");
    settings.clear();
    settings.sync();
    settings.beginGroup("Preferences");
    settings.setValue("color1", c1);
    settings.setValue("color2", c2);
    settings.setValue("OS", ui->OS->currentIndex());
    settings.setValue("PBar", ui->disableProg->isChecked());
    settings.endGroup();

    settings.beginGroup("Options");
    settings.setValue("alpha", ui->alphaCB->isChecked());
    settings.setValue("algorithm", ui->algodrop->currentIndex());
    settings.setValue("stayontop", ui->stayTopCheck->isChecked());
    settings.setValue("artboard", ui->artboardrop->currentIndex());
    settings.endGroup();

    settings.beginGroup("DrawSpeed");
    if(imageDrawObject->get_clicks_before_sleep()!=20)
    {
        settings.setValue("cbs", imageDrawObject->get_clicks_before_sleep());
    }
    else
    {
        settings.setValue("cbs", "");
    }

    if(imageDrawObject->get_default_cbs_sleep()!=1)
    {
        settings.setValue("defaultsleep", imageDrawObject->get_default_cbs_sleep());
    }
    else
    {
        settings.setValue("defaultsleep", "");
    }
    if(imageDrawObject->get_ui_click_sleep()!=100)
    {
        settings.setValue("uisleep", imageDrawObject->get_ui_click_sleep());
    }
    else
    {
        settings.setValue("uisleep", "");
    }


    settings.endGroup();

    ui->Console->setText("Settings and Preferences Saved");

}


void ImageDraw::loadSaveState()
{
    QSettings settings("usr", "ImageDraw");

    settings.beginGroup("Preferences");
    c1 = settings.value("color1", QColor(0,17,212)).value<QColor>();
    c2 = settings.value("color2", QColor(212,118,0)).value<QColor>();
    bool chk0 = settings.value("PBar", false).toBool();
    ui->disableProg->setChecked(chk0);
    int osInd = settings.value("OS", 0).toInt();
    ui->OS->setCurrentIndex(osInd);
    settings.endGroup();

    updateColors();

    settings.beginGroup("Options");
    bool chk = settings.value("alpha", true).toBool();
    ui->alphaCB->setChecked(chk);
    int ind = settings.value("algorithm", 0).toInt();
    ui->algodrop->setCurrentIndex(ind);
    bool chk2 = settings.value("stayontop", false).toBool();
    ui->stayTopCheck->setChecked(chk2);
    int ind3 = settings.value("artboard", 0).toInt();
    ui->artboardrop->setCurrentIndex(ind3);
    settings.endGroup();

    settings.beginGroup("DrawSpeed");
    QString val1 = settings.value("cbs", "").toString();
    ui->clicksEdit->setText(val1);
    QString v2 = settings.value("defaultsleep", "").toString();
    ui->globEdit->setText(v2);
    QString v3 = settings.value("uisleep", "").toString();
    ui->uisleepinterval->setText(v3);
    settings.endGroup();
}


void ImageDraw::on_defaultState_clicked()
{
    QSettings settings("usr", "ImageDraw");
    settings.clear();
    settings.sync();
    settings.beginGroup("Preferences");
    settings.setValue("color1", QColor(0,17,212));
    settings.setValue("color2", QColor(212,118,0));
    settings.setValue("OS", 0);
    settings.setValue("PBar", 0);
    settings.endGroup();

    settings.beginGroup("Options");
    settings.setValue("alpha", 1);
    settings.setValue("algorithm", 0);
    settings.setValue("stayontop", 0);
    settings.setValue("artboard", 0);
    settings.endGroup();

    settings.beginGroup("DrawSpeed");
    settings.setValue("cbs", "");
    settings.setValue("defaultsleep", "");
    settings.setValue("uisleep", "");
    settings.endGroup();

    ui->Console->setText("Settings and Preferences Save Cleared.");
}


void ImageDraw::on_clicksEdit_textChanged()
{
    if(ui->clicksEdit->text()=="")
    {
        imageDrawObject->set_clicks_before_sleep(20);
    }
    else
    {
        imageDrawObject->set_clicks_before_sleep(ui->clicksEdit->text().toInt());
    }

}


void ImageDraw::updateColors()
{
    drawNeon(c1, c2);
    ui->color1But->setStyleSheet(QString(
                                     "QPushButton{"
                                     "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(116, 116, 116, 255));"
                                     "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255, 255, 255, 255), stop:0.452514 %1, stop:1 rgba(0, 0, 0, 255));"
                                     "}"
                                     "QPushButton:hover{"
                                     "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, 182, 182, 255), stop:1 rgba(139, 139, 139, 255));"
                                     "}").arg(c1.name()));
    ui->color2But->setStyleSheet(QString(
                                     "QPushButton{"
                                     "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(116, 116, 116, 255));"
                                     "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255, 255, 255, 255), stop:0.452514 %1, stop:1 rgba(0, 0, 0, 255));"
                                     "}"
                                     "QPushButton:hover{"
                                     "border: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, 182, 182, 255), stop:1 rgba(139, 139, 139, 255));"
                                     "}").arg(c2.name()));
}

//Default between tick sleep time
void ImageDraw::on_globEdit_textChanged()
{
    if(ui->globEdit->text()=="")
    {
        imageDrawObject->set_default_cbs_sleep(1);
    }
    else
    {
        imageDrawObject->set_default_cbs_sleep(ui->globEdit->text().toInt());
    }

}


void ImageDraw::on_uisleepinterval_textChanged()
{
    if(ui->uisleepinterval->text()=="")
    {
        imageDrawObject->set_ui_click_sleep(100);
    }
    else
    {
        imageDrawObject->set_ui_click_sleep(ui->uisleepinterval->text().toInt());
    }

}


void ImageDraw::on_minimize_clicked()
{
    showMinimized();
}


void ImageDraw::on_helpbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void ImageDraw::on_letsgobutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void ImageDraw::incrementProgressBar()
{
    ui->progressBar->setValue(ui->progressBar->value()+1);
}


void ImageDraw::on_OS_currentIndexChanged(int index)
{
    if(ui->OS->currentIndex()==0)
    {
        yOffset = 0;
    }
    else
    {
        yOffset = -1;
    }
}


void ImageDraw::on_disableProg_stateChanged()
{
    imageDrawObject->pBarEnable(!ui->disableProg->isChecked());
}


void ImageDraw::on_rotateline_textChanged()
{
    if(ui->rotateline->text().toInt()>359)
    {
        ui->rotateline->setText("359");
    }
    ui->rotateslide->setValue(ui->rotateline->text().toInt());
    rotateImageToDraw();
}


void ImageDraw::on_rotateslide_valueChanged()
{
    ui->rotateline->setText(QString::number(ui->rotateslide->value()));
}


void ImageDraw::rotateImageToDraw()
{
    if(!rawImage.isNull())
    {
        int rotate = ui->rotateline->text().toInt();
        if(rotate!=0)
        {

            //if larger than current artboard size, we need an extra large pixmap to rotate the image to account for diagonal length.
            int newSize = qSqrt(rawImage.width() * rawImage.width() + rawImage.height() * rawImage.height());
            QPixmap rotatedPm(newSize, newSize);
            rotatedPm.fill(Qt::transparent);

            QPainter rotPaint(&rotatedPm);
            rotPaint.setRenderHint(QPainter::Antialiasing);
            rotPaint.setRenderHint(QPainter::SmoothPixmapTransform);
            rotPaint.translate(newSize/2, newSize/2);
            rotPaint.rotate(rotate);
            rotPaint.translate(-rawImage.width()/2, -rawImage.height()/2);
            rotPaint.drawPixmap(0,0, rawImage);
            rotPaint.end();

            rotatedImage = rotatedPm;

        }
        else
        {
            rotatedImage = rawImage;
        }
        on_CropComboBox_currentIndexChanged(ui->CropComboBox->currentIndex());

    }

}


bool ImageDraw::checkForPr3()
{
    //This part initiates coords if pr3 is open and detected
    LPCWSTR windowTitle = L"Platform Racing 3 Preloader";
    HWND hwnd = FindWindow(NULL, windowTitle);
    if(hwnd!=NULL)
    {
        RECT pr3Rect;
        if(GetWindowRect(hwnd, &pr3Rect))
        {
            pr3Detected = true;
            coordsFound = true;
            imageDrawObject->set_ui_positions(pr3Rect.left + 338,pr3Rect.top + 251 + yOffset);
            leftPr3 = pr3Rect.left + 338;
            topPr3 = pr3Rect.top + 251 + yOffset;
            QString tlCoords = "(x,y) (" + QString::number(pr3Rect.left + 338) + "," + QString::number(pr3Rect.top + 251 + yOffset) + ")";
            ui->coordPosLine->setText(tlCoords);
            imageDrawObject->setTopLeftToUse(ui->artboardrop->currentIndex());
            currentCoordsPreview();
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}


void ImageDraw::on_clearMan_clicked()
{
    manualCoordsSet = false;
    ui->pr3found->setText("Coords: <span style=' color:#00ff00;'>Auto</span>");
    if(!checkForPr3())
    {
        ui->coordPosLine->setText("");
    }
}

