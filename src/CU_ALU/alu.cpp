#include "includes/alu.h"

int ALU::execute(const int &comand, const int &operand, s_computerUI *ui)
{
    int value;
    switch (comand)
    {
    case ADD:
    {
        ui->computer->memoryGet(operand, value);
        ui->accumulator += value;
        break;
    }
    case SUB:
    {
        ui->computer->memoryGet(operand, value);
        ui->accumulator -= value;
        break;
    }
    case DIVIDE:
    {
        ui->computer->memoryGet(operand, value);
        ui->accumulator /= value;
        break;
    }
    case MUL:
    {
        ui->computer->memoryGet(operand, value);
        ui->accumulator *= value;
        break;
    }
    case SHR:
    {
        ui->computer->memoryGet(operand, value);
        ui->accumulator = value >> 1;
        break;
    }
    }
}