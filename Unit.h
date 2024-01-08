#ifndef UNIT_H
#define UNIT_H

#include <string>

class Unit
{
private:
    int row;
    int col;
    char symbol;
    std::string type;

public:
    Unit(int row, int col, char symbol, std::string type);
    void setRow(int row);
    void setCol(int col);
    int getRow();
    int getCol();
    char getSymbol();
    std::string getType();
};

#endif
