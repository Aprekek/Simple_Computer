#include "includes/mySimpleComputer.h"

SimpleComputer *SimpleComputer::instance = 0;

SimpleComputer::SimpleComputer()
{
    memory = new int[MEM_SIZE];
    if (memory != nullptr)
    {
        memInit();
        regInit();
    }
    else
    {
        std::cout << "Cannoy allocate memory (SimpleComputer <memory>)\n";
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

int SimpleComputer::memorySet(const size_t &adress, const int &value)
{
    if (adress >= MEM_SIZE)
    {
        regSet(MEMORY_OVERRUN, 1);
        return 0;
    }

    memory[adress] = value;
    return 1;
}

int SimpleComputer::memoryGet(const size_t &adress, int &value)
{
    if (adress >= MEM_SIZE)
    {
        regSet(MEMORY_OVERRUN, 1);
        return 0;
    }

    value = memory[adress];
    return 1;
}

int SimpleComputer::memorySave(const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Cannot open file \"" << fileName << "\"\n";
        return 0;
    }

    file.write((char *)memory, MEM_SIZE * sizeof(int));

    return 1;
}

int SimpleComputer::memoryLoad(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Cannot open file \"" << fileName << "\"\n";
        return 0;
    }

    file.read((char *)memory, MEM_SIZE * sizeof(int));

    return 1;
}

void SimpleComputer::memInit()
{
    for (size_t i = 0; i < MEM_SIZE; i++)
        memory[i] = 0;
    // delete !!!
    memory[10] = 6587;
    memory[28] = 4438;
    memory[45] = 7587;
    //
}

void SimpleComputer::regInit()
{
    flagRegister = 0;
}

void SimpleComputer::init()
{
    memInit();
    regInit();
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

    int tmpCommand, tmpOperand;

    tmpOperand = tmpCommand = 0x0;
    tmpOperand |= value & 0x7f;
    tmpCommand |= (value >> 7) & 0x7f;

    if ((tmpCommand < 0x10) || (tmpCommand > 0x11 && tmpCommand < 0x20) || (tmpCommand > 0x21 && tmpCommand < 0x30) ||
        (tmpCommand > 0x33 && tmpCommand < 0x40) || (tmpCommand > 0x43 && tmpCommand < 0x51) || (tmpCommand > 0x76) ||
        (tmpOperand > 0x7f))
    {
        regSet(WRONG_COMAND, 1);
        return WRONG_COMAND;
    }

    command = tmpCommand;
    operand = tmpOperand;

    return 1;
}

/*
int main()
{
    int command = 0x51;
    int operand = 0x33;
    int value = 0x0;
    SimpleComputer *inst = SimpleComputer::getInstance();
    inst->commandEncode(command, operand, value);
    std::cout << std::hex << value << std::endl;
    command = operand = 0x0;
    value = 1238;
    if (!inst->commandDecode(value, command, operand))
        std::cout << "Error\n";
    std::cout << command << std::endl
              << operand << std::endl;

    return 0;
}*/