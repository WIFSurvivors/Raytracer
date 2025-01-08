#include "includes/tcp_server/Commands/MoveCommand.hpp"
#include <boost/uuid/uuid_io.hpp>
#include "includes/Engine.hpp"
#include "includes/Entity.hpp"
#include "includes/utility/Log.hpp"
#include <string>
#include "includes/utility/NotImplementedError.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

using RT::Log;
using RT::Engine;

std::string MoveCommand::execute(Engine *engine) {
  auto scene = engine->get_scene();
  if (!scene) {
    std::string msg = "Scene is null";
    LOG_ERROR(msg);
    return msg;
  }
  uuid _uuid = this->get_uuid();
  auto entity_ptr = (*scene)[_uuid];
  if (!entity_ptr) {
    std::string msg =
        "Entity not found for UUID: " + boost::uuids::to_string(_uuid);
    LOG_ERROR(msg);
    return msg;
  }

  auto entity = *entity_ptr;
  if (!entity) {
    std::string msg =
        "Entity is null for UUID: " + boost::uuids::to_string(_uuid);
    LOG_ERROR(msg);
    return msg;
  }
  entity->set_local_position(this->get_new_position());
  std::string msg =
      "MoveCommand executed on " + boost::uuids::to_string(_uuid) +
      " with new x y z: " + glm::to_string(entity->get_local_position());
  LOG(msg);
  return msg;
}
std::string MoveCommand::undo() { throw NotImplementedError{}; }
