#ifndef ALU_H
#define ALU_H

#include "../../UI/includes/console_ui.h"

class s_computerUI; //redefined because ALU is a friend of s_computerUI

class ALU
{
public:
    static void execute(const int &comand, const int &operand, s_computerUI *ui);
};
#endif