#include "simple_system.hpp"
#include "entity.hpp"
#include "simple_component.hpp"
#include <iostream>
#include <utility>

simple_component *simple_system::create_component(std::shared_ptr<entity> e) {
  auto s = std::make_unique<simple_component>(e);
  _components[component_count++] = std::move(s);
  // _components.insert(1, std::move(s));
  return s.get();
}

simple_component *simple_system::create_component(std::shared_ptr<entity> e,
                                                  int value) {
  auto s = std::make_unique<simple_component>(e, value);

  // e.add_component(s.get());
  _components[component_count++] = std::move(s);
  return s.get();
}

// bool simple_system::remove_component(simple_component *c) {
//   return _components.erase(c);
// }

void simple_system::update(const float dt) const {
  for (auto &&c : _components) {
    c.second->update(dt);
  }
}

void simple_system::print_all_components() {
  std::cout << "SIMPLE SYSTEM COMPONENTS:\n";
  for (auto &&c : _components) {
    std::cout << "- " << c.first << " | " << c.second->get_value() << "\n";
  }
}

// void simple_system::init() const {
//   for (auto &&c : _components) {
//     c.second->init();
//   }
// }

// void simple_system::destroy() const {
//   for (auto &&c : _components) {
//     c.second->destroy();
//   }
// }
