#include "includes/assebler_tr.h"

std::string AssemblerTR::textString;
std::array<Node *, 100> AssemblerTR::arrList;
char AssemblerTR::tempBufer[128] = {0};
std::fstream AssemblerTR::file;
int AssemblerTR::offset = 0;
int AssemblerTR::lineNumber = 0;

int AssemblerTR::translateComand(const std::string &comand)
{
    if (comand.compare("READ") == 0)
        return 0x10;
    if (comand.compare("WRITE") == 0)
        return 0x11;
    if (comand.compare("LOAD") == 0)
        return 0x20;
    if (comand.compare("STORE") == 0)
        return 0x21;
    if (comand.compare("ADD") == 0)
        return 0x30;
    if (comand.compare("SUB") == 0)
        return 0x31;
    if (comand.compare("DIVIDE") == 0)
        return 0x32;
    if (comand.compare("MUL") == 0)
        return 0x33;
    if (comand.compare("JUMP") == 0)
        return 0x40;
    if (comand.compare("JNEG") == 0)
        return 0x41;
    if (comand.compare("JZ") == 0)
        return 0x42;
    if (comand.compare("HALT") == 0)
        return 0x43;
    if (comand.compare("SHR") == 0)
        return 0x61;
    if (comand.compare("=") == 0)
        return 0x01;

    return -1;
}

int AssemblerTR::cellValidCheck()
{
    static std::size_t position1, position2;

    position1 = textString.find_first_not_of(" ");
    if (position1 == std::string::npos)
        return -1;

    position2 = textString.find_first_of(" ");
    if (position1 == std::string::npos)
    {
        std::cout << "on line " << lineNumber << " cannot find cell number\n"
                  << "line: " << textString << std::endl;
        return -1;
    }

    int cell = std::stoi(textString.substr(position1, position2 - position1));
    if (cell > 100 || cell < 0)
    {
        std::cout << "on line " << lineNumber << ":\ncells must be numbered from 0 to 99"
                  << "line: " << textString << std::endl;
        return -1;
    }

    if (cell != lineNumber)
    {
        std::cout << "number of cell on line " << cell << " must be " << lineNumber << std::endl;
        return -1;
    }

    offset += 2;
    arrList[cell] = new Node;
    arrList[cell]->cellKey = cell;

    return 0;
}

int AssemblerTR::comandValidCheck()
{
    static std::size_t position1, position2;
    std::string comand;

    position1 = textString.find_first_not_of(" ", offset);
    if (position1 == std::string::npos)
    {
        std::cout << "on line " << lineNumber << " cannot find comand\n"
                  << "line: " << textString << std::endl;
        return -1;
    }

    position2 = textString.find_first_of(" ", position1);
    if (position2 == std::string::npos)
    {
        std::cout << "on line " << lineNumber << " cannot find comand\n"
                  << "line: " << textString << std::endl;
        return -1;
    }

    comand = textString.substr(position1, position2 - position1);
    int hash = translateComand(comand);
    if (hash == -1)
    {
        std::cout << "on line " << lineNumber << " cannot find comand \"" << comand
                  << "\"\nline: " << textString << std::endl;
        return -1;
    }
    arrList[lineNumber]->comand = hash;
    offset = position2;

    return 0;
};
int AssemblerTR::operandValidCheck()
{
    static std::size_t position1, position2;

    position1 = textString.find_first_not_of(" ", offset);
    if (position1 == std::string::npos)
    {
        std::cout << "on line " << lineNumber << " cannot find operand"
                  << "\nline: " << textString << std::endl;
        return -1;
    }

    position2 = textString.find_first_of("\n\0; ");
    if (position1 == std::string::npos)
    {
        std::cout << "on line " << lineNumber << " cannot find operand\n"
                  << "line: " << textString << std::endl;
        return -1;
    }

    int operand = std::stoi(textString.substr(position1, position2 - position1));
    if (arrList[lineNumber]->comand != 0x01)
    {
        if (operand > 100 || operand < 0)
        {
            std::cout << "on line " << lineNumber << ":\noperand must be numbered from 0 to 99"
                      << "line: " << textString << std::endl;
            return -1;
        }
    }
    arrList[lineNumber]->operand = operand;
    return 0;
};

int AssemblerTR::validationCheck()
{
    while (!file.eof())
    {
        file.getline((char *)&tempBufer, 128);
        textString = tempBufer;

        if (!textString.empty())
        {
            if (cellValidCheck() == -1)
                return -1;

            if (comandValidCheck() == -1)
                return -1;

            if (operandValidCheck() == -1)
                return -1;
        }
        ++lineNumber;
        offset = 0;
    }
    return 0;
}

int AssemblerTR::writeToObjFile(const std::string &fileName)
{
    file.close();
    std::size_t position = fileName.find_last_of(".");
    std::string objFile = fileName.substr(0, position) + ".o";
    file.open(objFile, std::ios::out | std::ios::binary);

    if (!file.is_open())
    {
        std::cout << "Cannot create \"" << objFile << "\"\n";
        return -1;
    }

    for (int i = 0; i < 100; ++i)
    {
        if (arrList[i] != nullptr)
            file.write((char *)(arrList[i]), 12);
    }

    return 0;
}

int AssemblerTR::translate(std::string fileName)
{
    file.open(fileName, std::ios::in);
    if (!file.is_open())
    {
        std::cout << "Cannot open file \"" << fileName << "\"\n";
        file.close();
        return -1;
    }

    if (validationCheck() == -1)
    {
        file.close();
        return -1;
    }
    if (writeToObjFile(fileName) == -1)
    {
        file.close();
        return -1;
    }

    file.close();
    return 0;
}