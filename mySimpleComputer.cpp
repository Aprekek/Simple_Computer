#include <iostream>
#include "mySimpleComputer.h"

SimpleComputer *SimpleComputer::instance = 0;

SimpleComputer::SimpleComputer()
{
    memmory = new int[MEM_SIZE];
    if (memmory != nullptr)
    {
        memInit();
        regInit();
    }
    else
    {
        std::cout << "Cannoy allocate memmory (SimpleComputer <memmory>)\n";
        exit(EXIT_FAILURE);
    }
}

SimpleComputer *SimpleComputer::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SimpleComputer();
    }
    return instance;
}

int SimpleComputer::memmorySet(const size_t &adress, const int &value)
{
    if (adress >= MEM_SIZE)
    {
        regSet(MEMORY_OVERRUN, 1);
        return 0;
    }

    memmory[adress] = value;
    return 1;
}

int SimpleComputer::memmoryGet(const size_t &adress, int &value)
{
    if (adress >= MEM_SIZE)
    {
        regSet(MEMORY_OVERRUN, 1);
        return 0;
    }

    value = memmory[adress];
    return 1;
}

int SimpleComputer::memmorySave(const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Cannot open file \"" << fileName << "\"\n";
        return 0;
    }

    file.write((char *)memmory, MEM_SIZE * sizeof(int));

    return 1;
}

int SimpleComputer::memmoryLoad(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Cannot open file \"" << fileName << "\"\n";
        return 0;
    }

    file.read((char *)memmory, MEM_SIZE * sizeof(int));

    return 1;
}

void SimpleComputer::memInit()
{
    for (size_t i = 0; i < MEM_SIZE; i++)
        memmory[i] = 0;
}

void SimpleComputer::regInit()
{
    flagRegister = 0;
}

int SimpleComputer::regSet(const size_t &flag, const bool &value)
{
    if (flag > MAX_FLAG)
        return 0;

    if (value)
        flagRegister |= 1 << flag;
    else
        flagRegister &= ~(1 << flag);

    return 1;
}

int SimpleComputer::regGet(const size_t &flag, bool &value)
{
    if (flag > MAX_FLAG)
        return 0;

    value = (flagRegister >> flag) & 0x1;
    return 1;
}

int SimpleComputer::commandEncode(const int &command, const int &operand, int &value)
{
    if ((command < 0x10) || (command > 0x11 && command < 0x20) || (command > 0x21 && command < 0x30) ||
        (command > 0x33 && command < 0x40) || (command > 0x43 && command < 0x51) || (command > 0x76) ||
        (operand > 0x7f))
    {
        regSet(WRONG_COMAND, 1);
        return WRONG_COMAND;
    }

    value = 0x0;
    value |= command;
    value <<= 7;
    value |= operand;
    return 1;
}

int SimpleComputer::commandDecode(const int &value, int &command, int &operand)
{
    if ((value & 0x4000) == 0x4000) //if 15 bit isnt 0
    {
        regSet(WRONG_COMAND, 1);
        return WRONG_COMAND;
    }

    operand = command = 0x0;
    operand |= value & 0x7f;
    command |= (value >> 7) & 0x7f;

    if ((command < 0x10) || (command > 0x11 && command < 0x20) || (command > 0x21 && command < 0x30) ||
        (command > 0x33 && command < 0x40) || (command > 0x43 && command < 0x51) || (command > 0x76) ||
        (operand > 0x7f))
    {
        regSet(WRONG_COMAND, 1);
        return WRONG_COMAND;
    }

    return 1;
}