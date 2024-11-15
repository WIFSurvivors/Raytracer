#pragma once
#include "includes/TcpCommand.hpp"
struct TcpCommandFactory {
  TcpCommandFactory();
  TcpCommand& create_command(std::string msg, std::map<std::string, std::string> previous_state);

private:
};
