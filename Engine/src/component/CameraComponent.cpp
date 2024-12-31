#include "includes/component/CameraComponent.hpp"
#include "includes/component/Component.hpp"
#include "includes/utility/FrameSnapshot.hpp"

CameraComponent::CameraComponent(uuid id, Entity *e)
    : IComponent(id, e, "CameraComponent"), _fov(60.f) {}

CameraComponent::CameraComponent(uuid id, Entity *e, float fov)
    : IComponent(id, e, "CameraComponent"), _fov(fov) {}

void CameraComponent::update(const FrameSnapshot& snapshot) {}

void CameraComponent::to_json_details(boost::json::object obj) {
  obj["is_main_camera"] = is_main_camera();
  obj["fov"] = get_fov();
}

boost::json::object CameraComponent::to_json_details() {
  boost::json::object obj;
  obj["is_main_camera"] = is_main_camera();
  obj["fov"] = get_fov();
  return obj;
}

void CameraComponent::set_from_json(boost::json::object obj) {
  set_fov(obj["fov"].as_double());
  set_is_main_camera(obj["is_main_camera"].as_bool());
}
