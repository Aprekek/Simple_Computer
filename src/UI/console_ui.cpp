#include "includes/console_ui.h"

//________UI___________
_UI_ *_UI_::instance = 0;
int _UI_::value = 0;

_UI_::_UI_(){};
_UI_::~_UI_(){};

int _UI_::initUI(Terminal::colors bgColor, Terminal::colors fgColor)
{
    bg = bgColor;
    fg = fgColor;
    return 0;
};

void _UI_::drawUI() const {};

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
    computer = SimpleComputer::getInstance();
    computer->init();
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

void s_computerUI::drawMemory() const
{
    Terminal::setColors(fg, bg);

    int value;
    char buf[10];
    for (size_t i = 0; i < 10; ++i)
    {
        for (size_t j = 0; j < 10; ++j)
        {
            Terminal::gotoXY(i + 2, j * 7 + 2);
            computer->memoryGet(10 * i + j, value);
            sprintf(buf, "+%04d", value);
            write(1, buf, strlen(buf));
        }
    }

    Terminal::setColors(Terminal::FG_DEFAULT, Terminal::BG_DEFAULT);
}

void s_computerUI::drawUI() const
{
    Terminal::clearScreen();
    drawMemory();
    AltTermMode::printBox(1, 1, 70, 12);
    Terminal::gotoXY(13, 0); // delete
}