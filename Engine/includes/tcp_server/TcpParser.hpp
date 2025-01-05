#pragma once
#include <string>
#include <memory>
#include "includes/tcp_server/ParsedTcpCommand.hpp"

struct TcpParser {
  TcpParser();
  std::unique_ptr<ParsedTcpCommand> parse(std::string msg);

private:
};
