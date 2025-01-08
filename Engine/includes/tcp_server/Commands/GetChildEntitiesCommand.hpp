#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_ENTITIES_COMMAND "GetChildEntities"

struct GetChildEntitiesCommand : public TcpCommand {
  explicit GetChildEntitiesCommand(uuid uuid) : TcpCommand(uuid) {}
  std::string execute(RT::Engine *e) override;
  std::string undo() override;

private:
};
