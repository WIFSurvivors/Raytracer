#include "includes/tcp_server/Commands/GetComponentsCommand.hpp"
#include "boost/json/array.hpp"
#include "includes/Engine.hpp"
#include "includes/Entity.hpp"
#include "includes/system/System.hpp"
#include "includes/component/Component.hpp"
#include <algorithm>
#include <format>
#include "includes/utility/Log.hpp"
#include <string>
#include "includes/utility/NotImplementedError.hpp"

using RT::Engine;
using RT::Log;

std::string GetComponentsCommand::execute(Engine *e) {
  try {
    auto scene = e->get_scene();
    if (!scene) {
      std::string msg = "Scene is null";
      LOG_ERROR(msg);
      return msg;
    }
    uuid _uuid = this->get_uuid();
    auto entity_ptr = (*scene)[_uuid];
    if (!entity_ptr.has_value()) {
      std::string msg = std::format("Entity not found for UUID: {}",
                                    boost::uuids::to_string(_uuid));
      LOG_ERROR(msg);
      return msg;
    }
    auto entity = entity_ptr.value();
    if (!entity) {
      std::string msg = std::format("Entity is null for UUID: {}",
                                    boost::uuids::to_string(_uuid));
      LOG_ERROR(msg);
      return msg;
    }
    auto components = entity->get_components();
    if (components.empty()) {
      std::string msg = "No components found";
      LOG_ERROR(msg);
      return msg;
    }

    auto components_json = get_components_short(scene, components);
    set_successfull(true);

    return boost::json::serialize(components_json);

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
  return "Entity not found";
}

boost::json::array GetComponentsCommand::get_components_short(
    RT::Scene *s, const std::vector<uuid> &components) {
  boost::json::array arr;
  std::for_each(components.begin(), components.end(),
                [s, &arr](const auto &id) {
                  auto sto = s->get_uuid_manager()->get_storage(id);

                  if (auto sys = dynamic_cast<RT::CameraSystem *>(sto)) {
                    auto c = sys->get_component(id);
                    if (c.has_value())
                      arr.push_back(c.value()->to_json_short());

                  } else if (auto sys = dynamic_cast<RT::LightSystem *>(sto)) {
                    auto c = sys->get_component(id);
                    if (c.has_value())
                      arr.push_back(c.value()->to_json_short());

                  } else if (auto sys = dynamic_cast<RT::RenderSystem *>(sto)) {
                    auto c = sys->get_component(id);
                    if (c.has_value())
                      arr.push_back(c.value()->to_json_short());
                  }
                });
  return arr;
}
