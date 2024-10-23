#include "dummy_component.hpp"

#include <iostream>

dummy_component::dummy_component() {
  std::cout << "dummy_component 1" << std::endl;
}

void dummy_component::init() { std::cout << "dummy_component 1" << std::endl; }

void dummy_component::update(float dt) {
  std::cout << "dummy_component 3" << std::endl;
}

void dummy_component::destroy() {
  std::cout << "dummy_component 4" << std::endl;
}

dummy_component::~dummy_component() {
  std::cout << "dummy_component 5" << std::endl;
}
