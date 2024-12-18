#include "includes/tcp_server/Commands/RotateCommand.hpp"
#include <boost/uuid/uuid_io.hpp>

std::string RotateCommand::execute(Engine * engine) {
    std::cout << "RotateCommand executed for " << boost::uuids::to_string(_uuid) << std::endl; 
    return "RotateCommand executed";
}
int RotateCommand::undo() { return 0; }

