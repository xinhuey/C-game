#include "Treasure.h"
#include <string>

using namespace std;

Treasure::Treasure(int row, int col, char symbol, string type, int value)
    : Item(row, col, symbol, type), value(value) { }

int Treasure::getValue() {
    return value;
}

Gold::Gold(int row, int col, int value, bool isPickable)
    : Treasure(row, col, 'G', "Gold", value), isPickable(isPickable) { }

bool Gold::getIsPickable() {
    return isPickable;
}

void Gold::makePickable() {
    isPickable = true;
}
