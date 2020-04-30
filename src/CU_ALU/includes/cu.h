#ifndef CU_H
#define CU_H

#include "../../UI/includes/console_ui.h"

class s_computerUI; //redefined because CU is a friend of s_computerUI

class CU
{
private:
    s_computerUI *ui;

public:
    CU();
    void set(s_computerUI *inst);
    int execute();
};
#endif