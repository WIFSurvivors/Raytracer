#include "includes/tcp_server/Commands/GetComponentOptionsCommand.hpp"
#include "includes/utility/Log.hpp"
#include "includes/Engine.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/system/System.hpp"
#include "includes/system/CameraSystem.hpp"
#include "includes/system/RenderSystem.hpp"
#include "includes/system/SimpleSystem.hpp"
#include "includes/system/LightSystem.hpp"
#include "includes/component/CameraComponent.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/component/LightComponent.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Log;
using RT::Engine;
using RT::CameraSystem;
using RT::RenderSystem;
using RT::SimpleSystem;
using RT::LightSystem;

std::string GetComponentOptions::execute(Engine *e) {
  auto uuid_manager = e->get_active_uuid_manager();
  if (!uuid_manager) {
    std::string error = "UUID Manager is null";
    LOG_ERROR(error);
    return error;
  }
  uuid _uuid = this->get_uuid();
  auto system = uuid_manager->get_storage(_uuid);
  if (system != nullptr) {
    if (auto camera_system = dynamic_cast<CameraSystem *>(system)) {
      auto camera = camera_system->get_component(_uuid);
      if (camera.has_value()) {
        auto component = camera.value();
        LOG("CameraComponent found");
        std::string json_msg = boost::json::serialize(
            component->to_json()["component_options"]); // SORRY SPYRO :C
        LOG(json_msg);
          set_successfull(true);

        return json_msg;
      }
    } else if (auto render_system = dynamic_cast<RenderSystem *>(system)) {
      auto render = render_system->get_component(_uuid);
      if (render.has_value()) {
        auto component = render.value();
        LOG("RenderComponent found");
        std::string json_msg = boost::json::serialize(
            component->to_json()["component_options"]); // SORRY SPYRO :C
        LOG(json_msg);
          set_successfull(true);

        return json_msg;
      }
    } else if (auto simple_system = dynamic_cast<SimpleSystem *>(system)) {
      auto simple = simple_system->get_component(_uuid);
      if (simple.has_value()) {
        auto component = simple.value();
        LOG("SimpleComponent found");
        std::string json_msg = boost::json::serialize(
            component->to_json()["component_options"]); // SORRY SPYRO :C
        LOG(json_msg);
          set_successfull(true);

        return json_msg;
      }
    } else if (auto light_system = dynamic_cast<LightSystem *>(system)) {
      auto light = light_system->get_component(_uuid);
      if (light.has_value()) {
        auto component = light.value();
        LOG("LightComponent found");
        std::string json_msg = boost::json::serialize(
            component->to_json()["component_options"]); // SORRY SPYRO :C
        LOG(json_msg);
          set_successfull(true);
        return json_msg;
      }
    } else {
      LOG_ERROR("System found but no component found");
      return "System found but no component found";
    }
  } else {
    LOG_ERROR("The UUID you provided does not belong to a System.");
    return "The UUID you provided does not belong to a System.";
  }
  return "Error";
}

