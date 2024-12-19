#include "includes/tcp_server/Commands/ScaleCommand.hpp"
#include <boost/uuid/uuid_io.hpp>
#include "includes/Engine.hpp"
#include "includes/Entity.hpp"
#include <iostream>
#include "includes/utility/Log.hpp"
#include <string>
#include "includes/utility/NotImplementedError.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
std::string ScaleCommand::execute(Engine *engine) {
  auto scene = engine->get_scene();
  if (!scene) {
    std::string msg = "Scene is null";
    Log::error(msg);
    return msg;
  }
  auto entity_ptr = (*scene)[_uuid];
  if (!entity_ptr) {
    std::string msg =
        "Entity not found for UUID: " + boost::uuids::to_string(_uuid);
    Log::error(msg);
    return msg;
  }

  auto entity = *entity_ptr;
  if (!entity) {
    std::string msg =
        "Entity is null for UUID: " + boost::uuids::to_string(_uuid);
    Log::error(msg);
    return msg;
  }
  entity->set_local_scale(_new_position);

  std::string msg =
      "ScaleCommand executed on" + boost::uuids::to_string(_uuid) +
      "with new x y z: " + glm::to_string(entity->get_local_scale());
  Log::message(msg);
  return msg;
}
int ScaleCommand::undo() { throw NotImplementedError{}; }
