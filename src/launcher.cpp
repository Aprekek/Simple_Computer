#include "UI/includes/console_ui.h"

int main()
{
    _UI_ *ins = s_computerUI::getInstance();

    Terminal::clearScreen();
    ins->initUI(Terminal::BG_BLACK, Terminal::FG_BLUE);
    ins->drawUI();
    ins->initUI(Terminal::BG_DEFAULT, Terminal::FG_DEFAULT);
    return 0;
}