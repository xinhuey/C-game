#ifndef POTION_H
#define POTION_H

#include "PotionType.h"
#include "Item.h"
#include <string>

class Potion : public Item
{
private:
    int effect;
    PotionType potionType;

public:
    Potion(int row, int col, int effect, PotionType type);
    int getEffect();
    std::string getPotionType();
    virtual std::string getPotionAbbreviation() = 0;
};

class RestoreHealthPotion : public Potion
{
public:
    RestoreHealthPotion(int row, int col);
    std::string getPotionAbbreviation() override;
};  

class PoisonHealthPotion : public Potion
{
public:
    PoisonHealthPotion(int row, int col);
    std::string getPotionAbbreviation() override;
};

class BoostAttackPotion : public Potion
{
public:
    BoostAttackPotion(int row, int col);
    std::string getPotionAbbreviation() override;
};

class WoundAttackPotion : public Potion
{
public:
    WoundAttackPotion(int row, int col);
    std::string getPotionAbbreviation() override;
};

class BoostDefencePotion : public Potion
{
public:
    BoostDefencePotion(int row, int col);
    std::string getPotionAbbreviation() override;
};

class WoundDefencePotion : public Potion
{
public:
    WoundDefencePotion(int row, int col);
    std::string getPotionAbbreviation() override;
};






#endif
