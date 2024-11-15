#pragma once
#include "includes/TcpCommand.hpp"
#include "includes/TcpExecuter.hpp"
#include "includes/MoveCommand.hpp"
#include <iostream>
#include <string>
struct TcpParser {
  TcpParser();
  TcpCommand& parse(std::string msg);

private:
};
