#include "includes/tcp_server/TcpParser.hpp"

TcpParser::TcpParser() {}

std::unique_ptr<ParsedTcpCommand> TcpParser::parse(std::string msg) {
  std::cout << "Got message: " << msg << std::endl;
  std::string command;
  std::string uuid;
  std::vector<std::string> parameters;
  std::istringstream iss(msg);
  iss >> command >> uuid;
  std::string parameter;
  while (iss >> parameter) {
    parameters.push_back(parameter);
  }
  return std::make_unique<ParsedTcpCommand>(command, uuid, parameters);
}
