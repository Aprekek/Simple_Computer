#ifndef ALT_CHARSET_MODE_H
#define ALT_CHARSET_MODE_H

#define TTL_SMBLS 19
#define PARTS 2

#include "../../my_term/includes/term.h"

#define BC_TOP_L "l"
#define BC_BOT_L "m"
#define BC_TOP_R "k"
#define BC_BOT_R "j"
#define BC_HORIZ "q"
#define BC_VERT "x"

class AltTermMode
{
protected:
    AltTermMode();
    virtual ~AltTermMode();

public:
    static const int bigChCode[TTL_SMBLS][PARTS];
    static int printString(char *str, int count = 1);
    static int printChar(char ch, size_t count);
    static void printBox(int x, int y, int width, int height);
    static void printBigChar(const int *codeBigCh, int x, int y,
                             Terminal::colors bgColor = Terminal::BG_DEFAULT,
                             Terminal::colors fgColor = Terminal::FG_DEFAULT);
    static int getBigCharPos(const int *codeBigCh, int x, int y, bool &value);
    //static int bigCharWrite(int fd, const int *codeBigCh, int count); ???
    //static int bigCharRead(int fd, const int *codeBigCh, int need_count, int &count); ???
};

#endif