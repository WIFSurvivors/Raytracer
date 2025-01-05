#include "includes/tcp_server/Commands/CreateComponentCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/Scene.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/utility/Log.hpp"
#include "includes/system/EntityStorage.hpp"
#include "includes/Entity.hpp"
#include <memory>
#include <format>
#include "includes/utility/NotImplementedError.hpp"

std::string CreateComponentCommand::execute(Engine *e) {
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
    if(_type.compare("simple") == 0) {
      auto system = scene->get_simple_system();
        if (!system) {
            LOG_ERROR("SimpleSystem is null");
            return "SimpleSystem is null";
        }
        auto component = system->create_component(parent.value(), 123);
        if (component == nullptr) {
            LOG_ERROR("Component could not be created.");
            return "Component could not be created.";
        }
        return "Component created";
    }
    else if(_type.compare("render") == 0) {
        auto system = scene->get_render_system();
            if (!system) {
                LOG_ERROR("RenderSystem is null");
                return "RenderSystem is null";
            }
            auto component = system->create_component(parent.value());
            if (component == nullptr) {
                LOG_ERROR("Component could not be created.");
                return "Component could not be created.";
            }
            return "Component created";
    }
    else if(_type.compare("camera") == 0) {
        auto system = scene->get_camera_system();
            if (!system) {
                LOG_ERROR("CameraSystem is null");
                return "CameraSystem is null";
            }
            auto component = system->create_component( parent.value());
            LOG(std::format("Get system name in camera: {}", system->get_name()));
            if (component == nullptr) {
                LOG_ERROR("Component could not be created.");
                return "Component could not be created.";
            }
            return "Component created";
    }
    else if(_type.compare("light") == 0) {
        auto system = scene->get_light_system();
            if (!system) {
                LOG_ERROR("LightSystem is null");
                return "LightSystem is null";
            }
            auto component = system->create_component(parent.value());
            if (component == nullptr) {
                LOG_ERROR("Component could not be created.");
                return "Component could not be created.";
            }
            return "Component created";
    }
    else {
    LOG_ERROR(std::format("Component type not found: {}", _type));
    return "Component type not found";
    }
    }
   catch (const std::bad_alloc &e) {
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
std::string CreateComponentCommand::undo() { throw NotImplementedError{}; }
