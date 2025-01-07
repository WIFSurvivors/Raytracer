#include "includes/component/LightComponent.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include "includes/utility/JsonConverter.hpp"

LightComponent::LightComponent(uuid id, Entity *e)
    : IComponent(id, e, "LightComponent") {}

void LightComponent::update(const FrameSnapshot &snapshot) {}

boost::json::object LightComponent::to_json_details() const {
  boost::json::object obj;
  obj["r"] = format_float(get_color().r);
  obj["g"] = format_float(get_color().g);
  obj["b"] = format_float(get_color().b);
  obj["intensity"] = format_float(get_intensity());
  return obj;
}

void LightComponent::set_from_json(boost::json::object obj) {
  set_color(obj["r"].as_double(), obj["g"].as_double(), obj["b"].as_double());
  set_intensity(obj["intensity"].as_double());
}
