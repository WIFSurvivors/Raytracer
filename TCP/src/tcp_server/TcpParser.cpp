#include "includes/TcpParser.hpp"

TcpParser::TcpParser() {}

TcpCommand& TcpParser::parse(std::string msg) {
  std::cout << "Got message: " << msg << std::endl;
  if(msg == "command1") {
    std::cout << "Create command 1" << std::endl;
  } else if(msg == "command2") {
    std::cout << "Create command 2" << std::endl;
  } else {
    std::cout << "Unknown command" << std::endl;
  }
  return *new MoveCommand();
}
