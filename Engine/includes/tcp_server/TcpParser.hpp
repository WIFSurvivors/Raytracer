#pragma once
#include <string>
#include <memory>
#include "includes/tcp_server/ParsedTcpCommand.hpp"

struct TcpParser {
  TcpParser();
  /// @brief Parse a message.
  /// @param msg The message to parse.
  /// @return A unique pointer to the parsed command.
  std::unique_ptr<ParsedTcpCommand> parse(std::string msg);

private:
};
