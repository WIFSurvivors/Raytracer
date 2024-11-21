#pragma once
#include <iostream>
#include <memory>
#include "includes/TcpCommand.hpp"

struct TcpExecuter {
  TcpExecuter();
  int execute(std::unique_ptr<TcpCommand> command);
  int undo(std::unique_ptr<TcpCommand> command);

private:
};
