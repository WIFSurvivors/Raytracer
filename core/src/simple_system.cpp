#include "simple_system.hpp"
#include "entity.hpp"
#include "simple_component.hpp"
#include <iostream>
#include <memory>
#include <utility>

// probably inefficent because std::move doesn't move the pointer underneath
simple_component *simple_system::create_component(std::shared_ptr<entity> e) {
  _components[component_count] = std::make_unique<simple_component>(e);
  auto c = _components[component_count].get();
  e->add_component(c);
  return c;
}

simple_component *simple_system::create_component(std::shared_ptr<entity> e,
                                                  int value) {
  _components[component_count] = std::make_unique<simple_component>(e, value);
  e->add_component(_components[component_count].get());
  return _components[component_count++].get();
}

void simple_system::clear() { _components.clear(); }

// bool simple_system::remove_component(simple_component *c) {
//   return _components.erase(c);
// }

void simple_system::update(const float dt) {
  for (auto &&c : _components) {
    c.second->update(dt);
  }
}

void simple_system::print_all_components() {
  std::cout << "CURRENT SIMPLE SYSTEM COMPONENTS:\n";
  std::cout << "c.uuid | c.adr          | c.value | c.entity-uuid:\n";
  for (auto &&c : _components) {
    std::cout << "- " << c.first << " | " << c.second << " | "
              << c.second->get_value() << " | e: " << (c.second->_entity.lock())
              << "\n";
  }
}
