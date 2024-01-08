#include "Unit.h"

Unit::Unit(int row, int col, char symbol, std::string type)
{
    this->row = row;
    this->col = col;
    this->symbol = symbol;
    this->type = type;
}

void Unit::setRow(int row)
{
    this->row = row;
}  

void Unit::setCol(int col)
{
    this->col = col;
}

int Unit::getRow()
{
    return row;
}

int Unit::getCol()
{
    return col;
}

char Unit::getSymbol()
{
    return symbol;
}

std::string Unit::getType()
{
    return type;
}
