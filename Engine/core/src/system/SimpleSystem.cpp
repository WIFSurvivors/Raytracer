#include "includes/system/SimpleSystem.hpp"
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

SimpleSystem::SimpleSystem() { SimpleLogger::print("-- created simple system"); }

SimpleComponent *SimpleSystem::create_component(uuid id, Entity *e, int value) {
  SimpleLogger::print("-- create simple component");
  auto c = create_component(id, e);
  c->set_value(value);
  return c;
}

void SimpleSystem::update(const float dt) {
  for (auto &&c : _components) {
    c.second->update(dt);
  }
}

void SimpleSystem::print_component(const SimpleComponent& c){
//   std::cout << "CURRENT SIMPLE SYSTEM COMPONENTS:\n";
//   std::cout << "c.uuid | c.adr          | c.value | c.Entity-uuid:\n";
//   for (auto &&c : _components) {
//     std::cout << "- " << c.first << " | " << c.second << " | "
//               << c.second->get_value()
//               << " | e: " << c.second->get_entity()->get_uuid() << "\n";
//   }
}
