#include "simple_system.hpp"
#include "entity.hpp"
#include "simple_component.hpp"
#include <iostream>
#include <utility>

simple_component *simple_system::create_component(std::shared_ptr<entity> e) {
  std::cout << "create 1\n";
  _components[component_count] = std::make_unique<simple_component>(e);
  e->add_component(_components[component_count].get());
  return _components[component_count++].get();
}

simple_component *simple_system::create_component(std::shared_ptr<entity> e,
                                                  int value) {
  std::cout << "create 2\n";
  _components[component_count] = std::make_unique<simple_component>(e, value);
  e->add_component(_components[component_count].get());
  return _components[component_count++].get();
}

simple_system::~simple_system() {
  std::cout << "~simple_system() 1\n";
  print_all_components();
  // _components.clear();
  // print_all_components();
  std::cout << "~simple_system() 2\n";
}

void simple_system::clear() {
  std::cout << "clear!\n";

  _components.clear();
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
    std::cout << "- " << c.first << " | " << c.second << " | "
              << c.second->get_value() << " | e: " << (c.second->_entity.lock())
              << "\n";
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
