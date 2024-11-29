#include "includes/CommandImplementations/RotateCommand.hpp"

std::string RotateCommand::execute(Engine * engine) {
    std::cout << "RotateCommand executed" << std::endl; 
    return "RotateCommand executed";
}
int RotateCommand::undo() { return 0; }

