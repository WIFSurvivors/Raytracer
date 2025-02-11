#include "includes/tcp_server/Commands/GetRootCommand.hpp"
#include "includes/Entity.hpp"
#include "includes/Engine.hpp"
#include "includes/Scene.hpp"
#include <boost/json.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <format>
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <string>

using RT::Engine;
using RT::Log;



std::string GetRootCommand::execute(Engine *engine) {
  try {
    if (!engine) {
      std::string msg = "Engine pointer is null";
      LOG_ERROR(msg);
      return msg;
    }

    auto scene = engine->get_scene();
    if (!scene) {
      std::string msg = "Scene pointer is null";
      LOG_ERROR(msg);
      return msg;
    }

    auto root_entity = scene->get_root().lock();
    if (root_entity) {
      if (root_entity->get_uuid().is_nil()) {
        std::string msg = "Root entity UUID is nil";
        LOG_ERROR(msg);
        return msg;
      }
      if (root_entity->get_name().empty()) {
        std::string msg = "Root entity name is empty";
        LOG_ERROR(msg);
        return msg;
      }
      boost::json::object root_json = root_entity->to_json();
        set_successfull(true);

      return boost::json::serialize(root_json);
    } else {
      std::string msg = "Root entity is null";
      LOG_ERROR(msg);
      return msg;
    }
  } catch (const std::exception &e) {
    std::string msg = std::format("Exception: {}", e.what());
    LOG_ERROR(msg);
    return msg;
  }
}
