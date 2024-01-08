#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H
#include <memory>
#include <string>
#include "Board.h"

class CommandInterpreter {
    private:
        std::shared_ptr<Board> game;
    
    public:
        void interpretCommand(const std::string &command);
        CommandInterpreter(Board* game);
        ~CommandInterpreter();
};

#endif
