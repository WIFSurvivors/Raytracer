#pragma once

#include <memory>
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/tcp_server/ParsedTcpCommand.hpp"
#include <boost/uuid/uuid.hpp>

struct TcpCommandFactory {
  using uuid = boost::uuids::uuid;
  /// @brief Factory that creates commands.
  TcpCommandFactory();
  /// @brief Create a command from a parsed command.
  /// The command is created based on the command string and the parameters.
  /// Each command has a unique string that is used to identify the command.
  /// @param parsed_command The parsed command that contains the command string and the parameters.
  /// @return A unique pointer to the created command.
  std::unique_ptr<TcpCommand> create_command(ParsedTcpCommand parsed_command);
};
