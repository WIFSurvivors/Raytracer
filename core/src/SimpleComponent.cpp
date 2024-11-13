#include "includes/SimpleComponent.hpp"
#include <iostream>
#include <string>

SimpleComponent::SimpleComponent(std::weak_ptr<Entity> e) : Component{e} {
  std::cout << "--- constructor | ";
  print();
  std::cout << "\n";
}

SimpleComponent::SimpleComponent(std::weak_ptr<Entity> e, int value)
    : Component{e}, _value{value} {
  std::cout << "--- constructor | ";
  print();
  std::cout << "\n";
}

void SimpleComponent::init() {
  std::cout << "--- init | ";
  print();
  std::cout << "\n";
}

void SimpleComponent::update(float dt) {
  // std::cout << "--- update | ";
  // print();
  // std::cout << "\n";
}

void SimpleComponent::destroy() {
  std::cout << "--- destroy | ";
  print();
  std::cout << "\n";
}

// SimpleComponent::~SimpleComponent() {
//   std::cout << "--- destructor | ";
//   print();
//   std::cout << "\n";
// }

void SimpleComponent::set_value(int value) { _value = value; }

int SimpleComponent::get_value() { return _value; }

void SimpleComponent::print() {
  std::cout << "c:" << _uuid << " (adr: " << this << ") | value: " << _value
            << " | Entity:" << _entity.lock()->get_uuid();
}
