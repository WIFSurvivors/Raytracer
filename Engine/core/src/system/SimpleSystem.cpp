#include "includes/system/SimpleSystem.hpp"
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include "includes/utility/VariadicTable.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

SimpleSystem::SimpleSystem() {
  SimpleLogger::print("-- created simple system");
}

SimpleComponent *SimpleSystem::create_component(uuid id, Entity *e, int value) {
  SimpleLogger::print("-- create simple component");
  auto c = create_component_base(id, e);
  c->set_value(value);
  return c;
}

void SimpleSystem::update(const float dt) {
  for (auto &&c : _components) {
    c.second->update(dt);
  }
}

void SimpleSystem::print() {
  VariadicTable<std::string, int, std::string, std::string> vt(
      {"SimpleComponent UUID", "Value", "Entity Name", "Entity UUID"});

  for (const auto &[key, value] : _components) {
    vt.addRow(boost::uuids::to_string(key), value->get_value(),
              value->get_entity()->get_name(),
              boost::uuids::to_string(value->get_entity()->get_uuid()));
  }

  vt.print(std::cout);
  std::cout << std::endl;
}
