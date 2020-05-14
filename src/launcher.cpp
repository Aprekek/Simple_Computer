#include "UI/includes/console_ui.h"

int main(int argc, char **argv)
{
    // std::cout << argc << std::endl;
    // if (argc > 2)
    // {
    //     std::cout << "Programm pust be run lile: ./launcher <path_to_basic_file>\n";
    //     return 0;
    // }
    // std::cout << argv[argc] << std::endl;
    // std::string pathToFile(argv[argc]);
    // std::cout << pathToFile << std::endl;
    std::string pathToFile;
    std::cout << "write path to basic file: ";
    std::cin >> pathToFile;

    _UI_ *ins = s_computerUI::getInstance(pathToFile);

    ins->initUI(Terminal::BG_BLACK, Terminal::FG_BLUE);
    ins->execute();
    Terminal::setColors(Terminal::BG_DEFAULT, Terminal::FG_DEFAULT);
    return 0;
}