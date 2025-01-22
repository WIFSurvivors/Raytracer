#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define REMOVE_COMPONENT_COMMAND "RemoveComponent"

struct RemoveComponent : public TcpCommand {
  explicit RemoveComponent(uuid uuid)
      : TcpCommand(uuid) {}
  std::string execute(RT::Engine *e) override;
private:
};
