#include "includes/TcpParser.hpp"

TcpParser::TcpParser() {}

std::unique_ptr<TcpCommand> TcpParser::parse(std::string msg) {
  std::cout << "Got message: " << msg << std::endl;
    std::string move= "MoveCommand";
  if(msg.compare(MOVE_COMMAND) == 0) {
    std::cout << "Create MoveCommand" << std::endl;
        return std::make_unique<MoveCommand>();
  } else if(msg.compare(ROTATE_COMMAND) == 0) {
    std::cout << "Create RotateCommand" << std::endl;
    return std::make_unique<RotateCommand>();
  } else {
    std::cout << "Unknown command" << std::endl;
  }
  return nullptr;
}
