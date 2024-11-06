#include "simple_component.hpp"
#include <iostream>
#include <string>

simple_component::simple_component(std::weak_ptr<entity> e) : component{e} {
  std::cout << "--- constructor | ";
  print();
  std::cout << "\n";
}

simple_component::simple_component(std::weak_ptr<entity> e, int value)
    : component{e}, _value{value} {
  std::cout << "--- constructor | ";
  print();
  std::cout << "\n";
}

void simple_component::init() {
  std::cout << "--- init | ";
  print();
  std::cout << "\n";
}

void simple_component::update(float dt) {
  std::cout << "--- update | ";
  print();
  std::cout << "\n";
}

void simple_component::destroy() {
  std::cout << "--- destroy | ";
  print();
  std::cout << "\n";
}

// simple_component::~simple_component() {
//   std::cout << "--- destructor | ";
//   print();
//   std::cout << "\n";
// }

void simple_component::set_value(int value) { _value = value; }

int simple_component::get_value() { return _value; }

void simple_component::print() {
  std::cout << "c:" << _uuid << " (adr: " << this << ") | value: " << _value
            << " | entity:" << _entity.lock()->get_uuid();
}
