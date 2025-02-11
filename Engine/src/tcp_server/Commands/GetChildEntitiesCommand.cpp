#include "includes/tcp_server/Commands/GetChildEntitiesCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/Scene.hpp"
#include "includes/Entity.hpp"
#include "includes/utility/Log.hpp"
#include <boost/json.hpp>
#include <format>
#include "includes/utility/NotImplementedError.hpp"

using RT::Log;
using RT::Engine;

std::string GetChildEntitiesCommand::execute(Engine *e) {
  auto scene = e->get_scene();
  if (!scene) {
    std::string msg = "Scene pointer is null";
    LOG_ERROR(msg);
    return msg;
  }
  auto entity = scene->get_entity(get_uuid());
  if (!entity.has_value()) {
    std::string msg = "Entity not found";
    LOG_ERROR(msg);
    return msg;
  }
  auto json_entity = entity.value()->to_json();
  std::cout << boost::json::serialize(json_entity) << std::endl;
    set_successfull(true);

  return boost::json::serialize(json_entity);
}
