#include "includes/tcp_server/Commands/SetComponentOptionsCommand.hpp"
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
#include "includes/component/LightComponent.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Engine;
using RT::Log;
using RT::CameraSystem;
using RT::RenderSystem;
using RT::SimpleSystem;
using RT::LightSystem;

std::string SetComponentOptions::execute(Engine *e) {
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
        try {
          _old_obj = component->to_json()["component_options"].as_object();
          component->set_from_json(this->get_obj());
          LOG("CameraComponent updated");
                set_successfull(true);
          return "CameraComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }
      }
    } else if (auto render_system = dynamic_cast<RenderSystem *>(system)) {
      auto render = render_system->get_component(_uuid);
      if (render.has_value()) {
        auto component = render.value();
        LOG("RenderComponent found");
        try {
          _old_obj = component->to_json()["component_options"].as_object();
          component->set_from_json(this->get_obj());
          LOG("RenderComponent updated");
                set_successfull(true);
          return "RenderComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }
      }
    } else if (auto simple_system = dynamic_cast<SimpleSystem *>(system)) {
      auto simple = simple_system->get_component(_uuid);
      if (simple.has_value()) {
        auto component = simple.value();
        LOG("SimpleComponent found");
        try {
          _old_obj = component->to_json()["component_options"].as_object();  
          component->set_from_json(this->get_obj());
          LOG("SimpleComponent updated");
                set_successfull(true);
          return "SimpleComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }
      }
    } else if (auto light_system = dynamic_cast<LightSystem *>(system)) {
      auto light = light_system->get_component(_uuid);
      if (light.has_value()) {
        auto component = light.value();
        LOG("LightComponent found");
        try {
          _old_obj = component->to_json()["component_options"].as_object();
          component->set_from_json(this->get_obj());
          LOG("LightComponent updated");
                set_successfull(true);
          return "LightComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }
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

std::string SetComponentOptions::undo(RT::Engine *engine) { 
  
  auto uuid_manager = engine->get_active_uuid_manager();
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
        try {
          component->set_from_json(this->get_old_obj());
          LOG("CameraComponent updated");
          return "CameraComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }
      }
    } else if (auto render_system = dynamic_cast<RenderSystem *>(system)) {
      auto render = render_system->get_component(_uuid);
      if (render.has_value()) {
        auto component = render.value();
        LOG("RenderComponent found");
        try {
          component->set_from_json(this->get_old_obj());
          LOG("RenderComponent updated");
          return "RenderComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }
      }
    } else if (auto simple_system = dynamic_cast<SimpleSystem *>(system)) {
      auto simple = simple_system->get_component(_uuid);
      if (simple.has_value()) {
        auto component = simple.value();
        LOG("SimpleComponent found");
        try {
          component->set_from_json(this->get_old_obj());
          LOG("SimpleComponent updated");
          return "SimpleComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }
      }
    } else if (auto light_system = dynamic_cast<LightSystem *>(system)) {
      auto light = light_system->get_component(_uuid);
      if (light.has_value()) {
        auto component = light.value();
        LOG("LightComponent found");
        try {
          component->set_from_json(this->get_old_obj());
          LOG("LightComponent updated");
          return "LightComponent updated";
        } catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return e.what();
        }

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
