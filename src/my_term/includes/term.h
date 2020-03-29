#ifndef TERM_H
#define TERM_H

#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
//#include <termio.h>
#include <cstdlib>
#include <cstring>

class Terminal
{
private:
    Terminal(){};

public:
    enum colors
    {
        FG_DEFAULT = 39,
        FG_BLACK = 30,
        FG_RED = 31,
        FG_GREEN = 32,
        FG_YELLOW = 33,
        FG_BLUE = 34,
        FG_MAGENTA = 35,
        FG_CYAN = 36,

        BG_DEFAULT = 49,
        BG_BLACK = 40,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_YELLOW = 43,
        BG_BLUE = 44,
        BG_MAGENTA = 45,
        BG_CYAN = 46
    };

    static int clearScreen();
    static int gotoXY(int x, int y);
    static int getScreanSize(int &rows, int &cols);
    static int setFgColor(colors fgColor);
    static int setBgColor(colors BgColor);
    static int setColors(colors bgColor, colors fgColor);
};

#endif
