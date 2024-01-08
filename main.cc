#include "Board.h"
#include "CommandInterpreter.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

int main(int argc, char* argv[]) {
    
    Board* board = Board::getInstance();
    CommandInterpreter *commandInterpreter = new CommandInterpreter(board);

    // Check if someone is inputting floor layout in .txt format
    if (argc == 2) {
        string inputFileName = argv[1];
        board->startGame(inputFileName);
    } else {
        board->startGame();
    }

    board->Display();

    string command;
    while(true) {
        getline(cin, command);
        if(cin.eof()) {
            break;
        }

        if(command == "") {
            continue;
        }
        commandInterpreter->interpretCommand(command);
        // cout << "command is : " << command << endl;
        board->getCurrentFloor()->update();
        board->Display();
    }
}


