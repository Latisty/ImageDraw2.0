#ifndef DRAWALGO_H
#define DRAWALGO_H


#include <QPixmap>
#include <QImage>
#include <QAtomicInt>
#include <QDebug>
#include <windows.h>
#include <string>
#include <vector>
#include <cmath>
#include <QCursor>


struct shiftCmd
{
    int x1;
    int x2;
    int y1;
    int y2;
    int reps;
    bool stop_short;
    bool even;
    //stopShort offsets
    int dx;
    int dy;
    bool single;
};

class DrawAlgo : public QObject
{
    Q_OBJECT
public:
    DrawAlgo();

public slots:
    int prepare_Line_Image();
    void start_line_drawing();
    void set_clicks_before_sleep(int cbs);
    int get_clicks_before_sleep();
    void set_default_cbs_sleep(int waitTime);
    int get_default_cbs_sleep();
    void set_ui_click_sleep(int waitTime);
    int get_ui_click_sleep();
    void set_ui_positions(int topLeftx, int topLefty);
    void quick_Test();
    void set_Image(QPixmap img);
    bool ready_to_draw();
    void setMainThreadInstance(QThread *mainT);
    void setTopLeftToUse(int index);
    void changeAlphaState(int state);
    void cleanResources();
    void setAtomics(QAtomicInt *stopper, QAtomicInt *pauser);
    int getTotalCommands();
    void pBarEnable(bool state);
    int getSingleCommands();
    int getShiftxCommands();
    int getShiftyCommands();
    int getAlphaCommands();

private:

    //What prepare Image uses
    int topleftToUsex;
    int topleftToUsey;

    //block top left
    int px;
    int py;

    //stamp top left
    int stamp_px;
    int stamp_py;

    int eraserx;
    int erasery;

    int eraserAlphax;
    int eraserAlphay;

    int eraserAlphaValx;
    int eraserAlphaValy;

    int cbx;
    int cby;

    int redx;
    int redy;

    int greenx;
    int greeny;

    int bluex;
    int bluey;

    int cyanx;
    int cyany;

    int yellowx;
    int yellowy;

    int magx;
    int magy;

    int blackx;
    int blacky;

    int whitex;
    int whitey;

    int alphx;
    int alphy;

    int alphvalx;
    int alphvaly;

    int exitClickx;
    int exitClicky;

    int waitLocationx;
    int waitLocationy;

    int clicks_before_sleep;
    int times_clicked;
    //amount of time slept after n clicks
    int default_sleep_time;

    int change_to_eraser_sleep_time;

    //amount of time slept after a colorbox or alpha click
    int ui_click_sleep;

    int clicks_needed;

    int total_commands;
    int single_commands;
    int shiftx_commands;
    int shifty_commands;
    int alpha_commands_count;
    //used to detect if ready for algorithm.
    bool pix_set;
    bool image_set;
    bool image_prepared;

    //bool used for pausing.
    QAtomicInt *shouldRun;
    //bool for ending execution;
    QAtomicInt *shouldStop;

    bool emitPBarSignal;
    //checked during the drawing process to know if it needs to switch to eraser for alpha.
    bool usingAlpha;

    QThread *mainThread;


    QPixmap *myImage;
    QImage myPixelData;
    int imgH;
    int imgW;

    //preoraganized vector commands.
    //y<x<color<alpha<reps
    std::vector<std::vector<std::vector<std::vector<int>>>> commands_by_coord;
    std::vector<std::vector<shiftCmd>> alpha_commands;
    std::vector<std::vector<shiftCmd>> red_commands;
    std::vector<std::vector<shiftCmd>> green_commands;
    std::vector<std::vector<shiftCmd>> blue_commands;
    std::vector<std::vector<shiftCmd>> cyan_commands;
    std::vector<std::vector<shiftCmd>> yellow_commands;
    std::vector<std::vector<shiftCmd>> magenta_commands;
    std::vector<std::vector<shiftCmd>> black_commands;
    std::vector<std::vector<shiftCmd>> white_commands;
    std::vector<std::vector<std::vector<shiftCmd>>*> color_commands;
    std::vector<std::vector<shiftCmd>>* cur_color_vector;

    std::vector<std::vector<int>> operations;
    std::vector<int> value_Corrections;
    std::vector<std::vector<int>> value_mappings;

    void changeColor(int colorx, int colory, INPUT &inputD, INPUT &inputU);
    //This one organizes all commands by pixel location -> reps instead of color -> reps
    void add_line_commands_to_coord(int dy, int dx, int cur_col, std::vector<int> &ops);
    //This one analyzes the by coord vector and generate shift commands.
    void calculate_line_commands();
    bool y_Line_Check(int dx, int dy, int a, int c, int xconsec);
    bool y_Thin_Line_Check(int dx, int dy, int a, int c, int xconsec);
    void x_Line_Check(int dx, int dy, int a, int c);
    int x_Full_Check(int dx, int dy, int c);
    int y_Full_Check(int dx, int dy, int c);
    void selectAll();
    void changeAlpha(std::string& text, INPUT &inputDown, INPUT &inputUp);
    bool check_if_ready();
    void changeEraserAlpha(std::string& text, INPUT &inputDown, INPUT &inputUp);
    void changeToEraser(INPUT &inputDown, INPUT &inputUp);

    void execute_Shift_Commands(std::vector<shiftCmd> &color_commands, INPUT &inputDown, INPUT &inputUp, CURSORINFO &cI, HCURSOR &HIBeam, INPUT &shiftDown, INPUT &shiftUp);

signals:
    void finishedDrawing();
    void earlyHalt();
    void progress();


};

#endif // DRAWALGO_H
