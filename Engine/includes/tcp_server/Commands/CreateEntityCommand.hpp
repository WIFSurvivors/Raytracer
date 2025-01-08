#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Engine.hpp"
#include <string>

#define CREATE_ENTITY_COMMAND "CreateEntity"

struct CreateEntityCommand : public TcpCommand {
  explicit CreateEntityCommand(uuid uuid, std::string entity_name)
      : TcpCommand(uuid), _entity_name(entity_name) {}
  std::string execute(RT::Engine *e) override;
  std::string undo() override;
  inline std::string get_entity_name() const { return _entity_name; }
  inline void set_entity_name(std::string entity_name) {
    _entity_name = entity_name;
  }

private:
  std::string _entity_name;
};
