#include "includes/tcp_server/TcpParser.hpp"
#include "includes/utility/Log.hpp"
#include <sstream>

using namespace RT;

TcpParser::TcpParser() {}

std::unique_ptr<ParsedTcpCommand> TcpParser::parse(std::string msg) {
  LOG_TCP("Got message: " + msg);
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
