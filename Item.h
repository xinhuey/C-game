#ifndef ITEM_H
#define ITEM_H

#include "Unit.h"

class Item : public Unit
{
private:
    bool isUsed;

public:
    Item(int row, int col, char symbol, std::string type);
    bool getIsUsed();
    void setIsUsed(bool isUsed);
};

#endif
