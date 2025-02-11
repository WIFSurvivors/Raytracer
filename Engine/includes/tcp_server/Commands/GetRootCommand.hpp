#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_ROOT_COMMAND "GetRoot"

struct GetRootCommand : public TcpCommand {
  GetRootCommand() = default;
  std::string execute(RT::Engine *e) override;

private:
};
