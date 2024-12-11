#include "includes/component/CameraComponent.hpp"
#include "includes/component/Component.hpp"

CameraComponent::CameraComponent(uuid id, Entity *e)
    : Component(id, e), _fov(60.f) {}

CameraComponent::CameraComponent(uuid id, Entity *e, float fov)
    : Component(id, e), _fov(fov) {}

void CameraComponent::update(const float dt) {}
