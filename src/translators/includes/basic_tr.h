#ifndef BASIC_TR_H
#define BASIC_TR_H

#include <fstream>
#include <string>
#include <iostream>
#include <array>
#include <vector>

#define _IF 0x100
#define _LET 0x101
#define _END 0x102
#define _REM 0x103
#define _READ 0x10
#define _WRITE 0x11
#define _JUMP 0x40
#define _SHR 0x61

struct GoToMap
{
    int goToTarget;
    int assStringNum;
};

class BasicTr
{
private:
    static std::vector<std::string> assStrings;
    static std::vector<std::string> basicStrings;
    static std::vector<GoToMap> goToTargets;
    static std::array<int, 25> variableCell;
    static int totalVariables;
    static int targetsFreed;
    static int offset;
    static int basicStrNum;
    static int assStrNum;

    static int simpleOperParams();
    static int complexOperParams();
    static int assComandToBasic(const std::string &strComand);

    static void readFile(std::fstream &file);
    static int checkLineNum();
    static int checkComand();
    static int checkParametrs();

    static void findGOTO();
    static int parsing();

public:
    static int translate(std::string fileName);
};

#endif