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
    bool isForward; // ex.: 20 goto 50 - true; 20 goto 15 - false
    int basicTargNum;
    int assGoToNum;
    int assTagrNum; 
    GoToMap *left;
    GoToMap *right;
};

class GTLinkedList
{
private:
    GoToMap *head;
    GoToMap *tail;
    size_t size;

public:
    GTLinkedList();
    ~GTLinkedList();

    size_t getSize();
    void offsetHead();
    void addNode(const GoToMap &n);
    void deleteCurNode();
    GoToMap *getHeadNode();
    bool assignAssStrNum(const int &assStrNum, const int &basicStrNum);
};

class BasicTr
{
private:
    static std::vector<std::string> assStrings;
    static std::vector<std::string> basicStrings;
    static GTLinkedList goToTargets;
    static std::array<int, 26> variableCell;
    static int totalVariables;
    static int targetsFreed;
    static int offset;
    static int basicStrNum;
    static int assStrNum;

    static int simpleOperParams();
    static int complexOperParams();
    static int basicComandToAss(const std::string &strComand);

    static void readFile(std::fstream &file);
    static int checkLineNum();
    static int checkComand();

    static void findGOTO(const int &i);
    static int parsing();

public:
    static int translate(std::string fileName);
};

#endif