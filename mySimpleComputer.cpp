#include <iostream>
#include "mySimpleComputer.h"

int main()
{
    SimpleComputer *sc_instance = SimpleComputer::getInstance();
    int value, command, operand, adress, flag, ch;
    std::string filePath;
    bool flagValue;

    do
    {
        std::cout << "1) Print memmory\n"
                  << "2) Print flagRegister\n"
                  << "3) Init memmory\n"
                  << "4) Init flagRegister\n"
                  << "5) Set memmory\n"
                  << "6) Get memmory\n"
                  << "7) Save memmory\n"
                  << "8) Read memmory\n"
                  << "9) Set flag to register\n"
                  << "10) Get flag from register\n"
                  << "11) Encode command\n"
                  << "12) Decode command\n"
                  << "0) Exit\n";
        std::cin >> ch;

        switch (ch)
        {
        case 1:
            sc_instance->memPrint();
            break;
        case 2:
            sc_instance->regPrint();
            break;
        case 3:
            sc_instance->memInit();
            break;
        case 4:
            sc_instance->regInit();
            break;
        case 5:
            std::cout << "adress: ";
            std::cin >> adress;
            std::cout << "value: ";
            std::cin >> value;
            sc_instance->memmorySet(adress, value);
            break;
        case 6:
            std::cout << "adress: ";
            std::cin >> adress;
            sc_instance->memmoryGet(adress, value);
            std::cout << "value: " << value << std::endl;
            break;
        case 7:
            std::cout << "path to file: ";
            std::cin >> filePath;
            sc_instance->memmorySave(filePath);
            break;
        case 8:
            std::cout << "path to file: ";
            std::cin >> filePath;
            sc_instance->memmoryLoad(filePath);
            break;
        case 9:
            std::cout << "flag: ";
            std::cin >> flag;
            std::cout << "value: ";
            std::cin >> flagValue;
            sc_instance->regSet(flag, flagValue);
            break;
        case 10:
            std::cout << "flag: ";
            std::cin >> flag;
            sc_instance->regGet(flag, flagValue);
            std::cout << "value: " << flagValue << std::endl;
            break;
        case 11:
            std::cout << "command: ";
            std::cin >> command;
            std::cout << "operand: ";
            std::cin >> operand;
            sc_instance->commandEncode(command, operand, value);
            std::cout << "value: " << value << std::endl;
            break;
        case 12:
            std::cout << "value: ";
            std::cin >> value;
            sc_instance->commandDecode(value, command, operand);
            std::cout << "command: " << command << "\noperand: " << operand << std::endl;
            break;
        case 0:
            return 0;
            break;
        default:
            std::cout << "dsa\n";
            break;
        }

    } while (ch != 'q');
    return 0;
}