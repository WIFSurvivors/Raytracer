#include "includes/MoveCommand.hpp"

int MoveCommand::execute() {
    std::cout << "MoveCommand executed" << std::endl; 
    return 0;
}
int MoveCommand::undo() { return 0; }