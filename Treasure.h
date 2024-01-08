#ifndef TREASURE_H
#define TREASURE_H

#include "Item.h"

class Treasure : public Item
{
private:
    const int value;

public:
    Treasure(int row, int col, char symbol, std::string type, const int value);
    int getValue();
};

class Gold : public Treasure
{
public:
    bool isPickable;
    Gold(int row, int col, int value, bool isPickable = true);
    bool getIsPickable();
    void makePickable();
};

#endif
