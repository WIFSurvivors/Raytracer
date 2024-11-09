#include "includes/SimpleSystem.hpp"
#include "includes/Entity.hpp"
#include "includes/SimpleComponent.hpp"
#include <iostream>
#include <memory>
#include <utility>

// probably inefficent because std::move doesn't move the pointer underneath
SimpleComponent *SimpleSystem::create_component(std::shared_ptr<Entity> e) {
  _components[component_count] = std::make_unique<SimpleComponent>(e);
  auto c = _components[component_count].get();
  e->add_component(c);
  return c;
}

SimpleComponent *SimpleSystem::create_component(std::shared_ptr<Entity> e,
                                                int value) {
  _components[component_count] = std::make_unique<SimpleComponent>(e, value);
  e->add_component(_components[component_count].get());
  return _components[component_count++].get();
}

void SimpleSystem::clear() { _components.clear(); }

// bool SimpleSystem::remove_component(SimpleComponent *c) {
//   return _components.erase(c);
// }

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
              << c.second->get_value() << " | e: " << (c.second->_entity.lock())
              << "\n";
  }
}
