#pragma once
#include "includes/TcpCommand.hpp"
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include "includes/MoveCommand.hpp"
#include "includes/RotateCommand.hpp"
#include "includes/ParsedTcpCommand.hpp"
struct TcpCommandFactory {
  TcpCommandFactory();
  std::unique_ptr<TcpCommand> create_command(ParsedTcpCommand parsed_command);

private:
};
