#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "includes/TcpCommand.hpp"

struct TcpExecuter {
  TcpExecuter();
  const std::string& execute(TcpCommand *command , Engine *engine);
  const std::string& undo(TcpCommand * command);

private:
};
