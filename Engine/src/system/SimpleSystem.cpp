#include "includes/system/SimpleSystem.hpp"

#include "includes/component/SimpleComponent.hpp"
#include "includes/Entity.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/VariadicTable.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>
#include <format>

namespace RT {
SimpleSystem::SimpleSystem(UUIDManager *um) : System{um} {
  LOG(std::format("created {}", get_name()));
}

SimpleComponent *SimpleSystem::create_component(Entity *e, int value) {
  LOG("-- create simple component (1)");
  auto c = create_component_base(e);
  c->set_value(value);
  return c;
}

SimpleComponent *SimpleSystem::create_component(Entity *e, uuid id, int value) {
  LOG("-- create simple component (2)");
  auto c = create_component_base(e, id);
  c->set_value(value);
  return c;
}

void SimpleSystem::update(const FrameSnapshot &snapshot) {
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
} // namespace RT
