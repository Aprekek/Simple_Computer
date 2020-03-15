#ifndef MY_KEY_BOARD_H
#define MY_KEY_BOARD_H

#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstring>

class MyKeyBoard
{
private:
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

    static int readKey(Keys &key);
    static int termSave();
    static int termRead();
    static int switchRegime(int regime, int vtime, int vmin, int echo, int sigint);
};

#endif