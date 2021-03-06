#ifndef CONSOLE_UI
#define CONSOLE_UI

#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <signal.h>
#include <sys/time.h>
#include "../../my_term/includes/term.h"
#include "../../my_simple_computer/includes/mySimpleComputer.h"
#include "../../big_chars/inclides/alt_charset_mode.h"
#include "../../read_key/includes/my_key_board.h"
#include "../../CU_ALU/includes/cu.h"
#include "../../translators/includes/assebler_tr.h"
#include "../../translators/includes/basic_tr.h"

//std::string sourceFile = "src/assabler.sa"; // !!! delete

class CU;  //redefined because CU is a friend of s_computerUI
class ALU; //redefined because ALU is a friend of s_computerUI

class _UI_
{
protected:
    static _UI_ *instance;
    static int instValue;
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
    CU *sComputerCU;
    static const std::string SYST_PATH;
    char operation[8];
    char outputAnswer[8];
    size_t instrCounter;
    int accumulator;
    static bool delayPassed;
    bool termRun;

    friend class CU;
    friend class ALU;

    s_computerUI(std::string pathToFile = "");
    virtual ~s_computerUI(){};
    void reset();
    int initRAMfromObjFile(std::string fileName);

    static void alarmSwitchOff(int sig);
    static void signalHandler(int sig);
    void checkDelayPassed();
    void printMemory();
    void printFlagReg();
    void drawBoxes() const;
    void printNames() const;
    void printKeys() const;
    void printBigCell() const;
    void printConditions();
    void highlightCell(size_t position);
    void changeCell();
    void changeAccum();
    void changeInstrCntr();
    void timerIncr();
    std::string getPath() const;
    int termSave(std::string path = SYST_PATH) const;
    int termLoad(std::string path = SYST_PATH);
    void delegation(MyKeyBoard::Keys key); //performs functions according to the pressed key
    void drawUI() override;

public:
    static _UI_ *getInstance(std::string pathToFile = "");
    void execute() override;
};

void flushSTDIN();
#endif