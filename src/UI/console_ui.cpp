#include "includes/console_ui.h"

void flushSTDIN()
{
    std::cout << std::dec;
    std::cin.sync();
    if (std::cin.rdbuf()->in_avail() == 0)
        return;

    char ch = getchar();
    while (ch != '\n' && ch != '\0')
    {
        ch = getchar();
    }
}

//________UI___________
_UI_ *_UI_::instance = 0;
int _UI_::instValue = 0;
bool s_computerUI::delayPassed = false;

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
    ++instValue;
    if (instance == nullptr)
    {
        instance = new _UI_();
    }

    return instance;
};

void _UI_::free()
{
    if (instValue >= 1)
    {
        if (instValue == 1)
            delete instance;
        --instValue;
    }
};

//___________S_COMPUTERUI__________

const std::string s_computerUI::SYST_PATH = "config/syst";

s_computerUI::s_computerUI() : _UI_::_UI_()
{
    computer = SimpleComputer::getInstance();
    reset();

    MyKeyBoard::switchToRaw();
    if (AssemblerTR::translate("config/assembler.sa") == -1)
    {
        std::cout << "Press any key to continue\n";
        getchar();

        computer->memoryLoad(SYST_PATH);
    }
    else
    {
        if (initRAMfromObjFile("config/assembler.sa") == -1)
        {
            std::cout << "Press any key to continue\n";
            getchar();

            computer->memoryLoad(SYST_PATH);
        }
    }
    sComputerCU = new CU();
};

void s_computerUI::reset()
{
    termRun = false;
    accumulator = instrCounter = 0;
    sprintf(operation, "%04d", 0);
    computer->init();
}

_UI_ *s_computerUI::getInstance()
{
    ++instValue;
    if (instance == nullptr)
    {
        instance = new s_computerUI();
    }

    return instance;
};

int s_computerUI::initRAMfromObjFile(std::string fileName)
{
    std::size_t position = fileName.find_last_of(".");
    std::string objFileName = fileName.substr(0, position) + ".o";
    std::fstream file(objFileName, std::ios::in | std::ios::binary);

    if (!file.is_open())

    {
        std::cout << "Cannot open file \"" << objFileName << "\"\n";
        file.close();
        return -1;
    }

    std::array<Node *, 100> arrayList;
    int value;
    int i = 0;

    while (!file.eof())
    {
        arrayList[i] = new Node;
        file.read((char *)(arrayList[i]), 12);
        ++i;
    }
    --i;

    for (int j = 0; j < i; ++j)
    {
        if (arrayList[j]->comand != 0x01)
            computer->commandEncode(arrayList[j]->comand, arrayList[j]->operand, value);
        else
        {

            std::string number;
            std::stringstream toOct;
            toOct << std::oct << abs(arrayList[j]->operand);
            toOct >> value;
            if (arrayList[j]->operand < 0)
                value *= (-1);
        }
        computer->memorySet(arrayList[j]->cellKey, value);
    }
}

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
    {
        sprintf(operation, "+%02x:%02d", command, operand);
        computer->regSet(WRONG_COMAND, 0);
    }
    else
    {
        if (value < 0)
            sprintf(operation, " -%04x", abs(value));
        else
            sprintf(operation, "  %04x", value);
        computer->regSet(WRONG_COMAND, 1);
    }
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

void s_computerUI::printFlagReg()
{
    static const int begPos = 81;
    bool value;

    Terminal::gotoXY(11, begPos);
    computer->regGet(WRONG_COMAND, value);
    write(1, ((value) ? "E" : "_"), 2);

    Terminal::gotoXY(11, begPos + 2);
    computer->regGet(IGNR_CLOCK_PULSES, value);
    write(1, ((value) ? "T" : "_"), 2);

    Terminal::gotoXY(11, begPos + 4);
    computer->regGet(MEMORY_OVERRUN, value);
    write(1, ((value) ? "M" : "_"), 2);

    Terminal::gotoXY(11, begPos + 6);
    computer->regGet(OPERATION_OVERFLOW, value);
    write(1, ((value) ? "P" : "_"), 2);

    Terminal::gotoXY(11, begPos + 8);
    computer->regGet(DEVISION_ZERO, value);
    write(1, ((value) ? "0" : "_"), 2);
};

void s_computerUI::drawBoxes() const
{
    AltTermMode::printBox(1, 1, 71, 12);   //memory box
    AltTermMode::printBox(1, 72, 30, 3);   //accumulator box
    AltTermMode::printBox(4, 72, 30, 3);   //instr. counter box
    AltTermMode::printBox(7, 72, 30, 3);   //operation box
    AltTermMode::printBox(10, 72, 30, 3);  //flags box
    AltTermMode::printBox(13, 1, 50, 10);  //big chars box
    AltTermMode::printBox(13, 51, 51, 10); //key box
    AltTermMode::printBox(23, 84, 18, 3);  //show box

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
    Terminal::gotoXY(23, 89);
    write(1, " Output ", 9);
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
    printFlagReg();

    sprintf(buf, "%ld", instrCounter); //print instruction counter
    len = strlen(buf);
    Terminal::gotoXY(5, 87 - len);
    write(1, buf, len);

    if (accumulator < 0)
        sprintf(buf, "-%04x", abs(accumulator)); //print accumulator
    else
        sprintf(buf, "%04x", accumulator);
    len = strlen(buf);
    Terminal::gotoXY(2, 89 - len);
    write(1, buf, len);

    len = strlen(operation);
    Terminal::gotoXY(8, 89 - len); //print operation
    write(1, operation, len);

    Terminal::setBgColor(Terminal::BG_YELLOW);
    len = strlen(outputAnswer);
    Terminal::gotoXY(24, 93 - (len / 2));
    write(1, outputAnswer, len);
    Terminal::setBgColor(Terminal::BG_DEFAULT);
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
    std::cout << "Please press ENTER to continue\n";
    flushSTDIN();

    int command, operand, value;
    int offset = 0;
    int choice;
    MyKeyBoard::switchToCanon();

    while (true)
    {
        std::cout << "to enter in the form: (\"command\", \"operand\")"
                  << " or enter just a number, press 1 or 2, respectively\n";

        std::cin >> choice;
        if (choice == 1)
        {
            while (true)
            {
                std::cout << "Command: ";
                std::cin >> std::hex >> command;
                std::cout << "Operand: ";
                std::cin >> std::dec >> operand;
                if (!computer->commandEncode(command, operand, value))
                {
                    offset += 5;
                    computer->regSet(WRONG_COMAND, 1);
                    printFlagReg();
                    Terminal::gotoXY(25 + offset, 0);
                    Terminal::setFgColor(Terminal::FG_RED);
                    std::cout << "Wrong command or operand\n";
                    Terminal::setFgColor(Terminal::FG_DEFAULT);
                }
                else
                {
                    computer->regSet(WRONG_COMAND, 0);
                    computer->memorySet(instrCounter, value);
                    MyKeyBoard::switchToRaw();
                    flushSTDIN();
                    return;
                }
            }
        }
        else if (choice == 2)
        {
            std::cout << "Value: ";
            std::cin >> std::hex >> value;
            computer->memorySet(instrCounter, value);
            MyKeyBoard::switchToRaw();
            flushSTDIN();
            return;
        }
        else
        {
            Terminal::setFgColor(Terminal::FG_RED);
            std::cout << "Wrong input\n";
            Terminal::setFgColor(Terminal::FG_DEFAULT);
            //flushSTDIN();
        }
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
        accumulator = std::stoi(digit);
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

void s_computerUI::checkDelayPassed()
{
    if (delayPassed)
    {
        if (sComputerCU->execute() == -1)
        {
            timerIncr();
            computer->regSet(WRONG_COMAND, 1);
            computer->regSet(IGNR_CLOCK_PULSES, 1);
            Terminal::gotoXY(23, 0);
            Terminal::setFgColor(Terminal::FG_RED);
            std::cout << "Wrong command or operand\nPress ENTER to continue" << std::endl;
            Terminal::setFgColor(Terminal::FG_DEFAULT);
            flushSTDIN();
            getchar();
        }
        delayPassed = false;
    }
}

void s_computerUI::alarmSwitchOff(int sig)
{
    alarm(0);
}

void s_computerUI::signalHandler(int sig)
{
    delayPassed = true;
}

void s_computerUI::timerIncr()
{
    static struct itimerval nval, oval;

    if (termRun)
    {
        termRun = 0;
        signal(SIGUSR1, alarmSwitchOff);
        raise(SIGUSR1);
        delayPassed = false;
        computer->regSet(IGNR_CLOCK_PULSES, 1);
    }
    else
    {
        computer->regSet(IGNR_CLOCK_PULSES, 0);
        signal(SIGALRM, signalHandler);
        nval.it_interval.tv_sec = 1;
        nval.it_interval.tv_usec = 0;
        nval.it_value.tv_sec = 1;
        nval.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &nval, &oval);
        termRun = 1;
    }
};

std::string s_computerUI::getPath() const
{
    std::string path;
    std::cout << "Write path to file: ";
    std::cin >> path;
    path.insert(0, "config/");

    return path;
};

void s_computerUI::delegation(MyKeyBoard::Keys key)
{
    flushSTDIN();
    if (termRun && key != MyKeyBoard::Keys::r_key)
        return;

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
        if (sComputerCU->execute() == -1)
        {
            Terminal::setFgColor(Terminal::FG_RED);
            std::cout << "Wrong command or operand\nPress any key\n";
            Terminal::setFgColor(Terminal::FG_DEFAULT);
            getchar();
        }
        break;
    case MyKeyBoard::i_key:
        reset();
        break;
    case MyKeyBoard::r_key:
        timerIncr();
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
    Terminal::gotoXY(25, 0);
}

void s_computerUI::execute()
{
    sComputerCU->set(this);
    MyKeyBoard::Keys key;

    MyKeyBoard::switchToRaw();
    computer->regSet(IGNR_CLOCK_PULSES, 1);

    while (key != MyKeyBoard::q_key)
    {
        key = MyKeyBoard::err_key;
        drawUI();
        MyKeyBoard::readKey(key, termRun);
        delegation(key);
        checkDelayPassed();
    }

    MyKeyBoard::switchToCanon();
}