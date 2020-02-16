#ifndef MYSIMPLECOMPUTER_H
#define MYSIMPLECOMPUTER_H

#include <string>
#include <fstream>

#define MEM_SIZE 10

//-------Comands-------
#define READ 0x10
#define WRITE 0x11

#define LOAD 0x20
#define STOKE 0X21

#define ADD 0x30
#define SUB 0x31
#define DIVIDE 0x32
#define MUD 0x33

#define JUMP 0x40
#define JNEG 0x41
#define JZ 0x42
#define HALT 0x43

#define NOT 0x51
#define AND 0x52
#define OR 0x53
#define XOR 0x54
#define JNS 0x55
#define JC 0x56
#define JNC 0x57
#define JP 0x58
#define JNP 0x59
#define CHL 0x60
#define SHR 0x61
#define RCL 0x62
#define RCR 0x63
#define NEG 0x64
#define ADDC 0x65
#define SUBC 0x66
#define LOGLC 0x67
#define LOGRC 0x68
#define RCCL 0x69
#define RCCR 0x69
#define MOVA 0x71
#define MOVR 0x72
#define MOVCA 0x73
#define MOVCR 0x74
#define ADDC2 0x75
#define SUBC2 0x76
//------------------

//------Flags------
#define MAX_FLAG 4
#define WRONG_COMAND 0
#define IGNR_CLOCK_PULSES 1
#define MEMORY_OVERRUN 2
#define OPERATION_OVERFLOW 3
#define DEVISION_ZERO 4
//------------------

class SimpleComputer //Singleton
{
private:
    int memmory[MEM_SIZE];
    uint32_t flagRegister;

    static SimpleComputer *instance;
    SimpleComputer();

public:
    static SimpleComputer *getInstance();
    int memmorySet(const size_t &adress, const int &value);
    int memmoryGet(const size_t &adress, int &value);
    int memmorySave(const std::string &fileName);
    int memmoryLoad(const std::string &fileName);
    void regInit();
    int regSet(const size_t &flag, const bool &value);
    int regGet(const size_t &flag, bool &value);

    static int commandEncode(const int &command, const int &operand, int &value);
    static int commandDecode(const int &value, int &command, int &operand);
};

SimpleComputer *SimpleComputer::instance = 0;

SimpleComputer::SimpleComputer()
{
    regInit();
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

int SimpleComputer::commandEncode(const int &command, const int &operand, int &value);

int SimpleComputer::commandDecode(const int &value, int &command, int &operand);
#endif