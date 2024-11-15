#pragma once 
#include "includes/TcpCommand.hpp"
#include <iostream>
#define ROTATE_COMMAND "RotateCommand"
class RotateCommand : public TcpCommand {
public:
    int execute() override;
    int undo() override;
};