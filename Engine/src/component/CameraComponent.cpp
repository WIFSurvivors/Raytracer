#include "includes/component/CameraComponent.hpp"
#include "includes/component/Component.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include "includes/utility/JsonConverter.hpp"

namespace RT {
CameraComponent::CameraComponent(uuid id, Entity *e)
    : CameraComponent(id, e, 60.f) {}

CameraComponent::CameraComponent(uuid id, Entity *e, float fov)
    : IComponent(id, e, "CameraComponent"), _fov(fov) {}

void CameraComponent::update(const FrameSnapshot &snapshot) {}

boost::json::object CameraComponent::to_json_details() const {
  boost::json::object obj;
  obj["is_main_camera"] = is_main_camera();
  obj["fov"] = format_float(get_fov());
  obj["far"] = format_float(get_far());
  obj["near"] = format_float(get_near());  
  return obj;
}

void CameraComponent::set_from_json(boost::json::object obj) {
  set_fov(obj["fov"].as_double());
  set_is_main_camera(obj["is_main_camera"].as_bool());
  set_far(obj["far"].as_double());
  set_near(obj["near"].as_double());
}
} // namespace RT
