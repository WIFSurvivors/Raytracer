#include "includes/CommandImplementations/RotateCommand.hpp"

const std::string& RotateCommand::execute(Engine * engine) {
    std::cout << "RotateCommand executed" << std::endl; 
    return "RotateCommand executed";
}
const std::string& RotateCommand::undo() { return "0"; }

