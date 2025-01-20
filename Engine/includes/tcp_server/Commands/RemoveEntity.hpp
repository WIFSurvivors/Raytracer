#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define REMOVE_ENTITY_COMMAND "RemoveEntity"

struct RemoveEntity : public TcpCommand {
  explicit RemoveEntity(uuid uuid)
      : TcpCommand(uuid) {}
  std::string execute(RT::Engine *e) override;
private:
};
