#include "includes/tcp_server/Commands/CreateEntityCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/Scene.hpp"
#include "includes/Entity.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/system/EntityStorage.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <memory>
#include <format>

using RT::Log;
using RT::Engine;

std::string CreateEntityCommand::execute(Engine *e) {
  try {
    auto scene = e->get_scene();
    if (!scene) {
      LOG_ERROR("Scene is null");
      return "Scene is null";
    }
    uuid _uuid = this->get_uuid();
    auto parent = scene->get_entity(_uuid);
    if (!parent.has_value()) {
      LOG_ERROR(std::format("Entity not found for UUID: {}",
                            boost::uuids::to_string(_uuid)));
      return "Entity not found";
    }
    auto entity = scene->create_entity(_entity_name, parent.value()->get_ptr());
    if (entity == nullptr) {
      LOG_ERROR("Entity could not be created.");
      return "Entity could not be created.";
    }
    return "Entity created";

  } catch (const std::bad_alloc &e) {
    LOG_ERROR(std::format("Memory allocation failed: {}", e.what()));
    return "Memory allocation failed";
  } catch (const std::exception &e) {
    LOG_ERROR(std::format("Exception: {}", e.what()));
    return "Exception occurred";
  } catch (...) {
    LOG_ERROR("Unknown exception occurred");
    return "Unknown exception occurred";
  }
}
std::string CreateEntityCommand::undo() { throw NotImplementedError{}; }
