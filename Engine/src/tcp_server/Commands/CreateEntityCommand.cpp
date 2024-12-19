#include "includes/tcp_server/Commands/CreateEntityCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/Scene.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/utility/Log.hpp"
#include "includes/system/EntityStorage.hpp"
#include "includes/Entity.hpp"
#include <memory>
#include <format>

std::string CreateEntityCommand::execute(Engine *e) {
  try {
    // auto uuid_manager = e->get_active_uuid_manager();
    // if (!uuid_manager) {
    //   std::cerr << "UUID Manager is null" << std::endl;
    //   return "UUID Manager is null";
    // }
    // auto storage =
    //     dynamic_cast<EntityStorage *>(uuid_manager->get_system(_uuid));
    // if (storage != nullptr) {
    //   auto parent = storage->get_entity(_uuid);
    //   uuid__ uuid_temp = uuid_manager->create_uuid_ownerless();
    //   auto entity = storage->create_entity(_entity_name, uuid_temp, std::make_shared<Entity>(*parent.value()));
    //   if (entity == nullptr) {
    //     Log::error("Entity could not be created.");
    //     return "Entity could not be created.";
    //   }
    // } else {
    //   Log::error("The UUID you provided does not belong to a Entity.");
    //   return "The UUID you provided does not belong to a Entity.";
    // }
    // return "Entity created";
    auto scene = e->get_scene();
    if (!scene) {
      Log::error("Scene is null");
      return "Scene is null";
    }
    auto parent = scene->get_entity(_uuid);
    if (!parent.has_value()) {
      Log::error(std::format("Entity not found for UUID: {}", boost::uuids::to_string(_uuid)));
      return "Entity not found";
    }
    auto entity = scene->create_entity(_entity_name, parent.value()->get_ptr());
    if (entity == nullptr) {
      Log::error("Entity could not be created.");
      return "Entity could not be created.";
    }
    return "Entity created";

  } catch (const std::bad_alloc &e) {
    Log::error(std::format("Memory allocation failed: {}", e.what()));
    return "Memory allocation failed";
  } catch (const std::exception &e) {
    Log::error(std::format("Exception: {}", e.what()));
    return "Exception occurred";
  } catch (...) {
    Log::error("Unknown exception occurred");
    return "Unknown exception occurred";
  }
}
int CreateEntityCommand::undo() { return 0; }
