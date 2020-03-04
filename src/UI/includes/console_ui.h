#ifndef CONSOLE_UI
#define CONSOLE_UI

#include <iostream>
#include "../../my_term/includes/term.h"
#include "../../my_simple_computer/includes/mySimpleComputer.h"

class _UI_
{
protected:
    static _UI_ *instance;
    static int value;

    _UI_();
    virtual ~_UI_();

public:
    virtual int initUI(Terminal::colors bgColor, Terminal::colors fgColor) const;
    virtual int drawUI() const;

    static _UI_ *getInstance();
    static void free();
};

class s_computerUI : public _UI_
{
protected:
    SimpleComputer *manager;

    s_computerUI();
    virtual ~s_computerUI() {}

public:
    static _UI_ *getInstance();
    int drawUI() const override;
};

#endif