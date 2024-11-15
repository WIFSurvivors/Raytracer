#pragma once 
#include "includes/TcpCommand.hpp"
#include <iostream>

class MoveCommand : public TcpCommand {
public:
    int execute() override;
    int undo() override;
};