#include "CommandInterpreter.h"

#include <iostream>
#include <memory>
#include <string>

using namespace std;

std::string directionHelper(std::string command) {
    if(command == "no") {
        return "North";
    } else if(command == "so") {
        return "South";
    } else if(command == "ea") {
        return "East";
    } else if(command == "we") {
        return "West";
    } else if(command == "ne") {
        return "North East";
    } else if(command == "nw") {
        return "North West";
    } else if(command == "se") {
        return "South East";
    } else if(command == "sw") {
        return "South West";
    }
}

void seePotionHelper() {
    Floor* currFloor = Board::getInstance()->getCurrentFloor();
    Player* player = Board::getInstance()->getPlayer();
    vector<vector<char>> &currLayout = currFloor->floorLayout;

    // directions to check around the enemy
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for(const auto& dir : directions) {
        int checkRow = player->getRow() + dir.first;
        int checkCol = player->getCol() + dir.second;
        // if player is within 1 block radius
        // what happens if many potions are in range?
        if (currLayout[checkRow][checkCol] == 'P') {
            Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + " PC sees an unknown potion.");
            return;
        }
    }
}


CommandInterpreter::CommandInterpreter(Board* game) : game{game} {}

CommandInterpreter::~CommandInterpreter() {}

void CommandInterpreter::interpretCommand(const string &command) {
    if (command == "no" || command == "so" || command == "ea" || command == "we" ||
        command == "ne" || command == "nw" || command == "se" || command == "sw") {
        if (!(game->movePlayer(command))) {
            Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() +"Invalid move. Can't move player in direction " + command + ".");
            return;
        }
        std::string direction = directionHelper(command);
        Board::getInstance()->setCurrentAction(Board::getInstance()->getCurrentAction() + "Player moves " + direction + ".");
        seePotionHelper();
    }
    else if (command[0] == 'u') {
        string direction = command.substr(2);
        if (!(game->usePotion(direction))) {
            Board::getInstance()->setCurrentAction("Can't use unit in direction " + direction + ".");
        }
    } else if (command[0] == 'a') {
        if(command.length() < 2) {
            Board::getInstance()->setCurrentAction("Invalid move. Attack needs direction specifier.");
            return;
        }
        string direction = command.substr(2);
        if (!(game->playerAttack(direction))) {
            Board::getInstance()->setCurrentAction("Can't attack unit in direction " + direction + ".");
        }
    }
    else if (command == "r") {
        game->restartGame();
    } else if(command == "f") {
        game->toggleFreeze();
    }
    else if (command == "q") {
        game->quitGame();
    } else {
        Board::getInstance()->setCurrentAction("Invalid command: " + command + ".");
    }
}
