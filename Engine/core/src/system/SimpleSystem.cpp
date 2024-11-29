#include "includes/system/SimpleSystem.hpp"
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

SimpleSystem::SimpleSystem() { SimpleLogger::print("simple system 1"); }

SimpleComponent *SimpleSystem::create_component(uuid id, Entity *e) {
  return create_component(id, e, 0);
}

SimpleComponent *SimpleSystem::create_component(uuid id, Entity *e, int value) {
  _components[id] = std::make_unique<SimpleComponent>(id, e, value);
  auto ptr = _components[id].get();
  e->add_component(ptr);
  return ptr;
}

bool SimpleSystem::remove(uuid id) { return _components.erase(id); }
bool SimpleSystem::remove(Component *c) { return remove(c->get_uuid()); }

void SimpleSystem::clear() { _components.clear(); }

void SimpleSystem::update(const float dt) {
  for (auto &&c : _components) {
    c.second->update(dt);
  }
}

void SimpleSystem::print_all_components() {
  std::cout << "CURRENT SIMPLE SYSTEM COMPONENTS:\n";
  std::cout << "c.uuid | c.adr          | c.value | c.Entity-uuid:\n";
  for (auto &&c : _components) {
    std::cout << "- " << c.first << " | " << c.second << " | "
              << c.second->get_value()
              << " | e: " << c.second->get_entity()->get_uuid() << "\n";
  }
}
