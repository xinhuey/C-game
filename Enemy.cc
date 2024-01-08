#include "Enemy.h"
#include "Board.h"
#include "Floor.h"
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <cmath>


using namespace std;

Enemy::Enemy(int row, int col, char symbol, string type, int hp, int atk, int def, int goldDrop) 
: Character(row, col, symbol, type, hp, atk, def), goldDrop(goldDrop) {}


int Enemy::getGoldDrop() {
    return goldDrop;
}

void Enemy::dies() {
    if(getHp() <= 0) {
        Floor* currFloor = Board::getInstance()->getCurrentFloor();
        vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
        int drop = getGoldDrop();
        // other enemies directly deposit gold into player's gold count
        currLayout[getRow()][getCol()] = '.';
        Board::getInstance()->getPlayer()->addGold(drop);
        pair<int, int> enemyPos = make_pair(getRow(), getCol());
        currFloor->enemies.erase(enemyPos);
    }
}

// Humans and Merchants drop gold on map
void Human::dies() {
    if(getHp() <= 0) {
        Floor* currFloor = Board::getInstance()->getCurrentFloor();
        vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
        int drop = getGoldDrop();
        currFloor->spawnGoldAt(getRow(), getCol(), drop);
        pair<int, int> enemyPos = make_pair(getRow(), getCol());
        currFloor->enemies.erase(enemyPos);
    }
}

bool Merchant::isHostile = false;

void Merchant::dies() {
    if(getHp() <= 0) {
        Floor* currFloor = Board::getInstance()->getCurrentFloor();
        vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
        int drop = getGoldDrop();
        currFloor->spawnGoldAt(getRow(), getCol(), drop);
        pair<int, int> enemyPos = make_pair(getRow(), getCol());
        currFloor->enemies.erase(enemyPos);
        setHostile(true);
    }
}

void Merchant::attackPlayer(int row, int col) {
    if(getIsHostile()) {
        Enemy::attackPlayer(row, col);
    }
}

void Merchant::takeDamage(int damage) {
    Enemy::takeDamage(damage);
    setHostile(true);
}

void Merchant::setHostile(bool hostile) {
    isHostile = hostile;
}

bool Merchant::getIsHostile() {
    return isHostile;
}

void Dragon::dies() {
    if(getHp() <= 0) {
        Floor* currFloor = Board::getInstance()->getCurrentFloor();
        vector<vector<char>> &currLayout = Board::getInstance()->getCurrentFloor()->floorLayout;
        currLayout[getRow()][getCol()] = '.';
        Gold* hoard = getHoard();
        hoard->makePickable();
        pair<int, int> enemyPos = make_pair(getRow(), getCol());
        currFloor->enemies.erase(enemyPos);
    }
}

pair<int, int> Enemy::getPlayerInRange() {
    // get current floor and layout
    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    vector<vector<char>> &currLayout = currFloor->floorLayout;

    // directions to check around the enemy
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (const auto& dir : directions) {
        int checkRow = getRow() + dir.first;
        int checkCol = getCol() + dir.second;
        // if player is within 1 block radius
        if (currLayout[checkRow][checkCol] == '@') {
            return {checkRow, checkCol};
        }
    }
    // return sentinel value if player not found
    return {-1, -1};
}

void Enemy::attackPlayer(int row, int col) {
    cout << getSymbol() << " attempts to attack player" << endl;
    int hitOrMiss = rand() % 2;  

    if (hitOrMiss == 1) {  
        Player *player = Board::getInstance()->getPlayer();
        int damage = ceil((100.0 / (100 + player->getDef())) * getAtk());
        player->takeDamage(damage);
        Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + " " + getType() + " deals " + to_string(damage) + " damage" + " to PC.");
    } else { 
        cout << getSymbol() << " misses player" << endl;
    }
}

void Orc::attackPlayer(int row, int col) {
    cout << getSymbol() << " attempts to attack player" << endl;
    int hitOrMiss = rand() % 2;  

    if (hitOrMiss == 1) {  
        Player *player = Board::getInstance()->getPlayer();
        if(player->getType() == "Goblin") {
            cout << "Orc attacks Goblin" << endl;
            int damage = ceil((100.0 / (100 + player->getDef())) * getAtk() * 1.5);
            player->takeDamage(damage);
            Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + " " + getType() + " deals " + to_string(damage) + " damage" + " to PC.");
            cout << getSymbol() << " hits player for " << damage << " damage" << endl;
        } else {
            int damage = ceil((100.0 / (100 + player->getDef())) * getAtk());
            player->takeDamage(damage);
            Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + " " + getType() + " deals " + to_string(damage) + " damage" + " to PC.");
            cout << getSymbol() << " hits player for " << damage << " damage" << endl;
        }
    } else { 
        cout << getSymbol() << " misses player" << endl;
    }
}

void Elf::attackPlayer(int row, int col) {
    Player *player = Board::getInstance()->getPlayer();
    cout << getSymbol() << " attempts to attack player" << endl;

    // Determine how many attacks based on the player's type
    int numAttacks = (player->getType() == "Drow") ? 1 : 2;

    for (int i = 0; i < numAttacks; ++i) {
        int hitOrMiss = rand() % 2;  

        if (hitOrMiss == 1) {  
            int damage = ceil((100.0 / (100 + player->getDef())) * getAtk());
            player->takeDamage(damage);
            Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + " " + getType() + " deals " + to_string(damage) + " damage" + " to PC.");
            if (player->getHp() > 0) {
            } else {
                break;  // break the loop if the player is dead
            }
        } else { 
            cout << getSymbol() << " misses player" << endl;
        }
    }
}

void Enemy::moveRandomly() {

    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    vector<vector<char>> &currLayout = currFloor->floorLayout;
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};


    while(true) {
        int randomDirectionIndex = rand() % directions.size();
        if(currLayout[getRow() + directions[randomDirectionIndex].first][getCol() + directions[randomDirectionIndex].second] == '.') {
            
            currFloor->enemies.erase(make_pair(getRow(), getCol()));
            currFloor->enemies[make_pair(getRow() + directions[randomDirectionIndex].first, getCol() + directions[randomDirectionIndex].second)] = this;
            
            currLayout[getRow()][getCol()] = '.';
            setRow(getRow() + directions[randomDirectionIndex].first);
            setCol(getCol() + directions[randomDirectionIndex].second);
            currLayout[getRow()][getCol()] = getSymbol();
            break;
        }
    }
}

Human::Human(int row, int col) 
: Enemy(row, col, 'H', "Human", 140, 20, 20, 4) {}

Dwarf::Dwarf(int row, int col)
: Enemy(row, col, 'W', "Dwarf", 100, 20, 30, 2) {}

Elf::Elf(int row, int col)
: Enemy(row, col, 'E', "Elf", 140, 30, 10, 2) {}

Orc::Orc(int row, int col)
: Enemy(row, col, 'O', "Orc", 180, 30, 25, 2) {}

Merchant::Merchant(int row, int col)
: Enemy(row, col, 'M', "Merchant", 30, 70, 5, 4) {}

Halfling::Halfling(int row, int col)
: Enemy(row, col, 'L', "Halfling", 100, 15, 20, 2) {}

Dragon::Dragon(int row, int col, Gold* hoard)
: Enemy(row, col, 'D', "Dragon", 150, 20, 20, 6), hoard(hoard) {}

void Dragon::moveRandomly() {
    return;
}

void Dragon::attackPlayer(int row, int col) {
    cout << getSymbol() << " attempts to attack player" << endl;
    int hitOrMiss = rand() % 2;  

    if (hitOrMiss == 1) {  
        Player *player = Board::getInstance()->getPlayer();
        int damage = ceil((100.0 / (100 + player->getDef())) * getAtk());
        player->takeDamage(damage);
        Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + " " + getType() + " deals " + to_string(damage) + " damage" + " to PC.");
        // cout << getSymbol() << " hits player for " << damage << " damage" << endl;
    } else { 
        cout << getSymbol() << " misses player" << endl;
    }
}

pair<int, int> Dragon::getPlayerNearHoard() {
    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    vector<vector<char>> &currLayout = currFloor->floorLayout;

    int hoardRow, hoardCol;

    if(getHoard() == nullptr) {
        return {-1, -1};
    } else {
        hoardRow = getHoard()->getRow();
        hoardCol = getHoard()->getCol();
    }

    // directions to check around the enemy
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (const auto& dir : directions) {
        int checkRow = hoardRow + dir.first;
        int checkCol = hoardCol + dir.second;
        // if player is within 1 block radius
        if (currLayout[checkRow][checkCol] == '@') {
            return {checkRow, checkCol};
        }
    }
    // return sentinel value if player not found
    return {-1, -1};
}

Gold* Dragon::getHoard() {
    return hoard;
}

