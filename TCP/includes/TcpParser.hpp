#pragma once
#include "includes/ParsedTcpCommand.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <sstream>

struct TcpParser {
  TcpParser();
  std::unique_ptr<ParsedTcpCommand> parse(std::string msg);

private:
};
