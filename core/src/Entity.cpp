#include "includes/Entity.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <optional>

Entity::Entity() : _name{"root"}, _uuid{-123} {}
Entity::Entity(std::string name, int64_t uuid) : _name{name}, _uuid{uuid} {}
Entity::Entity(std::string name, int64_t uuid, std::shared_ptr<Entity> parent)
    : _name{name}, _uuid{uuid}, _parent{parent} {}

std::shared_ptr<Entity> Entity::create(std::string name, int64_t uuid) {
  return std::make_shared<Entity>(name, uuid);
}

std::shared_ptr<Entity> Entity::create(std::string name, int64_t uuid,
                                       std::shared_ptr<Entity> parent) {
  auto e = std::make_shared<Entity>(name, uuid);
  parent->add_child_entity(e->get_ptr());
  return e;
}

std::optional<Component *> Entity::get_component(int64_t uuid) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [uuid](Component *c) { return c->get_uuid() == uuid; });

  if (it == _components.end())
    return std::nullopt; // same as "return {};"

  return std::make_optional<Component *>(*it);
}

void Entity::add_component(Component *c) {
  // check if component is already in list
  _components.push_back(c);
}

void Entity::remove_component(Component *c) {
  auto it = std::find(_components.begin(), _components.end(), c);
  if (it != _components.end())
    _components.erase(it);
  // add boolean return type
}

void Entity::remove_component(int64_t uuid) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [uuid](Component *c) { return c->get_uuid() == uuid; });
  if (it != _components.end())
    _components.erase(it);
  // add boolean return type
}

void Entity::add_child_entity(std::shared_ptr<Entity> e) {
  _child_entities.push_back(e);
}

std::vector<std::shared_ptr<Entity>> &Entity::get_child_entities() {
  return _child_entities;
}
std::weak_ptr<Entity> Entity::get_parent_entity() { return _parent; }

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
