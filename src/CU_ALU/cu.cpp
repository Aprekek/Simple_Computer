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
        //alu
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
            ui->computer->memorySet(operand, value);
            MyKeyBoard::switchToRaw();
            break;
        }
        case WRITE:
        {
            ui->computer->memoryGet(operand, value);
            std::cout << "Value: " << std::hex << value;
            getchar();
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
        }
    }

    ui->instrCounter++;
    ui->instrCounter %= 100;

    return 0;
}