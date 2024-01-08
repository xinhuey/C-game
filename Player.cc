#include "Player.h"
#include <string>
#include "Board.h"
#include "Floor.h"
#include "Potion.h"
#include "PotionType.h"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

Player::Player(int row, int col, char symbol, string type, int hp, int atk, int def)
    : Character(row, col, symbol, type, hp, atk, def), gold(0), initAtk(atk), initDef(def), maxHP(hp), onDragonHoard(false) { 
        Board::getInstance()->setCurrentAction("Player character has spawned.");
    }

bool Player::usePotion(int row, int col) {
    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
    vector<vector<char>> defaultLayout = Board::getInstance()->getDefaultFloorLayout();
    auto iter = currFloor->potions.find(make_pair(row, col));

    if(iter == currFloor->potions.end()) {
        return false;
    }

    if(iter->second->getPotionType() == "Health") {
        updateHp(iter->second->getEffect());
    } else if (iter->second->getPotionType() == "Attack") {
        updateAtk(iter->second->getEffect());
    } else if (iter->second->getPotionType() == "Defence") {
        updateDef(iter->second->getEffect());
    }

    Board::getInstance()->setCurrentAction("PC uses " + iter->second->getPotionAbbreviation() + ".");
    currLayout[row][col] = '.';
    currFloor->potions.erase(iter);
    return true;
}

bool Drow::usePotion(int row, int col) {
    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
    vector<vector<char>> defaultLayout = Board::getInstance()->getDefaultFloorLayout();
    auto iter = currFloor->potions.find(make_pair(row, col));

    if(iter == currFloor->potions.end()) {
        return false;
    }

    int effect = ceil(iter->second->getEffect() * 1.5);
    if(iter->second->getPotionType() == "Health") {
        updateHp(effect);
    } else if (iter->second->getPotionType() == "Attack") {
        updateAtk(effect);
    } else if (iter->second->getPotionType() == "Defence") {
        updateDef(effect);
    }

    Board::getInstance()->setCurrentAction("PC uses " + iter->second->getPotionAbbreviation() + ".");

    currLayout[row][col] = '.';
    currFloor->potions.erase(iter);
    return true;
}


void Player::addGold(int gold) {
    Board::getInstance()->setCurrentAction("Player character picks up " + to_string(gold) + " gold. ");
    this->gold += gold;
}

bool Player::attack(int row, int col) {
    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
    vector<vector<char>> defaultLayout = Board::getInstance()->getDefaultFloorLayout();

    if(!(currLayout[row][col] == 'H' || currLayout[row][col] == 'W' || currLayout[row][col] == 'E' || currLayout[row][col] == 'O' || currLayout[row][col] == 'M' || currLayout[row][col] == 'D' || currLayout[row][col] == 'L')) {
        return false;
    }

    // halfling makes player miss 50% of the time
    if(currLayout[row][col] == 'L') {
        int hitOrMiss = rand() % 2;
        if(hitOrMiss == 0) {
            cout << "Player missed" << endl;
            return false;
        }
    }

    
    auto iter = currFloor->enemies.find(make_pair(row, col));

    if(iter == currFloor->enemies.end()) {
        return false;
    }

    int damage = ceil((100.0 / (100 + iter->second->getDef())) * getAtk());
    iter->second->takeDamage(damage);
    Board::getInstance()->setCurrentAction("PC attacks " + iter->second->getType() + " with " + to_string(damage) + " damage." + " " + Board::getInstance()->getCurrentAction());

    return true;
}


bool Player::pickUpGold(int row, int col) {
    
    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
    vector<vector<char>> defaultLayout = Board::getInstance()->getDefaultFloorLayout();

    // update the player's gold
    auto iter = currFloor->treasures.find(make_pair(row, col));

    if(iter->second->getIsPickable() == false) {
        Board::getInstance()->setCurrentAction("Dragon guarding the hoard, PC can't pick up gold. ");
        currLayout[row][col] = '@';

        if(onDragonHoard == true) {
            currLayout[getRow()][getCol()] = 'G';
        } else {
            currLayout[getRow()][getCol()] = '.';
        }
        onDragonHoard = true;
        setRow(row);
        setCol(col);
        return true;
    }

    onDragonHoard = false;
    currLayout[row][col] = '@';
    currLayout[getRow()][getCol()] = defaultLayout[getRow()][getCol()];
    Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + "PC picks up " + to_string(iter->second->getValue()) + " gold. ");
    // update the player's position
    setRow(row);
    setCol(col);

    gold += iter->second->getValue();
    currFloor->treasures.erase(iter);
    return true;
}

void Player::updateHp(int effect) {
    if(effect < 0) {
        if((getHp() + effect) <= 0) {
            setHp(getHp()+effect);
            dies();
        } else {
            setHp(getHp() + effect);
        }
    } else {
        if(getHp() + effect > maxHP) {
            setHp(maxHP);
        } else {
            setHp(getHp() + effect);
        }
    }
}

void Vampire::updateHp(int effect) {
    if(effect < 0) {
        if(getHp() + effect <= 0) {
            setHp(getHp()+effect);
            dies();
        } else {
            setHp(getHp() + effect);
        }
    } else {
        setHp(getHp() + effect);
    }
}

bool Vampire::attack(int row, int col) {
    if(Player::attack(row, col)) {

        // if the enemy is a dwarf, vampire loses 5 hp
        if(Board::getInstance()->getCurrentFloor()->floorLayout[row][col] == 'W') {
            cout << "Vampire attacks dwarf and loses 5 hp" << endl;
            updateHp(-5);
        } else {
            cout << "Vampire gains 5 hp for succesful attack" << endl;
            updateHp(5);
        }
        return true;
    } else {
        return false;
    }
}

bool Goblin::attack(int row, int col) {
    if(Player::attack(row, col)) {
        if(Board::getInstance()->getCurrentFloor()->enemies.find(make_pair(row, col)) == Board::getInstance()->getCurrentFloor()->enemies.end()) {
            cout << "Goblin steals 5 gold from slain enemy" << endl;
            addGold(5);
        }
        return true;
    } else {
        return false;
    }
}

void Player::displayScore() {
    if(getHp()<=0){
        cout << "You lose! Score: "<<getGold()<< endl;
        return;
    }
    cout << "You win! Score: " << getGold() << endl;
}

void Shade::displayScore() {
    int score = getGold() * 1.5;
    if(getHp()<=0){
        cout << "You lose! Score: "<< score << endl;
        return;
    }
    cout << "You win! Score: " << score << endl;
}

void Player::dies() {
    displayScore();
    exit(0);
}

void Player::removeTempEffects() {
    setAtk(initAtk);
    setDef(initDef);
}

bool Player::move(int newRow, int newCol) {

    vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
    vector<vector<char>> defaultLayout = Board::getInstance()->getDefaultFloorLayout();

    if(currLayout[newRow][newCol] == '.' || currLayout[newRow][newCol] == '+' || currLayout[newRow][newCol] == '#') {
        // update the floor layout
        currLayout[newRow][newCol] = '@';

        if(onDragonHoard == true) {
            currLayout[getRow()][getCol()] = 'G';
            onDragonHoard = false;
        } else {
            currLayout[getRow()][getCol()] = defaultLayout[getRow()][getCol()];
        }

        // update the player's position
        setRow(newRow);
        setCol(newCol);

        return true;
    } else if (currLayout[newRow][newCol] == 'G') {
        return pickUpGold(newRow, newCol);
    } else if (currLayout[newRow][newCol] == '\\') {
        Board::getInstance()->moveToNextFloor();
        return true;
    }

    return false;
}

int Player::getGold() {
    return gold;
}



Shade::Shade(int row, int col) : Player(row, col, '@', "Shade", 125, 25, 25) {}

Drow::Drow(int row, int col) : Player(row, col, '@', "Drow", 150, 25, 15) {}

Vampire::Vampire(int row, int col) : Player(row, col, '@', "Vampire", 50, 25, 25) {}

Troll::Troll(int row, int col) : Player(row, col, '@', "Troll", 120, 25, 15) {}

Goblin::Goblin(int row, int col) : Player(row, col, '@', "Goblin", 110, 15, 20) {}
