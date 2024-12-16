#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include "includes/tcp_server/ParsedTcpCommand.hpp"

struct TcpParser {
  TcpParser();
  std::unique_ptr<ParsedTcpCommand> parse(std::string msg);

private:
};
