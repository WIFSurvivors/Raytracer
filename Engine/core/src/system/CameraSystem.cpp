#include "includes/system/CameraSystem.hpp"
#include "includes/utility/SimpleLogger.hpp"

CameraSystem::CameraSystem() {}

CameraComponent *CameraSystem::create_component(uuid id, Entity *e) {
  _components[id] = std::make_unique<CameraComponent>(id, e);
  auto ptr = _components[id].get();
  e->add_component(ptr);
  if (!_main_camera) {
    set_main_camera(ptr);
  }
  return ptr;
}

CameraComponent *CameraSystem::create_component(uuid id, Entity *e, float fov) {
  auto c = create_component(id, e);
  c->set_fov(fov);
  return c;
}

bool CameraSystem::remove(Component *c) { return remove(c->get_uuid()); }
bool CameraSystem::remove(uuid id) { return _components.erase(id); }

std::optional<CameraComponent *> CameraSystem::get_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](auto&& c) { return c.first == id; });
  if (it != _components.end()) {
    return std::nullopt;
  }
  return std::make_optional(it->second.get());
}

void CameraSystem::set_main_camera(CameraComponent *cc) {
  if (_main_camera) {
    _main_camera->set_is_main_camera(false);
  }
  _main_camera = cc;
  _main_camera->set_is_main_camera(true);
}
