#pragma once

#include <memory>
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/tcp_server/ParsedTcpCommand.hpp"
#include <boost/uuid/uuid.hpp>

struct TcpCommandFactory {
  using uuid = boost::uuids::uuid;

  TcpCommandFactory();
  std::unique_ptr<TcpCommand> create_command(ParsedTcpCommand parsed_command);
};
