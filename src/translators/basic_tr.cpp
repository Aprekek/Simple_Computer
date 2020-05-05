#include "includes/basic_tr.h"

GTLinkedList::GTLinkedList()
{
    tail = head = nullptr;
    size = 0;
}

GTLinkedList::~GTLinkedList()
{
    while (size > 0)
        deleteCurNode();
}

size_t GTLinkedList::getSize()
{
    return size;
}

void GTLinkedList::offsetHead()
{
    head = head->left;
};

void GTLinkedList::addNode(const GoToMap &n)
{
    GoToMap *node = new GoToMap;
    if (node == nullptr)
    {
        std::cout << "Cannot allocate memory\n";
        exit(EXIT_FAILURE);
    }

    node->basicTargNum = n.basicTargNum;
    node->isForward = n.isForward;
    node->assTagrNum = node->assGoToNum = 0;

    if (size == 0)
    {
        node->left = node;
        node->right = node;
        head = node;
        tail = node;
    }
    else
    {
        node->left = tail;
        node->right = tail->right;
        tail->right->left = node;
        tail->right = node;
        tail = node;
    }
    ++size;
}

void GTLinkedList::deleteCurNode()
{
    if (size == 0)
        return;
    if (size == 1)
    {
        delete head;
        --size;
        return;
    }

    GoToMap *temp = head->right;
    head->left->right = temp;
    temp->left = head->left;
    delete head;
    --size;
    head = temp;
}

GoToMap *GTLinkedList::getHeadNode()
{
    return head;
}

bool GTLinkedList::assignAssStrNum(const int &assStrNum, const int &basicStrNum)
{
    bool assign = false;
    GoToMap *temp = head;
    for (size_t i = 0; i < size; ++i)
    {
        if (basicStrNum == temp->basicTargNum)
        {
            temp->assTagrNum = assStrNum;
            assign = true;
        }
        temp = temp->right;
    }
    return assign;
}

//_____________Basic_____________

std::vector<std::string> BasicTr::assStrings;
std::vector<std::string> BasicTr::basicStrings;
GTLinkedList BasicTr::goToTargets;
std::array<int, 26> BasicTr::variableCell;
int BasicTr::totalVariables = 0;
int BasicTr::targetsFreed = 0;
int BasicTr::offset = 0;
int BasicTr::basicStrNum = 0;
int BasicTr::assStrNum = 0;

void BasicTr::findGOTO(const int &i)
{
    GoToMap target;
    std::size_t position1, position2;

    position1 = basicStrings[i].find("GOTO");
    if (position1 == std::string::npos)
        return;

    //finding agrument of GOTO
    position2 = basicStrings[i].find_first_of(" ", position1);
    position1 = basicStrings[i].find_first_not_of(" ", position2);

    //reading argument
    int digit = std::stoi(basicStrings[i].substr(position1, 2));
    (digit > i) ? target.isForward = 1 : target.isForward = 0;
    target.basicTargNum = digit;
    goToTargets.addNode(target);
}

int BasicTr::checkLineNum()
{
    static int prevLineNumb = -1;
    std::string buf;
    std::size_t position = basicStrings[basicStrNum].find_first_of(" ");

    buf = basicStrings[basicStrNum].substr(0, position);

    if (basicStrNum != std::stoi(buf))
    {
        std::cout << "Wrong order line number : " << basicStrings[basicStrNum]
                  << "; expected " << basicStrNum << std::endl;
        return -1;
    }

    if (prevLineNumb != assStrNum)
        assStrings.push_back(std::to_string(assStrNum));
    else
        assStrings[assStrNum] = std::to_string(assStrNum);

    prevLineNumb = assStrNum;
    offset = position;

    return 0;
}

int BasicTr::checkComand()
{
    std::size_t position1 = basicStrings[basicStrNum].find_first_not_of(" \t", offset);
    std::size_t position2 = basicStrings[basicStrNum].find_first_of(" \n\0", position1);
    if (position1 == std::string::npos || position2 == std::string::npos)
    {
        if (basicStrings[basicStrNum][position2] != '\0')
        {
            std::cout << "Cannot find comand : " << basicStrings[basicStrNum] << std::endl;
            return -1;
        }
    }

    std::string strComand = basicStrings[basicStrNum].substr(position1, position2 - position1);
    offset = position2;

    return basicComandToAss(strComand);
}

int BasicTr::basicComandToAss(const std::string &strComand)
{
    //checking if current line is target to goto and if it is REM - error
    bool isRem = goToTargets.assignAssStrNum(assStrNum, basicStrNum);

    if (strComand.compare("INPUT") == 0)
    {
        assStrings[assStrNum] += " READ ";
        return simpleOperParams();
    }
    if (strComand.compare("PRINT") == 0)
    {
        assStrings[assStrNum] += " WRITE ";
        return simpleOperParams();
    }
    if (strComand.compare("END") == 0)
    {
        assStrings[assStrNum] += " HALT 0";
        ++assStrNum;
        return 0;
    }
    if (strComand.compare("REM") == 0)
    {
        if (isRem)
        {
            std::cout << "at line " << basicStrNum << " REM can't be the goal of goto\n"
                      << basicStrings[basicStrNum] << std::endl;
            return -1;
        }
        return 0;
    }
    if (strComand.compare("SHR") == 0)
    {
        assStrings[assStrNum] += " SHR ";
        return simpleOperParams();
    }
    if (strComand.compare("GOTO") == 0)
    {
        GoToMap *node = goToTargets.getHeadNode();
        assStrings[assStrNum] += " GOTO ";
        if (!node->isForward)
        {
            assStrings[assStrNum] += std::to_string(node->assTagrNum);
            goToTargets.deleteCurNode();
        }
        else
        {
            node->assGoToNum = assStrNum;
            goToTargets.offsetHead();
        }
        ++assStrNum;
        return 0;
    }
    if (strComand.compare("IF") == 0)
        return _IF;
    if (strComand.compare("LET") == 0)
        return _LET;

    std::cout << "Underfined command \"" << strComand << "\" at line " << basicStrNum << std::endl;
    return -1;
}

int BasicTr::simpleOperParams()
{
    //checking if parametr is not 1-symbol variable
    char ch = basicStrings[basicStrNum][offset + 2];
    if (ch != '\0')
    {
        std::cout << "Make shure that your variable in range of A-Z in uppercase, or remove extra spaces\n"
                  << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    int index = basicStrings[basicStrNum][offset + 1] - 'A';
    if (index < 0 || index > 25)
    {
        std::cout << "Make shure that your variable in range of A-Z in uppercase\n";
        return -1;
    }

    if (variableCell[index] == -1)
    {
        variableCell[index] = 99 - totalVariables;
        ++totalVariables;
    }
    assStrings[assStrNum] += std::to_string(variableCell[index]);
    ++assStrNum;

    return 0;
};

int BasicTr::complexOperParams() { return 0; }

int BasicTr::parsing()
{
    for (int i = 0; i < 25; ++i)
        variableCell[i] = -1;

    int end = basicStrings.size();
    for (int i = 0; i < end; ++i) //check assembler lines count bounding
    {

        if (checkLineNum() == -1)
            return -1;

        if (checkComand() == -1)
            return -1;

        ++basicStrNum;
    }

    end = goToTargets.getSize();
    GoToMap *node = nullptr;
    for (int i = 0; i < end; ++i)
    {
        node = goToTargets.getHeadNode();
        assStrings[node->assGoToNum] += std::to_string(node->assTagrNum);
        goToTargets.deleteCurNode();
    }
    
    return 0;
}

void BasicTr::readFile(std::fstream &file)
{
    int i = 0;
    std::string tempLine;

    while (!file.eof())
    {
        std::getline(file, tempLine);
        if (tempLine.empty())
            continue;
        basicStrings.push_back(tempLine);
        findGOTO(i);
        ++i;
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

    if (parsing() == -1)
    {
        std::cout << "If you see this error, please check README to get more info about code style and CHECK you code\n";
        return -1;
    }

    file.close();

    for (int i = 0; i < assStrNum; ++i)
        std::cout << assStrings[i] << std::endl;

    return 0;
}

int main()
{
    BasicTr::translate("basic.b");

    return 0;
}