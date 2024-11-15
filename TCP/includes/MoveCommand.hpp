#pragma once 
#include "includes/TcpCommand.hpp"
#include <iostream>
#define MOVE_COMMAND "MoveCommand"
class MoveCommand : public TcpCommand {
public:
    int execute() override;
    int undo() override;
};