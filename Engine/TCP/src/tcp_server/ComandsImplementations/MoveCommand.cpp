#include "includes/CommandImplementations/MoveCommand.hpp"

std::string MoveCommand::execute(Engine * engine) {
    std::cout << "MoveCommand executed on" << _uuid << "with new x y z: " << _new_x << " "<< _new_y << " "<< _new_z<<std::endl; 
    return "MoveCommand executed";
}
int MoveCommand::undo() { return 0; }
