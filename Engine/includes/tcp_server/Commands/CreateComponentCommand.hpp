#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define CREATE_COMPONENT_COMMAND "CreateComponent"

struct CreateComponentCommand : public UndoableCommand {
  explicit CreateComponentCommand(uuid uuid, std::string type)
      : UndoableCommand(uuid), _type(type) {}
  std::string execute(RT::Engine *e) override;
  std::string undo(RT::Engine *e) override;

  inline std::string get_type() const { return _type; }
  inline void set_type(std::string type) { _type = type; }
  inline uuid get_uuid_component() const { return _uuid_component; }
  inline void set_uuid_component(uuid uuid_component) {
    _uuid_component = uuid_component;
  }

private:
  uuid _uuid_component;
  std::string _type;
};
