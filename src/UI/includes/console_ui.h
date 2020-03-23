#ifndef CONSOLE_UI
#define CONSOLE_UI

#include <iostream>
#include "../../my_term/includes/term.h"
#include "../../my_simple_computer/includes/mySimpleComputer.h"
#include "../../big_chars/inclides/alt_charset_mode.h"
#include "../../read_key/includes/my_key_board.h"

class _UI_
{
protected:
    static _UI_ *instance;
    static int value;
    Terminal::colors bg;
    Terminal::colors fg;

    _UI_();
    virtual ~_UI_();

    virtual void drawUI() const;

public:
    virtual int initUI(Terminal::colors bgColor, Terminal::colors fgColor);
    virtual void execute();

    static _UI_ *getInstance();
    static void free();
};

class s_computerUI : public _UI_
{
protected:
    SimpleComputer *computer;
    size_t instrCounter;

    s_computerUI();
    virtual ~s_computerUI() {}

    void printMemory() const;
    void drawBoxes() const;
    void printNames() const;
    void printKeys() const;
    void printBigMemory() const;
    void printConditions() const;

    void delegation(MyKeyBoard::Keys key); //performs functions according to the pressed key
    void drawUI() const override;

public:
    static _UI_ *getInstance();
    void execute() override;
};

#endif