#include "includes/term.h"

int Terminal::clearScreen()
{
    char buf[] = "\E[H\E[J";
    int length = strlen(buf);

    if (write(1, buf, length) < length)
        return -1;

    return 0;
}

int Terminal::gotoXY(int x, int y)
{
    char buf[20];
    sprintf(buf, "\E[%d;%dH", x, y);
    int length = strlen(buf);

    if (write(1, buf, length) < length)
        return -1;

    return 0;
}

int Terminal::getScreanSize(int &rows, int &cols)
{
    winsize ws;
    if (ioctl(1, TIOCGWINSZ, &ws) == 0)
    {
        rows = ws.ws_row;
        cols = ws.ws_col;
    }
    else
        return -1;
    return 0;
}

int Terminal::setFgColor(colors fgColor)
{
    char buf[20];
    sprintf(buf, "\E[%dm", fgColor);
    int length = strlen(buf);

    if (write(1, buf, length) < length)
        return -1;

    return 0;
}

int Terminal::setBgColor(colors BgColor)
{
    char buf[20];
    sprintf(buf, "\E[%dm", BgColor);
    int length = strlen(buf);

    if (write(1, buf, length) < length)
        return -1;

    return 0;
}
