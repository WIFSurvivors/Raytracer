#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "includes/TcpCommand.hpp"

struct TcpExecuter {
  TcpExecuter();
  std::string execute(TcpCommand *command , Engine *engine);
  int undo(TcpCommand * command);

private:
};
