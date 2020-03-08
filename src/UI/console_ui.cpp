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

void s_computerUI::printMemory() const
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

void s_computerUI::drawBoxes() const
{
    AltTermMode::printBox(1, 1, 70, 12);   //memory box
    AltTermMode::printBox(1, 71, 30, 3);   //accumulator box
    AltTermMode::printBox(4, 71, 30, 3);   //instr. counter box
    AltTermMode::printBox(7, 71, 30, 3);   //operation box
    AltTermMode::printBox(10, 71, 30, 3);  //flags box
    AltTermMode::printBox(13, 1, 50, 10);  //big chars box
    AltTermMode::printBox(13, 51, 50, 10); //key box

    printNames();
}

void s_computerUI::printNames() const
{
    Terminal::gotoXY(1, 31);
    write(1, " Memory ", 9);
    Terminal::gotoXY(1, 79);
    write(1, " Accumulator ", 14);
    Terminal::gotoXY(4, 75);
    write(1, " Instruction counter ", 22);
    Terminal::gotoXY(7, 80);
    write(1, " Operation ", 12);
    Terminal::gotoXY(10, 82);
    write(1, " Flags ", 8);
    Terminal::gotoXY(13, 72);
    write(1, " Keys ", 7);
}

void s_computerUI::drawUI() const
{
    Terminal::clearScreen();
    drawBoxes();
    printMemory();
    Terminal::gotoXY(40, 0); // delete
}