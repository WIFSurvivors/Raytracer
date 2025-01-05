#include "includes/component/LightComponent.hpp"
#include "includes/utility/FrameSnapshot.hpp"

LightComponent::LightComponent(uuid id, Entity *e)
    : IComponent(id, e, "LightComponent") {}

void LightComponent::update(const FrameSnapshot &snapshot) {}

boost::json::object LightComponent::to_json_details() const {
  boost::json::object obj;
  obj["r"] = get_color().r;
  obj["g"] = get_color().g;
  obj["b"] = get_color().b;
  obj["intensity"] = get_intensity();
  return obj;
}

void LightComponent::set_from_json(boost::json::object obj) {
  set_color(obj["r"].as_double(), obj["g"].as_double(), obj["b"].as_double());
  set_intensity(obj["intensity"].as_double());
}
