#include "entity.hpp"

#include <iostream>

entity::entity() : _name{"root"}, _uuid{-123} {}

entity::entity(std::string name, std::weak_ptr<entity> parent, long uuid)
    : _name{name}, _parent{parent}, _uuid{uuid} {
  std::cout << "E-" << _name << "(" << _uuid << ")->"
            << (_parent.lock()->_uuid);
  std::cout << "\n";
}

bool entity::get_component(long uuid, component &c) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [uuid](component *c) { return c->get_uuid() == uuid; });
  c = *(*it);
  return true;
}

void entity::add_component(component *c) { _components.push_back(c); }

void entity::remove_component(component *c) {
  auto it = std::find(_components.begin(), _components.end(), c);
  _components.erase(it);
};

void entity::remove_component(long uuid) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [uuid](component *c) { return c->get_uuid() == uuid; });
  _components.erase(it);
}

void entity::print() {
  std::cout << "E: " << _uuid << " | C size: " << _components.size() << "\n";
  for (auto &&c : _components) {
    c->print();
    std::cout << "\n";
  }
  for (auto &&e : _child_entities) {
    e->print();
  }
}