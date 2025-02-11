#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_COMPONENT_OPTIONS_COMMAND "GetComponentOptions"

struct GetComponentOptions : public TcpCommand {
  explicit GetComponentOptions(uuid uuid) : TcpCommand(uuid) {}
  std::string execute(RT::Engine *e) override;

private:
};
