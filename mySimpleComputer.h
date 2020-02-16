#ifndef MYSIMPLECOMPUTER_H
#define MYSIMPLECOMPUTER_H

#include <array>
#include <string>

#define MEM_SIZE 100
// Flags
#define MAX_FLAG 4
#define WRONG_COMAND 0
#define IGNR_CLOCK_PULSES 1
#define MEMORY_OVERRUN 2
#define OPERATION_OVERFLOW 3
#define DEVISION_ZERO 4

class SimpleComputer
{
private:
    std::array<int, MEM_SIZE> memorry;
    uint32_t flagRegister;

public:
    int memmorySet(const size_t &adress, const int &value);
    int memmoryGet(const size_t &adress, int &value) const;
    int memmorySave(const std::string &fileName) const;
    int memmoryLoad(const std::string &fileName);
    int regSet(const size_t &flag, const bool &value);
    int regGet(const size_t &flag, bool &value) const;

    static int commandEncode(const int &command, const int &operand, int &value) const;
    static int commandDecode(const int &value, int &command, int &operand) const;
};

int SimpleComputer::memmorySet(const size_t &adress, const int &value)
{
    if (adress >= memorry.size())
    {
        regSet(MEMORY_OVERRUN, 1);
        return 0;
    }

    memorry.at(adress) = value;
    return 1;
}

int SimpleComputer::memmoryGet(const size_t &adress, int &value) const
{
    if (adress >= memorry.size())
    {
        regSet(MEMORY_OVERRUN, 1);
        return 0;
    }

    value = memorry.at(adress);
    return 1;
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

int SimpleComputer::regGet(const size_t &flag, bool &value) const
{
    if (flag > MAX_FLAG)
        return 0;

    value = (flagRegister >> flag) & 0x1;
    return 1;
}

#endif