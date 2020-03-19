#include "includes/my_key_board.h"

struct termios MyKeyBoard::canone;

MyKeyBoard::MyKeyBoard(){};
MyKeyBoard::~MyKeyBoard(){};

void MyKeyBoard::init(){
    //canone.c_lflag |= (ECHO | ICANON |)
};

int MyKeyBoard::switchRegime(bool regime, bool vtime, bool vmin, bool echo, bool sigint)
{
}

int MyKeyBoard::switchToAlt(){

};

int MyKeyBoard::switchToCanon(){
    // tcsetattr(STDIN_FILENO, TCSANOW, &canone);
};

int MyKeyBoard::readKey(Keys &key) {}
int MyKeyBoard::termSave() {}
int MyKeyBoard::termRead() {}