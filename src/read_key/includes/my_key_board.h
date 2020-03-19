#ifndef MY_KEY_BOARD_H
#define MY_KEY_BOARD_H

#include <iostream>
#include <unistd.h>
#include <termios.h>

class MyKeyBoard
{
private:
    static struct termios canone;

    MyKeyBoard();
    virtual ~MyKeyBoard();

public:
    enum Keys
    {
        l_key,
        s_key,
        t_key,
        r_key,
        i_key,
        f5_key,
        f6_key,
        up_key,
        left_key,
        down_key,
        right_key,
        enter_key
    };

    static int switchRegime(bool regime, bool vtime, bool vmin, bool echo, bool sigint);
    static int switchToAlt();
    static int switchToCanon();
    static void init();
    static int readKey(Keys &key);
    static int termSave();
    static int termRead();
};

#endif