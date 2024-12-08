#include "includes/component/SimpleComponent.hpp"
#include <iostream>
#include <string>
#include <boost/uuid/uuid_io.hpp>

SimpleComponent::SimpleComponent(uuid id, Entity *e) : Component{id, e} {
  std::cout << "--- constructor | ";
  print();
  std::cout << "\n";
}

SimpleComponent::SimpleComponent(uuid id, Entity *e, int value)
    : Component{id, e}, _value{value} {
  std::cout << "--- constructor | ";
  print();
  std::cout << "\n";
}

void SimpleComponent::init() {
  std::cout << "--- init | ";
  print();
  std::cout << "\n";
}

void SimpleComponent::update(const float dt) {
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
            << " | Entity:"
            << (_entity ? boost::uuids::to_string(_entity->get_uuid())
                        : "unknown");
}
