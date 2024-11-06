#include "entity.hpp"
#include <iostream>
#include <memory>
#include <string>

entity::entity() : _name{"root"}, _uuid{-123} {}
entity::entity(std::string name, int64_t uuid) : _name{name}, _uuid{uuid} {}
entity::entity(std::string name, int64_t uuid, std::shared_ptr<entity> parent)
    : _name{name}, _uuid{uuid}, _parent{parent} {}

std::shared_ptr<entity> entity::create(std::string name, int64_t uuid) {
  return std::make_shared<entity>(name, uuid);
}

std::shared_ptr<entity> entity::create(std::string name, int64_t uuid,
                                       std::shared_ptr<entity> parent) {
  auto e = std::make_shared<entity>(name, uuid);
  parent->add_child_entity(e->get_ptr());
  return e;
}

bool entity::get_component(int64_t uuid, component &c) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [uuid](component *c) { return c->get_uuid() == uuid; });
  c = *(*it); // get value from iterator (*it), then turn it into a value type
              // (*(*it))
  return true;
}

void entity::add_component(component *c) {
  // check if component is already in list
  _components.push_back(c);
}

void entity::remove_component(component *c) {
  auto it = std::find(_components.begin(), _components.end(), c);
  _components.erase(it);
  // add boolean return type
}

void entity::remove_component(int64_t uuid) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [uuid](component *c) { return c->get_uuid() == uuid; });
  _components.erase(it);
  // add boolean return type
}

void entity::add_child_entity(std::shared_ptr<entity> e) {
  _child_entities.push_back(e);
}

std::weak_ptr<entity> entity::get_parent_entity() { return _parent; }

// remove magic number
void entity::print() { print(4); }

void entity::print(int indent) {
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
