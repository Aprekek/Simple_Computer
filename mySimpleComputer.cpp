#include <iostream>
#include "mySimpleComputer.h"

int main()
{
    SimpleComputer *smplcmp = SimpleComputer::getInstance();
    int value = 0x0;
    int command = 0x33;
    int operand = 0x59;
    smplcmp->commandEncode(command, operand, value);
    std::cout << value << std::endl;
    int command2 = 0x0;
    int operand2 = 0x0;
    smplcmp->commandDecode(value, command2, operand2);
    std::cout << command2 << std::endl;
    std::cout << operand2 << std::endl;
    return 0;
}