#include "includes/console_ui.h"

//________UI___________
_UI_ *_UI_::instance = 0;
int _UI_::value = 0;

_UI_::_UI_(){};
_UI_::~_UI_(){};

int _UI_::initUI(Terminal::colors bgColor, Terminal::colors fgColor) const
{
    if (Terminal::setBgColor(bgColor) != 0 ||
        Terminal::setFgColor(fgColor) != 0)
        return -1;

    return 0;
};

int _UI_::drawUI() const { return 0; };

_UI_ *_UI_::getInstance()
{
    ++value;
    if (instance == nullptr)
    {
        instance = new _UI_();
    }

    return instance;
};

void _UI_::free()
{
    if (value >= 1)
    {
        if (value == 1)
            delete instance;
        --value;
    }
};

//___________S_COMPUTERUI__________
s_computerUI::s_computerUI() : _UI_::_UI_()
{
    manager = SimpleComputer::getInstance();
    manager->memInit();
    manager->regInit();
};

_UI_ *s_computerUI::getInstance()
{
    ++value;
    if (instance == nullptr)
    {
        instance = new s_computerUI();
    }

    return instance;
};

int s_computerUI::drawUI() const
{
    //???---
    //Terminal::clearScreen();
    //initUI(Terminal::BG_BLACK, Terminal::FG_BLUE);
    //---???
    int value;
    char buf[10];
    for (size_t i = 1; i <= 10; ++i)
    {
        for (size_t j = 0; j < 10; ++j)
        {
            Terminal::gotoXY(i, j * 7 + 1);
            manager->memmoryGet(10 * i + j, value);
            sprintf(buf, "+%04d", value);
            write(1, buf, strlen(buf));
        }
    }
    Terminal::gotoXY(11, 0);
    return 0;
}

int main()
{

    _UI_ *ins = s_computerUI::getInstance();
    //???---
    Terminal::clearScreen();
    ins->initUI(Terminal::BG_BLACK, Terminal::FG_BLUE);
    ins->drawUI();
    //---???
    /*for (size_t i = 0; i <= 2; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            Terminal::gotoXY(i + 10, j * 7);
            write(1, "fds", 3);
        }
    }*/
    return 0;
}