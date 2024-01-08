#ifndef FLOOR_H
#define FLOOR_H

#include "Player.h"
#include "Enemy.h"
#include "Potion.h"
#include "Treasure.h"
#include "Unit.h"
#include "Chamber.h"
#include "Item.h"

#include <map>
#include <vector>
#include <string>
#include <utility> 



// original design uses Singleton, but our intention was to keep track of Floors using the Board class. Hence we don't need Singleton for Floor and don't need methods like getCurrentFloor()
// at the moment

class Floor {
private:
    Player *player;
    int enemyCount;
    int potionCount;
    int treasureCount;

public:
    std::vector<std::vector<char>> floorLayout;
    std::vector<Chamber*> chambers;
    std::map<std::pair<int, int>, Gold*> treasures;
    std::map<std::pair<int, int>, Potion*> potions;
    std::map<std::pair<int, int>, Enemy*> enemies;
    std::pair<int, int> stairsPos;

    Floor();
    Floor(std::vector<std::vector<char>> floorLayout);
    ~Floor();

    void Display();
    void DisplayStats();
    std::vector<int> getRandomPosition(); // returns chamber index, and coordinate within chamber
    void update();
    void spawnPlayer();

    std::vector<std::vector<std::pair<int, int>>> findChambers(std::vector<std::vector<char>> floorLayout);

    void testChambers();
    void initChambers();
    void applyCustomChamberLayout();
    void spawnStair();
    void spawnStairAt(int row, int col);
    void spawnGold();
    void spawnGoldAt(int row, int col, int value);
    void spawnPotions();
    void spawnDragonAround(int row, int col, Gold* hoard);

    void spawnEnemies();
    void spawnPotionAt(int row, int col, int potionType);
    void spawnTreasureAt(int row, int col, int treasureType);
    void spawnEnemyAt(int row, int col, char enemyType);

    void updateEnemies();
    std::pair<int, int> getRandomEmptyPositionAround(int row, int col);
};

#endif
