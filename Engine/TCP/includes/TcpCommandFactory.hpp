#pragma once
#include "includes/TcpCommand.hpp"
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include "includes/CommandImplementations/MoveCommand.hpp"
#include "includes/CommandImplementations/RotateCommand.hpp"
#include "includes/ParsedTcpCommand.hpp"
#include "includes/CommandImplementations/GetRootCommand.hpp"
#include "includes/CommandImplementations/CloseRenderCommand.hpp"
struct TcpCommandFactory {
  TcpCommandFactory();
  std::unique_ptr<TcpCommand> create_command(ParsedTcpCommand parsed_command);

private:
};
