#include "simple_system.hpp"
#include <iostream>
#include <utility>

simple_component *simple_system::create_component(entity &e) {
  auto s = std::make_unique<simple_component>();
  _components[1] = std::move(s);
  // _components.insert(1, std::move(s));
  return s.get();
}

simple_component *simple_system::create_component(entity &e, int value) {
  auto s = std::make_unique<simple_component>(value);
  // e.add_component(s.get());
  _components[1] = std::move(s);
  return s.get();
}

// bool simple_system::remove_component(simple_component *c) {
//   return _components.erase(c);
// }

void simple_system::update(const float dt) const {
  for (auto &&c : _components) {
    c->update(dt);
  }
}

void simple_system::print_all_components(bool print_new_line) {
  for (auto &&c : _components) {
    std::cout << c->get_value() << " ";
  }
  if (print_new_line)
    std::cout << "\n";
}

void simple_system::init() const {
  for (auto &&c : _components) {
    c->init();
  }
}

void simple_system::destroy() const {
  for (auto &&c : _components) {
    c->destroy();
  }
}
