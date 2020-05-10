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
    if (strComand.compare("LET") == 0)
    {
        return BasicExprToPolishStrParser(basicStrings[basicStrNum].substr(offset + 1), 0);
    }
    if (strComand.compare("IF") == 0)
    {
        return ifParser();
    }

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
        variableCell[index] = 94 - totalVariables;
        ++totalVariables;
    }
    assStrings[assStrNum] += std::to_string(variableCell[index]);
    ++assStrNum;

    return 0;
};

int BasicTr::BasicExprToPolishStrParser(const std::string &expr, int flag) //if LET => flag = 0; if left expretion in IF flag = 1; if right = 2
{
    std::string polishExpr;
    std::vector<char> stack;
    std::string expression = expr;

    int i = 0;
    int storingVar = 0;
    short openBracket = 0, closeBracket = 0;
    char symbol = ' ';
    bool incorrectExpr = false;

    if (flag == 0)
    {
        //looks for a variable storing the result of an expression
        do
        {
            symbol = expression[i];
            ++i;
        } while (symbol == ' ');

        storingVar = symbol - 'A';

        do
        {
            symbol = expression[i];
            ++i;
        } while (symbol != '=' && symbol != '\0');

        if (symbol != '=')
        {
            std::cout << "At line " << basicStrNum << ":\n\tCannot find storing variable (\'=\'?): "
                      << expression << std::endl;
            return -1;
        }

        if (variableCell[storingVar] == -1)
        {
            variableCell[storingVar] = 94 - totalVariables;
            ++totalVariables;
        }
    }
    else if (flag == 1)
        storingVar = 95;
    else if (flag == 2)
        storingVar = 96;
    else
        return -1;

    //checking brackets and floating numbers
    while (symbol != '\0')
    {
        symbol = expression[i];

        if (symbol == '(')
            ++openBracket;
        else if (symbol == ')')
        {
            ++closeBracket;
            if (closeBracket > openBracket)
            {
                std::cout << "At line " << basicStrNum << ":\n\tCheck the order of the brackets: "
                          << expression << std::endl;
                incorrectExpr = true;
            }
        }
        else if (symbol >= '1' && symbol <= '9')
        {
            std::cout << "At line " << basicStrNum << ":\n\tExpressions must be only with variables: "
                      << expression << std::endl;
            return -1;
        }
        ++i;
    }
    if (openBracket != closeBracket)
    {
        std::cout << "At line " << basicStrNum << ":\n\tThe number of open and closed brackets is not equal: "
                  << expression << std::endl;
        incorrectExpr = true;
    }
    if (incorrectExpr)
        return -1;

    //Parsing expression to polish
    int prevOperation = 0;         //needed when parsing a single minus ex: A * -B
    bool isVariablePassed = false; //excludes ex: AA + B
    if (flag == 0)
        i = 4; //excludes ex: "C ="
    else
        i = 0;

    symbol = expression[i];

    while (symbol != '\0')
    {
        while (symbol == ' ') //exclude spaces
        {
            symbol = expression[i];
            ++i;
        }

        if (((symbol - 'A') > -1) && ((symbol - 'A') < 26)) //if symbolic variable
        {
            if (isVariablePassed)
            {
                std::cout << "At line " << basicStrNum
                          << ":\n\tMake shure that your variable in range of A-Z in uppercase or its integer: "
                          << expression << std::endl;
                return -1;
            }
            if (variableCell[symbol - 'A'] == -1)
            {
                variableCell[symbol - 'A'] = 94 - totalVariables;
                ++totalVariables;
            }
            polishExpr.push_back(symbol);
            isVariablePassed = true;
        }
        else //if operation
        {
            switch (symbol)
            {
            case '+':
                prevOperation = P_ADD;
                break;
            case '-':
                if (isVariablePassed == false) // ex: A * -B
                    polishExpr += "0";

                prevOperation = P_SUB;
                break;
            case '*':
                prevOperation = P_MUL_DIV;
                break;
            case '/':
                prevOperation = P_MUL_DIV;
                break;
            case '(':
                prevOperation = P_OPEN_BR;
                break;
            case ')':
                prevOperation = P_CLOSE_BR;
                break;
            default:
            {
                std::cout << "At line " << basicStrNum
                          << ":\n\tUnknown symbol \'" << symbol << "\' : "
                          << expression << std::endl;
                return -1;
            }
            }
            pushPopStack(symbol, stack, polishExpr, prevOperation);
            isVariablePassed = false;
        }
        ++i;
        symbol = expression[i];
    }

    pushPopStack(' ', stack, polishExpr, P_ALL); //pop all symbols to polishExpr
    std::cout << polishExpr << std::endl;

    polishNotationToAssemler(polishExpr, storingVar);

    return 0;
}

int BasicTr::pushPopStack(const char &symbol, std::vector<char> &stack,
                          std::string &polishExpr, int priority)
{
    int i = stack.size() - 1;
    if (priority == P_ALL)
    {
        while (i > -1)
        {
            polishExpr.push_back(stack[i]);
            --i;
        }
        return 0;
    }

    if (stack.size() == 0)
    {
        stack.push_back(symbol);
        return 0;
    }

    char popedSymbol = ' ';

    if (priority == P_MUL_DIV)
    {
        while (i > -1)
        {
            popedSymbol = stack[i];
            if ((popedSymbol == '*') || (popedSymbol == '/'))
            {
                polishExpr.push_back(popedSymbol);
                stack.pop_back();
                --i;
            }
            else
                break;
        }
    }
    else if ((priority == P_ADD) || (priority == P_SUB))
    {
        while (i > -1)
        {
            popedSymbol = stack[i];
            if (popedSymbol == '(')
                break;

            polishExpr.push_back(popedSymbol);
            stack.pop_back();
            --i;
        }
    }
    else if (priority == P_CLOSE_BR)
    {
        while (i > -1)
        {
            popedSymbol = stack[i];
            stack.pop_back();
            --i;
            if (popedSymbol == '(') //
            {
                while (i > -1)
                {
                    popedSymbol = stack[i];
                    if ((popedSymbol == '*') || (popedSymbol == '/'))
                    {
                        polishExpr.push_back(popedSymbol);
                        stack.pop_back();
                        --i;
                    }
                    else
                        break;
                }
                break;
            }

            polishExpr.push_back(popedSymbol);
        }
        return 0;
    }

    stack.push_back(symbol);

    return 0;
}

//dont work
int BasicTr::polishNotationToAssemler(std::string expresision, int storingVar)
{
    //a - accumulator; 1 - 98 cell; 2 - 99 cell
    std::string assebmlerLine;
    std::size_t positionOper;
    std::size_t positionAccum;
    char symbol;
    int tempCell = 1; // if 1 cell = 98 if 0 cell = 99

    if (expresision.size() != 1)
    {
        positionOper = expresision.find_first_of("+-/*");
        symbol = expresision[positionOper - 2];
        expresision[positionOper - 2] = 'a';
    }
    else
    {
        positionOper = 0;
        symbol = expresision[positionOper];
    }
    if (assStrings.size() == assStrNum)
        assStrings.push_back(std::to_string(assStrNum) + " LOAD " + std::to_string(variableCell[symbol - 'A']));
    else
        assStrings[assStrNum] = (std::to_string(assStrNum) + " LOAD " + std::to_string(variableCell[symbol - 'A']));
    ++assStrNum;

    while (expresision.size() - 1 > 0)
    {
        symbol = expresision[positionOper]; //+-/*

        positionAccum = expresision.find("a");

        if (expresision[positionOper - 1] == 'a')
        {
            switch (symbol)
            {
            case '+':
                assebmlerLine = std::to_string(assStrNum) + " ADD ";
                targetAdress(assebmlerLine, expresision, positionOper - 2);
                break;
            case '-':
                assebmlerLine = std::to_string(assStrNum) + " STORE ";
                targetAdress2(assebmlerLine, " SUB ", expresision, positionOper - 2, tempCell);
                break;
            case '*':
                assebmlerLine = std::to_string(assStrNum) + " MUL ";
                targetAdress(assebmlerLine, expresision, positionOper - 2);
                break;
            case '/':
                assebmlerLine = std::to_string(assStrNum) + " STORE ";
                targetAdress2(assebmlerLine, " DIV ", expresision, positionOper - 2, tempCell);
                break;
            }
        }
        else
        {
            if (expresision[positionOper - 2] != 'a')
            {
                expresision[positionAccum] = (tempCell) ? '1' : '2';
                assStrings.push_back(std::to_string(assStrNum) + " STORE " + std::to_string(99 - tempCell));
                ++assStrNum;
                tempCell ^= 1;
                assebmlerLine = std::to_string(assStrNum) + " LOAD ";
                targetAdress(assebmlerLine, expresision, positionOper - 2);
            }
            switch (symbol)
            {
            case '+':
                assebmlerLine = std::to_string(assStrNum) + " ADD ";
                break;
            case '-':
                assebmlerLine = std::to_string(assStrNum) + " SUB ";
                break;
            case '*':
                assebmlerLine = std::to_string(assStrNum) + " MUL ";
                break;
            case '/':
                assebmlerLine = std::to_string(assStrNum) + " DIV ";
                break;
            }
            targetAdress(assebmlerLine, expresision, positionOper - 1);
        }
        expresision.erase(positionOper - 2, 2);
        expresision[positionOper - 2] = 'a';
        positionOper = expresision.find_first_of("+-/*");
    }

    int order = storingVar;
    if ((order > -1) && (order < 26))
        order = variableCell[storingVar];

    assStrings.push_back(std::to_string(assStrNum) + " STORE " + std::to_string(order));
    ++assStrNum;
}

void BasicTr::targetAdress(const std::string &assebmlerLine, std::string &expression, const int offset)
{
    if (expression[offset] == '1')
        assStrings.push_back(assebmlerLine + "98");
    else if (expression[offset] == '2')
        assStrings.push_back(assebmlerLine + "99");
    else if (expression[offset] == '0')
        assStrings.push_back(assebmlerLine + "97");
    else
        assStrings.push_back(assebmlerLine + std::to_string(variableCell[expression[offset] - 'A']));
    ++assStrNum;
}

void BasicTr::targetAdress2(const std::string &assebmlerLine, std::string assemblComand, std::string &expression, int offset, int tempCell)
{
    if (expression[offset] == '1') //if ex: 1a-
    {
        assStrings.push_back(assebmlerLine + "99");
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) + " LOAD 98");
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) + assemblComand + " 99");
    }
    else if (expression[offset] == '2') //if ex: 2a-
    {
        assStrings.push_back(assebmlerLine + "98");
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) + " LOAD 99");
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) + assemblComand + "98");
    }
    else if (expression[offset] == '0') //if ex: 0a-
    {
        assStrings.push_back(assebmlerLine + std::to_string(99 - tempCell));
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) + " LOAD 97");
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) + assemblComand + std::to_string(99 - tempCell));
        tempCell ^= 1;
    }
    else
    {
        assStrings.push_back(assebmlerLine + std::to_string(99 - tempCell));
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) +
                             " LOAD " + std::to_string(variableCell[expression[offset] - 'A']));
        ++assStrNum;
        assStrings.push_back(std::to_string(assStrNum) + assemblComand + std::to_string(99 - tempCell));
        tempCell ^= 1;
    }
    ++assStrNum;
}

int BasicTr::ifParser()
{
    std::size_t position = basicStrings[basicStrNum].find_first_not_of(" ", offset);
    if (position == std::string::npos)
    {
        std::cout << "at line " << basicStrNum << "\n\tСheck your \"IF\" condition for correctness:"
                  << basicStrings[basicStrNum];
        return -1;
    }
    std::size_t position2 = basicStrings[basicStrNum].find_first_of("><=", position);
    if (position == std::string::npos)
    {
        std::cout << "at line " << basicStrNum << "\n\tСheck your \"IF\" condition for correctness:"
                  << basicStrings[basicStrNum];
        return -1;
    }

    std::string leftIfExr, rightIfExpr;
    leftIfExr = basicStrings[basicStrNum].substr(position, position2 - position);
    if (leftIfExr[0] != '(')
    {
        std::cout << "at line " << basicStrNum << "\n\tExpresions if \"IF\" must be in brackets:"
                  << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    if (leftIfExr[leftIfExr.size() - 1] != ' ')
    {
        std::cout << "at line " << basicStrNum << "\n\tExpresions if \"IF\" must be in brackets or "
                  << "check you spases: " << basicStrings[basicStrNum] << std::endl;
        return -1;
    }
    if (leftIfExr[leftIfExr.size() - 2] != ')')
    {
        std::cout << "at line " << basicStrNum << "\n\tExpresions if \"IF\" must be in brackets or "
                  << "check you spases: " << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    leftIfExr = leftIfExr.substr(1, leftIfExr.size() - 3); //remove brackets
    char symbol = basicStrings[basicStrNum][position2];    //> < =

    if (basicStrings[basicStrNum][position2 + 1] != ' ')
    {
        std::cout << "at line " << basicStrNum << "\n\tThere must be a space after the condition sign"
                  << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    position = basicStrings[basicStrNum].find_first_not_of(" ", position2 + 1);
    if (position == std::string::npos)
    {
        std::cout << "at line " << basicStrNum << "\n\tСheck your \"IF\" condition for correctness:"
                  << basicStrings[basicStrNum];
        return -1;
    }

    if (basicStrings[basicStrNum][position] != '(')
    {
        std::cout << "at line " << basicStrNum << "\n\tExpresions if \"IF\" must be in brackets:"
                  << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    position2 = basicStrings[basicStrNum].find_first_of(" ", position);
    std::cout << basicStrings[basicStrNum][position2] << std::endl;

    if (basicStrings[basicStrNum][position2 - 1] != ')')
    {
        std::cout << "at line " << basicStrNum << "\n\tExpresions if \"IF\" must be in brackets:"
                  << basicStrings[basicStrNum] << std::endl;
        return -1;
    }
    rightIfExpr = basicStrings[basicStrNum].substr(position + 1, position2 - position - 2);

    BasicExprToPolishStrParser(leftIfExr, 1);
    if (rightIfExpr != "0")
        BasicExprToPolishStrParser(rightIfExpr, 2);

    switch (symbol)
    {
    case '>':
    {
        int jnegStr, jzStr;
        assStrings.push_back(std::to_string(assStrNum) + " LOAD " + "95");
        assStrings.push_back(std::to_string(assStrNum + 1) + " SUB " + "96");
        assStrings.push_back(std::to_string(assStrNum + 2) + " JNEG "); //+ std::to_string(assStrNum + 5));
        jnegStr = assStrings.size() - 1;
        assStrings.push_back(std::to_string(assStrNum + 3) + " JZ "); //+ std::to_string(assStrNum + 5));
        jzStr = jnegStr + 1;
        assStrNum += 4;

        offset = position2;
        assStrings.push_back(std::to_string(assStrNum));
        checkComand();

        assStrings[jnegStr] += std::to_string(assStrNum);
        assStrings[jzStr] += std::to_string(assStrNum);
        break;
    }
    case '<':
    {
        int jnegStr, jzStr;
        assStrings.push_back(std::to_string(assStrNum) + " LOAD " + "96");
        assStrings.push_back(std::to_string(assStrNum + 1) + " SUB " + "95");
        assStrings.push_back(std::to_string(assStrNum + 2) + " JNEG "); //+ std::to_string(assStrNum + 5));
        jnegStr = assStrings.size() - 1;
        assStrings.push_back(std::to_string(assStrNum + 3) + " JZ "); //+ std::to_string(assStrNum + 5));
        jzStr = jnegStr + 1;
        assStrNum += 4;

        offset = position2;
        assStrings.push_back(std::to_string(assStrNum));
        checkComand();

        assStrings[jnegStr] += std::to_string(assStrNum);
        assStrings[jzStr] += std::to_string(assStrNum);
        break;
    }
    case '=':
    {
        int jumpStr;
        if (rightIfExpr == "0")
        {
            assStrings.push_back(std::to_string(assStrNum) + " LOAD " + "95");
            ++assStrNum;
        }
        else
        {
            assStrings.push_back(std::to_string(assStrNum) + " LOAD " + "96");
            assStrings.push_back(std::to_string(assStrNum + 2) + " SUB " + "95");
            assStrNum += 2;
        }
        assStrings.push_back(std::to_string(assStrNum) + " JZ " + std::to_string(assStrNum + 2));
        assStrings.push_back(std::to_string(assStrNum + 1) + " JUMP "); //+ std::to_string(assStrNum + 3));
        jumpStr = assStrings.size() - 1;
        assStrNum += 2;

        offset = position2;
        assStrings.push_back(std::to_string(assStrNum));
        checkComand();

        assStrings[jumpStr] += std::to_string(assStrNum);
        break;
    }
    default:
        std::cout << "at line " << basicStrNum << "\n\tUncknown symbol \"" << symbol
                  << "\":" << basicStrings[basicStrNum] << std::endl;
        return -1;
    }

    // std::cout << basicStrings[basicStrNum][position2 + 1] << std::endl;

    std::cout << leftIfExr << std::endl;
    std::cout << symbol << std::endl;
    std::cout << rightIfExpr << std::endl;

    return 0;
}

int BasicTr::parsing()
{
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

    int order;
    for (int i = 24; i > -1; --i)
    {
        order = variableCell[i];
        if (order != -1)
        {
            assStrings.push_back(std::to_string(order) + " = +0000");
            ++assStrNum;
        }
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

    for (int i = 0; i < 25; ++i)
        variableCell[i] = -1;

    // std::cout << "1111\n";
    // BasicExprToPolishStrParser("A+B", 1);
    // std::cout << "2222\n";
    // BasicExprToPolishStrParser("0", 2);
    // std::cout << "3333\n";

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