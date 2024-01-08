#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Treasure.h"

class Enemy : public Character
{
private:
    int goldDrop;
    int chamber;

public:
    Enemy(int row, int col, char symbol, std::string type, int hp, int atk, int def, int goldDrop);
    virtual void moveRandomly();
    int getGoldDrop();
    virtual void attackPlayer(int row, int col);
    virtual void dies() override;
    std::pair<int, int> getPlayerInRange();
};

class Human : public Enemy
{
public:
    Human(int row, int col);
    void dies() override;
};

class Dwarf : public Enemy
{
public:
    Dwarf(int row, int col);
};

class Elf : public Enemy
{
public:
    Elf(int row, int col);
    void attackPlayer(int row, int col) override;
};

class Orc : public Enemy
{
public:
    Orc(int row, int col);
    void attackPlayer(int row, int col) override;
};

class Merchant : public Enemy
{
private:
    static bool isHostile;
public:
    Merchant(int row, int col);
    void attackPlayer(int row, int col) override;
    void dies() override;
    static void setHostile(bool hostile);
    static bool getIsHostile();
    void takeDamage(int damage) override;
};

class Halfling : public Enemy
{
public:
    Halfling(int row, int col);
};

class Dragon : public Enemy
{
private:
    Gold *hoard;
public:
    void dies() override;

    Dragon(int row, int col, Gold *hoard = nullptr);
    Gold *getHoard();
    std::pair<int, int> getPlayerNearHoard();
    void attackPlayer(int row, int col) override;
    void moveRandomly() override;
};


#endif

