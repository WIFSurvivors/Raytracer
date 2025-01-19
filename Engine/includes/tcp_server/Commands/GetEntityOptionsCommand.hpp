#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_ENTITIY_OPTIONS_COMMAND "GetEntityOptions"

struct GetEntityOptionsCommand : public TcpCommand {
  explicit GetEntityOptionsCommand(uuid uuid) : TcpCommand(uuid) {}
  std::string execute(RT::Engine *e) override;

private:
};
