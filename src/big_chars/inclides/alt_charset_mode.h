#ifndef ALT_CHARSET_MODE_H
#define ALT_CHARSET_MODE_H

#define TTL_SMBLS 19
#define PARTS 2

#include "../../my_term/includes/term.h"

class AltTermMode
{
protected:
    static int bigChCode[TTL_SMBLS][PARTS];

    AltTermMode();
    virtual ~AltTermMode();

public:
    static int printA(char *str);
    static int printBox(int x1, int y1, int x2, int y2);
    static int printBigChar(int *codeBigCh, int x, int y,
                            Terminal::colors bgColor, Terminal::colors fgColor);
    static int getBigCharPos(int *codeBigCh, int x, int y, int &value);
    static int bigCgarWrite(int fd, int *codeBigCh, int count);
    static int bigCharRead(int fd, int *codeBigCh, int need_count, int &count);
};

#endif