#include "includes/tcp_server/TcpParser.hpp"
#include "includes/utility/Log.hpp"
#include <sstream>

using RT::Log;
#define SEPARATOR ";"
TcpParser::TcpParser() {}

std::unique_ptr<ParsedTcpCommand> TcpParser::parse(std::string msg) {
  LOG_TCP("Got message: " + msg);
  std::string command;
  std::string uuid;
  std::vector<std::string> parameters;
  std::istringstream iss(msg);
  std::string token;
  std::getline(iss, command, ';');
  std::getline(iss, uuid, ';');
  while (std::getline(iss, token, ';')) {
    parameters.push_back(token);
  }
  return std::make_unique<ParsedTcpCommand>(command, uuid, parameters);
}
