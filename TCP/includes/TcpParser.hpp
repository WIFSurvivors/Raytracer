#pragma once
#include "includes/TcpCommand.hpp"
#include "includes/TcpExecuter.hpp"
#include "includes/MoveCommand.hpp"
#include "includes/RotateCommand.hpp"
#include <iostream>
#include <string>
#include <memory>
struct TcpParser {
  TcpParser();
  std::unique_ptr<TcpCommand> parse(std::string msg);

private:
};
