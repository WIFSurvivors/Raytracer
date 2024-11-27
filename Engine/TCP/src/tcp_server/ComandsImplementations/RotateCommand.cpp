#include "includes/CommandImplementations/RotateCommand.hpp"

int RotateCommand::execute(Engine * engine) {
    std::cout << "RotateCommand executed" << std::endl; 
    return 0;
}
int RotateCommand::undo() { return 0; }

