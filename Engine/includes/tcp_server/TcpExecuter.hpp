#pragma once
#include <string>
#include "includes/tcp_server/TcpCommand.hpp"

struct TcpExecuter {
  TcpExecuter();
  std::string execute(TcpCommand *command , Engine *engine);
  std::string undo(TcpCommand * command);

private:
};
