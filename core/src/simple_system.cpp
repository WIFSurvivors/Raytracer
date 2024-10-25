#include "simple_system.hpp"
#include <iostream>
#include <utility>

simple_component *simple_system::create_component(entity &e) {
  auto s = std::make_unique<simple_component>();
  _components.insert(std::move(s));

  return s.get();
}

simple_component *simple_system::create_component(entity &e, int value) {
  auto s = std::make_unique<simple_component>(value);
  e.get_components().insert(s);
  _components.insert(std::move(s));
  return s.get();
}

// bool simple_system::remove_component(simple_component *c) {
//   return _components.erase(c);
// }

void simple_system::print_all_components() {
  for (auto &&c : _components) {
    std::cout << c->get_value() << " ";
  }
  std::cout << "\n";
}
