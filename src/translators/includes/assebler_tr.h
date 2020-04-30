#ifndef ASSEMBLER_TR_H
#define ASSEMBLER_TR_H

#include <fstream>
#include <string>
#include <iostream>
#include <array>

struct Node
{
    int cellKey;
    int comand;
    int operand;
};

class AssemblerTR
{
private:
    static std::string textString;
    static std::array<Node *, 100> arrList;
    static char tempBufer[128];
    static std::fstream file;
    static int semicolonPos;
    static int offset;
    static int lineNumber;

    static int translateComand(const std::string &comand);

    static void commentCheck();
    static int cellValidCheck();
    static int comandValidCheck();
    static int operandValidCheck();
    static int validationCheck();

public:
    static int translate(std::string fileName);
};

#endif