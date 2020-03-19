#ifndef CONSOLE_UI
#define CONSOLE_UI

#include <iostream>
#include "../../my_term/includes/term.h"
#include "../../my_simple_computer/includes/mySimpleComputer.h"
#include "../../big_chars/inclides/alt_charset_mode.h"

class _UI_
{
protected:
    static _UI_ *instance;
    static int value;
    Terminal::colors bg;
    Terminal::colors fg;

    _UI_();
    virtual ~_UI_();

public:
    virtual int initUI(Terminal::colors bgColor, Terminal::colors fgColor);
    virtual void drawUI() const;

    static _UI_ *getInstance();
    static void free();
};

class s_computerUI : public _UI_
{
protected:
    SimpleComputer *computer;

    s_computerUI();
    virtual ~s_computerUI();

    void printMemory() const;
    void drawBoxes() const;
    void printNames() const;
    void printKeys() const;

public:
    static _UI_ *getInstance();
    void drawUI() const override;
};

#endif