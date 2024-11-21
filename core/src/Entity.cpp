#include "includes/Entity.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <optional>

Entity::Entity() : _name{"root"}, _uuid{} {} // i dislike the empty uuid :/
Entity::Entity(std::string name, uuid id) : _name{name}, _uuid{id} {}
Entity::Entity(std::string name, uuid id, std::shared_ptr<Entity> parent)
    : _name{name}, _uuid{id}, _parent{parent} {}

std::shared_ptr<Entity> Entity::create(std::string name, uuid id) {
  return std::make_shared<Entity>(name, id);
}

std::shared_ptr<Entity> Entity::create(std::string name, uuid id,
                                       std::shared_ptr<Entity> parent) {
  auto e = std::make_shared<Entity>(name, id);
  parent->add_child_entity(e->get_ptr());
  return e;
}

std::optional<Component *> Entity::get_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](Component *c) { return c->get_uuid() == id; });

  if (it == _components.end())
    return std::nullopt; // same as "return {};"

  return std::make_optional<Component *>(*it);
}

void Entity::add_component(Component *c) {
  // check if component is already in list
  _components.push_back(c);
}

bool Entity::remove_component(Component *c) {
  auto it = std::find(_components.begin(), _components.end(), c);
  if (it != _components.end()) {
    _components.erase(it);
    return true;
  }
  return false;
}

bool Entity::remove_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](Component *c) { return c->get_uuid() == id; });
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
std::weak_ptr<Entity> Entity::get_parent_entity() { return _parent; }

const std::string &Entity::get_name() { return _name; }
void Entity::set_name(const std::string &name) { _name = name; }

// remove magic number
void Entity::print() { print(4); }

void Entity::print(int indent) {
  std::cout << "E: " << _name << "(" << _uuid
            << ") | components: " << _components.size() << "\n";

  for (auto &&c : _components) {
    for (int i = 0; i < indent; i++)
      std::cout << " ";
    c->print();
    std::cout << "\n";
  }

  int new_indent = indent + 4;
  for (auto &&e : _child_entities) {
    for (int i = 0; i < indent; i++)
      std::cout << " ";
    e->print(new_indent);
  }
}
