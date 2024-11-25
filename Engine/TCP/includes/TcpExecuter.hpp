#pragma once
#include <iostream>
#include <memory>
#include "includes/TcpCommand.hpp"

struct TcpExecuter {
  TcpExecuter();
  int execute(TcpCommand *command);
  int undo(TcpCommand * command);

private:
};
