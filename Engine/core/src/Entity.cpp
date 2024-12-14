#include "includes/Entity.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <optional>

Entity::Entity() : _name{"root"}, _uuid{} {} // i dislike the empty uuid :/
Entity::Entity(const std::string &name, uuid id) : _name{name}, _uuid{id} {}
Entity::Entity(const std::string &name, uuid id, std::shared_ptr<Entity> parent)
    : _name{name}, _uuid{id}, _parent{parent} {}

std::shared_ptr<Entity> Entity::create(const std::string &name, uuid id) {
  return std::make_shared<Entity>(name, id);
}

std::shared_ptr<Entity> Entity::create(const std::string &name, uuid id,
                                       std::shared_ptr<Entity> parent) {
  auto e = std::make_shared<Entity>(name, id, parent);
  parent->add_child_entity(e->get_ptr());
  return e;
}

std::optional<IComponent *> Entity::get_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](IComponent *c) { return c->get_uuid() == id; });

  if (it == _components.end())
    return std::nullopt; // same as "return {};"

  return std::make_optional<IComponent *>(*it);
}

void Entity::add_component(IComponent *c) {
	// maybe TODO? check if component already exists here
  _components.push_back(c);
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

void Entity::add_child_entity(std::shared_ptr<Entity> e) {
  _child_entities.push_back(e);
}

std::vector<std::shared_ptr<Entity>> &Entity::get_child_entities() {
  return _child_entities;
}

bool Entity::remove_child_entity(std::shared_ptr<Entity> e) {
  throw NotImplementedError{};
}

glm::vec3 Entity::get_world_position() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka it's the root)
    auto vec = p->get_world_position();
    return vec + get_local_position();
  }
  return get_local_position();
}

glm::vec3 Entity::get_world_rotation() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka it's the root)
    auto vec = p->get_world_rotation();
    return vec + get_local_rotation();
  }
  return get_local_rotation();
}

glm::vec3 Entity::get_world_scale() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka it's the root)
    auto vec = p->get_world_scale();
    return vec + get_local_scale();
  }
  return get_local_scale();
}


boost::json::object Entity::to_json(){
    boost::json::object obj = self_to_json();

    boost::json::array children;
    children = std::move(children_to_json());
    obj["children"] = std::move(children);
    return obj;
}

boost::json::object Entity::self_to_json(){
    boost::json::object obj;
    obj["uuid"] = boost::uuids::to_string(get_uuid());
    obj["name"] = get_name();
    obj["children_count"] = get_child_entities().size();
    return obj;
}

boost::json::array Entity::children_to_json(){
    boost::json::array arr;
    for (const auto& e : get_child_entities()) {
    	boost::json::object obj = e->self_to_json();
        arr.push_back(std::move(obj));
    }
    return arr;
}

void Entity::print() { std::cout << to_json() << "\n"; }
