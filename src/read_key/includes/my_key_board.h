#ifndef MY_KEY_BOARD_H
#define MY_KEY_BOARD_H

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <cstring>

#define CANON_FLAGS (ECHO | ICANON | ISIG)

class MyKeyBoard
{
private:
    MyKeyBoard();
    virtual ~MyKeyBoard();

public:
    enum Keys
    {
        up_key,
        left_key,
        down_key,
        right_key,
        enter_key,
        f5_key,
        f6_key,
        t_key,
        i_key,
        r_key,
        l_key,
        s_key,
        q_key,
        err_key
    };

    static int switchToRaw(bool vtime = 0, bool vmin = 1, bool echo = 0, bool isig = 1);
    static int switchToCanon();
    static int readKey(Keys &key, bool ifRun);
};

#endif