#include "includes/console_ui.h"

inline void flushSTDIN()
{
    std::cin.clear();
    while (std::cin.get() != '\n')
        ;
}

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

void _UI_::drawUI(){};

void _UI_::execute(){};

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

const std::string s_computerUI::SYST_PATH = "config/syst";

s_computerUI::s_computerUI() : _UI_::_UI_()
{
    computer = SimpleComputer::getInstance();
    accumulator = instrCounter = 0;
    if (!termLoad())
    {
        Terminal::setFgColor(Terminal::FG_RED);
        std::cout << "Cannot loading last terminal states. Terminal will be reset\n";
        Terminal::setFgColor(Terminal::FG_DEFAULT);
        std::cout << "Press any key!\n";
        MyKeyBoard::switchToRaw();
        getchar();

        computer->init();
    }
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

void s_computerUI::printBigCell() const
{
    int offset;

    for (size_t i = 0; i < strlen(operation); ++i)
    {
        offset = 8 * i + 2;
        switch (operation[i])
        {
        case '0':
            AltTermMode::printBigChar(AltTermMode::bigChCode[0], 14, offset);
            break;
        case '1':
            AltTermMode::printBigChar(AltTermMode::bigChCode[1], 14, offset);
            break;
        case '2':
            AltTermMode::printBigChar(AltTermMode::bigChCode[2], 14, offset);
            break;
        case '3':
            AltTermMode::printBigChar(AltTermMode::bigChCode[3], 14, offset);
            break;
        case '4':
            AltTermMode::printBigChar(AltTermMode::bigChCode[4], 14, offset);
            break;
        case '5':
            AltTermMode::printBigChar(AltTermMode::bigChCode[5], 14, offset);
            break;
        case '6':
            AltTermMode::printBigChar(AltTermMode::bigChCode[6], 14, offset);
            break;
        case '7':
            AltTermMode::printBigChar(AltTermMode::bigChCode[7], 14, offset);
            break;
        case '8':
            AltTermMode::printBigChar(AltTermMode::bigChCode[8], 14, offset);
            break;
        case '9':
            AltTermMode::printBigChar(AltTermMode::bigChCode[9], 14, offset);
            break;
        case 'a':
            AltTermMode::printBigChar(AltTermMode::bigChCode[10], 14, offset);
            break;
        case 'b':
            AltTermMode::printBigChar(AltTermMode::bigChCode[11], 14, offset);
            break;
        case 'c':
            AltTermMode::printBigChar(AltTermMode::bigChCode[12], 14, offset);
            break;
        case 'd':
            AltTermMode::printBigChar(AltTermMode::bigChCode[13], 14, offset);
            break;
        case 'e':
            AltTermMode::printBigChar(AltTermMode::bigChCode[14], 14, offset);
            break;
        case 'f':
            AltTermMode::printBigChar(AltTermMode::bigChCode[15], 14, offset);
            break;
        case '+':
            AltTermMode::printBigChar(AltTermMode::bigChCode[16], 14, offset);
            break;
        case ' ':
            AltTermMode::printBigChar(AltTermMode::bigChCode[17], 14, offset);
            break;
        case ':':
            AltTermMode::printBigChar(AltTermMode::bigChCode[18], 14, offset);
            break;
        }
    }
};

void s_computerUI::highlightCell(size_t position)
{
    int operand, command, value;

    Terminal::gotoXY(position / 10 + 2, 7 * (position % 10) + 2);
    computer->memoryGet(position, value);
    if (computer->commandDecode(value, command, operand))
        sprintf(operation, "+%02x:%02x", command, operand);
    else
        sprintf(operation, "  %04x", value);

    write(1, operation, strlen(operation));
};

void s_computerUI::printMemory()
{
    for (size_t i = 0; i < 10; ++i)
    {
        for (size_t j = 0; j < 10; ++j)
        {
            highlightCell(10 * i + j);
        }
    }
}

void s_computerUI::drawBoxes() const
{
    AltTermMode::printBox(1, 1, 71, 12);   //memory box
    AltTermMode::printBox(1, 72, 30, 3);   //accumulator box
    AltTermMode::printBox(4, 72, 30, 3);   //instr. counter box
    AltTermMode::printBox(7, 72, 30, 3);   //operation box
    AltTermMode::printBox(10, 72, 30, 3);  //flags box
    AltTermMode::printBox(13, 1, 50, 10);  //big chars box
    AltTermMode::printBox(13, 51, 50, 10); //key box

    printNames();
    printKeys();
}

void s_computerUI::printNames() const
{
    Terminal::gotoXY(1, 32);
    write(1, " Memory ", 9);
    Terminal::gotoXY(1, 80);
    write(1, " Accumulator ", 14);
    Terminal::gotoXY(4, 76);
    write(1, " Instruction counter ", 22);
    Terminal::gotoXY(7, 81);
    write(1, " Operation ", 12);
    Terminal::gotoXY(10, 83);
    write(1, " Flags ", 8);
    Terminal::gotoXY(13, 73);
    write(1, " Keys ", 7);
}

void s_computerUI::printKeys() const
{
    char *strings[9] = {
        "l - load",
        "s - save",
        "r - run",
        "t - step",
        "i - reset",
        "f5 - accumulator",
        "f6 - instruction counter",
        "q - exit",
        "enter - select to change"};

    for (int i = 0; i < 8; ++i)
    {
        Terminal::gotoXY(14 + i, 53);
        write(1, strings[i], strlen(strings[i]));
    }

    Terminal::gotoXY(14, 74);
    write(1, strings[8], strlen(strings[8]));
}

void s_computerUI::printConditions()
{
    char buf[8];
    short len;

    Terminal::setColors(fg, bg); //print memmory
    printMemory();

    Terminal::setColors(Terminal::BG_CYAN, Terminal::FG_DEFAULT); //print cell by current possition
    highlightCell(instrCounter);
    printBigCell();

    Terminal::setColors(Terminal::FG_DEFAULT, Terminal::BG_DEFAULT);
    sprintf(buf, "%ld", instrCounter); //print instruction counter
    len = strlen(buf);
    Terminal::gotoXY(5, 87 - len);
    write(1, buf, len);

    sprintf(buf, "+%04ld", accumulator); //print accumulator
    len = strlen(buf);
    Terminal::gotoXY(2, 89 - len);
    write(1, buf, len);

    len = strlen(operation);
    Terminal::gotoXY(8, 89 - len); //print operation
    write(1, operation, len);
};

int s_computerUI::termSave(std::string path) const
{
    return computer->memorySave(path);
};

int s_computerUI::termLoad(std::string path)
{
    return computer->memoryLoad(path);
};

void s_computerUI::changeCell()
{
    int command, operand, value;

    MyKeyBoard::switchToCanon();
    std::cout << "Command: ";
    std::cin >> std::hex >> command;
    std::cout << "Operand: ";
    std::cin >> std::hex >> operand;
    if (!computer->commandEncode(command, operand, value))
    {
        Terminal::setFgColor(Terminal::FG_RED);
        std::cout << "Wrong command or operand\n";
        Terminal::setFgColor(Terminal::FG_DEFAULT);
        std::cout << "Press any key!\n";
        MyKeyBoard::switchToRaw();
        flushSTDIN();
        getchar();
    }
    else
    {
        computer->memorySet(instrCounter, value);
        MyKeyBoard::switchToRaw();
    }
};

void s_computerUI::changeAccum()
{
    MyKeyBoard::switchToCanon();

    std::string buf;
    std::string digit;

    std::cout << "Accumulator << ";
    std::getline(std::cin, buf);
    std::copy_if(buf.cbegin(), buf.cend(), std::back_inserter(digit), ::isdigit);

    if (!digit.empty())
    {
        digit.resize(4);
        accumulator = abs(std::stoi(digit));
    }

    MyKeyBoard::switchToRaw();
};

void s_computerUI::changeInstrCntr()
{
    MyKeyBoard::switchToCanon();

    std::string buf;
    std::string digit;

    std::cout << "Instruction counter << ";
    std::getline(std::cin, buf);
    std::copy_if(buf.cbegin(), buf.cend(), std::back_inserter(digit), ::isdigit);

    if (!digit.empty())
    {
        digit.resize(2);
        instrCounter = abs(std::stoi(digit));
    }

    MyKeyBoard::switchToRaw();
};

std::string s_computerUI::getPath() const
{
    std::string path;
    std::cout << "Write path to file: ";
    std::cin >> path;
    path.insert(0, "config/");

    return path;
}

void s_computerUI::delegation(MyKeyBoard::Keys key)
{
    switch (key)
    {
    case MyKeyBoard::up_key:
        (instrCounter / 10 == 0) ? instrCounter += 90 : instrCounter -= 10;
        break;
    case MyKeyBoard::left_key:
        (instrCounter % 10 == 0) ? instrCounter += 9 : --instrCounter;
        break;
    case MyKeyBoard::right_key:
        (instrCounter % 10 == 9) ? instrCounter -= 9 : ++instrCounter;
        break;
    case MyKeyBoard::down_key:
        (instrCounter / 10 == 9) ? instrCounter -= 90 : instrCounter += 10;
        break;
    case MyKeyBoard::enter_key:
        changeCell();
        break;
    case MyKeyBoard::f5_key:
        changeAccum();
        break;
    case MyKeyBoard::f6_key:
        changeInstrCntr();
        break;
    case MyKeyBoard::t_key:
        //
        break;
    case MyKeyBoard::i_key:
        //
        break;
    case MyKeyBoard::r_key:
        //
        break;
    case MyKeyBoard::l_key:
        MyKeyBoard::switchToCanon();
        if (!termLoad(getPath()))
        {
            std::cout << "Press any key!\n";
            flushSTDIN();
            MyKeyBoard::switchToRaw();
            getchar();
        }
        else
            MyKeyBoard::switchToRaw();
        break;
    case MyKeyBoard::s_key:
        MyKeyBoard::switchToCanon();
        if (!termSave(getPath()))
        {
            std::cout << "\nPress any key!";
            flushSTDIN();
            MyKeyBoard::switchToRaw();
            getchar();
        }
        else
            MyKeyBoard::switchToRaw();
        break;
    case MyKeyBoard::q_key:
        termSave();
        break;
    default:
        break;
    }
};

void s_computerUI::drawUI()
{
    Terminal::clearScreen();
    drawBoxes();
    printConditions();
    Terminal::gotoXY(23, 0); // delete
}

void s_computerUI::execute()
{
    MyKeyBoard::Keys key;

    MyKeyBoard::switchToRaw();

    while (key != MyKeyBoard::q_key)
    {
        drawUI();
        MyKeyBoard::readKey(key);
        delegation(key);
    }

    MyKeyBoard::switchToCanon();
}