#include "includes/my_key_board.h"

MyKeyBoard::MyKeyBoard(){};
MyKeyBoard::~MyKeyBoard(){};

int MyKeyBoard::switchToRaw(bool vtime, bool vmin, bool echo, bool isig)
{
    termios rawMode;
    tcgetattr(1, &rawMode);

    rawMode.c_lflag &= ~ICANON;
    (echo) ? rawMode.c_lflag |= ECHO : rawMode.c_lflag &= ~ECHO;
    (isig) ? rawMode.c_lflag |= ISIG : rawMode.c_lflag &= ~ISIG;
    rawMode.c_cc[VTIME] = vtime;
    rawMode.c_cc[VMIN] = vmin;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawMode) != 0)
        return -1;

    return 0;
}

int MyKeyBoard::switchToCanon()
{
    termios canone;
    tcgetattr(1, &canone);
    canone.c_lflag |= CANON_FLAGS;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &canone) != 0)
        return -1;

    return 0;
}

int MyKeyBoard::readKey(Keys &key)
{
    char buf[8];
    if (read(1, buf, 7) > 0)
    {
        if (strncmp(buf, "\033[A", 3) == 0)
            key = Keys::up_key;
        else if (strncmp(buf, "\033[B", 3) == 0)
            key = Keys::down_key;
        else if (strncmp(buf, "\033[C", 3) == 0)
            key = Keys::right_key;
        else if (strncmp(buf, "\033[D", 3) == 0)
            key = Keys::left_key;
        else if (strncmp(buf, "\n", 1) == 0)
            key = Keys::enter_key;
        else if (strncmp(buf, "\033[15~", 5) == 0)
            key = Keys::f5_key;
        else if (strncmp(buf, "\033[17~", 5) == 0)
            key = Keys::f6_key;
        else if (strncmp(buf, "t", 1) == 0)
            key = Keys::t_key;
        else if (strncmp(buf, "i", 1) == 0)
            key = Keys::i_key;
        else if (strncmp(buf, "r", 1) == 0)
            key = Keys::r_key;
        else if (strncmp(buf, "l", 1) == 0)
            key = Keys::l_key;
        else if (strncmp(buf, "s", 1) == 0)
            key = Keys::s_key;
        else if (strncmp(buf, "q", 1) == 0)
            key = Keys::q_key;
        else
            key = Keys::err_key;
    }
    else
        return -1;

    return 0;
}