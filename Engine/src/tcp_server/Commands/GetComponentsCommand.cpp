#include "includes/tcp_server/Commands/GetComponentsCommand.hpp"
#include "boost/json/array.hpp"
#include "includes/Engine.hpp"
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include <algorithm>

std::string GetComponentsCommand::execute(Engine *e) {
  try {
    auto scene = e->get_scene();
    if (!scene) {
      std::cerr << "Scene is null" << std::endl;
      return "Scene is null";
    }

    auto entity_ptr = (*scene)[_uuid];
    if (!entity_ptr.has_value()) {
      std::cerr << "Entity not found for UUID: "
                << boost::uuids::to_string(_uuid) << std::endl;
      return "Entity not found";
    }

    auto entity = entity_ptr.value();
    if (!entity) {
      std::cerr << "Entity is null for UUID: " << boost::uuids::to_string(_uuid)
                << std::endl;
      return "Entity is null";
    }
    auto components = entity->get_components();
    if (components.empty()) {
      std::cout << "No components found" << std::endl;
      return "No components found";
    }

    auto components_json = get_components_short(components);
    return boost::json::serialize(components_json);

  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    return "Memory allocation failed";
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return "Exception occurred";
  } catch (...) {
    std::cerr << "Unknown exception occurred" << std::endl;
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

int GetComponentsCommand::undo() { return 0; }
