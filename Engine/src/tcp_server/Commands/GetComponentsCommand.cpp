#include "includes/tcp_server/Commands/GetComponentsCommand.hpp"
#include "boost/json/array.hpp"
#include "includes/Engine.hpp"
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include <algorithm>
#include <format>
#include "includes/utility/Log.hpp"
#include <string>
#include "includes/utility/NotImplementedError.hpp"


std::string GetComponentsCommand::execute(Engine *e) {
  try {
    auto scene = e->get_scene();
    if (!scene) {
      std::string msg = "Scene is null";
      Log::error(msg);
      return msg;
    }
    auto entity_ptr = (*scene)[_uuid];
    if (!entity_ptr.has_value()) {
      std::string msg = std::format("Entity not found for UUID: {}", boost::uuids::to_string(_uuid));
      Log::error(msg);
      return msg;
    }
    auto entity = entity_ptr.value();
    if (!entity) {
      std::string msg = std::format("Entity is null for UUID: {}", boost::uuids::to_string(_uuid));
      Log::error(msg);
      return msg;
    }
    auto components = entity->get_components();
    if (components.empty()) {
      std::string msg = "No components found";
      Log::error(msg);
      return msg;
    }

    auto components_json = get_components_short(components);
    return boost::json::serialize(components_json);

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
  return "Entity not found";
}

boost::json::array GetComponentsCommand::get_components_short(
    const std::vector<IComponent *> &components) {
  boost::json::array arr;
  std::for_each(components.begin(), components.end(),
                [&arr](const auto &c) { arr.push_back(c->to_json_short()); });
  return arr;
}

int GetComponentsCommand::undo() { throw NotImplementedError{}; }
