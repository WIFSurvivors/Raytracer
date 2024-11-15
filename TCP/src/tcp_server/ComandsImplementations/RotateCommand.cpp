#include "includes/RotateCommand.hpp"

int RotateCommand::execute() {
    std::cout << "RotateCommand executed" << std::endl; 
    return 0;
}
int RotateCommand::undo() { return 0; }

