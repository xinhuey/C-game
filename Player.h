#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character
{
private:
    int gold; // dont need score as an attribute, can just calculate it from gold
    int initAtk;
    int initDef; // to keep track of potions consumed on floor
    const int maxHP; // we set this at initialization and never change it
    bool onDragonHoard;

public:
    Player(int row, int col, char symbol, std::string type, int hp, int atk, int def);
    int getGold();
    void updateHp(int effect) override;
    void removeTempEffects();
    void addGold(int gold);
    virtual bool attack(int row, int col); 
    bool move(int row, int col); // returns true if the move was successful, false otherwise
    virtual bool usePotion(int row, int col); // returns true if the potion was used, false otherwise
    bool pickUpGold(int row, int col); // returns true if the gold was picked up, false otherwise
    virtual void displayScore();
    virtual void dies();
};

class Shade : public Player
{
public:
    Shade(int row, int col);
    void displayScore() override;
};

class Drow : public Player
{
public:
    Drow(int row, int col);
    bool usePotion(int row, int col) override;
};

class Vampire : public Player
{   
public:
    Vampire(int row, int col);
    void updateHp(int effect) override;
    bool attack(int row, int col) override;
};

class Troll : public Player
{
public:
    Troll(int row, int col);
};

class Goblin : public Player
{
public:
    Goblin(int row, int col);
    bool attack(int row, int col) override;
};


#endif
