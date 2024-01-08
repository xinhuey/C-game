#include "Character.h"
#include <string>
#include <iostream>

using namespace std;

Character::Character(int row, int col, char symbol, std::string type, int hp, int atk, int def) : Unit(row, col, symbol, type)
{
    this->hp = hp;
    this->atk = atk;
    this->def = def;
    isDead = false;
}

void Character::setIsDead(bool isDead)
{
    this->isDead = isDead;
} 

void Character::setHp(int hp)
{
    this->hp = hp;
}

void Character::setAtk(int atk) {
    this->atk = atk;
}

void Character::setDef(int def) {
    this->def = def;
}

int Character::getHp()
{
    return hp;
}

int Character::getAtk()
{
    return atk;
}

int Character::getDef()
{
    return def;
}

bool Character::getIsDead()
{
    return isDead;
}

void Character::takeDamage(int damage) {
    updateHp(-damage);
}

void Character::updateHp(int hp)
{
    this->hp += hp;

    if(this->hp <= 0)
    {
        dies();
    }
    
}

void Character::updateAtk(int atk)
{
    if(this->atk + atk < 0)
    {
        this->atk = 0;
    }
    else
    {
        this->atk += atk;
    }
}

void Character::updateDef(int def)
{
    if(this->def + def < 0)
    {
        this->def = 0;
    }
    else
    {
        this->def += def;
    }
}
