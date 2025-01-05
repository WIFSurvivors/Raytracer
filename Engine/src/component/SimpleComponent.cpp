#include "includes/component/SimpleComponent.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include <iostream>
#include <string>

SimpleComponent::SimpleComponent(uuid id, Entity *e)
    : IComponent{id, e, "SimpleComponent"}, _value{0} {}

SimpleComponent::SimpleComponent(uuid id, Entity *e, int value)
    : IComponent{id, e, "SimpleComponent"}, _value{value} {}

void SimpleComponent::update(const FrameSnapshot& snapshot) {}

boost::json::object SimpleComponent::to_json_details() const {
  boost::json::object obj;
  obj["value"] = get_value();
  return obj;
}
void SimpleComponent::set_from_json(boost::json::object obj) {
  set_value(obj.at("value").as_int64());
}
