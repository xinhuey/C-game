#include "Floor.h"
#include "Board.h"
#include "Potion.h"
#include <iostream>
#include <utility>
#include <cstdlib>
#include "Enemy.h"
#include "Treasure.h"
#include <unordered_map>
#include <set>

using namespace std;

Floor::Floor(vector<vector<char>> floorLayout) : enemyCount(0), potionCount(0), treasureCount(0), floorLayout(floorLayout) {
    player = Board::getInstance()->getPlayer();
    initChambers();
    // Check to see if player is already on the floor
    bool hasPlayer = false;
    for(int row = 0; row < floorLayout.size(); ++row) {
        for(int col = 0; col < floorLayout[row].size(); ++col) {
            if(floorLayout[row][col] == '@') {
                player->setRow(row);
                player->setCol(col);
                hasPlayer = true;
            }
        }
    }
    if (hasPlayer == false) {
        spawnPlayer();
    }
    applyCustomChamberLayout();
}

pair<int, int> findDragonAround(int row, int col) {
    cout << "find dragon around" << endl;
    vector<vector<char>> floorLayout = Board::getInstance()->customFloorLayouts[Board::getInstance()->getCurrentFloorNum() - 1]; // get current floor
    for(int i = row - 1; i <= row + 1; i++) {
        for(int j = col - 1; j <= col + 1; j++) {
            if(floorLayout[i][j] == 'D') {
                auto enemyPos = make_pair(i, j);
                return enemyPos;
            }
        }
    }
    return make_pair(-1, -1);
}

void Floor::applyCustomChamberLayout() {
    for(int i = 0; i < 5; i++) {
        Chamber* chamber = chambers[i];
        vector<pair<int, int>> chamberCoordinates = chamber->getCoordinates();
        for(int j = 0; j < chamberCoordinates.size(); j++) {
            pair<int, int> coordinate = chamberCoordinates[j];
            char c = floorLayout[coordinate.first][coordinate.second];

            if(c == '.') {
                continue;
            }

            if(c == '\\') {
                spawnStairAt(coordinate.first, coordinate.second);
            }

            if(c == '@') {
                Board::getInstance()->getPlayer()->setRow(coordinate.first);
                Board::getInstance()->getPlayer()->setCol(coordinate.second);
            } else if (c >= '0' && c <= '5') {
                int potionType = c - '0';
                spawnPotionAt(coordinate.first, coordinate.second, potionType);
            } else if (c >= '6' && c <= '9') {
                int goldType = c - '0';

                if(goldType == 6) {
                    spawnGoldAt(coordinate.first, coordinate.second, 2);
                } else if (goldType == 7) {
                    spawnGoldAt(coordinate.first, coordinate.second, 1);
                } else if (goldType == 8) {
                    spawnGoldAt(coordinate.first, coordinate.second, 4);
                } else if (goldType == 9) {
                    spawnGoldAt(coordinate.first, coordinate.second, 6);
                    pair<int, int> dragonPos = findDragonAround(coordinate.first, coordinate.second);
                    enemies[dragonPos] = new Dragon(dragonPos.first, dragonPos.second, treasures[make_pair(coordinate.first, coordinate.second)]);
                }
            } else {
                if(c == 'D') {
                    continue; // ignore dragon -> they are handled above
                }
                spawnEnemyAt(coordinate.first, coordinate.second, c);
            }
        }

    }
}

Floor::Floor() : enemyCount(0), potionCount(0), treasureCount(0) {
    player = Board::getInstance()->getPlayer();
    floorLayout = Board::getInstance()->getDefaultFloorLayout();
    initChambers();
    spawnPlayer();
    spawnStair();
    spawnPotions();
    spawnGold();
    spawnEnemies();
}

void Floor::spawnPlayer() {
    vector<int> playerPos = getRandomPosition();
    chambers[playerPos[0]]->hasPlayer = true;
    player->setRow(playerPos[1]);
    player->setCol(playerPos[2]);
    floorLayout[playerPos[1]][playerPos[2]] = '@';
}

void Floor::spawnStair() {
    vector<int> stairsCoord = getRandomPosition();
    while (chambers[stairsCoord[0]]->hasPlayer && floorLayout[stairsCoord[1]][stairsCoord[2]] != '.') {
        stairsCoord = getRandomPosition();
    }
    floorLayout[stairsCoord[1]][stairsCoord[2]] = '\\';
    stairsPos = make_pair(stairsCoord[1], stairsCoord[2]);
}

void Floor::spawnStairAt(int row, int col) {
    floorLayout[row][col] = '\\';
    stairsPos = make_pair(row, col);
}


void Floor::spawnGold() {
    for(int i = 0; i < 10; i++) {
        vector<int> goldPos = getRandomPosition();
        while (floorLayout[goldPos[1]][goldPos[2]] != '.') {
            goldPos = getRandomPosition();
        }

        int goldType = rand() % 8;
        if (goldType < 5) {
            spawnGoldAt(goldPos[1], goldPos[2], 2); // normal hoard
        } else if (goldType < 7) {
            spawnGoldAt(goldPos[1], goldPos[2], 1); // small hoard
        } else {
            spawnGoldAt(goldPos[1], goldPos[2], 6); // dragon hoard
            Gold* hoard = treasures[make_pair(goldPos[1], goldPos[2])];
            spawnDragonAround(goldPos[1], goldPos[2], hoard);
        }
    }
}

void Floor::spawnDragonAround(int row, int col, Gold* hoard) {
    pair<int, int> dragonPos = getRandomEmptyPositionAround(row, col);
    floorLayout[dragonPos.first][dragonPos.second] = 'D';
    enemies[dragonPos] = new Dragon(dragonPos.first, dragonPos.second, hoard);
}

void Floor::spawnGoldAt(int row, int col, int value) {
    floorLayout[row][col] = 'G';
    pair<int, int> goldCoord = make_pair(row, col);
    if(value == 6) {
        treasures[goldCoord] = new Gold(goldCoord.first, goldCoord.second, value, false);
    } else {
        treasures[goldCoord] = new Gold(goldCoord.first, goldCoord.second, value, true);
    }
}

void Floor::spawnPotions() {
    for(int i = 0; i < 10; i++) {
        vector<int> potionPos = getRandomPosition();
        while (floorLayout[potionPos[1]][potionPos[2]] != '.') {
            potionPos = getRandomPosition();
        }
        int randPotionType = rand() % 6;
        spawnPotionAt(potionPos[1], potionPos[2], randPotionType);
    }
}


void Floor::spawnPotionAt(int row, int col, int potionType) {
    floorLayout[row][col] = 'P';
    switch(potionType) {
        case 0:
            potions[make_pair(row, col)] = new RestoreHealthPotion(row, col);
            break;
        case 1:
            potions[make_pair(row, col)] = new BoostAttackPotion(row, col);
            break;
        case 2:
            potions[make_pair(row, col)] = new BoostDefencePotion(row, col);
            break;
        case 3:
            potions[make_pair(row, col)] = new PoisonHealthPotion(row, col);
            break;
        case 4:
            potions[make_pair(row, col)] = new WoundAttackPotion(row, col);
            break;
        case 5:
            potions[make_pair(row, col)] = new WoundDefencePotion(row, col);
            break;
    }
}

void Floor::spawnTreasureAt(int row, int col, int treasureType) {
    floorLayout[row][col] = 'G';
    switch(treasureType) {
        case 6:
            treasures[make_pair(row, col)] = new Gold(row, col, 2, true);
            break;
        case 7:
            treasures[make_pair(row, col)] = new Gold(row, col, 1, true);
            break;
        case 8:
            treasures[make_pair(row, col)] = new Gold(row, col, 4, true);
            break;
        case 9:
            treasures[make_pair(row, col)] = new Gold(row, col, 6, false);
            break;
    }
}

void Floor::spawnEnemyAt(int row, int col, char enemyType) {
    floorLayout[row][col] = enemyType;
    if(enemyType == 'H') {
        enemies[make_pair(row, col)] = new Human(row, col);
    } else if(enemyType == 'W') {
        enemies[make_pair(row, col)] = new Dwarf(row, col);
    } else if(enemyType == 'E') {
        enemies[make_pair(row, col)] = new Elf(row, col);
    } else if(enemyType == 'O') {
        enemies[make_pair(row, col)] = new Orc(row, col);
    } else if(enemyType == 'M') {
        enemies[make_pair(row, col)] = new Merchant(row, col);
    } else if(enemyType == 'D') {
        enemies[make_pair(row, col)] = new Dragon(row, col);
    } else if(enemyType == 'L') {
        enemies[make_pair(row, col)] = new Halfling(row, col);
    }
}

void Floor::spawnEnemies() {
    for(int i = 0; i < 20; i++) {
        vector<int> enemyPos = getRandomPosition();
        while (floorLayout[enemyPos[1]][enemyPos[2]] != '.') {
            enemyPos = getRandomPosition();
        }

        pair<int, int> enemyCoord = make_pair(enemyPos[1], enemyPos[2]);
        int randEnemyType = rand() % 18;

        // Assign enemy type based on random number and provided ratio
        if(randEnemyType < 4) {
            // Human: 2/9 -> 4/18
            enemies[enemyCoord] = new Human(enemyCoord.first, enemyCoord.second);
        } else if(randEnemyType < 7) {
            // Dwarf: 3/18
            enemies[enemyCoord] = new Dwarf(enemyCoord.first, enemyCoord.second);
        } else if(randEnemyType < 12) {
            // Halfling: 5/18
            enemies[enemyCoord] = new Halfling(enemyCoord.first, enemyCoord.second);
        } else if(randEnemyType < 14) {
            // Elf: 1/9 -> 2/18
            enemies[enemyCoord] = new Elf(enemyCoord.first, enemyCoord.second);
        } else if(randEnemyType < 16) {
            // Orc: 1/9 -> 2/18
            enemies[enemyCoord] = new Orc(enemyCoord.first, enemyCoord.second);
        } else {
            // Merchant: 1/9 -> 2/18
            enemies[enemyCoord] = new Merchant(enemyCoord.first, enemyCoord.second);
        }

        floorLayout[enemyPos[1]][enemyPos[2]] = enemies[enemyCoord]->getSymbol();
    }
}

vector<int> Floor::getRandomPosition() {
    int chamberNum = rand() % 5;
    pair<int,int> randomCoord = chambers[chamberNum]->getRandomCoordinate();
    return {chamberNum, randomCoord.first, randomCoord.second};
}

pair<int, int> Floor::getRandomEmptyPositionAround(int row, int col) {
    vector<pair<int, int>> possiblePositions = {
        {row - 1, col},     
        {row + 1, col},     
        {row, col - 1},     
        {row, col + 1},     
        {row - 1, col - 1}, 
        {row - 1, col + 1}, 
        {row + 1, col - 1}, 
        {row + 1, col + 1}  
    };

    vector<pair<int, int>> emptyPositions;
    for (const auto& pos : possiblePositions) {
        if (pos.first >= 0 && pos.first < floorLayout.size() && 
            pos.second >= 0 && pos.second < floorLayout[0].size() &&
            floorLayout[pos.first][pos.second] == '.') {
            emptyPositions.push_back(pos);
        }
    }

    int randomIndex = rand() % emptyPositions.size();
    return emptyPositions[randomIndex];
}

bool isEnemyCharacter(char c) {
    switch(c) {
        case 'H': // Human
        case 'W': // Dwarf
        case 'E': // Elf
        case 'O': // Orc
        case 'M': // Merchant
        case 'D': // Dragon
        case 'L': // Halfling
            return true;
        default:
            return false;
    }
}

void Floor::updateEnemies() {

    if(Board::getInstance()->getIsPaused()) {
        return; // dont attack OR move
    }
    
    set<pair<int, int>> movedEnemiesPositions;

    for(int row = 0; row < floorLayout.size(); ++row) {
        for(int col = 0; col < floorLayout[row].size(); ++col) {
            char cell = floorLayout[row][col];

            if(isEnemyCharacter(cell)) {
                pair<int, int> pos = make_pair(row, col);
                
                // If this enemy has already moved, skip it
                if(movedEnemiesPositions.find(pos) != movedEnemiesPositions.end()) {
                    continue;
                }

                if(enemies.find(pos) != enemies.end()) {
                    Enemy* enemy = enemies[pos];

                    
                    if(enemy->getType() == "Dragon") {
                        // see if this works tmr
                        Dragon* dragon = static_cast<Dragon*>(enemy); // Cast to Dragon pointer to access Dragon-specific methods
                        pair<int, int> hoardPlayerPos = dragon->getPlayerNearHoard();
                        if(hoardPlayerPos.first != -1 && hoardPlayerPos.second != -1) {
                            dragon->attackPlayer(hoardPlayerPos.first, hoardPlayerPos.second);
                        } else {
                            pair<int, int> playerPos = dragon->getPlayerInRange();
                            if(playerPos.first != -1 && playerPos.second != -1) {
                                dragon->attackPlayer(playerPos.first, playerPos.second);
                            }
                        }
                        continue;
                    }

                    if(enemy->getType() == "Merchant" && Merchant::getIsHostile() == false) {
                        enemy->moveRandomly();
                        movedEnemiesPositions.insert(make_pair(enemy->getRow(), enemy->getCol()));
                        continue;
                    }


                    pair<int, int> playerPos = enemy->getPlayerInRange();
                    if(playerPos.first != -1 && playerPos.second != -1) {
                        enemy->attackPlayer(playerPos.first, playerPos.second);
                    } else {
                        enemy->moveRandomly();
                        // Store the new position of the moved enemy
                        movedEnemiesPositions.insert(make_pair(enemy->getRow(), enemy->getCol()));
                    }
                }
            }
        }
    }
}

void Floor::update() {
    if(player->getType() == "Troll") {
        player->updateHp(5);
    }
    updateEnemies();
}


void Floor::Display() {
    for (const auto &row : floorLayout) {
        for (const auto &ch : row) {
            if (ch == '@' || ch == '\\') { // Player is blue
                std::cout << "\033[34m" << ch << "\033[0m"; // ANSI code for blue
            } else if (ch == 'G') { // Gold is yellow
                std::cout << "\033[33m" << ch << "\033[0m"; // ANSI code for yellow
            } else if (ch == 'P') { // Potion is green
                std::cout << "\033[32m" << ch << "\033[0m"; // ANSI code for green
            } else if (ch == 'H' || ch == 'W' || ch == 'E' || ch == 'O' || ch == 'M' || ch == 'D' || ch == 'L') { // Enemies are red
                std::cout << "\033[31m" << ch << "\033[0m"; // ANSI code for red
            } else {
                std::cout << ch; // Other characters are printed normally
            }
        }
        std::cout << '\n';
    }
    DisplayStats();
}

void Floor::DisplayStats() {
    cout << "Race: " << player->getType() << " Gold: " << player->getGold() << endl;
    cout << "HP: " << player->getHp() << endl;
    cout << "Atk: " << player->getAtk() << endl;
    cout << "Def: " << player->getDef() << endl;
    cout << "Action: " << Board::getInstance()->getCurrentAction() << endl;
    Board::getInstance()->setCurrentAction("");
}


void Floor::initChambers() {
    // since chambers never change
    vector<vector<char>> board = Board::getInstance()->getDefaultFloorLayout();
    vector<vector<pair<int,int>>> chamber_coords = findChambers(board);
    for (const auto& chamber : chamber_coords) {
        chambers.push_back(new Chamber(chamber));
    }
}


bool isValid(int x, int y, vector<vector<char>>& board, vector<vector<bool>>& visited) {
    return x >= 0 && y >= 0 && x < board.size() && y < board[0].size() && !visited[x][y] && board[x][y] == '.';
}

void dfs(int x, int y, vector<pair<int,int>>& chamber, vector<vector<char>>& board, vector<vector<bool>>& visited) {
    vector<pair<int,int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    visited[x][y] = true;
    chamber.push_back({x, y});
    for (const auto& dir : dirs) {
        int newX = x + dir.first, newY = y + dir.second;
        if (isValid(newX, newY, board, visited)) {
            dfs(newX, newY, chamber, board, visited);
        }
    }

}


vector<vector<pair<int,int>>> Floor::findChambers(vector<vector<char>> board) {
    int n = board.size(), m = board[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<pair<int,int>>> chambers;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (isValid(i, j, board, visited)) {
                vector<pair<int,int>> chamber;
                dfs(i, j, chamber, board, visited);
                chambers.push_back(chamber);
            }
        }
    }

    return chambers;
}

void displayChambers(const std::vector<std::vector<std::pair<int, int>>>& chambers) {
    for (int i = 0; i < chambers.size(); i++) {
        std::cout << "Chamber " << (i+1) << " contains:\n";
        for (const auto& coord : chambers[i]) {
            std::cout << "(" << coord.first << ", " << coord.second << ")" << "\n";
        }
        std::cout << std::endl;
    }
}

void Floor::testChambers() {
    vector<vector<char>> board = floorLayout;
    vector<vector<pair<int,int>>> chambers = findChambers(board);
    displayChambers(chambers);
}
