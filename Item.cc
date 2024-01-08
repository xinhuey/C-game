#include "Item.h"

Item::Item(int row, int col, char symbol, std::string type)
    : Unit(row, col, symbol, type), isUsed(false) { }

bool Item::getIsUsed() {
    return isUsed;
}

void Item::setIsUsed(bool isUsed) {
    this->isUsed = isUsed;
}
