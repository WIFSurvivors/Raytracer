#include "includes/component/CameraComponent.hpp"
#include "includes/component/Component.hpp"

CameraComponent::CameraComponent(uuid id, Entity *e) : Component(id, e) {}
CameraComponent::CameraComponent(uuid id, Entity *e, float fov)
    : Component(id, e), _fov(fov) {}

void CameraComponent::init() {}
void CameraComponent::update(const float dt) {}
void CameraComponent::destroy() {}