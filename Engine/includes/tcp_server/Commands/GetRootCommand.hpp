#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include "includes/Engine.hpp"

#define GET_ROOT_COMMAND "GetRoot"

struct GetRootCommand : public TcpCommand {
  GetRootCommand() = default;
  std::string execute(RT::Engine *e) override;
  std::string undo() override;

private:
};
