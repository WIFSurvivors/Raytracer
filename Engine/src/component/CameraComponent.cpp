#include "includes/component/CameraComponent.hpp"
#include "includes/component/Component.hpp"

CameraComponent::CameraComponent(uuid id, Entity *e)
    : IComponent(id, e, "CameraComponent"), _fov(60.f) {}

CameraComponent::CameraComponent(uuid id, Entity *e, float fov)
    : IComponent(id, e, "CameraComponent"), _fov(fov) {}

void CameraComponent::update(const float dt) {}

void CameraComponent::to_json_details(boost::json::object &obj) {
  obj["is_main_camera"] = is_main_camera();
  obj["fov"] = get_fov();
}