#include "includes/my_key_board.h"

MyKeyBoard::MyKeyBoard(){};
MyKeyBoard::~MyKeyBoard(){};

int MyKeyBoard::switchToRaw(bool vtime , bool vmin , bool echo , bool sigint)
{
    termios rawMode;
    tcgetattr(1, &rawMode);

    rawMode.c_lflag &= ~ICANON;
    (echo) ? rawMode.c_lflag |= ECHO : rawMode.c_lflag &= ~ECHO;
    (sigint) ? rawMode.c_lflag |= ISIG : rawMode.c_lflag &= ~ISIG;
    rawMode.c_cc[VTIME] = vtime;
    rawMode.c_cc[VMIN] = vmin;

    if (tcsetattr(1, TCSADRAIN, &rawMode) != 0)
        return -1;

    return 0;
}

int MyKeyBoard::switchToCanon()
{
    static termios canone;
    canone.c_lflag |= CANON_FLAGS;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &canone);
}

int MyKeyBoard::readKey(Keys &key)
{
    char buf[8];
    if (read(1, buf, 7) > 0)
    {
        if (strcmp(buf, "\033[A") == 0)
            key = Keys::up_key;
        else if (strcmp(buf, "\033[B") == 0)
            key = Keys::down_key;
        else if (strcmp(buf, "\033[C") == 0)
            key = Keys::left_key;
        else if (strcmp(buf, "\033[D") == 0)
            key = Keys::right_key;
        else if (strcmp(buf, "\n") == 0)
            key = Keys::enter_key;
        else if (strcmp(buf, "\033[15~") == 0)
            key = Keys::f5_key;
        else if (strcmp(buf, "\033[17~") == 0)
            key = Keys::f6_key;
        else if (strcmp(buf, "t") == 0)
            key = Keys::t_key;
        else if (strcmp(buf, "i") == 0)
            key = Keys::i_key;
        else if (strcmp(buf, "r") == 0)
            key = Keys::r_key;
        else if (strcmp(buf, "l") == 0)
            key = Keys::l_key;
        else if (strcmp(buf, "s") == 0)
            key = Keys::s_key;
        else if (strcmp(buf, "q") == 0)
            key = Keys::q_key;
    }
    else
        return -1;

    return 0;
}

int MyKeyBoard::termSave() {}
int MyKeyBoard::termRead() {}