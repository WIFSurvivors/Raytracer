#include "includes/Entity.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <algorithm>
#include <utility>

namespace RT {
Entity::Entity(const std::string &name, uuid id) : _name{name}, _uuid{id} {}
Entity::Entity(const std::string &name, uuid id, std::shared_ptr<Entity> parent)
    : _name{name}, _uuid{id}, _parent{parent} {}

std::shared_ptr<Entity> Entity::create(const std::string &name, uuid id) {
  return std::make_shared<make_shared_enabler>(name, id);
}

std::shared_ptr<Entity> Entity::create(const std::string &name, uuid id,
                                       std::shared_ptr<Entity> parent) {
  auto e = std::make_shared<make_shared_enabler>(name, id, parent);
  parent->add_child_entity(e->get_ptr());
  return e;
}

std::optional<IComponent *> Entity::get_component(uuid id) const {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](IComponent *c) { return c->get_uuid() == id; });

  if (it == _components.end())
    return std::nullopt; // same as "return {};"

  return std::make_optional<IComponent *>(*it);
}

bool Entity::add_component(IComponent *c) {
  // maybe TODO? check if component already exists here
  if (contains_component(c)) {
    return false;
  }
  _components.push_back(c);
  return true;
}

std::optional<IComponent *> Entity::get_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](IComponent *c) { return c->get_uuid() == id; });
  if (it == _components.end()) {
    return {};
  }
  return std::make_optional<IComponent *>(*it);
}

bool Entity::contains_component(IComponent *c) {
  auto it = std::find(_components.begin(), _components.end(), c);
  if (it == _components.end()) {
    return false;
  }
  return true;
}

bool Entity::contains_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](IComponent *c) { return c->get_uuid() == id; });
  if (it == _components.end()) {
    return false;
  }
  return true;
}

bool Entity::remove_component(IComponent *c) {
  auto it = std::find(_components.begin(), _components.end(), c);
  if (it != _components.end()) {
    // ?? set component owner to none ??
    _components.erase(it);
    return true;
  }
  return false;
}

bool Entity::remove_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](IComponent *c) { return c->get_uuid() == id; });
  if (it != _components.end()) {
    _components.erase(it);
    return true;
  }
  return false;
}

void Entity::remove_components(UUIDManager *um) {
  std::vector<uuid> to_remove;
  std::transform(_components.begin(), _components.end(),
                 std::back_inserter(to_remove),
                 [](auto &c) { return c->get_uuid(); });
  _components.clear();
  for (auto id : to_remove) {
    if (um->remove(id)) {
    }
  }
}

void Entity::add_child_entity(std::shared_ptr<Entity> e) {
  // TODO: check that 'e' doesn't have a parent....
  _child_entities.push_back(e);
}

bool Entity::remove_child_entity(std::shared_ptr<Entity> e) {
  return remove_child_entity(e.get());
}

bool Entity::remove_child_entity(Entity *e) {
  auto it = std::find_if(_child_entities.begin(), _child_entities.end(),
                         [e](auto e_ptr) { return e_ptr.get() == e; });

  if (it == _child_entities.end()) {
    return false;
  }
  _child_entities.erase(it);
  return true;
}

glm::vec3 Entity::get_world_position() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka
                                 // it's the root)
    auto vec = p->get_world_position();
    return vec + get_local_position();
  }
  return get_local_position();
}

glm::vec3 Entity::get_world_rotation() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka
                                 // it's the root)
    auto vec = p->get_world_rotation();
    return vec * get_local_rotation();
  }
  return get_local_rotation();
}

glm::vec3 Entity::get_world_scale() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka
                                 // it's the root)
    auto vec = p->get_world_scale();
    return vec * get_local_scale();
  }
  return get_local_scale();
}

boost::json::object Entity::to_json() const {
  boost::json::object obj = self_to_json();

  boost::json::array children;
  children = std::move(children_to_json());
  obj["children"] = std::move(children);
  return obj;
}

boost::json::object Entity::self_to_json() const {
  boost::json::object obj;
  obj["uuid"] = boost::uuids::to_string(get_uuid());
  obj["name"] = get_name();
  obj["children_count"] = get_child_entities().size();
  return obj;
}

boost::json::array Entity::children_to_json() const {
  boost::json::array arr;
  for (const auto &e : get_child_entities()) {
    boost::json::object obj = e->self_to_json();
    arr.push_back(std::move(obj));
  }
  return arr;
}

void Entity::print() { LOG(boost::json::serialize(to_json())); }
} // namespace RT
