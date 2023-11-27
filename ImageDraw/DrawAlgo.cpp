#include <drawalgo.h>

void DrawAlgo::changeColor(int colorx, int colory, INPUT &inputD, INPUT &inputU)
{
    SetCursorPos(cbx,cby);
    SendInput(1, &inputD, sizeof(INPUT));
    SendInput(1, &inputU, sizeof(INPUT));
    Sleep(ui_click_sleep);
    SetCursorPos(colorx,colory);
    Sleep(ui_click_sleep);
    SendInput(1, &inputD, sizeof(INPUT));
    SendInput(1, &inputU, sizeof(INPUT));
    Sleep(ui_click_sleep);
    SetCursorPos(exitClickx,exitClicky);
    SendInput(1, &inputD, sizeof(INPUT));
    SendInput(1, &inputU, sizeof(INPUT));
    Sleep(ui_click_sleep);
}

void DrawAlgo::changeToEraser(INPUT &inputD, INPUT &inputU)
{
    SetCursorPos(eraserx,erasery);
    SendInput(1, &inputD, sizeof(INPUT));
    SendInput(1, &inputU, sizeof(INPUT));
    Sleep(change_to_eraser_sleep_time);
}

void DrawAlgo::execute_Shift_Commands(std::vector<shiftCmd> &color_commands, INPUT &inputDown, INPUT &inputUp, CURSORINFO &cI, HCURSOR &HIBeam, INPUT &shiftDown, INPUT &shiftUp)
{
    for(auto &coord : color_commands)
    {
        if(emitPBarSignal)
        {
            emit progress();
        }
        if(coord.single)
        {
            int reps = coord.reps;
            SetCursorPos(coord.x1+topleftToUsex,coord.y1+topleftToUsey);
            for(int rep = 0; rep < reps; rep++)
            {
                if(*shouldRun==1)
                {
                    SendInput(1, &inputDown, sizeof(INPUT));
                    times_clicked++;
                    if(times_clicked>=clicks_before_sleep)
                    {
                        SendInput(1, &inputUp, sizeof(INPUT));
                        SetCursorPos(waitLocationx, waitLocationy);
                        times_clicked = 0;
                        cI.cbSize = sizeof(CURSORINFO);
                        GetCursorInfo(&cI);
                        while (cI.hCursor != HIBeam)
                        {
                            Sleep(default_sleep_time);
                            cI.cbSize = sizeof(CURSORINFO);
                            GetCursorInfo(&cI);
                        }
                        SetCursorPos(exitClickx, exitClicky);
                        SendInput(1, &inputDown, sizeof(INPUT));
                        SendInput(1, &inputUp, sizeof(INPUT));
                        SetCursorPos(coord.x1+topleftToUsex, coord.y1+topleftToUsey);
                    }
                }
                else
                {
                    while(*shouldRun==0)
                    {
                        Sleep(100);
                        if(*shouldStop==1)
                        {
                            return;
                        }
                    }
                    rep--;
                }

            }
            SendInput(1, &inputUp, sizeof(INPUT));
        }
        else
        {
            int reps = coord.reps;
            //used to account for pausing.
            bool completed = false;
            while(!completed)
            {
                if(*shouldRun==1)
                {
                    if(coord.even)
                    {
                        SetCursorPos(coord.x1+topleftToUsex, coord.y1+topleftToUsey);
                        SendInput(1, &inputDown, sizeof(INPUT));
                        SendInput(1, &inputUp, sizeof(INPUT));
                        SetCursorPos(coord.x2+topleftToUsex, coord.y2+topleftToUsey);
                        SendInput(1, &shiftDown, sizeof(INPUT));
                        SendInput(1, &inputDown, sizeof(INPUT));
                        SendInput(1, &inputUp, sizeof(INPUT));
                        reps--;
                        while(reps>2)
                        {
                            SetCursorPos(coord.x1+topleftToUsex, coord.y1+topleftToUsey);
                            SendInput(1, &inputDown, sizeof(INPUT));
                            SendInput(1, &inputUp, sizeof(INPUT));
                            SetCursorPos(coord.x2+topleftToUsex, coord.y2+topleftToUsey);
                            SendInput(1, &inputDown, sizeof(INPUT));
                            SendInput(1, &inputUp, sizeof(INPUT));
                            reps=reps-2;
                        }
                        SetCursorPos(coord.x1+coord.dx+topleftToUsex, coord.y1+coord.dy+topleftToUsey);
                        SendInput(1, &inputDown, sizeof(INPUT));
                        SendInput(1, &inputUp, sizeof(INPUT));
                        SendInput(1, &shiftUp, sizeof(INPUT));
                    }
                    else
                    {
                        SetCursorPos(coord.x1+topleftToUsex, coord.y1+topleftToUsey);
                        SendInput(1, &inputDown, sizeof(INPUT));
                        SendInput(1, &inputUp, sizeof(INPUT));
                        SetCursorPos(coord.x2+topleftToUsex, coord.y2+topleftToUsey);
                        SendInput(1, &shiftDown, sizeof(INPUT));
                        SendInput(1, &inputDown, sizeof(INPUT));
                        SendInput(1, &inputUp, sizeof(INPUT));
                        reps--;
                        while(reps>2)
                        {
                            SetCursorPos(coord.x1+topleftToUsex, coord.y1+topleftToUsey);
                            SendInput(1, &inputDown, sizeof(INPUT));
                            SendInput(1, &inputUp, sizeof(INPUT));
                            SetCursorPos(coord.x2+topleftToUsex, coord.y2+topleftToUsey);
                            SendInput(1, &inputDown, sizeof(INPUT));
                            SendInput(1, &inputUp, sizeof(INPUT));
                            reps=reps-2;
                        }
                        if(reps==2)
                        {
                            //to the offset now if we want =3 or og if first is 4 and rest is 3. (using 3 as understanding)
                            SetCursorPos(coord.x1+coord.dx+topleftToUsex, coord.y1+coord.dy+topleftToUsey);
                            SendInput(1, &inputDown, sizeof(INPUT));
                            SendInput(1, &inputUp, sizeof(INPUT));

                            //back to far end
                            SetCursorPos(coord.x2+topleftToUsex, coord.y2+topleftToUsey);
                            SendInput(1, &inputDown, sizeof(INPUT));
                            SendInput(1, &inputUp, sizeof(INPUT));
                            SendInput(1, &shiftUp, sizeof(INPUT));
                            if(coord.stop_short)
                            {

                                SetCursorPos(coord.x1+topleftToUsex, coord.y1+topleftToUsey);
                                SendInput(1, &inputDown, sizeof(INPUT));
                                SendInput(1, &inputUp, sizeof(INPUT));
                            }
                        }
                        else
                        {
                            //if single make sure to release shift
                            SendInput(1, &shiftUp, sizeof(INPUT));
                        }


                    }
                    completed = true;
                    times_clicked+=reps;
                    if(times_clicked>=clicks_before_sleep)
                    {
                        times_clicked=0;
                        SetCursorPos(waitLocationx, waitLocationy);
                        times_clicked = 0;
                        cI.cbSize = sizeof(CURSORINFO);
                        GetCursorInfo(&cI);
                        while (cI.hCursor != HIBeam)
                        {
                            Sleep(default_sleep_time);
                            cI.cbSize = sizeof(CURSORINFO);
                            GetCursorInfo(&cI);
                        }
                        SetCursorPos(exitClickx, exitClicky);
                        SendInput(1, &inputDown, sizeof(INPUT));
                        SendInput(1, &inputUp, sizeof(INPUT));
                    }
                }
                else
                {
                    while(*shouldRun==0)
                    {
                        Sleep(100);
                        if(*shouldStop==1)
                        {
                            return;
                        }
                    }
                }
            }

        }


    }
}

void DrawAlgo::add_line_commands_to_coord(int dy, int dx, int cur_col, std::vector<int> &ops)
{
    int prev_cmd = -1;
    int consecutive_ops_count = 0;
    for(auto op : ops)
    {
        if(op!=prev_cmd)
        {
            if(prev_cmd==-1)
            {
                prev_cmd = op;
            }
            else
            {
                commands_by_coord[dy][dx][cur_col][prev_cmd] = consecutive_ops_count + 1;
                prev_cmd = op;
                consecutive_ops_count = 0;
            }
        }
        else
        {
            consecutive_ops_count++;
        }
    }
    //because the last command doesnt trigger an appendage.
    //if had commands (255 doesnt)
    if(prev_cmd>=0)
    {
        commands_by_coord[dy][dx][cur_col][prev_cmd] = consecutive_ops_count + 1;
    }
}

void DrawAlgo::selectAll()
{
    INPUT inputs[2] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'A';

    SendInput(2, inputs, sizeof(INPUT));

    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, inputs, sizeof(INPUT));
    Sleep(ui_click_sleep);
}

void DrawAlgo::changeAlpha(std::string& text, INPUT &inputDown, INPUT &inputUp)
{
    SetCursorPos(alphx,alphy);
    SendInput(1, &inputDown, sizeof(INPUT));
    SendInput(1, &inputUp, sizeof(INPUT));
    Sleep(ui_click_sleep);
    SetCursorPos(alphvalx,alphvaly);
    SendInput(1, &inputDown, sizeof(INPUT));
    SendInput(1, &inputUp, sizeof(INPUT));
    selectAll();
    for(char c : text)
    {
        INPUT textInput = {0};
        textInput.type = INPUT_KEYBOARD;
        textInput.ki.wVk = VkKeyScan(c);
        SendInput(1, &textInput, sizeof(INPUT));
        textInput.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &textInput, sizeof(INPUT));
    }
    Sleep(ui_click_sleep);
    SetCursorPos(exitClickx,exitClicky);
    SendInput(1, &inputDown, sizeof(INPUT));
    SendInput(1, &inputUp, sizeof(INPUT));
    Sleep(ui_click_sleep);
}

void DrawAlgo::changeEraserAlpha(std::string& text, INPUT &inputDown, INPUT &inputUp)
{
    SetCursorPos(eraserAlphax,eraserAlphay);
    SendInput(1, &inputDown, sizeof(INPUT));
    SendInput(1, &inputUp, sizeof(INPUT));
    Sleep(ui_click_sleep);
    SetCursorPos(eraserAlphaValx,eraserAlphaValy);
    SendInput(1, &inputDown, sizeof(INPUT));
    SendInput(1, &inputUp, sizeof(INPUT));
    selectAll();
    for(char c : text)
    {
        INPUT textInput = {0};
        textInput.type = INPUT_KEYBOARD;
        textInput.ki.wVk = VkKeyScan(c);
        SendInput(1, &textInput, sizeof(INPUT));
        textInput.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &textInput, sizeof(INPUT));
    }
    Sleep(ui_click_sleep);
    SetCursorPos(exitClickx,exitClicky);
    SendInput(1, &inputDown, sizeof(INPUT));
    SendInput(1, &inputUp, sizeof(INPUT));
    Sleep(ui_click_sleep);
}

bool DrawAlgo::check_if_ready()
{
    if(image_set && pix_set)
    {
        return true;
    }
    else
    {
        return false;
    }
}


DrawAlgo::DrawAlgo()
{
    for(int i = 0; i < 100;i++)
    {
        alpha_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        red_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        green_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        blue_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        cyan_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        yellow_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        magenta_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        black_commands.push_back({});
    }
    for(int i = 0; i < 6;i++)
    {
        white_commands.push_back({});
    }
    for(int i = 0; i < 8; i++)
    {
        color_commands.push_back({});
    }
    commands_by_coord = std::vector<std::vector<std::vector<std::vector<int>>>>(256, std::vector<std::vector<std::vector<int>>>(256, std::vector<std::vector<int>>(8, std::vector<int>(6,0))));
    color_commands[0] = &white_commands;
    color_commands[1] = &cyan_commands;
    color_commands[2] = &yellow_commands;
    color_commands[3] = &magenta_commands;
    color_commands[4] = &red_commands;
    color_commands[5] = &green_commands;
    color_commands[6] = &blue_commands;
    color_commands[7] = &black_commands;
    cur_color_vector = &white_commands;
    pix_set = false;
    image_set = false;
    image_prepared = false;
    usingAlpha = true;
    times_clicked = 0;
    default_sleep_time = 1;
    clicks_before_sleep = 20;
    ui_click_sleep = 100;
    change_to_eraser_sleep_time = 500;
    clicks_needed=0;
    total_commands=0;
    single_commands = 0;
    shiftx_commands = 0;
    shifty_commands = 0;
    alpha_commands_count = 0;
    emitPBarSignal = true;
    //The operations used to get from 255 to the value
    operations =
        {
        {3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3},
        {2,3,3,3,3,3,3},
        {3,3,3,3,3,3},
        {2,2,3,3,3,3,3},
        {2,3,3,3,3,3},
        {1,1,3,3,3,3,3},
        {3,3,3,3,3},
        {2,2,3,3,3,3},
        {1,1,2,3,3,3,3},
        {1,2,3,3,3,3},
        {2,3,3,3,3},
        {1,1,3,3,3,3},
        {2,2,2,3,3,3},
        {1,3,3,3,3},
        {3,3,3,3},
        {1,2,2,3,3,3},
        {2,2,3,3,3},
        {1,2,2,2,2,3,3},
        {1,1,2,3,3,3},
        {0,0,1,2,3,3,3},
        {1,2,3,3,3},
        {0,1,1,1,3,3,3},
        {2,3,3,3},
        {1,2,2,2,3,3},
        {1,1,3,3,3},
        {2,2,2,3,3},
        {0,0,1,3,3,3},
        {1,3,3,3},
        {2,2,2,2,2,3},
        {0,0,0,3,3,3},
        {3,3,3},
        {1,2,2,3,3},
        {2,2,2,2,2,2,2},
        {0,0,2,2,3,3},
        {2,2,3,3},
        {1,2,2,2,2,3},
        {0,0,1,1,2,3,3},
        {1,1,2,3,3},
        {2,2,2,2,3},
        {0,0,1,1,1,1,3,3},
        {0,0,1,2,3,3},
        {0,1,2,3,3},
        {1,2,3,3},
        {2,2,2,2,2,2},
        {0,1,1,1,3,3},
        {0,0,2,3,3},
        {2,3,3},
        {1,2,2,2,3},
        {0,1,1,3,3,4},
        {0,1,1,3,3},
        {1,1,3,3},
        {2,2,2,3,4},
        {2,2,2,3},
        {0,0,1,3,3,4},
        {0,0,1,3,3},
        {0,1,3,3},
        {1,3,3},
        {2,2,2,2,2,4},
        {2,2,2,2,2},
        {0,3,3,4,4},
        {0,3,3,4},
        {0,3,3},
        {3,3},
        {1,2,2,3},
        {0,0,2,2,3,4,4,4,4},
        {0,0,2,2,3,4,4,4},
        {0,0,2,2,3,4,4},
        {0,0,2,2,3,4},
        {0,0,2,2,3},
        {0,2,2,3},
        {2,2,3},
        {1,2,2,2,2,4},
        {1,2,2,2,2},
        {1,1,1,1,1,3},
        {0,0,1,1,2,3},
        {0,1,1,2,3},
        {1,1,2,3},
        {2,2,2,2,4},
        {2,2,2,2},
        {0,0,0,1,1,1,1,3},
        {0,0,1,1,1,1,3},
        {0,0,1,2,3,4},
        {0,0,1,2,3},
        {0,1,2,3},
        {1,2,3,4},
        {1,2,3},
        {0,0,0,1,1,1,3,4,4},
        {0,0,0,1,1,1,3,4},
        {0,0,0,1,1,1,3},
        {0,0,1,1,1,3},
        {0,1,1,1,3},
        {1,1,1,3},
        {0,0,2,3},
        {0,2,3},
        {2,3},
        {1,2,2,2,4},
        {1,2,2,2},
        {0,0,0,1,1,3,4},
        {0,0,0,1,1,3},
        {0,0,1,1,3},
        {0,1,1,3},
        {1,1,3,4},
        {1,1,3},
        {0,0,2,2,2},
        {2,2,2,4},
        {2,2,2},
        {0,0,0,1,3,4,4,4},
        {0,0,0,1,3,4,4},
        {0,0,0,1,3,4},
        {0,0,0,1,3},
        {0,0,1,3},
        {0,1,3,4},
        {0,1,3},
        {1,3},
        {1,1,2,2},
        {0,0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,0,3},
        {0,0,0,0,0,0,3},
        {0,0,0,0,0,3},
        {0,0,0,0,3},
        {0,0,0,3},
        {0,0,3},
        {0,3},
        {3,4},
        {3},
        {1,2,2,4},
        {1,2,2},
        {0,0,0,1,1,1,1,1,1},
        {0,0,1,1,1,1,1,1,4},
        {0,0,1,1,1,1,1,1},
        {0,1,1,1,1,1,1},
        {1,1,1,1,1,1},
        {0,0,1,1,1,2,4},
        {0,0,1,1,1,2},
        {0,1,1,1,2},
        {0,0,0,2,2},
        {0,0,2,2},
        {0,2,2,4},
        {0,2,2},
        {2,2},
        {0,0,0,0,1,1,1,1,1,4},
        {0,0,0,0,1,1,1,1,1},
        {0,0,0,1,1,1,1,1},
        {0,0,1,1,1,1,1,4},
        {0,0,1,1,1,1,1},
        {0,1,1,1,1,1},
        {1,1,1,1,1},
        {0,1,1,2,4},
        {0,0,1,1,2},
        {0,1,1,2},
        {0,0,0,1,1,1,1,4,4,4,4},
        {1,1,2},
        {0,0,0,1,1,1,1,4,4,4},
        {1,1,1,1,4,4,4,4,4},
        {0,0,0,1,1,1,1,4,4},
        {1,1,1,1,4,4,4,4},
        {0,0,0,1,1,1,1,4},
        {1,1,1,1,4,4,4},
        {0,0,0,1,1,1,1},
        {1,1,1,1,4,4},
        {0,0,1,1,1,1},
        {1,1,1,1,4},
        {0,0,1,2,4},
        {1,1,1,1},
        {0,0,1,2},
        {1,2,4,4},
        {0,1,2},
        {1,2,4},
        {0,0,0,1,1,1,4,4,4,4},
        {1,2},
        {0,0,0,1,1,1,4,4,4},
        {0,0,0,0,0,1,1,1,4},
        {0,0,0,1,1,1,4,4},
        {0,0,0,0,0,1,1,1},
        {0,0,0,1,1,1,4},
        {0,0,0,0,1,1,1},
        {0,0,0,1,1,1},
        {0,0,0,2,4,4},
        {0,0,1,1,1},
        {0,0,0,2,4},
        {0,1,1,1},
        {0,0,0,2},
        {1,1,1},
        {0,0,2},
        {0,0,0,1,1,4,4,4,4,4,4},
        {0,2},
        {0,0,0,1,1,4,4,4,4,4},
        {2},
        {0,0,0,1,1,4,4,4,4},
        {0,0,0,0,0,0,1,1,4},
        {0,0,0,1,1,4,4,4},
        {0,0,0,0,0,0,1,1},
        {0,0,0,1,1,4,4},
        {0,0,0,0,0,1,1},
        {0,0,0,1,1,4},
        {0,0,0,0,1,1},
        {0,0,0,1,1},
        {1,1,4,4,4},
        {0,0,1,1},
        {1,1,4,4},
        {0,1,1},
        {1,1,4},
        {0,0,0,1,4,4,4,4,4,4,4,4},
        {1,1},
        {0,0,0,1,4,4,4,4,4,4,4},
        {0,0,0,0,0,0,0,0,1,4,4},
        {0,0,0,1,4,4,4,4,4,4},
        {0,0,0,0,0,0,0,0,1,4},
        {0,0,0,1,4,4,4,4,4},
        {0,0,0,0,0,0,0,0,1},
        {0,0,0,1,4,4,4,4},
        {0,0,0,0,0,0,0,1},
        {0,0,0,1,4,4,4},
        {0,0,0,0,0,0,1},
        {0,0,0,1,4,4},
        {0,0,0,0,0,1},
        {0,0,0,1,4},
        {0,0,0,0,1},
        {0,0,0,1},
        {0,1,4,4},
        {0,0,1},
        {0,1,4},
        {1,4,4},
        {0,1},
        {1,4},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {1},
        {0,0,0,0,0,0,0,0,0,0,0},
        {},
        {0,0,0,0,0,0,0,0,0,0},
        {},
        {0,0,0,0,0,0,0,0,0},
        {},
        {0,0,0,0,0,0,0,0},
        {},
        {0,0,0,0,0,0,0},
        {},
        {0,0,0,0,0,0},
        {},
        {0,0,0,0,0},
        {},
        {0,0,0,0},
        {},
        {0,0,0},
        {},
        {0,0},
        {},
        {},
        {0},
        {},
        {},
        {},
        };
    //For making sure a value can reach 255
    value_Corrections =
        {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        19,
        20,
        21,
        22,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        32,
        33,
        34,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        47,
        48,
        49,
        50,
        51,
        52,
        53,
        54,
        55,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        64,
        65,
        66,
        67,
        68,
        69,
        70,
        71,
        72,
        73,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        85,
        86,
        87,
        88,
        89,
        90,
        91,
        92,
        93,
        94,
        95,
        96,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        123,
        124,
        125,
        126,
        127,
        128,
        129,
        130,
        131,
        132,
        133,
        134,
        135,
        136,
        137,
        138,
        139,
        140,
        141,
        142,
        143,
        144,
        145,
        146,
        147,
        148,
        149,
        150,
        151,
        152,
        153,
        154,
        155,
        156,
        157,
        158,
        159,
        160,
        161,
        162,
        163,
        164,
        165,
        166,
        167,
        168,
        169,
        170,
        171,
        172,
        173,
        174,
        175,
        176,
        177,
        178,
        179,
        180,
        181,
        182,
        183,
        184,
        185,
        186,
        187,
        188,
        189,
        190,
        191,
        192,
        193,
        194,
        195,
        196,
        197,
        198,
        199,
        200,
        201,
        202,
        203,
        204,
        205,
        206,
        207,
        208,
        209,
        210,
        211,
        212,
        213,
        214,
        215,
        216,
        217,
        218,
        219,
        220,
        221,
        222,
        223,
        224,
        225,
        226,
        227,
        228,
        229,
        230,
        231,
        233,
        233,
        235,
        235,
        237,
        237,
        239,
        239,
        241,
        241,
        243,
        243,
        245,
        245,
        247,
        247,
        249,
        249,
        249,
        252,
        252,
        252,
        255,
        255,};
    //For working up to 255
    value_mappings =
        {
        {1,1,1,1},
        {2,2,2,3},
        {3,3,4,5},
        {4,4,5,7},
        {5,5,6,9},
        {6,7,8,11},
        {7,8,9,13},
        {8,9,10,15},
        {9,10,12,17},
        {10,11,13,19},
        {11,12,14,21},
        {12,13,16,23},
        {13,14,17,25},
        {14,15,18,27},
        {15,17,20,29},
        {16,18,21,31},
        {17,19,22,33},
        {18,20,24,35},
        {19,21,25,37},
        {20,22,26,39},
        {21,23,28,41},
        {22,24,29,43},
        {23,25,30,45},
        {24,27,32,47},
        {25,28,33,49},
        {26,29,34,51},
        {27,30,36,53},
        {28,31,37,55},
        {29,32,38,57},
        {30,33,40,59},
        {31,34,41,61},
        {32,35,42,63},
        {33,37,44,65},
        {34,38,45,67},
        {35,39,46,69},
        {36,40,48,71},
        {37,41,49,73},
        {38,42,50,75},
        {39,43,52,77},
        {40,44,53,79},
        {41,45,54,81},
        {42,47,56,83},
        {43,48,57,85},
        {44,49,58,87},
        {45,50,60,89},
        {46,51,61,91},
        {47,52,62,93},
        {48,53,64,95},
        {49,54,65,97},
        {50,55,66,99},
        {52,57,68,101},
        {53,58,69,103},
        {54,59,70,105},
        {55,60,72,107},
        {56,61,73,109},
        {57,62,74,111},
        {58,63,76,113},
        {59,64,77,115},
        {60,65,78,117},
        {61,67,80,119},
        {62,68,81,121},
        {63,69,82,123},
        {64,70,84,125},
        {65,71,85,127},
        {66,72,86,129},
        {67,73,88,131},
        {68,74,89,133},
        {69,75,90,135},
        {70,77,92,137},
        {71,78,93,139},
        {72,79,94,141},
        {73,80,96,143},
        {74,81,97,145},
        {75,82,98,147},
        {76,83,100,149},
        {77,84,101,151},
        {78,85,102,153},
        {79,87,104,155},
        {80,88,105,157},
        {81,89,106,159},
        {82,90,108,161},
        {83,91,109,163},
        {84,92,110,165},
        {85,93,112,167},
        {86,94,113,169},
        {87,95,114,171},
        {88,97,116,173},
        {89,98,117,175},
        {90,99,118,177},
        {91,100,120,179},
        {92,101,121,181},
        {93,102,122,183},
        {94,103,124,185},
        {95,104,125,187},
        {96,105,126,189},
        {97,107,128,191},
        {98,108,129,193},
        {99,109,130,195},
        {100,110,132,197},
        {101,111,133,199},
        {102,112,134,201},
        {103,113,136,203},
        {104,114,137,205},
        {105,115,138,207},
        {106,117,140,209},
        {107,118,141,211},
        {108,119,142,213},
        {109,120,144,215},
        {110,121,145,217},
        {111,122,146,219},
        {112,123,148,221},
        {113,124,149,223},
        {114,125,150,225},
        {115,127,152,227},
        {116,128,153,229},
        {117,129,154,231},
        {118,130,156,233},
        {119,131,157,235},
        {120,132,158,237},
        {121,133,160,239},
        {122,134,161,241},
        {123,135,162,243},
        {124,137,164,245},
        {125,138,165,247},
        {126,139,166,249},
        {127,140,168,251},
        {128,141,169,253},
        {129,142,170,255},
        {130,143,172,0},
        {131,144,173,0},
        {132,145,174,0},
        {133,147,176,0},
        {134,148,177,0},
        {135,149,178,0},
        {136,150,180,0},
        {137,151,181,0},
        {138,152,182,0},
        {139,153,184,0},
        {140,154,185,0},
        {141,155,186,0},
        {142,157,188,0},
        {143,158,189,0},
        {144,159,190,0},
        {145,160,192,0},
        {146,161,193,0},
        {147,162,194,0},
        {148,163,196,0},
        {149,164,197,0},
        {150,165,198,0},
        {152,167,200,0},
        {153,168,201,0},
        {154,169,202,0},
        {155,170,204,0},
        {156,171,205,0},
        {157,172,206,0},
        {158,173,208,0},
        {159,174,209,0},
        {160,175,210,0},
        {161,177,212,0},
        {162,178,213,0},
        {163,179,214,0},
        {164,180,216,0},
        {165,181,217,0},
        {166,182,218,0},
        {167,183,220,0},
        {168,184,221,0},
        {169,185,222,0},
        {170,187,224,0},
        {171,188,225,0},
        {172,189,226,0},
        {173,190,228,0},
        {174,191,229,0},
        {175,192,230,0},
        {176,193,232,0},
        {177,194,233,0},
        {178,195,234,0},
        {179,197,236,0},
        {180,198,237,0},
        {181,199,238,0},
        {182,200,240,0},
        {183,201,241,0},
        {184,202,242,0},
        {185,203,244,0},
        {186,204,245,0},
        {187,205,246,0},
        {188,207,248,0},
        {189,208,249,0},
        {190,209,250,0},
        {191,210,252,0},
        {192,211,253,0},
        {193,212,254,0},
        {194,213,0,0},
        {195,214,0,0},
        {196,215,0,0},
        {197,217,0,0},
        {198,218,0,0},
        {199,219,0,0},
        {200,220,0,0},
        {201,221,0,0},
        {202,222,0,0},
        {203,223,0,0},
        {204,224,0,0},
        {205,225,0,0},
        {206,227,0,0},
        {207,228,0,0},
        {208,229,0,0},
        {209,230,0,0},
        {210,231,0,0},
        {211,232,0,0},
        {212,233,0,0},
        {213,234,0,0},
        {214,235,0,0},
        {215,237,0,0},
        {216,238,0,0},
        {217,239,0,0},
        {218,240,0,0},
        {219,241,0,0},
        {220,242,0,0},
        {221,243,0,0},
        {222,244,0,0},
        {223,245,0,0},
        {224,247,0,0},
        {225,248,0,0},
        {226,249,0,0},
        {227,250,0,0},
        {228,251,0,0},
        {229,252,0,0},
        {230,253,0,0},
        {231,254,0,0},
        {232,255,0,0},
        {233,0,0,0},
        {234,0,0,0},
        {235,0,0,0},
        {236,0,0,0},
        {237,0,0,0},
        {238,0,0,0},
        {239,0,0,0},
        {240,0,0,0},
        {241,0,0,0},
        {242,0,0,0},
        {243,0,0,0},
        {244,0,0,0},
        {245,0,0,0},
        {246,0,0,0},
        {247,0,0,0},
        {248,0,0,0},
        {249,0,0,0},
        {250,0,0,0},
        {252,0,0,0},
        {253,0,0,0},
        {254,0,0,0},
        {255,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        };

}

int DrawAlgo::prepare_Line_Image()
{
    bool case_dups = false;
    bool case_base = false;
    bool case_zero = false;
    //0 white
    //1 cyan
    //2 yellow
    //3 magenta
    //4 red
    //5 green
    //6 blue
    //7 black
    int cur_col = 0;

    cur_color_vector = &white_commands;
    if(image_set)
    {
        for(int dy = 0; dy < myPixelData.height(); dy++)
        {

            for(int dx = 0; dx < myPixelData.width(); dx++)
            {
                case_dups = false;
                case_base = false;
                case_zero = false;
                //cur_color_vector = &white_commands;
                cur_col = 0;
                QColor pixColor = myPixelData.pixelColor(dx,dy);
                if(pixColor.alpha()>0)
                {
                    int r = pixColor.red();
                    int g = pixColor.green();
                    int b = pixColor.blue();
                    int a = std::round(pixColor.alphaF() * 100);

                    shiftCmd cur_coord;
                    cur_coord.x1 = dx;
                    cur_coord.y1 = dy;
                    cur_coord.reps = 1;
                    cur_coord.single = true;
                    if(a < 100)
                    {
                        if(usingAlpha)
                        {
                            alpha_commands[a].push_back(cur_coord);
                            total_commands+=1;
                            alpha_commands_count++;
                        }

                    }


                    int hi;
                    int lo;
                    int hi_val;
                    int cur_mid;
                    int cur_lo;

                    //what we need to know: the black alphas, the secondary color and their alphas, and primary
                    // We always do black unless base. We always do primary before black unless hi==mid.
                    // We always do secondary first unless mid==lo.
                    //It looks like we always calculate the hi unless base color.
                    //The base-case algorithm is as follows: calculate hi- map mid and lo, calculate mid, map lo,
                    //  calculate lo.

                    //we need to know this so we can determine if only using black (all same) or a prim(lows same)
                    //  or secondary (high's same)
                    //later on
                    if(r==g || g==b || r==b)
                    {
                        case_dups=true;
                        //std::cout <<"case dup!"<<std::endl;
                    }

                    //we need this to know whether or not to use a base color other than white for alpha 100
                    if(r==0||g==0||b==0)
                    {
                        if(r==0&&g==0&&b==0)
                        {
                            case_base = true;
                            cur_col = 7;
                            //cur_color_vector = &black_commands;
                        }
                        case_zero=true;
                        //std::cout <<"case zero!"<<std::endl;
                    }
                    if(r==255&&g==255&&b==255)
                    {
                        //white
                        case_base=true;
                    }
                    else if(case_zero&&case_dups)
                    {
                        if(r==255)
                        {
                            if(g==255)
                            {
                                //yellow
                                case_base=true;
                                cur_col = 2;
                                //cur_color_vector = &yellow_commands;
                            }
                            else if(b==255)
                            {
                                //magenta
                                case_base=true;
                                cur_col = 3;
                                //cur_color_vector = &magenta_commands;
                            }
                            else if(g==b)
                            {
                                //red
                                case_base=true;
                                cur_col = 4;
                                //cur_color_vector = &red_commands;
                            }
                        }
                        else if(g==255)
                        {
                            if(b==255)
                            {
                                //cyan
                                case_base=true;
                                cur_col = 1;
                                //cur_color_vector = &cyan_commands;
                            }
                            else if(b==r)
                            {
                                //green
                                case_base=true;
                                cur_col = 5;
                                //cur_color_vector = &green_commands;
                            }
                        }
                        else if(b==255)
                        {
                            //because we know there is a zero
                            if(r==g)
                            {
                                //blue
                                case_base=true;
                                cur_col = 6;
                                //cur_color_vector = &blue_commands;
                            }
                        }
                    }


                    //hard coded for speed. The easy way would be to store into a vector as a pair. Sort based off
                    // of value, and then run the adjacent comparison for dups, and then one more for 0's.
                    //Depending on how long it can take I may assign each pixel to a core and split the work apart.

                    //what if I store as hexadecimal for for easy switch cases? I switch case for a base color.

                    //1 red. 2 green. 3 blue
                    //whicheever is mid is the color used for part 2.
                    //if 1 low, use cyan, if 2 low, use magenta, if 3 low, use yellow.

                    //Just append the coord to the base color alpha 100 and move to next pixel
                    if(case_base)
                    {
                        //(*cur_color_vector)[5].push_back(cur_coord);
                        commands_by_coord[dy][dx][cur_col][5]++;
                        //std::cout <<"case base!"<<std::endl;
                    }
                    else
                    {
                        //We determine the base for the pixel. Since case_base was false we know its not base
                        //  and still needs ops done. Also dont need to check for all 3 since black is  a base case color.
                        //if no 0's white is base.
                        if(case_zero)
                        {
                            if(r==0)
                            {
                                if(g==0)
                                {
                                    //blue, r,g = 0
                                    //cur_color_vector = &blue_commands;
                                    cur_col = 6;
                                }
                                else if(b==0)
                                {
                                    //green, r,b = 0
                                    //cur_color_vector = &green_commands;
                                    cur_col = 5;
                                }
                                else
                                {
                                    //cyan, r=0
                                    //cur_color_vector = &cyan_commands;
                                    cur_col = 1;
                                }
                            }
                            else if(g==0)
                            {
                                if(b==0)
                                {
                                    //red b,g = 0
                                    //cur_color_vector = &red_commands;
                                    cur_col = 4;
                                }
                                else
                                {
                                    //magenta, g = 0
                                    //cur_color_vector = &magenta_commands;
                                    cur_col = 3;
                                }
                            }
                            else
                            {
                                //yellow, blue = 0
                                //cur_color_vector = &yellow_commands;
                                cur_col = 2;
                            }
                        }

                        //add the base color
                        //(*cur_color_vector)[5].push_back(cur_coord);
                        commands_by_coord[dy][dx][cur_col][5]++;

                        //now we figure out the order of highest to lowest.
                        //red 1, green 2, blue 3
                        hi = 1;
                        if(g > r)
                        {
                            hi = 2;
                            if(b > g)
                            {
                                hi = 3;
                                hi_val = b;
                                cur_mid = g;
                                lo = 1;
                                cur_lo = r;
                            }
                            else
                            {
                                if(r > b)
                                {
                                    cur_mid = r;
                                    lo = 3;
                                    cur_lo = b;
                                    hi_val = g;
                                }
                                else
                                {
                                    cur_mid = b;
                                    lo = 1;
                                    cur_lo = r;
                                    hi_val = g;
                                }
                            }
                        }
                        else if(b >r)
                        {
                            hi = 3;
                            hi_val = b;
                            cur_mid = r;
                            lo = 2;
                            cur_lo = g;
                        }
                        else
                        {
                            if(g > b)
                            {
                                cur_mid = g;
                                lo = 3;
                                cur_lo = b;
                                hi_val = r;
                            }
                            else
                            {
                                cur_mid = b;
                                lo = 2;
                                cur_lo = g;
                                hi_val = r;
                            }
                        }
                        //std::cout << "hi: " << hi << std::endl;
                        //std::cout << "mid: " << mid << std::endl;
                        //std::cout << "lo: " << lo << std::endl;
                        int steps_left = 3;
                        //if hi == mid, secondary performed on lo, black performed after.
                        //if mid == low, primary performed on lo, black after.
                        //if all 3, only black.
                        //if low = 0, treat as already done.
                        if(case_dups)
                        {
                            if(hi_val==cur_lo||case_zero)
                            {
                                steps_left = 1;
                                //std::cout <<"steps 1!"<<std::endl;
                                //Add to the black the respective alpha commands and move to next pixel
                            }
                            else
                            {
                                steps_left = 2;
                                //std::cout <<"steps 2!"<<std::endl;
                            }
                        }
                        else if(case_zero)
                        {
                            steps_left = 2;
                            cur_lo = cur_mid;
                            //std::cout <<"steps 2 zero!"<<std::endl;
                        }

                        //std::cout <<"before: " << hi_val<<std::endl;
                        //make sure the current value has a possible list of operations to reach.
                        if(hi_val==cur_mid)
                        {
                            cur_mid = value_Corrections[cur_mid-1];
                        }
                        hi_val = value_Corrections[hi_val-1];
                        //std::cout <<"after: " << hi_val<<std::endl;

                        //Add to the black the respective alpha commands.
                        //add_to_color_commands(black_commands, operations[hi_val-1], cur_coord);
                        add_line_commands_to_coord(dy,dx,7, operations[hi_val - 1]);
                        int cmd;
                        switch(steps_left)
                        {
                        //only black because all 3 were same color. Move to next pixel
                        case 1:
                            break;

                            //figure out which 2 were same and assign next color to be added due to that.
                        case 2:
                            //next color is secondary, else primary
                            //also works in case of a single 0 because we want primary color.
                            if(hi_val==cur_mid)
                            {
                                switch(lo)
                                {
                                case 1:
                                    //cur_color_vector = &cyan_commands;
                                    cur_col = 1;
                                    break;
                                case 2:
                                    //cur_color_vector = &magenta_commands;
                                    cur_col = 3;
                                    break;
                                case 3:
                                    //cur_color_vector = &yellow_commands;
                                    cur_col = 2;
                                    break;
                                }
                            }
                            else
                            {
                                //primary's
                                switch(hi)
                                {
                                case 1:
                                    cur_col = 4;
                                    //cur_color_vector = &red_commands;
                                    break;
                                case 2:
                                    cur_col = 5;
                                    //cur_color_vector = &green_commands;
                                    break;
                                case 3:
                                    cur_col = 6;
                                    //cur_color_vector = &blue_commands;
                                    break;
                                }
                            }

                            //now we do value mappings. the initial ops are only done on lo.
                            cmd = operations[hi_val-1].size()-1;
                            while(cmd >=0)
                            {
                                if(operations[hi_val-1][cmd]==4)
                                {
                                    cur_lo = value_mappings[cur_lo-1][0];
                                }
                                else
                                {
                                    cur_lo = value_mappings[cur_lo-1][operations[hi_val-1][cmd]];
                                }
                                //std::cout << "cur_low: " << cur_lo << std::endl;
                                cmd--;
                            }

                            //value now corrected.
                            cur_lo = value_Corrections[cur_lo-1];

                            //add_to_color_commands(*cur_color_vector, operations[cur_lo-1], cur_coord);
                            add_line_commands_to_coord(dy,dx,cur_col, operations[cur_lo - 1]);
                            //Method for addings ops to cur_color_vector.
                            break;

                            //default;
                        case 3:
                            //value mappings
                            cmd = operations[hi_val-1].size()-1;
                            while(cmd >=0)
                            {
                                if(operations[hi_val-1][cmd]==4)
                                {
                                    cur_lo = value_mappings[cur_lo-1][0];
                                    cur_mid = value_mappings[cur_mid-1][0];
                                }
                                else
                                {
                                    cur_lo = value_mappings[cur_lo-1][operations[hi_val-1][cmd]];
                                    cur_mid = value_mappings[cur_mid-1][operations[hi_val-1][cmd]];
                                }
                                //std::cout << "cur_low: " << cur_lo << std::endl;
                                //std::cout << "cur_mid: " << cur_mid << std::endl;
                                cmd--;
                            }
                            switch(hi)
                            {
                            case 1:
                                //cur_color_vector = &red_commands;
                                cur_col = 4;
                                break;
                            case 2:
                                //cur_color_vector = &green_commands;
                                cur_col = 5;
                                break;
                            case 3:
                                //cur_color_vector = &blue_commands;
                                cur_col = 6;
                                break;
                            }
                            //std::cout <<"before mid : " << cur_mid<<std::endl;
                            //value now corrected.
                            cur_mid = value_Corrections[cur_mid-1];
                            //std::cout <<"after mid: " << cur_mid<<std::endl;

                            //add_to_color_commands(*cur_color_vector, operations[cur_mid-1], cur_coord);
                            add_line_commands_to_coord(dy,dx, cur_col, operations[cur_mid - 1]);

                            cmd = operations[cur_mid-1].size()-1;
                            while(cmd >=0)
                            {
                                if(operations[cur_mid-1][cmd]==4)
                                {
                                    cur_lo = value_mappings[cur_lo-1][0];
                                }
                                else
                                {
                                    cur_lo = value_mappings[cur_lo-1][operations[cur_mid-1][cmd]];
                                }
                                //std::cout << "cur_low2: " << cur_lo << std::endl;
                                cmd--;
                            }

                            switch(lo)
                            {
                            case 1:
                                cur_col = 1;
                                cur_color_vector = &cyan_commands;
                                break;
                            case 2:
                                cur_col = 3;
                                //cur_color_vector = &magenta_commands;
                                break;
                            case 3:
                                cur_col = 2;
                                //cur_color_vector = &yellow_commands;
                                break;
                            }

                            //std::cout <<"before lo : " << cur_lo<<std::endl;
                            //value now corrected.
                            cur_lo = value_Corrections[cur_lo-1];
                            //std::cout <<"after lo : " << cur_lo<<std::endl;

                            //add_to_color_commands(*cur_color_vector, operations[cur_lo-1], cur_coord);
                            add_line_commands_to_coord(dy,dx, cur_col, operations[cur_lo - 1]);
                            break;
                        }

                    }
                }


            }
        }
        //where we convert many points into shift draw commands
        qDebug("calculate now");
        calculate_line_commands();
        image_prepared = true;
        return 0;
    }
    else
    {
        return 2;
    }

}

int DrawAlgo::x_Full_Check(int olddx, int olddy, int c)
{
    int dx = olddx;
    int consec = 0;
    dx++;
    while(dx < imgW &&commands_by_coord[olddy][dx][c][5]==1)
    {
        dx++;
        consec++;
    }
    return consec;
}

int DrawAlgo::y_Full_Check(int olddx, int olddy, int c)
{
    int dy = olddy;
    int consec = 0;
    dy++;
    while(dy < imgH && commands_by_coord[dy][olddx][c][5]==1)
    {
        dy++;
        consec++;
    }
    return consec;
}

bool DrawAlgo::y_Line_Check(int olddx, int olddy, int a, int c, int xconsec)
{
    int dx = olddx;
    int dy = olddy;
    int required_length = xconsec + 1; //was (imgH - dy)/2
    int lowest1 = commands_by_coord[dy][dx][c][a];
    int lowest2 = commands_by_coord[dy][dx][c][a];
    int consec = 1;
    dy++;
    while(dy < imgH&&commands_by_coord[dy][dx][c][a]>0)
    {
        if(commands_by_coord[dy][dx][c][a]<lowest2)
        {
            lowest2 = commands_by_coord[dy][dx][c][a];
        }
        consec++;
        dy++;

    }
    //the >= 3 for if its the bottom of the image.
    if(consec >= required_length && consec >= 3)
    {
        //append the new shift command.
        shiftCmd new_command;
        if(lowest2<lowest1)
        {
            new_command.stop_short = false;
            commands_by_coord[olddy][olddx][c][a]--;
            new_command.dx = 0;
            new_command.dy = 0;
        }
        else
        {
            new_command.stop_short = true;
            new_command.dx = 0;
            new_command.dy = 1;
        }
        new_command.reps = lowest2;
        if(lowest2%2==0)
        {
            new_command.even = true;
        }
        else
        {
            new_command.even = false;
        }
        new_command.x1 = olddx;
        new_command.y1 = olddy;
        new_command.x2 = olddx;
        new_command.y2 = dy - 1;
        new_command.single = false;
        total_commands+=1;
        shifty_commands++;
        (*color_commands[c])[a].push_back(new_command);
        //Now subtract from all of those points.
        for(int p = olddy; p < dy; p++)
        {
            commands_by_coord[p][olddx][c][a]-=lowest2;
        }

        return true;
    }
    else
    {
        return false;
    }

}

//In this one, we know the top starts with a 1. We simply go down halfway of the remaining, and if we make it that far,
//  we go intil 2 consecutive >2's, or we reach the end. Not gonna worry about drawing up or down.
bool DrawAlgo::y_Thin_Line_Check(int olddy, int olddx, int a, int c, int xconsec)
{
    int dx = olddx;
    int dy = olddy;
    int required_length = xconsec + 1; //old (imgH - dy)/2
    int consec = 1;
    dy++;
    while(consec < required_length && dy < imgH && commands_by_coord[dy][dx][c][a]>0)
    {
        dy++;
        consec++;
    }
    if(consec>=3 && consec>= required_length)
    {
        bool found_req = false;
        int inarow = 0;
        int g1_1 = -1;
        int topwall = -1;
        int botwall = 0;
        //we go until 2 consective 2's or end
        while(!found_req && dy < imgH && commands_by_coord[dy][dx][c][a]>0)
        {
            if(commands_by_coord[dy][dx][c][a]>1)
            {
                inarow++;
                g1_1 = dy;
                topwall = dy - olddy;
                botwall = 0;
            }
            else
            {
                inarow = 0;
                botwall++;
            }
            if(inarow==2)
            {
                //add shift click commands
                shiftCmd new_command;
                new_command.x1 = olddx;
                new_command.y1 = dy-1;
                new_command.x2 = olddx;
                new_command.y2 = olddy;
                new_command.even = false;
                new_command.reps = 1;
                new_command.single = false;
                new_command.dx = 0;
                new_command.dy = 0;
                total_commands+=1;
                shifty_commands++;
                commands_by_coord[dy-1][olddx][c][a]--;
                for(int p = olddy; p < dy; p++)
                {
                    commands_by_coord[p][olddx][c][a]--;
                }
                (*color_commands[c])[a].push_back(new_command);
                found_req = true;

            }
            dy++;
            if(dy == imgH && !found_req)
            {
                //shift click up from out of bounds
                //add shift click commands
                shiftCmd new_command;
                new_command.x1 = olddx;
                new_command.y1 = dy;
                new_command.x2 = olddx;
                new_command.y2 = olddy;
                new_command.even = false;
                new_command.reps = 1;
                new_command.single = false;
                new_command.dx = 0;
                new_command.dy = 0;
                total_commands+=1;
                shifty_commands++;
                for(int p = olddy; p < dy; p++)
                {
                    commands_by_coord[p][olddx][c][a]--;
                }
                (*color_commands[c])[a].push_back(new_command);
                found_req = true;
            }
        }
        if(found_req)
        {
            return true;
        }
        else
        {
            if(botwall < topwall)
            {
                //shift command from g1_1 to the olddy
                shiftCmd new_command;
                new_command.x1 = olddx;
                new_command.y1 = g1_1;
                commands_by_coord[g1_1][olddx][c][a]--;
                new_command.x2 = olddx;
                new_command.y2 = olddy;
                new_command.even = false;
                new_command.reps = 1;
                new_command.single = false;
                new_command.dx = 0;
                new_command.dy = 0;
                total_commands+=1;
                shifty_commands++;
                for(int p = olddy; p <= g1_1; p++)
                {
                    commands_by_coord[p][olddx][c][a]--;
                }
                (*color_commands[c])[a].push_back(new_command);
                return true;
            }
            else
            {
                return false;
            }

        }
    }
    else
    {
        return false;
    }
}

void DrawAlgo::x_Line_Check(int olddx, int olddy, int a, int c)
{
    int consec1 = 0;
    int consec2 = 0;
    int dx = olddx;
    int dy = olddy;
    //Only using verticle shift distance if greater than 50% of remaining distance and horiz is <=4.
    //if a 1 go right until non one found
    if(commands_by_coord[dy][dx][c][a]==1)
    {
        //if on edge, go as far right as possible first else, proceed as normal.
        if(olddx != 0)
        {            
            consec1++;
            dx++;
            //while still 1
            while(dx < imgW&&commands_by_coord[dy][dx][c][a]==1)
            {
                consec1++;
                dx++;
            }
            //if 0, do y for each. For each that fails, reduce by one and add click to color commands default.
            if(dx < imgW &&commands_by_coord[dy][dx][c][a]==0)
            {
                for(int p = 0; p < consec1; p++)
                {
                    //if not doing the y-axis draw just add the click to standard color command vector.
                    bool kept = y_Thin_Line_Check(olddy, olddx + p, a, c, 0);
                    if(!kept)
                    {
                        //standard click
                        shiftCmd new_single;
                        new_single.x1 = olddx + p;
                        new_single.y1 = olddy;
                        new_single.reps = commands_by_coord[olddy][olddx+p][c][a];
                        new_single.single = true;
                        total_commands+=1;
                        single_commands++;
                        (*color_commands[c])[a].push_back(new_single);
                        commands_by_coord[olddy][olddx+p][c][a]=0;
                    }
                }

            }
            else if(dx == imgW)
            {
                //shiftclick to the left from out of bounds if consec >= 4, else do the y check first
                int doX = true;
                if(1) // old consec1 <= 4
                {
                    doX = false;
                    bool kept = y_Thin_Line_Check(olddy, olddx, a, c, consec1);
                    if(!kept)
                    {
                        if(consec1 > 2)
                        {
                            doX = true;
                        }
                        else
                        {
                            //standard click
                            shiftCmd new_single;
                            new_single.x1 = olddx;
                            new_single.y1 = olddy;
                            new_single.reps = commands_by_coord[olddy][olddx][c][a];
                            new_single.single = true;
                            total_commands+=1;
                            single_commands++;
                            (*color_commands[c])[a].push_back(new_single);
                            commands_by_coord[olddy][olddx][c][a]=0;
                        }

                    }
                }
                //shift click from out of bounds.
                if(doX)
                {
                    shiftCmd new_command;
                    new_command.x1 = dx;
                    new_command.y1 = olddy;
                    new_command.x2 = olddx;
                    new_command.y2 = olddy;
                    new_command.even = false;
                    new_command.reps = 1;
                    new_command.single = false;
                    new_command.dx = 0;
                    new_command.dy = 0;
                    total_commands+=1;
                    shiftx_commands++;
                    (*color_commands[c])[a].push_back(new_command);
                    for(int p = olddx; p < dx; p++)
                    {
                        commands_by_coord[olddy][p][c][a]--;
                    }
                }
            }
            else
            {
                dx++;
                //we found a >1, if immediate right is one, count to 0 or 2 consecutive >1's
                if(dx < imgW && commands_by_coord[dy][dx][c][a]==1)
                {
                    int g1_1 = dx-1;
                    int leftwall = g1_1 - olddx;
                    int g1_2 = g1_1;
                    int inarow = 0;
                    bool inarow_done = false;
                    consec2++;
                    while(dx < imgW&&commands_by_coord[dy][dx][c][a]>0 &&!inarow_done)
                    {
                        //found a >1 so reset consec and set new right side greatest
                        if(commands_by_coord[dy][dx][c][a]>1)
                        {
                            consec2=0;
                            g1_2 = dx;
                            inarow++;
                            //2 in a row, so we can go left on the left and right on the right
                            if(inarow==2)
                            {
                                bool kept = y_Thin_Line_Check(olddy, olddx, a, c, dx-olddx-1);
                                if(!kept)
                                {
                                    //shift click left here
                                    shiftCmd new_command;
                                    new_command.x1 = dx-1;
                                    commands_by_coord[olddy][dx-1][c][a]--;
                                    new_command.y1 = olddy;
                                    new_command.x2 = olddx;
                                    new_command.y2 = olddy;
                                    new_command.even = false;
                                    new_command.reps = 1;
                                    new_command.single = false;
                                    new_command.dx = 0;
                                    new_command.dy = 0;
                                    total_commands+=1;
                                    shiftx_commands++;
                                    (*color_commands[c])[a].push_back(new_command);
                                    for(int p = olddx; p < dx; p++)
                                    {
                                        commands_by_coord[olddy][p][c][a]--;
                                    }

                                }
                                inarow_done = true;

                            }
                        }
                        else
                        {
                            inarow = 0;
                            consec2++;
                        }

                        dx++;
                        if(dx == imgW && !inarow_done)
                        {
                            //do out of bounds click.
                            shiftCmd new_command;
                            if(inarow==1)
                            {
                                new_command.x1 = dx-1;
                                commands_by_coord[olddy][dx-1][c][a]--;
                            }
                            else
                            {
                                new_command.x1 = dx;
                            }
                            new_command.y1 = olddy;
                            new_command.x2 = olddx;
                            new_command.y2 = olddy;
                            new_command.even = false;
                            new_command.reps = 1;
                            new_command.single = false;
                            new_command.dx = 0;
                            new_command.dy = 0;
                            total_commands+=1;
                            shiftx_commands++;
                            (*color_commands[c])[a].push_back(new_command);
                            for(int p = olddx; p < dx; p++)
                            {
                                commands_by_coord[olddy][p][c][a]--;
                            }
                            inarow_done = true;
                        }
                    }
                    //0, so shift click the longer distance.
                    if(!inarow_done)
                    {
                        if(consec2>leftwall)
                        {
                            //go left to right
                            shiftCmd new_command;
                            new_command.x1 = g1_1;
                            new_command.y1 = olddy;
                            new_command.x2 = dx-1;
                            new_command.y2 = olddy;
                            new_command.even = false;
                            new_command.reps = 1;
                            new_command.single = false;
                            new_command.dx = 0;
                            new_command.dy = 0;
                            total_commands+=1;
                            shiftx_commands++;
                            commands_by_coord[olddy][g1_1][c][a]--;
                            (*color_commands[c])[a].push_back(new_command);
                            for(int p = g1_1; p < dx; p++)
                            {
                                commands_by_coord[olddy][p][c][a]--;
                            }
                        }
                        else
                        {
                            //go right to left.
                            shiftCmd new_command;
                            new_command.x1 = g1_2;
                            new_command.y1 = olddy;
                            new_command.x2 = olddx;
                            new_command.y2 = olddy;
                            new_command.even = false;
                            new_command.reps = 1;
                            new_command.single = false;
                            new_command.dx = 0;
                            new_command.dy = 0;
                            total_commands+=1;
                            shiftx_commands++;
                            commands_by_coord[olddy][g1_2][c][a]--;
                            (*color_commands[c])[a].push_back(new_command);
                            for(int p = olddx; p <= g1_2; p++)
                            {
                                commands_by_coord[olddy][p][c][a]--;
                            }
                        }
                    }
                }
                //immediate right greater than 1, run a shift to the left.
                else if(dx < imgH &&commands_by_coord[dy][dx][c][a]>1)
                {
                    if(dx-1-olddx > 2)
                    {
                        shiftCmd new_command;
                        new_command.x1 = dx-1;
                        new_command.y1 = olddy;
                        new_command.x2 = olddx;
                        new_command.y2 = olddy;
                        new_command.even = false;
                        new_command.reps = 1;
                        new_command.single = false;
                        new_command.dx = 0;
                        new_command.dy = 0;
                        commands_by_coord[olddy][dx-1][c][a]--;
                        total_commands+=1;
                        shiftx_commands++;
                        (*color_commands[c])[a].push_back(new_command);
                        for(int p = olddx; p < dx; p++)
                        {
                            commands_by_coord[olddy][p][c][a]--;
                        }
                    }
                    else
                    {
                        //if not doing the y-axis draw just add the click to standard color command vector.
                        bool kept = y_Thin_Line_Check(olddy, olddx, a, c, 0);
                        if(!kept)
                        {
                            shiftCmd new_single;
                            new_single.x1 = olddx;
                            new_single.y1 = olddy;
                            new_single.reps = commands_by_coord[olddy][olddx][c][a];
                            new_single.single = true;
                            total_commands+=1;
                            single_commands++;
                            (*color_commands[c])[a].push_back(new_single);
                            commands_by_coord[olddy][olddx][c][a]=0;
                        }
                    }

                }
                //it was a 0 or edge of image so we just go left anyways.
                else
                {
                    if(dx-1-olddx > 2)
                    {
                        shiftCmd new_command;
                        new_command.x1 = dx-1;
                        new_command.y1 = olddy;
                        new_command.x2 = olddx;
                        new_command.y2 = olddy;
                        new_command.even = false;
                        new_command.reps = 1;
                        new_command.single = false;
                        new_command.dx = 0;
                        new_command.dy = 0;
                        commands_by_coord[olddy][dx-1][c][a]--;
                        total_commands+=1;
                        shiftx_commands++;
                        (*color_commands[c])[a].push_back(new_command);
                        for(int p = olddx; p < dx; p++)
                        {
                            commands_by_coord[olddy][p][c][a]--;
                        }
                    }
                    else
                    {
                        //if not doing the y-axis draw just add the click to standard color command vector.
                        bool kept = y_Thin_Line_Check(olddy, olddx, a, c, 0);
                        if(!kept)
                        {
                            shiftCmd new_single;
                            new_single.x1 = olddx;
                            new_single.y1 = olddy;
                            new_single.reps = commands_by_coord[olddy][olddx][c][a];
                            new_single.single = true;
                            total_commands+=1;
                            single_commands++;
                            (*color_commands[c])[a].push_back(new_single);
                            commands_by_coord[olddy][olddx][c][a]=0;
                        }
                    }
                }

            }
        }
        //we are along the edge for guaranteed shift click.
        else
        {
            //go right until 0.
            while(dx < imgW&&commands_by_coord[dy][dx][c][a]>0)
            {
                dx++;
            }
            //curent dx is a 0
            if(dx-olddx > 3)
            {
                //shift click
                shiftCmd new_command;
                //if the point before a 0 was >1, we click from there instead
                if(commands_by_coord[dy][dx-1][c][a]>1)
                {
                    new_command.x1 = dx-1;
                    new_command.y1 = olddy;
                    new_command.x2 = olddx;
                    new_command.y2 = olddy;
                    commands_by_coord[olddy][dx-1][c][a]--;
                }
                else
                {
                    new_command.x1 = olddx-1;
                    new_command.y1 = olddy;
                    new_command.x2 = dx-1;
                    new_command.y2 = olddy;
                }
                new_command.even = false;
                new_command.reps = 1;
                new_command.single = false;
                new_command.dx = 0;
                new_command.dy = 0;
                total_commands+=1;
                shiftx_commands++;
                (*color_commands[c])[a].push_back(new_command);
                for(int p = olddx; p < dx; p++)
                {
                    commands_by_coord[olddy][p][c][a]--;
                }
            }
            else
            {
                //if not doing the y-axis draw just add the click to standard color command vector.
                bool kept = y_Thin_Line_Check(olddy, olddx, a, c, 0);
                if(!kept)
                {
                    shiftCmd new_single;
                    new_single.x1 = olddx;
                    new_single.y1 = olddy;
                    new_single.reps = commands_by_coord[olddy][olddx][c][a];
                    new_single.single = true;
                    total_commands+=1;
                    single_commands++;
                    (*color_commands[c])[a].push_back(new_single);
                    commands_by_coord[olddy][olddx][c][a]=0;
                }
            }
        }

    }
    //else go right until a 0 is found keeping track of the lowest number.
    else
    {
        int lowest1 = commands_by_coord[dy][dx][c][a];
        int lowest2 = commands_by_coord[dy][dx][c][a];
        dx++;
        consec1++;
        while(dx < imgW&&commands_by_coord[dy][dx][c][a]>0)
        {

            if(commands_by_coord[dy][dx][c][a]<lowest2)
            {
                lowest2 = commands_by_coord[dy][dx][c][a];
            }
            dx++;
            consec1++;
        }
        //Now do y. If y> 50% of imgH-y, and consec1 < 4.
        bool doX = true;
        if(1) //was consec1<=4
        {
            doX = false;
            //Since we know the starting point is greater > 2, we just need to look down until 0 is found.
            bool kept = y_Line_Check(olddx, olddy, a, c, consec1);
            if(!kept)
            {
                if(consec1 > 2)
                {
                   doX = true;
                }
                else if(consec1>1 && lowest1 > 2 && lowest2 > 1)
                {
                   doX = true;
                }
                else
                {
                   //Single clicks
                   shiftCmd new_single;
                   new_single.x1 = olddx;
                   new_single.y1 = olddy;
                   new_single.reps = commands_by_coord[olddy][olddx][c][a];
                   new_single.single = true;
                   total_commands+=1;
                   single_commands++;
                   (*color_commands[c])[a].push_back(new_single);
                   commands_by_coord[olddy][olddx][c][a]=0;
                }

            }
        }
        if(doX)
        {
            //append the new shift command.
            shiftCmd new_command;
            if(lowest2<lowest1)
            {
                new_command.stop_short = false;
                commands_by_coord[olddy][olddx][c][a]--;
                new_command.dx = 0;
                new_command.dy = 0;
            }
            else
            {
                new_command.stop_short = true;
                new_command.dx = 1;
                new_command.dy = 0;
            }
            new_command.reps = lowest2;
            if(lowest2%2==0)
            {
                new_command.even = true;
            }
            else
            {
                new_command.even = false;
            }
            new_command.x1 = olddx;
            new_command.y1 = olddy;
            new_command.x2 = dx - 1;
            new_command.y2 = olddy;
            new_command.single = false;
            total_commands+=1;
            shiftx_commands++;
            for(int p = olddx; p < dx; p++)
            {
                commands_by_coord[olddy][p][c][a]-=lowest2;
            }
            (*color_commands[c])[a].push_back(new_command);
        }

    }
}

void DrawAlgo::calculate_line_commands()
{
    //for color, for alpha
    for(int c = 0; c < 8; c++)
    {
        //different algorithm for 100 alpha
        for(int a = 0; a < 5; a++)
        {
            for(int dy = 0; dy < imgH; dy++)
            {
                for(int dx = 0; dx < imgW; dx++)
                {
                    //We want to repeat this process with the current pixel until its count = 0;
                    while(commands_by_coord[dy][dx][c][a]>0)
                    {
                        x_Line_Check(dx, dy, a, c);
                    }

                }
            }
        }
    }
    qDebug("did alpha");
    //now for the alpha 100's
    for(int c = 0; c < 8; c++)
    {
        for(int dy = 0; dy < imgH; dy++)
        {
            for(int dx = 0; dx < imgW; dx++)
            {
                if(commands_by_coord[dy][dx][c][5]==1)
                {
                    //because overlaying isnt an issue, we simply check the right and down for the longer consec. Which ever is longer we do.
                    int xcons = x_Full_Check(dx, dy, c);
                    int ycons = y_Full_Check(dx, dy, c);
                    if(xcons >= ycons)
                    {
                        //from dx we move right xcons times to apply color.
                        //if xcons <3 we just do normal clicks without shift.
                        if(xcons>=2)
                        {
                            shiftCmd new_command;
                            new_command.x1 = dx;
                            new_command.y1 = dy;
                            new_command.x2 = dx+xcons;
                            new_command.y2 = dy;
                            new_command.even = false;
                            new_command.reps = 1;
                            new_command.single = false;
                            shiftx_commands++;
                            total_commands++;
                            for(int p = dx; p <= dx+xcons; p++)
                            {
                                commands_by_coord[dy][p][c][5]--;
                            }
                            (*color_commands[c])[5].push_back(new_command);
                        }
                        else
                        {
                            shiftCmd new_single;
                            new_single.x1 = dx;
                            new_single.y1 = dy;
                            new_single.reps = 1;
                            new_single.single = true;
                            single_commands++;
                            total_commands++;
                            (*color_commands[c])[5].push_back(new_single);
                            commands_by_coord[dy][dx][c][5]=0;
                        }
                    }
                    else
                    {
                        if(ycons>=2)
                        {
                            shiftCmd new_command;
                            new_command.x1 = dx;
                            new_command.y1 = dy;
                            new_command.x2 = dx;
                            new_command.y2 = dy+ycons;
                            new_command.even = false;
                            new_command.reps = 1;
                            new_command.single = false;
                            shifty_commands++;
                            total_commands++;
                            for(int p = dy; p <= dy+ycons; p++)
                            {
                                commands_by_coord[p][dx][c][5]--;
                            }
                            (*color_commands[c])[5].push_back(new_command);
                        }
                        else
                        {
                            shiftCmd new_single;
                            new_single.x1 = dx;
                            new_single.y1 = dy;
                            new_single.reps = 1;
                            new_single.single = true;
                            single_commands++;
                            total_commands++;
                            (*color_commands[c])[5].push_back(new_single);
                            commands_by_coord[dy][dx][c][5]=0;
                        }
                    }
                }

            }
        }
    }
    qDebug("did 100");

}

void DrawAlgo::start_line_drawing()
{
    if(image_prepared&&pix_set)
    {
        CURSORINFO cI = {0};
        HCURSOR HIBeam = LoadCursor(NULL, IDC_IBEAM);
        INPUT inputDown = {0};
        INPUT inputUp = {0};
        inputDown.type = INPUT_MOUSE;
        inputDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        inputUp.type = INPUT_MOUSE;
        inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        INPUT shiftDown= { 0 };
        shiftDown.type = INPUT_KEYBOARD;
        shiftDown.ki.wVk = VK_SHIFT;

        INPUT shiftUp = { 0 };
        shiftUp.type = INPUT_KEYBOARD;
        shiftUp.ki.wVk = VK_SHIFT;
        shiftUp.ki.dwFlags = KEYEVENTF_KEYUP;

        INPUT ctrlDown = { 0 };
        ctrlDown.type = INPUT_KEYBOARD;
        ctrlDown.ki.wVk = VK_CONTROL;

        INPUT ctrlUp = { 0 };
        ctrlUp.type = INPUT_KEYBOARD;
        ctrlUp.ki.wVk = VK_CONTROL;
        ctrlUp.ki.dwFlags = KEYEVENTF_KEYUP;

        INPUT zDown = { 0 };
        zDown.type = INPUT_KEYBOARD;
        zDown.ki.wVk = 'Z';

        INPUT zUp = { 0 };
        zDown.type = INPUT_KEYBOARD;
        zDown.ki.wVk = 'Z';
        zUp.ki.dwFlags = KEYEVENTF_KEYUP;

        std::string alpha1 = "1";
        std::string alpha10 = "10";
        std::string alpha25 = "25";
        std::string alpha50 = "50";
        std::string alpha100 = "100";

        //Set alpha to 100
        changeAlpha(alpha100, inputDown, inputUp);

        //set all the bases.
        for(int c = 0; c < 8; c++)
        {

            //change color-ingame
            switch(c)
            {
            case 0:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(whitex, whitey, inputDown, inputUp);
                    execute_Shift_Commands(white_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

            //from white to cyan etc..
            case 1:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(cyanx, cyany, inputDown, inputUp);
                    execute_Shift_Commands(cyan_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

                //cyan to yellow
            case 2:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(yellowx, yellowy, inputDown, inputUp);
                    execute_Shift_Commands(yellow_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

                //yellow to mag
            case 3:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(magx, magy, inputDown, inputUp);
                    execute_Shift_Commands(magenta_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

                //mag to red
            case 4:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(redx, redy, inputDown, inputUp);
                    execute_Shift_Commands(red_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

                //red to green
            case 5:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(greenx, greeny, inputDown, inputUp);
                    execute_Shift_Commands(green_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

                //green blue
            case 6:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(bluex, bluey, inputDown, inputUp);
                    execute_Shift_Commands(blue_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

                //blue to black
            case 7:
                if((*color_commands[c])[5].size()>0)
                {
                    changeColor(blackx, blacky, inputDown, inputUp);
                    execute_Shift_Commands(black_commands[5], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                break;

            }

            if(*shouldStop==1)
            {
                this->moveToThread(mainThread);
                emit earlyHalt();
                return;
            }
        }



        //for this part the same pattern is repeated with primary and secondary, so a function which accepts pointers to the color commands could be usued instead.
        //could also break further into a function that accepts the [pos] cor the commands vector.

        //change alpha to .01
        changeAlpha(alpha1, inputDown, inputUp);



        //change color to cyan;
        if(cyan_commands[0].size()>0)
        {
            changeColor(cyanx, cyany, inputDown, inputUp);
            execute_Shift_Commands(cyan_commands[0], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }


        //change color to yellow
        if(yellow_commands[0].size()>0)
        {
            changeColor(yellowx, yellowy, inputDown, inputUp);
            execute_Shift_Commands(yellow_commands[0], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change color to magenta
        if(magenta_commands[0].size()>0||magenta_commands[1].size()>0)
        {
            changeColor(magx, magy, inputDown, inputUp);
            execute_Shift_Commands(magenta_commands[0], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change alpha to .1
        changeAlpha(alpha10, inputDown, inputUp);



        execute_Shift_Commands(magenta_commands[1], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change color to yellow
        if(yellow_commands[1].size()>0)
        {
            changeColor(yellowx, yellowy, inputDown, inputUp);
            execute_Shift_Commands(yellow_commands[1], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change color to cyan;
        if(cyan_commands[1].size()>0||cyan_commands[2].size()>0)
        {
            changeColor(cyanx, cyany, inputDown, inputUp);
            execute_Shift_Commands(cyan_commands[1], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change alpha to .25
        changeAlpha(alpha25, inputDown, inputUp);



        execute_Shift_Commands(cyan_commands[2], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change color to yellow
        if(yellow_commands[2].size()>0)
        {
            changeColor(yellowx, yellowy, inputDown, inputUp);
            execute_Shift_Commands(yellow_commands[2], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change color to magenta
        if(magenta_commands[2].size()>0||magenta_commands[3].size()>0)
        {
            changeColor(magx, magy, inputDown, inputUp);
            execute_Shift_Commands(magenta_commands[2], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change alpha to .5
        changeAlpha(alpha50, inputDown, inputUp);



        execute_Shift_Commands(magenta_commands[3], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change color to yellow
        if(yellow_commands[3].size()>0)
        {
            changeColor(yellowx, yellowy, inputDown, inputUp);
            execute_Shift_Commands(yellow_commands[3], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change color to cyan
        if(cyan_commands[3].size()>0||cyan_commands[4].size()>0)
        {
            changeColor(cyanx, cyany, inputDown, inputUp);
            execute_Shift_Commands(cyan_commands[3], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //change alpha back to .01
        changeAlpha(alpha1, inputDown, inputUp);


        execute_Shift_Commands(cyan_commands[4], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to yellow
        if(yellow_commands[4].size()>0)
        {
            changeColor(yellowx, yellowy, inputDown, inputUp);
            execute_Shift_Commands(yellow_commands[4], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to magenta
        if(magenta_commands[4].size()>0)
        {
            changeColor(magx, magy, inputDown, inputUp);
            execute_Shift_Commands( magenta_commands[4], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //because we start at .01 we just switch to primary colors now.

        //change color to red;
        if(red_commands[0].size()>0)
        {
            changeColor(redx, redy, inputDown, inputUp);
            execute_Shift_Commands(red_commands[0], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to green
        if(green_commands[0].size()>0)
        {
            changeColor(greenx, greeny, inputDown, inputUp);
            execute_Shift_Commands(green_commands[0], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to blue
        if(blue_commands[0].size()>0||blue_commands[1].size()>0)
        {
            changeColor(bluex, bluey, inputDown, inputUp);
            execute_Shift_Commands(blue_commands[0], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha to .1
        changeAlpha(alpha10, inputDown, inputUp);



        execute_Shift_Commands(blue_commands[1], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to green
        if(green_commands[1].size()>0)
        {
            changeColor(greenx, greeny, inputDown, inputUp);
            execute_Shift_Commands(green_commands[1], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to red;
        if(red_commands[1].size()>0||red_commands[2].size()>0)
        {
            changeColor(redx, redy, inputDown, inputUp);
            execute_Shift_Commands(red_commands[1], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha to .25
        changeAlpha(alpha25, inputDown, inputUp);


        execute_Shift_Commands(red_commands[2], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to green
        if(green_commands[2].size()>0)
        {
            changeColor(greenx, greeny, inputDown, inputUp);
            execute_Shift_Commands(green_commands[2], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to blue
        if(blue_commands[2].size()>0||blue_commands[3].size())
        {
            changeColor(bluex, bluey, inputDown, inputUp);
            execute_Shift_Commands(blue_commands[2], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha to .5
        changeAlpha(alpha50, inputDown, inputUp);



        execute_Shift_Commands(blue_commands[3], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to green
        if(green_commands[3].size()>0)
        {
            changeColor(greenx, greeny, inputDown, inputUp);
            execute_Shift_Commands(green_commands[3], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to red
        if(red_commands[3].size()>0||red_commands[4].size()>0)
        {
            changeColor(redx, redy, inputDown, inputUp);
            execute_Shift_Commands(red_commands[3], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha back to .01
        changeAlpha(alpha1, inputDown, inputUp);



        execute_Shift_Commands(red_commands[4], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to green
        if(green_commands[4].size()>0)
        {
            changeColor(greenx, greeny, inputDown, inputUp);
            execute_Shift_Commands(green_commands[4], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change color to blue
        if(blue_commands[4].size()>0)
        {
            changeColor(bluex, bluey, inputDown, inputUp);
            execute_Shift_Commands(blue_commands[4], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
        }
        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }



        //now we just have black.
        //change color to black
        changeColor(blackx, blacky, inputDown, inputUp);
        execute_Shift_Commands(black_commands[0], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha to .1
        changeAlpha(alpha10, inputDown, inputUp);
        execute_Shift_Commands(black_commands[1], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha to .25
        changeAlpha(alpha25, inputDown, inputUp);
        execute_Shift_Commands(black_commands[2], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha to .5
        changeAlpha(alpha50, inputDown, inputUp);
        execute_Shift_Commands(black_commands[3], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }
        //change alpha back to .01
        changeAlpha(alpha1, inputDown, inputUp);
        execute_Shift_Commands(black_commands[4], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);

        if(*shouldStop==1)
        {
            this->moveToThread(mainThread);
            emit earlyHalt();
            return;
        }

        //If no alpha then drawing is finished

        //run alpha commands
        if(usingAlpha)
        {
            //change to eraser
            changeToEraser(inputDown, inputUp);
            for(int a = 0; a < 100; a++)
            {
                if(alpha_commands[a].size()>0)
                {
                    std::string cur_alpha = std::to_string(100-a);
                    changeEraserAlpha(cur_alpha, inputDown, inputUp);
                    execute_Shift_Commands(alpha_commands[a], inputDown, inputUp, cI, HIBeam, shiftDown, shiftUp);
                }
                if(*shouldStop==1)
                {
                    this->moveToThread(mainThread);
                    emit earlyHalt();
                    return;
                }
            }
        }
        //clears all the commands in the vectors.
        for(auto &color : color_commands)
        {
            for(auto &alp : *color)
            {
                alp.clear();
            }
        }
        for(int a = 0; a < 100; a++)
        {
            alpha_commands[a].clear();
        }
    }
    clicks_needed = 0;
    total_commands = 0;
    single_commands = 0;
    shiftx_commands = 0;
    shifty_commands = 0;
    alpha_commands_count = 0;
    image_prepared = false;
    this->moveToThread(mainThread);
    emit finishedDrawing();
}

void DrawAlgo::setMainThreadInstance(QThread *t)
{
    mainThread = t;
}

void DrawAlgo::set_clicks_before_sleep(int cbs)
{
    clicks_before_sleep = cbs;
}

    //in milliseconds
void DrawAlgo::set_default_cbs_sleep(int waitTime)
{
    default_sleep_time = waitTime;
}

void DrawAlgo::set_ui_click_sleep(int waitTime)
{
    ui_click_sleep = waitTime;
}

void DrawAlgo::set_ui_positions(int topLeftx, int topLefty)
{
    px = topLeftx;
    py = topLefty;

    stamp_px = px - 108;
    stamp_py = py - 108;

    eraserx = px - 301;
    erasery = py - 160;

    eraserAlphax = px - 301;
    eraserAlphay = py;

    eraserAlphaValx = px - 230;
    eraserAlphaValy = py;

    cbx = px-301;
    cby = py-33;

    redx = px-266;
    redy = py+121;

    greenx = px-266;
    greeny = py+131;

    bluex = px-266;
    bluey = py+141;

    cyanx = px-266;
    cyany = py+161;

    yellowx = px-266;
    yellowy = py+151;

    magx = px-266;
    magy = py+171;

    blackx = px-266;
    blacky = py+61;

    whitex = px-266;
    whitey = py+111;

    alphx = px - 301;
    alphy = py + 36;

    alphvalx = px - 230;
    alphvaly = py + 35;

    exitClickx = px - 154;
    exitClicky = py - 110;

    waitLocationx = px - 173;
    waitLocationy = py + 135;

    pix_set = true;
}

void DrawAlgo::quick_Test()
{

}

void DrawAlgo::changeAlphaState(int state)
{
    if(state==0)
    {
        usingAlpha = true;
    }
    else
    {
        usingAlpha = false;
    }
}

void DrawAlgo::set_Image(QPixmap img)
{
    myImage = &img;
    myPixelData = myImage->toImage();
    image_set = true;
}

bool DrawAlgo::ready_to_draw()
{
    return image_prepared;
}

void DrawAlgo::setTopLeftToUse(int index)
{
    if(index==0)
    {
        topleftToUsex = px;
        topleftToUsey = py;
        imgH = 40;
        imgW = 40;
    }
    else
    {
        topleftToUsex = stamp_px;
        topleftToUsey = stamp_py;
        imgH = 256;
        imgW = 256;
    }
}


void DrawAlgo::cleanResources()
{
    //clears all the commands in the vectors.
    for(auto &color : color_commands)
    {
        for(auto &alp : *color)
        {
            alp.clear();
        }
    }
    for(int a = 0; a < 100; a++)
    {
        alpha_commands[a].clear();
    }
    image_prepared = false;
    times_clicked = 0;
    total_commands = 0;
    single_commands = 0;
    shiftx_commands = 0;
    shifty_commands = 0;
    alpha_commands_count = 0;
}

void DrawAlgo::setAtomics(QAtomicInt *stopper, QAtomicInt *pauser)
{
    shouldRun = pauser;
    shouldStop = stopper;
}

int DrawAlgo::get_clicks_before_sleep()
{
    return clicks_before_sleep;
}

int DrawAlgo::get_default_cbs_sleep()
{
    return default_sleep_time;
}

int DrawAlgo::get_ui_click_sleep()
{
    return ui_click_sleep;
}

int DrawAlgo::getTotalCommands()
{
    return total_commands;
}

int DrawAlgo::getShiftxCommands()
{
    return shiftx_commands;
}

int DrawAlgo::getShiftyCommands()
{
    return shifty_commands;
}

int DrawAlgo::getSingleCommands()
{
    return single_commands;
}

int DrawAlgo::getAlphaCommands()
{
    return alpha_commands_count;
}

void DrawAlgo::pBarEnable(bool state)
{
    emitPBarSignal = state;
}
