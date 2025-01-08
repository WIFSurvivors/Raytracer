#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/json.hpp>
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/RenderComponent.hpp"
#include <iomanip>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <utility>

namespace RT {
inline std::string format_double(double value) {
  std::ostringstream out;
  out << std::fixed << std::setprecision(3) << value;
  return out.str();
}
inline boost::json::array
entities_to_json(const std::vector<std::shared_ptr<Entity>> &entities) {
  boost::json::array arr;
  for (const auto &entity : entities) {
    boost::json::object obj;
    obj["uuid"] = boost::uuids::to_string(entity->get_uuid());
    obj["name"] = entity->get_name();
    obj["children_count"] = entity->get_child_entities().size();
    arr.push_back(std::move(obj));
  }
  return arr;
}

inline boost::json::object
entity_to_json(const std::shared_ptr<Entity> &entity) {
  boost::json::object obj;
  obj["uuid"] = boost::uuids::to_string(entity->get_uuid());
  obj["name"] = entity->get_name();
  obj["children_count"] = entity->get_child_entities().size();

  boost::json::array children;
  children = std::move(entities_to_json(entity->get_child_entities()));
  obj["children"] = std::move(children);

  return obj;
}

inline boost::json::object
entity_options_to_json(const std::shared_ptr<Entity> &entity) {
  boost::json::object obj;
  obj["uuid"] = boost::uuids::to_string(entity->get_uuid());
  obj["name"] = entity->get_name();
  // boost::json::array options;
  boost::json::object _options;
  boost::json::object loc_options;
  loc_options["x"] = format_double(entity->get_local_position().x);
  loc_options["y"] = format_double(entity->get_local_position().y);
  loc_options["z"] = format_double(entity->get_local_position().z);
  _options["Translation"] = std::move(loc_options);
  boost::json::object rotation;
  boost::json::object rot_options;
  rot_options["x"] = format_double(entity->get_local_rotation().x);
  rot_options["y"] = format_double(entity->get_local_rotation().y);
  rot_options["z"] = format_double(entity->get_local_rotation().z);
  _options["rotation"] = std::move(rot_options);
  boost::json::object scale;
  boost::json::object scale_options;
  scale_options["x"] = format_double(entity->get_local_scale().x);
  scale_options["y"] = format_double(entity->get_local_scale().y);
  scale_options["z"] = format_double(entity->get_local_scale().z);
  _options["scale"] = std::move(scale_options);

  obj["options"] = std::move(_options);
  return obj;
}

inline boost::json::object
component_to_json(const std::shared_ptr<IComponent> &component) {
  boost::json::object obj;
  obj["uuid"] = boost::uuids::to_string(component->get_uuid());
  // obj["type"] = component->get_type();
  return obj;
}

inline boost::json::object
components_to_json(std::vector<IComponent *> components) {
  boost::json::object obj;
  boost::json::object obc;
  boost::json::array arr;
  for (const auto &component : components) {
    obc["uuid"] = boost::uuids::to_string(component->get_uuid());

    arr.push_back(std::move(obc));
  }
  obj["components"] = std::move(arr);
  return obj;
}
} // namespace RT