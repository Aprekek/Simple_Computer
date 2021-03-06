#include "includes/cu.h"

CU::CU()
{
    ui = nullptr;
}

void CU::set(s_computerUI *inst)
{
    if (inst == nullptr)
        throw(std::string)("UI isnt defined");

    ui = (s_computerUI *)inst->getInstance();
}

int CU::execute()
{
    int value, comand, operand;

    ui->computer->memoryGet(ui->instrCounter, value);
    if (!ui->computer->commandDecode(value, comand, operand))
    {
        return -1;
    }

    if ((comand >= 0x30 && comand <= 0x33) || comand == 0x61)
    {
        ALU::execute(comand, operand, ui);
    }
    else
    {
        switch (comand)
        {
        case READ:
        {
            MyKeyBoard::switchToCanon();
            std::cout << "Enter value: ";
            std::cin >> std::hex >> value;
            std::cout << std::dec; //
            ui->computer->memorySet(operand, value);
            MyKeyBoard::switchToRaw();
            break;
        }
        case WRITE:
        {
            ui->computer->memoryGet(operand, value);
            sprintf(ui->outputAnswer, "%x", value);
            break;
        }
        case LOAD:
        {
            ui->computer->memoryGet(operand, ui->accumulator);
            break;
        }
        case STORE:
        {
            ui->computer->memorySet(operand, ui->accumulator);
            break;
        }
        case JUMP:
        {
            ui->instrCounter = operand - 1;
            break;
        }
        case JNEG:
        {
            if (ui->accumulator < 0)
                ui->instrCounter = operand - 1;
            break;
        }
        case JZ:
        {
            if (ui->accumulator == 0)
                ui->instrCounter = operand - 1;
            break;
        }
        case HALT:
        {
            ui->instrCounter = operand;
            if (ui->termRun)
            {
                ui->timerIncr();
                flushSTDIN();
            }
            Terminal::setFgColor(Terminal::FG_BLUE);
            std::cout << "Programm is finished\nPress enter to continue\n";
            Terminal::setFgColor(Terminal::FG_DEFAULT);
            flushSTDIN();
            getchar();
            return 0;
        }
        }
    }

    ui->instrCounter++;
    ui->instrCounter %= 100;

    return 0;
}