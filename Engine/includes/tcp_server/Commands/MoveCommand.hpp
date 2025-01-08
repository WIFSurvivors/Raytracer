#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Engine.hpp"
#include <string>
#include <glm/vec3.hpp>

#define MOVE_COMMAND "MoveCommand"

class TcpCommand;

class MoveCommand : public TcpCommand {
public:
  MoveCommand(uuid uuid, float new_x, float new_y, float new_z)
      : TcpCommand(uuid), _new_position(new_x, new_y, new_z) {}
  std::string execute(RT::Engine *engine) override;
  std::string undo() override;
  inline glm::vec3 get_new_position() const { return _new_position; }
  inline void set_new_position(glm::vec3 new_position) {
    _new_position = new_position;
  }
  inline glm::vec3 get_old_position() const { return _old_position; }
  inline void set_old_position(glm::vec3 old_position) {
    _old_position = old_position;
  }

private:
  glm::vec3 _old_position;
  glm::vec3 _new_position;
};
