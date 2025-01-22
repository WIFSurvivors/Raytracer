#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define PRINT_TABLE_COMMAND "PrintTable"

struct PrintTable : public TcpCommand {
  PrintTable() {}
  std::string execute(RT::Engine *engine) override;

private:
};
