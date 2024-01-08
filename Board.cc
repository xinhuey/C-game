#include "Board.h"
#include "Player.h"
#include "Floor.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <memory>


using namespace std;

Board* Board::instance;

Floor* Board::getCurrentFloor() {
    return currentFloor;
}

Board::Board() : player(nullptr), currentFloor(nullptr), currentFloorNum(1), totalFloors(5) {
}

Board* Board::getInstance() {
    if (!instance) {
        instance = new Board();
    }
    return instance;
}

Board::~Board() {
}

void Board::setCurrentAction(const std::string &action) {
    currentAction = action;
}

std::string Board::getCurrentAction() {
    return currentAction;
}

void Board::setIsPaused(bool state) {
    isPaused = state;
}

bool Board::getIsPaused() {
    return isPaused;
}

void Board::toggleFreeze() {

    if(isPaused == false) {
        Board::getInstance()->setCurrentAction("PC freezes enemies.");
    } else {
        Board::getInstance()->setCurrentAction("PC unfreezes enemies.");
    }
    isPaused = !isPaused;
}


bool Board::usePotion(const std::string &direction) {
    int newRow = player->getRow();
    int newCol = player->getCol();

    if (direction == "no") {
        newRow--;
    } else if (direction == "so") {
        newRow++;
    } else if (direction == "ea") {
        newCol++;
    } else if (direction == "we") {
        newCol--;
    } else if (direction == "ne") {
        newRow--;
        newCol++;
    } else if (direction == "nw") {
        newRow--;
        newCol--;
    } else if (direction == "se") {
        newRow++;
        newCol++;
    } else if (direction == "sw") {
        newRow++;
        newCol--;
    } else {
        return false;
    }

    return player->usePotion(newRow, newCol);
}

bool Board::playerAttack(const std::string &direction) {
    int newRow = player->getRow();
    int newCol = player->getCol();

    if (direction == "no") {
        newRow--;
    } else if (direction == "so") {
        newRow++;
    } else if (direction == "ea") {
        newCol++;
    } else if (direction == "we") {
        newCol--;
    } else if (direction == "ne") {
        newRow--;
        newCol++;
    } else if (direction == "nw") {
        newRow--;
        newCol--;
    } else if (direction == "se") {
        newRow++;
        newCol++;
    } else if (direction == "sw") {
        newRow++;
        newCol--;
    } else {
        return false;
    }

    return player->attack(newRow, newCol);
}

bool Board::movePlayer(const std::string &direction) {
    int newRow = player->getRow();
    int newCol = player->getCol();

    if (direction == "no") {
        newRow--;
    } else if (direction == "so") {
        newRow++;
    } else if (direction == "ea") {
        newCol++;
    } else if (direction == "we") {
        newCol--;
    } else if (direction == "ne") {
        newRow--;
        newCol++;
    } else if (direction == "nw") {
        newRow--;
        newCol--;
    } else if (direction == "se") {
        newRow++;
        newCol++;
    } else if (direction == "sw") {
        newRow++;
        newCol--;
    } else {
        return false;
    }

    return player->move(newRow, newCol);
}

int Board::getCurrentFloorNum() {
    return currentFloorNum;
}

void Board::moveToNextFloor() {
    currentFloorNum++;
    if (currentFloorNum > totalFloors) {
        Board::getInstance()->getPlayer()->displayScore();
        quitGame();
    } else {
        if(customFloorLayouts.size() > 0) {
            Board::getInstance()->setCurrentAction("PC moves to next floor! ");
            currentFloor = new Floor(customFloorLayouts[currentFloorNum - 1]);
        } else {
            Board::getInstance()->setCurrentAction("PC moves to next floor! ");
            currentFloor = new Floor();
        }
    }
    player->removeTempEffects();
}

void Board::restartGame() {
    cout << "Restarting game!" << endl;
    Merchant::setHostile(false);
    if(customFileName != "") {
        startGame(customFileName);
        return;
    }
    startGame();
}

void Board::quitGame() {
    cout << "Quitting Game - Game over!" << endl;
    exit(0);
}

void Board::choosePlayer(const std::string &type) {
    if (type == "shade") {
        player = new Shade(4, 27);
    } 
    else if (type == "drow") {
        player = new Drow(4, 27);
    } else if (type == "vampire") {
        player = new Vampire(4, 27);
    } else if (type == "troll") {
        player = new Troll(4, 27);
    } else if (type == "goblin") {
        player = new Goblin(4, 27);
    } else {
        player = new Shade(4, 27);
    }
}

void Board::startGame(const std::string &filename) {
    // Let user choose PC
    cout << "Choose a character!" << endl;
    cout << "Type 's' for Shade, 'd' for Drow, 'v' for Vampire, 't' for Troll and 'g' for Goblin" << endl;
    string input;
    cin >> input;
    if (input == "s") {
        choosePlayer("shade");
    } else if (input == "d") {
        choosePlayer("drow");
    } else if (input == "v") {
        choosePlayer("vampire");
    } else if (input == "t") {
        choosePlayer("troll");
    } else if (input == "g") {
        choosePlayer("goblin");
    } else {
        cout << "No character: " << input << " found. You are a Shade!" << endl;
        choosePlayer("shade");
    }

    ifstream file(filename);

    if (!file) {
        cout << "Can't access file: " << filename << endl;
        cout << "Using default map!" << endl;
        readDefaultFloorLayout("cc3k-emptySingleFloor.txt");
        currentFloor = new Floor();
        return;
    }

    readDefaultFloorLayout("cc3k-emptySingleFloor.txt");
    readAllFloorLayouts(filename);
    customFileName = filename;

    currentFloorNum = 1;
    currentFloor = new Floor(customFloorLayouts[currentFloorNum - 1]);
}

void Board::startGame() {
    // Let user choose PC
    cout << "Choose a character!" << endl;
    cout << "Type 's' for Shade, 'd' for Drow, 'v' for Vampire, 't' for Troll and 'g' for Goblin" << endl;
    string input;
    cin >> input;
    if (input == "s") {
        choosePlayer("shade");
    } else if (input == "d") {
        choosePlayer("drow");
    } else if (input == "v") {
        choosePlayer("vampire");
    } else if (input == "t") {
        choosePlayer("troll");
    } else if (input == "g") {
        choosePlayer("goblin");
    } else {
        cout << "No character: " << input << " found. You are a Shade!" << endl;
        choosePlayer("shade");
    }
    currentFloorNum = 1;
    readDefaultFloorLayout("cc3k-emptySingleFloor.txt");
    currentFloor = new Floor();
}

void Board::readDefaultFloorLayout(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;

    defaultFloorLayout.clear();

    while (std::getline(file, line)) {
        std::vector<char> row(line.begin(), line.end());
        defaultFloorLayout.push_back(row);
    }
}

void Board::readAllFloorLayouts(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::vector<char>> singleFloorLayout;

    customFloorLayouts.clear();

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 25; j++) {
            std::getline(file, line);
            std::vector<char> row(line.begin(), line.end());
            singleFloorLayout.push_back(row);
        }
        customFloorLayouts.push_back(singleFloorLayout);
        singleFloorLayout.clear();
    }
}

void Board::Display() {
    currentFloor->Display();
}

Player* Board::getPlayer() {
    return player;
}

std::vector<std::vector<char>> Board::getDefaultFloorLayout() {
    return defaultFloorLayout;
}

void Board::displayAllFloorLayouts() {
    for (int floorNum = 0; floorNum < customFloorLayouts.size(); ++floorNum) {
        std::cout << "Floor " << (floorNum + 1) << ":\n";
        for (const auto &row : customFloorLayouts[floorNum]) {
            for (const auto &ch : row) {
                std::cout << ch;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}
