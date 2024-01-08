#include "Potion.h"
#include <string>

using namespace std;

Potion::Potion(int row, int col, int effect, PotionType type)
    : Item(row, col, 'P', "Potion"), effect(effect), potionType(type) { }

int Potion::getEffect() {
    return effect;
}

string Potion::getPotionType() {
    if(potionType == PotionType::Health) {
        return "Health";
    } else if(potionType == PotionType::Attack) {
        return "Attack";
    } else if(potionType == PotionType::Defence) {
        return "Defence";
    }
}

RestoreHealthPotion::RestoreHealthPotion(int row, int col)
    : Potion(row, col, 10, PotionType::Health) { }

PoisonHealthPotion::PoisonHealthPotion(int row, int col)
    : Potion(row, col, -10, PotionType::Health) { }

BoostAttackPotion::BoostAttackPotion(int row, int col)
    : Potion(row, col, 5, PotionType::Attack) { }

WoundAttackPotion::WoundAttackPotion(int row, int col)
    : Potion(row, col, -5, PotionType::Attack) { }

BoostDefencePotion::BoostDefencePotion(int row, int col)
    : Potion(row, col, 5, PotionType::Defence) { }

WoundDefencePotion::WoundDefencePotion(int row, int col)
    : Potion(row, col, -5, PotionType::Defence) { }


string RestoreHealthPotion::getPotionAbbreviation() {
    return "RH";
}

string PoisonHealthPotion::getPotionAbbreviation() {
    return "PH";
}

string BoostAttackPotion::getPotionAbbreviation() {
    return "BA";
}

string WoundAttackPotion::getPotionAbbreviation() {
    return "WA";
}

string BoostDefencePotion::getPotionAbbreviation() {
    return "BD";
}

string WoundDefencePotion::getPotionAbbreviation() {
    return "WD";
}


