#include "includes/basic_tr.h"

std::vector<std::string> BasicTr::assStrings;
std::vector<std::string> BasicTr::basicStrings;
std::vector<GoToMap> BasicTr::goToTargets;
std::array<int, 25> variableCell;
int BasicTr::totalVariables = 0;
int BasicTr::targetsFreed = 0;
int BasicTr::offset = 0;
int BasicTr::basicStrNum = 0;
int BasicTr::assStrNum = 0;

void BasicTr::findGOTO()
{
    GoToMap target;
    std::size_t position1, position2;
    int lengh;
    int end = basicStrings.size();

    for (int i = 0; i < end; ++i)
    {
        position1 = basicStrings[i].find("GOTO");
        if (position1 == std::string::npos)
            continue;

        //finding agrument of GOTO
        position2 = basicStrings[i].find_first_of(" ", position1);
        position1 = basicStrings[i].find_first_not_of(" ", position2);

        //reading argument
        target.goToTarget = std::stoi(basicStrings[i].substr(position1, 2));
        goToTargets.push_back(target);
    }
}

int BasicTr::checkLineNum()
{
    std::size_t position = basicStrings[basicStrNum].find_first_of(" ");

    assStrings[assStrNum] = basicStrings[basicStrNum].substr(0, position);

    if (basicStrNum != std::stoi(assStrings[assStrNum]))
    {
        std::cout << "Wrong order line number : " << basicStrings[basicStrNum]
                  << "; expected " << basicStrNum << std::endl;
        return -1;
    }

    offset = position;

    return 0;
}

int BasicTr::checkComand()
{

    std::size_t position1 = basicStrings[basicStrNum].find_first_not_of(" ", offset);
    std::size_t position2 = basicStrings[basicStrNum].find_first_of(" \n\0", position1);
    if (position1 == std::string::npos || position2 == std::string::npos)
    {
        std::cout << "Cannot find comand : " << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    std::string strComand = basicStrings[basicStrNum].substr(position1, position2 - position1);
    offset = position2;

    int result = assComandToBasic(strComand);
    if (result == -1)
    {
        std::cout << "Wrong comand \"" << strComand << "\" : "
                  << basicStrings[basicStrNum] << std::endl;
        return -1;
    }
}

int BasicTr::assComandToBasic(const std::string &strComand)
{
    static const size_t size = goToTargets.size(); // troubles???

    //checking if current line is target to goto
    for (size_t i = targetsFreed; i < size; ++i)
    {
        if (basicStrNum == goToTargets[i].goToTarget)
        {
            ++targetsFreed;
            goToTargets[i].assStringNum = assStrNum;
        }
    }

    if (strComand.compare("INPUT") == 0)
    {
        assStrings[assStrNum] += " READ ";
        return simpleOperParams();
    }
    if (strComand.compare("OUTPUT") == 0)
        return _WRITE;
    if (strComand.compare("GOTO") == 0)
        return _JUMP;
    if (strComand.compare("END") == 0)
        return _END;
    if (strComand.compare("REM") == 0)
        return _REM;
    if (strComand.compare("SHR") == 0)
        return _SHR;
    if (strComand.compare("IF") == 0)
        return _IF;
    if (strComand.compare("LET") == 0)
        return _LET;

    return -1;
}

int BasicTr::simpleOperParams()
{
    //checking if parametr is not 1-symbol variable
    char ch = basicStrings[basicStrNum][offset + 2];
    if (ch != '\n')
    {
        std::cout << "Make shure that your variable in range of A-Z in uppercase, or remove extra spaces\n"
                  << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    int index = basicStrings[basicStrNum][offset + 1] - 'A';
    if (index < 0 || index > 24)
    {
        std::cout << "Make shure that your variable in range of A-Z in uppercase\n";
        return -1;
    }

    variableCell[index] = 99 - totalVariables;
    ++totalVariables;

    assStrings[assStrNum] += ch;
    ++assStrNum;

    return 0;
};

int BasicTr::complexOperParams() {}

int BasicTr::checkParametrs() {}

int BasicTr::parsing()
{
    for (int i = 0; i < 25; ++i)
        variableCell[i] = -1;

    std::string buf;

    int end = basicStrings.size();
    for (int i = 0; i < end; ++i) //check assembler lines count bounding
    {

        if (checkLineNum() == -1)
            return -1;

        if (checkComand() == -1)
            return -1;

        if (checkParametrs() == -1)
            return -1;

        ++basicStrNum;
    }

    return 0;
}

void BasicTr::readFile(std::fstream &file)
{
    std::string tempLine;
    while (!file.eof())
    {
        std::getline(file, tempLine);
        basicStrings.push_back(tempLine);
    }
}

int BasicTr::translate(std::string fileName)
{
    std::fstream file(fileName, std::ios::in);

    if (!file.is_open())
    {
        std::cout << "Cannot open file \"" << fileName << "\"";
        return -1;
    }

    readFile(file);

    if (parsing() == -1) //<< "If you see this error, please check README to get more info about code style\n"
        return -1;

    file.close();

    return 0;
}