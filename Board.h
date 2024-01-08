#ifndef BOARD_H
#define BOARD_H

#include "Player.h"
#include "Floor.h"
#include "Unit.h"

#include <memory>
#include <vector>
#include <string>
#include <utility>

class Board {

private:
    static Board *instance;
    Player *player;
    Floor *currentFloor;
    int currentFloorNum;
    int const totalFloors;
    bool isPaused;
    std::string customFileName;
    std::string currentAction;
    Board();

public:
    std::vector<std::vector<std::vector<char>>> customFloorLayouts;
    std::vector<std::vector<char>> defaultFloorLayout;

    ~Board();
    static Board *getInstance();
    void startGame();
    void startGame(const std::string &filename);
    void restartGame();
    void quitGame();
    void setCurrentAction(const std::string &action);
    std::string getCurrentAction();
    
    void toggleFreeze();
    bool getIsPaused();
    void setIsPaused(bool state);

    void moveToNextFloor();
    Floor *getCurrentFloor();
    int getCurrentFloorNum();
    bool movePlayer(const std::string &direction);
    bool playerAttack(const std::string &direction);
    bool usePotion(const std::string &direction);
    void choosePlayer(const std::string &race);
    void readAllFloorLayouts(const std::string &filename);
    void readDefaultFloorLayout(const std::string &filename);
    void Display();

    Player *getPlayer();
    std::vector<std::vector<char>> getDefaultFloorLayout();
    void displayAllFloorLayouts();
    void applyCustomChamberLayout();
};

#endif
