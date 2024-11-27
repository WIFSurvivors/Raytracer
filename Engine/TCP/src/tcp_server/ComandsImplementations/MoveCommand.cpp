#include "includes/CommandImplementations/MoveCommand.hpp"

int MoveCommand::execute() {
    std::cout << "MoveCommand executed on" << _uuid << "with new x y z: " << _new_x << " "<< _new_y << " "<< _new_z<<std::endl; 
    return 0;
}
int MoveCommand::undo() { return 0; }
