#include "includes/system/SimpleSystem.hpp"
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/VariadicTable.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

SimpleSystem::SimpleSystem() { LOG("created simple system"); }

SimpleComponent *SimpleSystem::create_component(uuid id, Entity *e, int value) {
  LOG("-- create simple component");
  auto c = create_component_base(id, e);
  c->set_value(value);
  return c;
}

void SimpleSystem::update(const FrameSnapshot& snapshot) {
  for (auto &&c : _components) {
    c.second->update(snapshot);
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
