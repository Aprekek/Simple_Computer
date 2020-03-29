#ifndef CONSOLE_UI
#define CONSOLE_UI

#include <iostream>
#include <fstream>
#include <algorithm>
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

    virtual void drawUI();

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
    size_t accumulator;
    static const std::string SYST_PATH;
    char operation[8];

    s_computerUI();
    virtual ~s_computerUI(){};

    void printMemory();
    void drawBoxes() const;
    void printNames() const;
    void printKeys() const;
    void printBigCell() const;
    void printConditions();
    void highlightCell(size_t position);

    void changeCell();
    void changeAccum();
    void changeInstrCntr();
    inline void step();
    std::string getPath() const;
    int termSave(std::string path = SYST_PATH) const;
    int termLoad(std::string path = SYST_PATH);
    void delegation(MyKeyBoard::Keys key); //performs functions according to the pressed key
    void drawUI() override;

public:
    static _UI_ *getInstance();
    void execute() override;
};

#endif