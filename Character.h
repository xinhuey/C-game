#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Unit.h"

class Character : public Unit
{
private:
    int hp;
    int atk;
    int def;
    bool isDead;

public:
    Character(int row, int col, char symbol, std::string type, int hp, int atk, int def);
    void setIsDead(bool isDead);
    void setHp(int hp);
    void setAtk(int atk);
    void setDef(int def);
    int getHp();
    int getAtk();
    int getDef();
    bool getIsDead();
    void virtual takeDamage(int damage);
    virtual void updateHp(int hp);
    void updateAtk(int atk);
    void updateDef(int def);
    virtual void dies() = 0; // see Enemy and Character for why this needs to be virtual
};

#endif
