#include "inclides/alt_charset_mode.h"

int AltTermMode::bigChCode[19][2] = {{0x4242423C, 0x3C424242},  // 0
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

int AltTermMode::printA(char *str)
{
    write(1, "\E(0", 3);

    int len = strlen(str);
    if (write(1, str, len) < len)
        return 1;

    write(1, "\E(B", 3);
    return 0;
}

int AltTermMode::printBox(int x1, int y1, int x2, int y2){};
int AltTermMode::printBigChar(int *codeBigCh, int x, int y,
                              Terminal::colors bgColor, Terminal::colors fgColor){};
int AltTermMode::getBigCharPos(int *codeBigCh, int x, int y, int &value){};
int AltTermMode::bigCgarWrite(int fd, int *codeBigCh, int count){};
int AltTermMode::bigCharRead(int fd, int *codeBigCh, int need_count, int &count){};