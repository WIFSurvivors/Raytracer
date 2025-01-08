#pragma once
#include <string>
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Engine.hpp"
using RT::Engine;
struct TcpExecuter {
  TcpExecuter();
  std::string execute(TcpCommand *command, RT::Engine *engine);
  std::string undo(TcpCommand *command);

private:
};
