#include "inclides/alt_charset_mode.h"

const int AltTermMode::bigChCode[19][2] = {{0x4242423C, 0x3C424242},  // 0
                                           {0x48506040, 0x40404040},  // 1
                                           {0x2042423C, 0x7E020418},  // 2
                                           {0x6064647C, 0x7C646030},  // 3
                                           {0x66666666, 0x6060607E},  // 4
                                           {0x7E02027E, 0x7E626060},  // 5
                                           {0x3E02423C, 0x3C424242},  // 6
                                           {0x2040407E, 0x10101010},  // 7
                                           {0x3C42423C, 0x3C424242},  // 8
                                           {0x7E42427E, 0xC183060},   // 9
                                           {0x66663C18, 0x66667E7E},  // A
                                           {0x3E66663E, 0x3E666666},  // B
                                           {0x0202423C, 0x3C420202},  // C
                                           {0x4444443E, 0x3E444444},  // D
                                           {0x3E02027E, 0x7E020202},  // E
                                           {0x1E02027E, 0x02020202},  // F
                                           {0x7E181800, 0x0018187E},  // +
                                           {0x00000000, 0x00000000},  //
                                           {0x00181800, 0x00181800}}; // :
AltTermMode::AltTermMode(){};
AltTermMode::~AltTermMode(){};

int AltTermMode::printString(char *str, int count)
{
    write(1, "\E(0", 3);

    int len = strlen(str);
    if (len == 1)
    {
        for (int i = 0; i < count; ++i)
            write(1, str, 1);
    }
    else
    {
        if (write(1, str, len) < len)
            return 1;
    }
    write(1, "\E(B", 3);
    return 0;
}

int AltTermMode::printBox(int x, int y, int width, int height)
{
    int x2 = x + height - 1;
    int y2 = y + width - 1;

    //Top----
    Terminal::gotoXY(x, y);
    printString(BC_TOP_L);
    printString(BC_HORIZ, width - 2);
    printString(BC_TOP_R);

    //Bottom----
    Terminal::gotoXY(x2, y);
    printString(BC_BOT_L);
    printString(BC_HORIZ, width - 2);
    printString(BC_BOT_R);

    //Left
    for (int i = x + 1; i < x2; ++i)
    {
        Terminal::gotoXY(i, y);
        printString(BC_VERT);
    }

    //Right
    for (int i = x + 1; i < x2; ++i)
    {
        Terminal::gotoXY(i, y2);
        printString(BC_VERT);
    }
}

int AltTermMode::getBigCharPos(const int *codeBigCh, int x, int y, int &value)
{
    if (x < 0 || x > 7 || y < 0 || y > 7)
        return 1;

    unsigned short part = (x < 4) ? 0 : 1;
    value = codeBigCh[part] & (0x1 << (8 * (x % 4) + y));
}

int AltTermMode::printBigChar(const int *codeBigCh, int x, int y,
                              Terminal::colors bgColor, Terminal::colors fgColor)
{
    int value;
    Terminal::setColors(bgColor, fgColor);

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            getBigCharPos(codeBigCh, i, j, value);
            Terminal::gotoXY(x + i, y + j);
            if (value)
                printString("a");
            else
                write(1, " ", sizeof(char));
        }
    }

    Terminal::setColors(Terminal::BG_DEFAULT, Terminal::FG_DEFAULT);
}
int AltTermMode::bigCgarWrite(int fd, const int *codeBigCh, int count){};
int AltTermMode::bigCharRead(int fd, const int *codeBigCh, int need_count, int &count){};