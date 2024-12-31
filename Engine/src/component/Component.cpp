#include "includes/component/Component.hpp"
#include "includes/Entity.hpp"
#include <iostream>
#include <cassert>

IComponent::IComponent(uuid id, Entity *e, std::string name)
    : _uuid{id}, _entity{e}, _component_name{name} {
  // has to be rewritten for windows apparently?
  // https://en.cppreference.com/w/cpp/error/assert
  assert(e != nullptr && "Entity is a nullptr");
  e->add_component(this);
}

IComponent::~IComponent() {
  if (_entity != nullptr) {
    get_entity()->remove_component(this);
  }
}

boost::json::object IComponent::to_json() const {
  boost::json::object obj = to_json_base();
  obj["component_options"] = to_json_details();
  return obj;
}

boost::json::object IComponent::to_json_short() const {
  boost::json::object obj;
  obj["name"] = get_component_name();
  obj["uuid"] = boost::uuids::to_string(get_uuid());
  return obj;
}

boost::json::object IComponent::to_json_base() const {
  boost::json::object obj;
  obj["name"] = get_component_name();
  obj["uuid"] = boost::uuids::to_string(get_uuid());
  obj["entity_uuid"] = boost::uuids::to_string(get_entity()->get_uuid());
  obj["entity_name"] = get_entity()->get_name();
  return obj;
}


